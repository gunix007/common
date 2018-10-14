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


cnoremap <C-p> <Up>
cnoremap <C-n> <Down>

