" Nocompatible mode, but not the vi compatible mode. Same as 'set nocp'
set nocompatible
" Enable filetype detection and filetype plugins/indent
filetype plugin indent on
" High light search. Same as 'set hls'
set hlsearch
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
" Completion mode that similar to bash shell
set wildmode=longest,list

set hidden

" mapping <C-p> and <C-n> to <Up> and <Down> so that they can filter the
" command-lines history
cnoremap <C-p> <Up>
cnoremap <C-n> <Down>

" mapping F5 key to update the tags
nnoremap <f5> :!ctags -R<CR>
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

autocmd fileType python setlocal makeprg=python3\ %
autocmd FileType c setlocal makeprg=gcc\ -lm\ -pthread\ -g\ %\ -o\ %<
autocmd FileType cpp setlocal makeprg=g++\ -lm\ -pthread\ -g\ %\ -o\ %<

