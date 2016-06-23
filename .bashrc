#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

# prompt
PS1="\[\e[1;32m\]\u@\h \[\e[1;34m\]\W \$ \[\e[0m\]"

# aliases
alias ls='ls --color=auto'
alias timidty='timidity --reverb=0'
alias vim='vim -p'
alias gambatte='gambatte_sdl -v 2 -y'
alias tvmode='xrandr --output HDMI-0 --right-of VGA-0 --mode'
alias tvoff='xrandr --output HDMI-0 --off'

# START SELECT A B UP DOWN LEFT RIGHT
alias gbc='gambatte_sdl -i js0b7 js0b6 js0b0 js0b1 js0h0u js0h0d js0h0l js0h0r -v 2 -y'

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

# audio level adjustment
level() {
	in="$1"
	out="${in%.wav}.flac"
	if [ $(echo "$2" | cut -c 1) == '-' ]; then
		sox "$in" "$out" vol "$2" dB gain -1
	else
		sox "$in" "$out" vol "$2" dB 0.05 gain -1
	fi
	m=$(loudness dump -m 0.2 "$out" | sort -rn | head -n 1)
	s=$(loudness dump -s 1.5 "$out" | sort -rn | head -n 1)
	echo "($m + $s) * 0.5" | bc
}

mp3ify() {
	sox "$1" -C 320 "${1%.flac}.mp3"
}

pklev() {
	sox "$1" -n stats 2>&1 | grep 'Pk lev'
}

upload() {
	resp=$(postfile -echo http://lightcones.net/submit "$1")
	echo -n "http://lightcones.net"
	echo "$resp" | grep -o 'href=/tmp/[^>]\+' | sed 's/href=//'
}
