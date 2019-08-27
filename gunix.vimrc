""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" vundle start
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" nocompatible mode, but not the vi compatible mode
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
Plugin 'tpope/vim-surround'
Plugin 'tpope/vim-commentary'
Plugin 'tpope/Vim-repeat'
Plugin 'scrooloose/nerdtree'
" Plugin 'jistr/vim-nerdtree-tabs'
Plugin 'Xuyuanp/nerdtree-git-plugin'
Plugin 'majutsushi/tagbar'
Plugin 'ctrlpvim/ctrlp.vim'
Plugin 'Valloric/YouCompleteMe'
Plugin 'altercation/vim-colors-solarized'
Plugin 'bronson/Vim-trailing-whitespace'

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
" map the leader key and remap the revert character search command
let mapleader=","
noremap \ ,
" map shortcut key for system clipboard in visual mode
vnoremap <Leader>y "+y
nmap <Leader>p "+p
" split current window in two horizontally
nnoremap <Leader>ws <C-W>s
" split current window in two vertically
nnoremap <Leader>wv <C-W>v
" jump to the next window
nnoremap <Leader>ww <C-W>w
" jump to the left window
nnoremap <Leader>wh <C-W>h
" jump to the right window
nnoremap <Leader>wl <C-W>l
" jump to the below window
nnoremap <Leader>wj <C-W>j
" jump to the above window
nnoremap <Leader>wk <C-W>k
" close the current window
nnoremap <Leader>wc <C-W>c
" make the current window the only one on the screen
nnoremap <Leader>wo <C-W>o
" jump to the match item
nmap <Leader>m %
" do not create backup file
set nobackup
" do not create swap file
set noswapfile
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
" mini number of screen lines to keep above and below the cursor
set scrolloff=3
" shortcut to mute search highlighting
nnoremap <silent> <C-l> :<C-u>nohlsearch<CR><C-l>
" mapping for counting the last pattern
nnoremap <silent> <Leader>* :%s///gn<CR>
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
set history=256
" command-line completion operates in an enhanced mode
set wildmenu
" completion mode that similar to bash shell
set wildmode=list,full
" set complete-=k complete+=k
" navigate away from a modified file without first saving it.
set hidden
" enable mouse in all modes
" set mouse=a
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
" the silver searcher
if executable('ag')
    " use ag over grep
    set grepprg=ag\ --nogroup\ --nocolor
    " use ag in ctrlp for listing files. lightning fast and respects .gitignore
    let g:ctrlp_user_command = 'ag %s -l --nocolor -g ""'
    " ag is fast enough that ctrlp doesn't need to cache
    let g:ctrlp_use_caching = 0
endif

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

" imformation from: help e526
set viminfo='50,<1000
" '50       Marks will be remembered for the last 50 files you edited.
" <1000     Contents of registers (up to 1000 lines each) will be remembered.

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

" auto sources vimrc when save it
if has("autocmd")
    autocmd BufWritePost .vimrc source %
endif

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
function! ClosePair(char)
    if getline('.')[col('.') - 1] == a:char
        return "\<Right>"
    else
        return a:char
    endif
endfunction

inoremap ( ()<ESC>i
inoremap ) <c-r>=ClosePair(')')<CR>
inoremap { {<CR>}<ESC>O
inoremap } <c-r>=ClosePair('}')<CR>
inoremap [ []<ESC>i
inoremap ] <c-r>=ClosePair(']')<CR>
inoremap < <><ESC>i
inoremap > <c-r>=ClosePair('>')<CR>
inoremap " ""<ESC>i
inoremap ' ''<ESC>i
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" auto match end
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" nerdtree config start
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" set the size of the nerd tree
" let NERDTreeWinSize=20
" set the position of nerd tree
" let NERDTreeWinPos="right"
" open NERDTree automatically when vim starts up
" autocmd vimenter * NERDTree
" open NERDTree automatically when vim starts up if no files were specified
" autocmd StdinReadPre * let s:std_in=1
" autocmd VimEnter * if argc() == 0 && !exists("s:std_in") | NERDTree | endif
" open NERDTree automatically when vim starts up on opening a directory
autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * if argc() == 1 && isdirectory(argv()[0]) && !exists("s:std_in") | exe 'NERDTree' argv()[0] | wincmd p | ene | endif
" map <F2> to open NERDTree
map <F2> :NERDTreeToggle<CR>
" close vim if the only window left open is a NERDTree
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTree") && b:NERDTree.isTabTree()) | q | endif
" show line number
let NERDTreeShowLineNumbers=1
" ignore files
let NERDTreeIgnore=['\~$','\.pyc$','\.swp$']
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" nerdtree config end
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" tagbar config start
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
let g:tagbar_width=31
nmap <F3> :TagbarToggle<CR>
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" tagbar config end
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" ctrlp config start
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Change the default mapping and the default command to invoke CtrlP
let g:ctrlp_map = '<c-p>'
let g:ctrlp_cmd = 'CtrlP'
" set local working directory when starting without directory
let g:ctrlp_working_path_mode = 'ra'
" ignore option
set wildignore+=*/tmp/*,*.so,*.swp,*.zip
let g:ctrlp_custom_ignore = {
    \ 'dir':  '\v[\/]\.(git|hg|svn)$',
    \ 'file': '\v\.(exe|so|dll)$',
    \ 'link': 'SOME_BAD_SYMBOLIC_LINKS',
    \ }
" customize file listing command
" let g:ctrlp_user_command = 'find %s -type f'
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" ctrlp config end
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" vim-commentary config start
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
autocmd FileType python,shell set commentstring=#\ %s
autocmd FileType java,c,cpp set commentstring=//\ %s
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" vim-commentary config end
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" vim-colors-solarized config start
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
syntax enable
set background=dark
" colorscheme solarized
" colorscheme desert
" colorscheme koehler
" colorscheme industry
colorscheme elflord
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" vim-colors-solarized config end
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" Vim-repeat config start
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
silent! call repeat#set("\<Plug>MyWonderfulMap", v:count)
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" Vim-repeat config end
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" youcompleteme config start
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
let g:ycm_server_python_interpreter = '/usr/bin/python'
let g:ycm_key_list_select_completion = ['<c-n>', '<Down>']
let g:ycm_key_list_previous_completion = ['<c-p>', '<Up>']
let g:ycm_global_ycm_extra_conf = '~/.vim/bundle/YouCompleteMe/third_party/ycmd/.ycm_extra_conf.py'
" turn off/on YCM's diagnostic display features
let g:ycm_show_diagnostics_ui = 0
" show the completion menu even when typing inside comments.
" let g:ycm_complete_in_comments = 1
" don't show the confirm when loading config
" g:ycm_confirm_extra_conf = 0
" collect identifiers from tags files
" let g:ycm_collect_identifiers_from_tags_files = 1
" seed identifier database with the keywords of the programming language
" let g:ycm_seed_identifiers_with_syntax = 1
" nnoremap <leader>gl :YcmCompleter GoToDeclaration<CR>
" nnoremap <leader>gf :YcmCompleter GoToDefinition<CR>
" nnoremap <leader>gg :YcmCompleter GoToDefinitionElseDeclaration<CR>
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""" youcompleteme config end
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

