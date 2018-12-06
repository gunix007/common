" Nocompatible mode, but not the vi compatible mode. Same as 'set nocp'
set nocompatible
" Enable filetype detection and filetype plugins/indent
filetype plugin indent on
" High light search. Same as 'set hls'
set hlsearch
" show a preview of the first match based on what has been entered
" so far into the search field.
set incsearch
" Ignore case, while 'set noignorecase' option is case sensitivity
set ignorecase
" Display the line number
set number
" Automatic indent
set autoindent
" Use 4 spaces for a <Tab> key
set tabstop=4 shiftwidth=4 softtabstop=4 expandtab
" Set the number of command-lines history
set history=1000
" command-line completion operates in an enhanced mode
set wildmenu
" Completion mode that similar to bash shell
set wildmode=list,full
" set complete-=k complete+=k
set hidden
" enable mouse in all modes
" set mouse=a

" set spell
" Scan spell dictionaries for completion in addition to standard places
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


