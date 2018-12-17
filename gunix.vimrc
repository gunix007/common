""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" vundle start
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" nocompatible mode, but not the vi compatible mode. same as 'set nocp'
set nocompatible
" required for vundle vim
filetype off
" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
" call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'

" The following are examples of different formats supported.
" Keep Plugin commands between vundle#begin/end.

" plugin on GitHub repo
" Plugin 'tpope/vim-fugitive'
Plugin 'scrooloose/nerdtree'
Plugin 'majutsushi/tagbar'
Plugin 'kien/ctrlp.vim'
" Plugin 'Valloric/YouCompleteMe'

" plugin from http://vim-scripts.org/vim/scripts.html
" Plugin 'L9'

" Git plugin not hosted on GitHub
" Plugin 'git://git.wincent.com/command-t.git'

" git repos on your local machine (i.e. when working on your own plugin)
" Plugin 'file:///home/gmarik/path/to/plugin'

" The sparkup vim script is in a subdirectory of this repo called vim.
" Pass the path to set the runtimepath properly.
" Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}

" Install L9 and avoid a Naming conflict if you've already installed a
" different version somewhere else.
" Plugin 'ascenator/L9', {'name': 'newL9'}

" All of your Plugins must be added before the following line
call vundle#end()                   " required for vundle vim
" load indentation rules and plugins according to the detected filetype
filetype plugin indent on           " required for vundle vim
" To ignore plugin indent changes, instead use:
" filetype plugin on

" Brief help
" :PluginList           - lists configured plugins
" :PluginInstall        - installs plugins; append `!` to update or just :PluginUpdate
" :PluginSearch foo     - searches for foo; append `!` to refresh local cache
" :PluginClean          - confirms removal of unused plugins; append `!` to auto-approve removal
"
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" vundle end
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" basic setting start
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" encoding setting
set encoding=utf-8
set fileencodings=ucs-bom,utf-8,gb2312,gbk,gb18030,big5,latin1
set fileencoding=utf-8
set termencoding=utf-8
set langmenu=zh_CN.UTF-8
set helplang=cn
" show a preview of the first match based on the search field
set incsearch
" high light search. same as 'set hls'
set hlsearch
" shortcut to mute search highlighting
nnoremap <silent> <C-l> :<C-u>nohlsearch<CR><C-l>
" mapping for counting the last pattern
nnoremap <silent> ,* :%s///gn<CR>
" ignore case, while 'set noignorecase' option is case sensitivity
set ignorecase
" display the line number
set number
" automatic indent
set autoindent
" auto indent for C program
set cindent
" use 4 spaces for a <Tab> key
set tabstop=4 shiftwidth=4 softtabstop=4 expandtab
" set the number of command-lines history
set history=1000
" command-line completion operates in an enhanced mode
set wildmenu
" completion mode that similar to bash shell
set wildmode=list,full
" set complete-=k complete+=k
" navigate away from a modified file without first saving it.
set hidden
" enable mouse in all modes
set mouse=a
" show (partial) command in status line
set showcmd
" show matching brackets
set showmatch
" automatically save before commands like :next and :make
set autowrite
" highlight the screen line of the cursor
set cursorline
" show the line and column number of the cursor position
set ruler
" options for the status line
set statusline=%F%m%r%h%w\ [FORMAT=%{&ff}]\ [TYPE=%Y]\ [POS=%l,%v][%p%%]\ %{strftime(\"%d/%m/%y\ -\ %H:%M\")}
set laststatus=2
" number of screen lines to use for the command-line
set cmdheight=2
set magic
" prompt to save or abandon buffer
set confirm
" auto read file that has been changed
set autoread
" disable the bell for error messages
set noerrorbells

" set spell
" scan spell dictionaries for completion in addition to standard places
set complete+=k

set pastetoggle=<F4>
" add all subdirectories beneath current directory to path,
" so that we can find any file beneath current directory.
set path+=./**

" customer the grep in vim, ignore some files
" set grepprg=grep\ -nrs\ --exclude={*.o,*.patch,*.obj,*~,*.pyc,tags}\ --exclude-dir={.git,.svn}\ $*
"
" need to install ack-grep first, as:
" sudo apt-get install ack-grep
" sudo ln -s /usr/bin/ack-grep /usr/local/bin/ack
set grepprg=ack\ --nogroup\ --column\ $*\ --ignore-file=is:tags\ --ignore-file=ext:patch
set grepformat=%f:%l:%c:%m


" mapping <C-p> and <C-n> to <Up> and <Down> so that they can filter the
" command-lines history
cnoremap <C-p> <Up>
cnoremap <C-n> <Down>
cnoremap <expr> %% getcmdtype( ) == ':' ? expand('%:h').'/' : '%%'

" mapping F5 key to update the tags
nnoremap <f5> :!ctags -R<CR>

" mapping & as :&& to preserves substitute flags in normal mode
nnoremap & :&&<CR>
" mapping & as :&& to preserves substitute flags in visual mode
xnoremap & :&&<CR>

" mapping for scrolling through the buffer lists
nnoremap <silent> [b :bprevious<CR>
nnoremap <silent> ]b :bnext<CR>
nnoremap <silent> [B :bfirst<CR>
nnoremap <silent> ]B :blast<CR>
" mapping for scrolling through the args lists
nnoremap <silent> [a :aprevious<CR>
nnoremap <silent> ]a :anext<CR>
nnoremap <silent> [A :afirst<CR>
nnoremap <silent> ]A :alast<CR>
" mapping for scrolling through the quickfix lists
nnoremap <silent> [q :qprevious<CR>
nnoremap <silent> ]q :qnext<CR>
nnoremap <silent> [Q :qfirst<CR>
nnoremap <silent> ]Q :qlast<CR>
" mapping for scrolling through the location lists
nnoremap <silent> [l :lprevious<CR>
nnoremap <silent> ]l :lnext<CR>
nnoremap <silent> [L :lfirst<CR>
nnoremap <silent> ]L :llast<CR>
" mapping for scrolling through the tag lists
nnoremap <silent> [t :tprevious<CR>
nnoremap <silent> ]t :tnext<CR>
nnoremap <silent> [T :tfirst<CR>
nnoremap <silent> ]T :tlast<CR>
" mapping for scrolling through the quickfix lists
nnoremap <silent> [c :cprevious<CR>
nnoremap <silent> ]c :cnext<CR>
nnoremap <silent> [C :cfirst<CR>
nnoremap <silent> ]C :clast<CR>

" customize compile command for different file type
autocmd fileType python setlocal makeprg=python3\ %
autocmd FileType c setlocal makeprg=gcc\ -lm\ -pthread\ -g\ %\ -o\ %<
autocmd FileType cpp setlocal makeprg=g++\ -lm\ -pthread\ -g\ %\ -o\ %<

" jump to the last position when reopening a file
if has("autocmd")
    autocmd BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
endif

" enables syntax highlighting
if has("syntax")
    syntax on
endif
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" basic setting end
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" auto match start
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
function! AutoPair(open, close)
    let line = getline('.')
    if col('.') > strlen(line) || line[col('.') - 1] == ' '
        return a:open.a:close."\<ESC>i"
    else
        return a:open
    endif
endfunction

function! ClosePair(char)
    if getline('.')[col('.') - 1] == a:char
        return "\<Right>"
    else
        return a:char
    endif
endfunction

function! SamePair(char)
    let line = getline('.')
    if col('.') > strlen(line) || line[col('.') - 1] == ' '
        return a:char.a:char."\<ESC>i"
    elseif line[col('.') - 1] == a:char
        return "\<Right>"
    else
        return a:char
    endif
endfunction

inoremap ( <c-r>=AutoPair('(', ')')<CR>
inoremap ) <c-r>=ClosePair(')')<CR>
inoremap { <c-r>=AutoPair('{', '}')<CR>
inoremap } <c-r>=ClosePair('}')<CR>
inoremap [ <c-r>=AutoPair('[', ']')<CR>
inoremap ] <c-r>=ClosePair(']')<CR>
inoremap < <c-r>=AutoPair('<', '>')<CR>
inoremap > <c-r>=ClosePair('>')<CR>
inoremap " <c-r>=SamePair('"')<CR>
inoremap ' <c-r>=SamePair("'")<CR>
inoremap ` <c-r>=SamePair('`')<CR>

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" auto match end
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Tagbar
let g:tagbar_width=35
let g:tagbar_autofocus=1
let g:tagbar_left = 1
nmap <F3> :TagbarToggle<CR>

