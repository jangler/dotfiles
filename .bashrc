#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

alias ls='ls --color=auto'
PS1='[\u@\h \W]\$ '

function ffsplit () {
	ffmpeg -ss "$3" -i "$1" -t "$4" -c:a copy -c:v copy "$2"
}

function ffjoin () {
	rm -f inputs.txt
	for f in *.mkv; do
		echo "file '$f'" >> inputs.txt
	done
	ffmpeg -f concat -i inputs.txt -c copy "$1"
}

export GOPATH=/home/brandon
export PATH=$PATH:~/bin
