# openwrt架构页面上传下载文件

`openwrt`架构在`web`页面接口将文件的导出导入，例如固件升级，配置文件导出导入；主要流程如下

1. 在`js`上创建一个`from`表单，表单中携带着文件传输的请求
2. `uhttp`模块处理这个请求，在`/rom/www/cgi-bin`目录下去寻找对应的接口
3. 底层`cgi-io`模块实现具体业务

整体流程如下图

![OpenWrt架构导出导入文件](./img/OpenWrt%E6%9E%B6%E6%9E%84%E5%AF%BC%E5%87%BA%E5%AF%BC%E5%85%A5%E6%96%87%E4%BB%B6.jpg)



以下用配置文件导出举例

## js代码

代码路径：`package/feeds/luci/luci-mod-system/htdocs/luci-static/resources/view/system/backup.js`

```js
handleBackup: function() {
    var thit= this;
    return thit.handleCreartBackup().then(function(){
        var form = E('form', {
            method: 'post',
            action: L.env.cgi_base + '/cgi-backup',
            enctype: 'application/x-www-form-urlencoded'
        }, E('input', { type: 'hidden', name: 'sessionid', value: rpc.getSessionID() }));

        var div = document.querySelector('#view');
        div.appendChild(form);

        form.submit();
        form.parentNode.removeChild(form);
    });
},
```



## uhttpd模块

`uhttpd`在`/rom/www/cgi-bin`目录下调用对应`cgi`接口

代码路径：`build_dir/target-arm/uhttpd-2020-10-01-3abcc891/main.c`

```c
// 初始化cgi接口路径
static void init_defaults_pre(void)
{
	conf.script_timeout = 60;
	conf.network_timeout = 30;
	conf.http_keepalive = 20;
	conf.max_script_requests = 3;
	conf.max_connections = 100;
	conf.realm = "Protected Area";
	conf.cgi_prefix = "/cgi-bin";
	conf.cgi_path = "/sbin:/usr/sbin:/bin:/usr/bin";
	INIT_LIST_HEAD(&conf.cgi_alias);
	INIT_LIST_HEAD(&conf.lua_prefix);
}
```

查看`uhttp`相关配置文件可以找到这个字段的含义

代码路径：`build_dir/target-arm/uhttpd-2020-10-01-3abcc891/.pkgdir/uhttpd/etc/config/uhttpd`

```
# CGI url prefix, will be searched in docroot.
# Default is /cgi-bin
option cgi_prefix	/cgi-bin
```



## cgi-io模块

`cig-io`模块实际上主要是实现业务上的逻辑，按照业务要求将数据获取后写入到自身进程的标准输出中，此处详细代码省略；

**由此可以断定浏览器与cgi-io之间还有一个中间层即uhttpd**

**编译产物**

`cgi-io`模块编译产物与自身同名也叫`cgi-io`，同时会创建数个软连接指向这个编译产物

代码路径：`package/feeds/packages/cgi-io/Makefile`

```makefile
define Package/cgi-io/install
	$(INSTALL_DIR) $(1)/usr/libexec $(1)/www/cgi-bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cgi-io $(1)/usr/libexec
	$(LN) ../../usr/libexec/cgi-io $(1)/www/cgi-bin/cgi-upload
	$(LN) ../../usr/libexec/cgi-io $(1)/www/cgi-bin/cgi-download
	$(LN) ../../usr/libexec/cgi-io $(1)/www/cgi-bin/cgi-backup
	$(LN) ../../usr/libexec/cgi-io $(1)/www/cgi-bin/cgi-exportlogs
	$(LN) ../../usr/libexec/cgi-io $(1)/www/cgi-bin/cgi-tarlogs
	$(LN) ../../usr/libexec/cgi-io $(1)/www/cgi-bin/cgi-exec
endef
```

