# hostapd configuration模块梳理

[TOC]

## 概述

在官方的架构图中`configuration`模块，是一个负责处理加载和处理配置文件的模块；具体对应的源代码在`hostapd-2019-08-08-ca8c2bd2/hostapd/config_file.c`文件中

## hostapd配置文件读取

> 笔者注：这里仅记录大概流程，不做详细展开

### 流程概述

从主程序的`main`函数中注册了读取配置文件的回调

```c
// hostapd\main.c:main
interfaces.config_read_cb = hostapd_config_read;
```

这个函数在这里注册后，有如下几处调用点，不难看出都是流程的开始阶段

1. `src/ap/hostapd.c:hostapd_init`
    分配并初始化每个接口的数据

2. `src/ap/hostapd.c:hostapd_config_read`
    读取并解析配置文件

3. `src/ap/hostapd.c:hostapd_interface_init_bss`
    读取配置文件并初始化 `BSS` 数据

4. `src/ap/hostapd.c:hostapd_add_iface`
    根据输入的参数 `buf`，解析并初始化一个新的接口或 BSS

这里具体的实现是，从配置文件中一行行的读取值

> 笔者注：此处为了便于阅读省略了不必要的逻辑

```c
// hostapd\config_file.c:hostapd_config_read
struct hostapd_config * hostapd_config_read(const char *fname)
{
    struct hostapd_config *conf;
    FILE *f;
    char buf[4096], *pos;
    int line = 0;
    int errors = 0;
    size_t i;

    f = fopen(fname, "r");

    // 根据struct hostapd_config来填充conf的默认值
    conf = hostapd_config_defaults();
    conf->driver = wpa_drivers[0];
    conf->last_bss = conf->bss[0];

    while (fgets(buf, sizeof(buf), f)) {
        struct hostapd_bss_config *bss;

        bss = conf->last_bss;
        line++;

        if (buf[0] == '#')
            continue;
        pos = os_strchr(buf, '=');
        pos++;
        errors += hostapd_config_fill(conf, bss, buf, pos, line);
    }

    fclose(f);

    // 根据struct hostapd_bss_data来填充bss的默认值
    for (i = 0; i < conf->num_bss; i++)
        hostapd_set_security_params(conf->bss[i], 1);

    if (hostapd_config_check(conf, 1))
        errors++;

    return conf;
}
```

关于`hostapd_config_fill`函数画风如下，这里不再做多余解释

> 笔者注：此处为了便于阅读省略了不必要的逻辑

```c
// hostapd\config_file.c:hostapd_config_fill
static int hostapd_config_fill(struct hostapd_config *conf,
                struct hostapd_bss_config *bss,
                const char *buf, char *pos, int line)
{
    if (os_strcmp(buf, "interface") == 0) {
        os_strlcpy(conf->bss[0]->iface, pos,
            sizeof(conf->bss[0]->iface));
    } else if (os_strcmp(buf, "bridge") == 0) {
        os_strlcpy(bss->bridge, pos, sizeof(bss->bridge));
    } else if (os_strcmp(buf, "vlan_bridge") == 0) {
        os_strlcpy(bss->vlan_bridge, pos, sizeof(bss->vlan_bridge));
    } else if (os_strcmp(buf, "wds_bridge") == 0) {
        os_strlcpy(bss->wds_bridge, pos, sizeof(bss->wds_bridge));
    } else if (os_strcmp(buf, "driver_params") == 0) {
        os_free(conf->driver_params);
        conf->driver_params = os_strdup(pos);
    } else if (os_strcmp(buf, "debug") == 0) {
    } else if (os_strcmp(buf, "logger_syslog_level") == 0) {
        bss->logger_syslog_level = atoi(pos);
    } else if (os_strcmp(buf, "logger_stdout_level") == 0) {
        bss->logger_stdout_level = atoi(pos);
    } else if (os_strcmp(buf, "logger_syslog") == 0) {
        bss->logger_syslog = atoi(pos);
    } else if (os_strcmp(buf, "logger_stdout") == 0) {
        bss->logger_stdout = atoi(pos);
    } else if (os_strcmp(buf, "dump_file") == 0) {
    }
    ....
}
```

`hostapd`这里的设计是解耦的，读取配置文件然后使用一个结构体填充记录；配置文件的具体生效，是在各个子模块中，这里不做展开
