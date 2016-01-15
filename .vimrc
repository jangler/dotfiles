" pathogen
execute pathogen#infect()

" vim-sensible stuff
filetype plugin indent on
set autoindent
set autoread
set backspace=indent,eol,start
set display+=lastline
set encoding=utf-8
set formatoptions+=j
set hlsearch
set incsearch
set laststatus=2
set nrformats-=octal
set ruler
set scrolloff=1
set sessionoptions-=options
set smarttab
set t_Co=16
set viminfo=
set wildmenu
syntax enable

" custom
colorscheme reset
map ! :!
set noswf
set ts=8 sw=0 sts=-1 noet
set tw=79

" filetypes
au filetype css set et ts=2
au filetype go set ts=4
au filetype html set et ts=2
au filetype json set et ts=2
au filetype lua set et ts=2
au filetype python set et ts=4
au filetype ruby set et ts=2
au filetype svg set et ts=2
au filetype xml set et ts=2
