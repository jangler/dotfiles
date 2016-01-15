#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

# prompt
PS1="\[\e[1;32m\]\u@\h \[\e[1;34m\]\w \$ \[\e[0m\]"

# aliases
alias ls='ls --color=auto'
alias timidty='timidity --reverb=0'
alias vim='vim -p'

# colored man pages
man() {
	env LESS_TERMCAP_mb=$'\e[0m' \
	LESS_TERMCAP_md=$'\e[1;37m' \
	LESS_TERMCAP_me=$'\e[0m' \
	LESS_TERMCAP_se=$'\e[0m' \
	LESS_TERMCAP_so=$'\e[0m' \
	LESS_TERMCAP_ue=$'\e[0m' \
	LESS_TERMCAP_us=$'\e[1;36m' \
	man "$@"
}
