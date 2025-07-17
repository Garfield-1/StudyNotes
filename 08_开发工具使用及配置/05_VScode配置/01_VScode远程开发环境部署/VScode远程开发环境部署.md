# VScode远程环境部署

## 1. 资源下载

**VScode官方下载地址**

> [Visual Studio Code - Code Editing. Redefined](https://code.visualstudio.com/)

**VScode插件市场**

> [Extensions for Visual Studio family of products | Visual Studio Marketplace](https://marketplace.visualstudio.com/VSCode)

## 2. SSH远程连接服务器

* 下载下图中的`Remote - SSH`和`Remote - SSH: Editing Configuration Files`插件并安装,配置对应ssh连接

<img src="./image/图8_VSCode SSH插件.png" alt="图8_VSCode SSH插件" />

* 连接建立后会在`~/.vscode-server`目录下生成一个与本地`VS Code`提交序号名称相同的文件夹,内部存放着远程`SSH`连接相关配置文件

<img src="./image/图9_ VSCode关于.png" alt="图9_ VSCode关于" />

* 若出现连接时一直处于加载状态

1. 可进入`~/.vscode-server/bin`中对应的文件夹，中是否存在一个大小为零的压缩包,若存在则需手动下载相关配置文件并解压至 `\~ /.vscode-server/bin/b3e4e68a0bc097f0ae7907b217c1119af9e03435`目录下,其中`commit_id`为上图中的`提交`

   ```html
   https://vscode.cdn.azure.cn/stable/${commit_id}/vscode-server-linux-x64.tar.gz
   ```

2.   进入`~/.vscode-server/bin/b3e4e68a0bc097f0ae7907b217c1119af9e03435`目录中查看是否存在如下文件`vscode-remote-lock.pi.b3e4e68a0bc097f0ae7907b217c1119af9e03435`若此前非正常退出则可能导致锁未被释放导致后续无法正常接入，手动删除锁即可

## 3. 公钥部署实现免密登录

* 在`windows`的命令行环境下手动执行`ssh-keygen`生成公钥`C:\Users\username\.ssh\id_rsa.pub`和私钥`C:\Users\username\.ssh\id_rsa`
* 在`Linux`环境下`~/.ssh`目录下创建文件`authorized_keys`将`windows`生成的公钥复制粘贴在最后即可

## 4.参数配置

* 在顶部栏显示搜索框和前进后退箭头键位

  > 文件–>首选项–>设置–>搜索`Command Center`并勾选开启

* 设置120字符提示线

  > 文件–>首选项–>设置–>搜索`editor.rulers`--》更改设置`"editor.rulers": [120]`

## 5. 常用插件

* `Chinese (Simplified)` (简体中文) `Language Pack for Visual Studio Code`

  为`VS Code`提供本地汉化界面

  <img src="./image/图1_简体中文汉化包.png" alt="图1_简体中文汉化包" />

* `Git History`

  查看`Git`提交历史记录

  <img src="./image/图2_Git History插件.png" alt="图2_Git History插件" />

* `GitLens`

  提供强大的`Git`管理功能

<img src="./image/图3_GitLens.png" alt="图3_GitLens" />

