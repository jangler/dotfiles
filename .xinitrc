# xorg settings
setxkbmap us altgr-intl -option compose:caps
xrandr --output HDMI-0 --mode 1280x720
xset s off -dpms
xsetwacom --set "Wacom Intuos S 2 Pen pad" MapToOutput HEAD-0
xsetwacom --set "Wacom Intuos S 2 Pen stylus" MapToOutput HEAD-0

# background processes
redshift &
ibus-daemon &

# wm
exec awesome
