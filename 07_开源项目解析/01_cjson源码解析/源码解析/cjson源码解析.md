# cjson源码解析

> 源码下载链接https://sourceforge.net/projects/cjson/

笔者注：**核心逻辑**是指函数简化处理后只的逻辑，去除了校验和边界条件处理，只保留了关键逻辑便于读者理解

## 核心数据结构

相同等级的元素使用双向链表链接，不同等级的元素使用`child`指针连接。例如：多个`object`并列时或着`object`内部的成员使用双向链表连接，`object`名称与其内部的成员通过`child`连接

上述可得`cjson`底层维护的数据结构实际是一个桶结构而非树结构

```c
typedef struct cJSON {
	struct cJSON *next,*prev;	// 双向链表头节点和尾节点
	struct cJSON *child;		// 存储子项
	int type;					// 节点类型
	char *valuestring;			// string值的内容
	int valueint;				// number值的整形存储
	double valuedouble;			// number值的浮点型存储
	char *string;				// 存储子项的名称
} cJSON;

/* cJSON Types: */
#define cJSON_False 0
#define cJSON_True 1
#define cJSON_NULL 2
#define cJSON_Number 3
#define cJSON_String 4
#define cJSON_Array 5
#define cJSON_Object 6
```



## 全局变量

`char *ep;`记录内部错误码通过`cJSON_GetErrorPtr()`获取



## json解析流程

参考`cjson`源代码中给出`demo`的流程

核心函数为`cJSON_Parse`

**核心逻辑如下**

```c
void doit(char *text)
{
	char *out;
    cJSON *json;
	
	json = cJSON_Parse(text);
	out = cJSON_Print(json);
	cJSON_Delete(json);
	printf("%s\n", out);
	free(out);
}

int main (int argc, const char * argv[])
{
	char text1[] = "{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}";	

    doit(text1);
	create_objects();
	
	return 0;
}

```



## 关键函数接口及实现逻辑

### `cJSON_New_Item()`函数

内部构造函数，创建一个`cjson`节点，并返回节点地址

**核心逻辑如下**

```c
cJSON *cJSON_New_Item(void)
{
	cJSON* node = (cJSON*)cJSON_malloc(sizeof(cJSON));
	return node;
}
```

### `parse_value()`函数

文本解析处理函数，对文本进行初步分类。对不同的情况使用不同的处理流程

* `json`格式中的**对象**对应`{`开头
* `json`格式中的**数组**对应`[`开头
* `json`格式中字符串与数值被统称为**值**使用`""`包裹

**核心逻辑如下**

```c
static const char *parse_value(cJSON *item,const char *value)
{
    if (!strncmp(value,"null",4)) {
        item->type = cJSON_NULL;
        return value + 4;
    }

	if (*value=='\"') {
		return parse_string(item,value);
	}

	if (*value=='-' || (*value>='0' && *value<='9')) {
		return parse_number(item,value);
	}

	if (*value=='[') {
		return parse_array(item,value);
	}

	if (*value=='{') {
		return parse_object(item,value);
	}

	return 0;	/* failure. */
}
```

### `parse_object()`函数

`object`解析函数，将传入的value当作一个非常长的字符串，不断的分割字符串。将解析出来的数据用来填充item，同级别元素填充在双向链表中，低级别元素填充在child节点中。使用递归的方式实现

实际上，绝大部分`json`都是以`{`开头，因此每次解析时第一个进入的函数都是`parse_object()`

**核心逻辑如下**

   * 将`item`作为根节点，创建一个桶结构存储解析出的`json`内容
   * 函数前半段创建并新增`child`节点。

```c
/* Build an object from the text. */
static const char *parse_object(cJSON *item,const char *value)
{
	/* child在函数前半段为新增节点，函数后半段为链表哨兵位指针 */
	cJSON *child;
    child = cJSON_New_Item();
	item->child = child;
    item->type = cJSON_Object;
	
    /* 取出前半部分存入string,可能是对象或数组的名称，或是键值对的前半部分 */
	value = skip(parse_string(child, skip(value + 1)));
	child->string = child->valuestring;
    child->valuestring = NULL;
	if (*value != ':') {
		return NULL;
	}
	
    /*
     * 此处可能造成递归调用
     * 递归的出口: value开头到第一个“,”的部分是普通键值对,不包含object或array深层嵌套
	 */
	value = skip(parse_value(child, skip(value + 1)));

	while (*value == ',') {
		cJSON *new_item;
		new_item = cJSON_New_Item();

        /*
         * 创建双向不循环链表，保存之前的解析结果。链表中仅保存同级元素
         * 此处child视为链表的哨兵位指针而不是新增节点指针
         */
		child->next = new_item;
		new_item->prev = child;
        child = new_item;

		value = skip(parse_string(child, skip(value + 1)));
		child->string = child->valuestring;
		child->valuestring = NULL;
		if (*value != ':') {
			return NULL;
		}

		value = skip(parse_value(child, skip(value + 1)));
	}
	
	if (*value == '}') {
		return value + 1;
	}
    
	return NULL;
}
```

### `parse_array()`函数

其核心逻辑与`parse_object()`函数非常相似，最大的区别在于在函数前半段不需要取出`:`负号的前半部分

**核心逻辑如下**

```c
/* Build an array from input text. */
static const char *parse_array(cJSON *item,const char *value)
{
    /* child在函数前半段为新增节点，函数后半段为链表哨兵位指针 */
    cJSON *child;
    
    child = cJSON_New_Item();
	item->child = child;
    item->type = cJSON_Array;

    /*
     * 此处可能造成递归调用
     * 递归的出口: value开头到第一个“,”的部分是普通键值对,不包含object或array深层嵌套
	 */
	value = skip(parse_value(child, skip(value)));

	while (*value == ',') {
		cJSON *new_item;
		new_item = cJSON_New_Item()

        /* 此处child视为链表的哨兵位指针而不是新增节点指针 */
		child->next = new_item;
		new_item->prev = child;
		child = new_item;

		value = skip(parse_value(child, skip(value + 1)));
	}

	if (*value == ']') {
		return value + 1;	
	}

	return 0;
}
```



## 字符串处理接口

`cjson`中设计大量的字符串处理逻辑，其中有许多设计可以学习借鉴，在此列出

### `skip`函数

跳过空格和无用字符

**原函数实现：**

```c
static const char *skip(const char *in) 
{
	while (in && *in && (unsigned char)*in <= 32) {
		in++;
	}

	return in;
}
```

**函数核心思想**

函数通过`while`循环遍历整个字符串，首先判断**指针不为空**，指针指向的**值不为空**

不断向后偏移，直到指向的字符的`ACSII`码大于`32`;函数作者非常巧妙的利用了`ASCII`码表的排列的规则，将前`32`位在字符串解析时无法用到的字符舍弃，只保留了有效字符

![ACSII码表](.\img\ACSII码表.jpg)

### `parse_number`函数

将字符串解析为一个数字，同时支持复数，浮点数，可解析科学记数法的特性。

**原函数实现如下**

```c
/* Parse the input text to generate a number, and populate the result into item. */
static const char *parse_number(cJSON *item, const char *num)
{
	double n=0, sign=1, scale=0;
	int subscale=0, signsubscale=1;

	if (*num == '-') {
		sign=-1,num++;	/* Has sign? */
	}

	if (*num=='0') {
		num++;			/* is zero */
	}

	if (*num>='1' && *num<='9')	{
		do {
			n=(n*10.0)+(*num++ -'0');
		} while (*num>='0' && *num<='9');	/* Number? */
	}

	if (*num=='.' && num[1]>='0' && num[1]<='9') {
		num++;
		do {
			n=(n*10.0)+(*num++ -'0'),scale--;
		} while (*num>='0' && *num<='9');
	}	/* Fractional part? */

	if (*num=='e' || *num=='E') {		/* Exponent? */
		num++;
		if (*num=='+') {
			num++;
		} else if (*num=='-') {
			signsubscale=-1,num++;		/* With sign? */
		}

		while (*num>='0' && *num<='9') {
			subscale=(subscale*10)+(*num++ - '0');	/* Number? */
		}
	}

	n=sign*n*pow(10.0,(scale+subscale*signsubscale));	/* number = +/- number.fraction * 10^+/- exponent */
	
	item->valuedouble = n;
	item->valueint = (int)n;
	item->type = cJSON_Number;
	return num;
}
```

**函数核心思想**

首先申请数个变量存储不同的特性值

```c
static const char *parse_number(cJSON *item, const char *num)
{
    /*
     * n：存储数字
     * sign：存储正负号
     * scale：存储小数点后的位数
     * subscale：存储科学计数法部分的值
     * signsubscale：存储科学计数法的正负号
     */
	double n=0, sign=1, scale=0;
	int subscale=0, signsubscale=1;
	...
}
```

依次遍历整个字符串，将不同部分的值存储最开始不同的变量中

```c
static const char *parse_number(cJSON *item, const char *num)
{
    ...
    /* 取出符号位，使用sign记录 */
	if (*num == '-') {
		sign=-1,num++;
	}
	
    /* 跳过多余的0 */
	if (*num=='0') {
		num++;
	}

    /* 将字符串中的连续数字转换为整形 */
	if (*num >= '1' && *num <= '9')	{
		do {
            /* *num++ -'0';等价于*num-'0';num++; */
			n = (n * 10.0) + (*num++ -'0');
		} while (*num >= '0' && *num <= '9');
	}

    /*
     * 将小数点后的连续数字转化为整形
     * 与上述流程区别在于使用scale变量记录小数点后数字位数
     */
	if (*num == '.' && num[1] >= '0' && num[1] <= '9') {
		num ++;
		do {
            /* *num++ -'0';等价于*num-'0';num++;*/
			n = (n * 10.0) + (*num++ -'0'), scale--;
		} while (*num >= '0' && *num <= '9');
	}

     /* 首先记录科学计数法的符号 */
	if (*num == 'e' || *num == 'E') {
		num++;
		if (*num == '+') {
			num++;
		} else if (*num == '-') {
			signsubscale = -1, num++;
		}
		
        /*
         * 将字符串中的连续数字转化为整形
         * 此处不使用do while 可能是考虑输入非法字符的情况
         */
		while (*num >= '0' && *num <= '9') {
            /* *num++ -'0';等价于*num-'0';num++;*/
			subscale = (subscale * 10) + (*num++ - '0');
		}
	}
	...
}
```

将此前记录的不同位置的值通过运算转化成浮点数

```c
static const char *parse_number(cJSON *item, const char *num)
{
	...
    /*
     * 带符号的n：sign * n
     * 科学计数法的系数：pow(10.0, (scale + subscale * signsubscale));
     * 此处计算的是科学计数法所以底数是10
     */
	n = sign * n * pow(10.0, (scale + subscale * signsubscale));
	
	item->valuedouble = n;
	item->valueint = (int)n;
	item->type = cJSON_Number;

	return num;
}
```

