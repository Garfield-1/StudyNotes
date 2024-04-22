# cjson源码解析

> 源码下载链接https://sourceforge.net/projects/cjson/



## 核心数据结构

相同等级的元素使用双向链表链接，不同等级的元素使用`child`指针连接。例如：多个`object`并列时或着`object`内部的成员使用双向链表连接，`object`名称与其内部的成员通过`child`连接

上述可得`cjson`底层维护的数据结构实际是一个桶结构而非树结构

```c
typedef struct cJSON {
	struct cJSON *next,*prev;	// 双向链表头节点和尾节点
	struct cJSON *child;		// 存放低一级元素
	int type;					// 节点类型
	char *valuestring;			// 本节前半段的值，可能是object或array的名称或键值对的前半段
	int valueint;
	double valuedouble;
	char *string;				// 本节点后半段的值
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



## 关键函数接口及实现逻辑

1. `cJSON_New_Item()`函数

   内部构造函数，创建一个`cjson`节点，并返回节点地址

   **核心逻辑如下**

   ```c
   cJSON *cJSON_New_Item(void)
   {
   	cJSON* node = (cJSON*)cJSON_malloc(sizeof(cJSON));
   	return node;
   }
   ```

2. `parse_value()`函数

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

3. `parse_object()`函数

   `object`解析函数，当待解析的值是一个`object`时进行解析；实际上，绝大部分`json`都是以`{`开头，每次解析时第一个进入的函数都是`parse_object()`

   **核心逻辑如下**

      * 函数将输入值当作一个非常长的字符串，不断的分割字符串；

      * 将`item`作为根节点，创建一个桶结构存储解析出的`json`内容
      * 函数前半段创建并新增`child`节点。
      * 函数后半段当一行解析完成后，将同级别成员添加到双向链表中，不同级别成员添加到`child`节点中

   ```c
   /* Build an object from the text. */
   static const char *parse_object(cJSON *item,const char *value)
   {
   	// child在函数前半段为新增节点，函数后半段为链表哨兵位指针
   	cJSON *child;
       child = cJSON_New_Item();
   	item->child = child;
       item->type = cJSON_Object;
   	
       // 取出前半部分存入string,可能是对象或数组的名称，或是键值对的前半部分
   	value = skip(parse_string(child, skip(value + 1)));
   	child->string = child->valuestring;
       child->valuestring = NULL;
   	if (*value != ':') {
   		return NULL;
   	}	/* fail! */
   	
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
   		}	/* fail! */
   
   		value = skip(parse_value(child, skip(value + 1)));
   	}
   	
   	if (*value == '}') {
   		return value + 1;	/* end of array */
   	}
       
   	return NULL;	/* malformed. */
   }
   ```
   
   

## 字符串处理接口

`cjson`中设计大量的字符串处理逻辑，其中有许多设计可以学习借鉴，在此列出

1. `skip`函数

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

   函数通过`while`循环遍历整个字符串，首先判断指针不为空，指针指向的值不为空

   不断向后偏移，直到指向的字符的`ACSII`码大于`32`;函数作者非常巧妙的利用了`ASCII`码表的排列的规则，将前`32`位在字符串解析时无法用到的字符舍弃，只保留了有效字符

   ![ACSII码表](.\img\ACSII码表.jpg)







