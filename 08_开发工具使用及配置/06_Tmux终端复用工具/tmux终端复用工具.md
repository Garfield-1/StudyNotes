# tmux终端复用工具

## 常用命令

* **tmux attach -t 0** 

    重接会话 使用伪窗口编号

* **tmux new**

    创建默认名称的会话

* **tmux new -s mysession**

    创建名为`mysession`的会话

* **tmux ls**

    显示会话列表

* **tmux a**

    连接上一个会话

* **tmux a -t mysession**

    连接指定会话`<>`

* **tmux rename -t s1 s2**

    重命名会话`s1`为`s2`

* **tmux kill-session**

    关闭上次打开的会话

* **tmux kill-session -t s1**

    关闭会话`s1`

* **tmux kill-session -a -t s1**

    关闭除`s1`外的所有会话

* **tmux kill-server**

    关闭所有会话



## 交互模式

按住`ctrl+b`进入交互模式



## 命令行模式

进入交互模式后再按下`:`可进入命令行模式



## 常用快捷键

* **ctrl+b ?**

    显示快捷键帮助

* **ctrl+b d**

    脱离当前会话；这样可以暂时返回Shell界面，输入tmux attach能够重新进入之前的会话

* **ctrl+b**

    空格键 : 采用下一个内置布局，这个很有意思，在多屏时，用这个就会将多有屏幕竖着展示

* **ctrl+b !**

    把当前窗口变为新窗口

* **ctrl+b "**

    横向向分隔窗口

* **ctrl+b**

    上下键: 上一个及下一个分隔窗口

* **ctrl+b &**

    确认后退出当前tmux

* **ctrl+b c**

    创建新窗口

* **ctrl+b n**

    选择下一个窗口

* **ctrl+b l**

    最后使用的窗口

* **ctrl+b p**

    选择前一个窗口

* **ctrl+b w**

    以菜单方式显示及选择窗口

* **ctrl+b s**

    以菜单方式显示和选择会话

* **ctrl+b t**

    显示时钟，然后按`enter`键后就会恢复到`shell`终端状态



## 配置文件修改

`tmux`使用的配置文件默认是`~/.tmux.conf`

可以通过`tmux source-file ~/.tmux.conf`来手动使配置文件生效



## 鼠标滚轮支持

`tmux`默认不支持鼠标滚轮上下滚动，需要修改配置文件。然后再按住`shift`键向上滑动滚轮进入鼠标模式，向下滑动到最底进入键盘模式



