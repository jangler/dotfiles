set ts=4 sw=0 sts=-1

call pathogen#infect()
call pathogen#helptags()
let g:neocomplete#enable_at_startup = 1
let g:pymode_folding = 0

colorscheme Tomorrow-Night

au Filetype c set ts=8
au FileType css set et ts=2
au FileType html set et ts=2
au FileType javascript set et
au FileType json set et ts=2
au FileType lua set et ts=2
au FileType python set et
au FileType ruby set et ts=2
au FileType svg set et ts=2
au FileType xml set et ts=2

map ! :!
