" ============================================================
" 常用 Vim 配置
" Windows 默认读取 %USERPROFILE%\_vimrc
" ============================================================

" --- 基础设置 ---
set nocompatible              " 关闭 Vi 兼容模式，启用 Vim 特性
set encoding=utf-8              " 内部编码
set fileencodings=utf-8,gbk,gb2312,big5,cp936
set fileencoding=utf-8
set backspace=indent,eol,start  " 退格键可删除缩进、换行和行首
set history=1000                " 命令历史行数
set autoread                    " 文件在外部修改后自动重新加载
set confirm                     " 未保存退出时弹出确认
set hidden                      " 允许切换未保存的 buffer

" --- 界面 ---
syntax enable                   " 语法高亮
set number                      " 显示行号
set cursorline                  " 高亮当前行
set showcmd                     " 显示未完成命令
set showmode                    " 显示 -- INSERT -- 等模式提示
set ruler                       " 显示光标位置
set laststatus=2                " 始终显示状态栏
set signcolumn=yes              " 左侧预留符号列，避免弹窗时抖动
set scrolloff=8                 " 光标距上下边缘 8 行时开始滚动
set sidescrolloff=8
set colorcolumn=120             " 在第120 列显示参考线
set wildmenu                    " 命令行补全菜单
set wildmode=longest:full,full  " 补全行为：先最长匹配，再循环
set termguicolors               " 终端真彩色（Neovim / 支持 GUI 的终端）

" 配色（按优先级尝试，找不到则跳过）
if has('termguicolors') && (has('nvim') || $TERM !=# 'linux')
  silent! colorscheme default
else
  silent! colorscheme desert
endif

" --- 缩进与制表符 ---
set autoindent                  " 新行继承上一行缩进
set smartindent                 " 智能缩进（C 风格等）
set expandtab                   " Tab 转为空格
set tabstop=4                   " Tab 显示宽度
set shiftwidth=4                " >> << 缩进宽度
set softtabstop=4               " 编辑时 Tab/Backspace 宽度
set shiftround                  " 缩进对齐到 shiftwidth 倍数

" 按文件类型微调缩进
autocmd FileType c,cpp,java,go,rust,python,javascript,typescript,html,css,scss,json,yaml,markdown setlocal shiftwidth=4 tabstop=4 expandtab
autocmd FileType make setlocal noexpandtab tabstop=4 shiftwidth=4

" --- 搜索 ---
set hlsearch                    " 高亮搜索结果
set incsearch                   " 输入时实时搜索
set ignorecase                  " 搜索忽略大小写
set smartcase                   " 含大写字母时区分大小写
set wrapscan                    " 搜索到文件末尾后从头继续

" --- 编辑体验 ---
set clipboard=unnamedplus       " 与系统剪贴板互通（Windows / 部分终端）
set completeopt=menuone,noselect,noinsert
set updatetime=300              " 更快触发 CursorHold 等事件（毫秒）
set timeoutlen=500              " 组合键超时
set ttimeoutlen=10
set undofile                    " 持久化 undo
set undodir=~/.vim/undo
set nobackup                    " 不生成 .bak（可按需改为 set backup）
set nowritebackup
set noswapfile                  " 不生成 .swp（可按需开启）

" 自动创建 undo 目录
if !isdirectory(expand('~/.vim/undo'))
  call mkdir(expand('~/.vim/undo'), 'p')
endif

" --- 分屏与窗口 ---
set splitbelow                  " 水平分屏在下方
set splitright                  " 垂直分屏在右侧

" --- 鼠标（终端 Vim 可用时）---
if has('mouse')
  set mouse=a
endif

" --- 文件类型 ---
filetype on
filetype plugin on
filetype indent on

