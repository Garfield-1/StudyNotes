# 注册ubus

[TOC]

## 概述

本文以`ubus`源码中的`./examples`目录下的官方`demo`为例子，说明如何注册`ubus`接口

## 注册object方法

1. **定义`ubus`消息封装的结构**

   ```c
   enum {
       COUNT_TO,
       COUNT_STRING,
       __COUNT_MAX
   };
   
   static const struct blobmsg_policy count_policy[__COUNT_MAX] = {
       [COUNT_TO] = { .name = "to", .type = BLOBMSG_TYPE_INT32 },
       [COUNT_STRING] = { .name = "string", .type = BLOBMSG_TYPE_STRING },
   };
   ```

   `ubus`通信使用的是`json`格式，这里会预先定义好每个`ubus`消息的`json`格式的内容

2. **定义`ubus`回调函数**

   ```c
   static int test_count(struct ubus_context *ctx, struct ubus_object *obj,
                 struct ubus_request_data *req, const char *method,
                 struct blob_attr *msg)
    {
        struct blob_attr *tb[__COUNT_MAX];
        char *s1, *s2;
        uint32_t num;
   
        blobmsg_parse(count_policy, __COUNT_MAX, tb, blob_data(msg), blob_len(msg));
        if (!tb[COUNT_TO] || !tb[COUNT_STRING])
            return UBUS_STATUS_INVALID_ARGUMENT;
   
        num = blobmsg_get_u32(tb[COUNT_TO]);
        s1 = blobmsg_get_string(tb[COUNT_STRING]);
        s2 = count_to_number(num);
        if (!s1 || !s2) {
            free(s2);
            return UBUS_STATUS_UNKNOWN_ERROR;
        }
        blob_buf_init(&b, 0);
        blobmsg_add_u32(&b, "rc", strcmp(s1, s2));
        ubus_send_reply(ctx, req, b.head);
        free(s2);
   
        return 0;
    }
   ```

    ​其中的`blobmsg_parse`函数是用于解析传入的数据，解析后的数据存放在数组`tb`中，再根据之前已经定义好的数据类型进行转换

3. **注册`ubus`回调**

   ```c
   static const struct ubus_method test_methods[] = {
       UBUS_METHOD("hello", test_hello, hello_policy),
       UBUS_METHOD("watch", test_watch, watch_policy),
       UBUS_METHOD("count", test_count, count_policy),
   };
   ```

4. **定义`object`**

   ```c
   static struct ubus_object test_object = {
       .name = "test",
       .type = &test_object_type,
       .methods = test_methods,
       .n_methods = ARRAY_SIZE(test_methods),
   };
   ```

5. **将`ubus`对象注册到系统中**

   ```c
   static void server_main(void)
   {
       int ret;
   
       ret = ubus_add_object(ctx, &test_object);
       if (ret)
           fprintf(stderr, "Failed to add object: %s\n", ubus_strerror(ret));
   
       ...
   
       uloop_run();
   }
   ```
