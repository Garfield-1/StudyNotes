# tmux终端复用工具

## 常用命令

* tmux attach -t 0   重接会话 使用伪窗口编号

* tmux new创建默认名称的会话（在tmux命令模式使用new命令可实现同样的功能，其他命令同理，后文不再列出tmux终端命令）

* tmux new -s mysession　　创建名为mysession的会话

* tmux ls　　显示会话列表

* tmux a　　连接上一个会话

* tmux a -t mysession　　连接指定会话<>

* tmux rename -t s1 s2　　重命名会话s1为s2

* tmux kill-session　　关闭上次打开的会话

* tmux kill-session -t s1　　关闭会话s1

* tmux kill-session -a -t s1　　关闭除s1外的所有会话

* tmux kill-server　　关闭所有会话

## 常用快捷键

* ctrl+b ? : 显示快捷键帮助

* ctrl+b d : 脱离当前会话；这样可以暂时返回Shell界面，输入tmux attach能够重新进入之前的会话

* ctrl+b 空格键 : 采用下一个内置布局，这个很有意思，在多屏时，用这个就会将多有屏幕竖着展示

* ctrl+b ! : 把当前窗口变为新窗口

* ctrl+b " : 横向向分隔窗口

* ctrl+b 上下键: 上一个及下一个分隔窗口

* ctrl+b & : 确认后退出当前tmux

* ctrl+b c : 创建新窗口

* ctrl+b n : 选择下一个窗口

* ctrl+b l : 最后使用的窗口

* ctrl+b p : 选择前一个窗口

* ctrl+b w : 以菜单方式显示及选择窗口

* ctrl+b s : 以菜单方式显示和选择会话。

* ctrl+b t : 显示时钟。然后按enter键后就会恢复到shell终端状态
