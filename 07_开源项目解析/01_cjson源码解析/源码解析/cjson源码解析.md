# cjson源码解析

> 源码下载链接https://sourceforge.net/projects/cjson/

## 核心数据结构

```c
typedef struct cJSON {
	struct cJSON *next,*prev;	// 双向链表头节点和尾节点
	struct cJSON *child;		
	int type;					// 节点类型
	char *valuestring;			// 本节点字符串
	int valueint;
	double valuedouble;
	char *string;				// 存储父节点值
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

   函数将输入值当作一个非常长的字符串，不断的分割字符串；

   将`item`作为根节点，创建一个树状结构存储解析出的`json`内容

   **核心逻辑如下**

   ```c
   /* Build an object from the text. */
   static const char *parse_object(cJSON *item,const char *value)
   {
   	cJSON *child;
   	//创建新的child节点
   	item->type = cJSON_Object;
   	item->child = child = cJSON_New_Item();
   	
   	/* 解析字符串并填充cJSON树 */
   	value = skip(parse_string(child, skip(value + 1)));//value+1是为了跳过{符号
   	child->string = child->valuestring;
       child->valuestring = NULL;
   	if (*value != ':') {//json每个键值对使用“:”分割
   		return 0;
   	}	/* fail! */
   
   	value=skip(parse_value(child,skip(value+1)));
   	if (!value) {
   		return 0;
   	}
   	//每个键值对的结束已”,“为标志
   	while (*value == ',') {
   		cJSON *new_item;
   		new_item = cJSON_New_Item();
   		
           //创建双向链表，并使child指向双向链表头节点
   		child->next = new_item;
   		new_item->prev = child;
   		child = new_item;
           
   		value = skip(parse_string(child, skip(value+1)));
   		child->string = child->valuestring;
   		child->valuestring = NULL;
   		if (*value != ':') {
   			return 0;
   		}	/* fail! */
   
   		value = skip(parse_value(child,skip(value+1)));
   	}
   	
   	if (*value == '}') {
   		return value + 1;	/* end of array */
   	}
       
   	return 0;	/* malformed. */
   }
   ```

   

## 字符串处理接口

`cjson`中设计大量的字符串处理逻辑，其中有许多设计可以学习借鉴，在此列出

1. `skip`函数

函数通过`while`循环遍历整个字符串，首先判断指针不为空，指针指向的值不为空

不断向后偏移，直到指向的字符的`ACSII`码大于`32`;函数作者非常巧妙的利用了`ASCII`码表的排列的规则，将前`32`位在字符串解析时无法用到的字符舍弃，只保留了有效字符

![ACSII码表](.\img\ACSII码表.jpg)

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



