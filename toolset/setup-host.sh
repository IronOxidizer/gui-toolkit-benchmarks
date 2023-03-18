#!/bin/sh

if [ `whoami` != 'root' ];
then
    echo "This program needs to be run using 'sudo'"
    exit
fi

apt-get update && apt-get upgrade

# Install dependencies
apt-get install --no-install-recommends -y curl gnupg lsb-release xorg openbox dbus-user-session python3-pip

# Install docker
mkdir -m 0755 -p /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/debian/gpg | gpg --dearmor -o /etc/apt/keyrings/docker.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/debian $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list > /dev/null
apt-get update
apt-get install --no-install-recommends -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin

# Setup window manager
echo 'exec openbox' > "/home/$(logname)/.xinitrc"
echo 'if [[ -z $DISPLAY ]] && [[ $(tty) = /dev/tty1 ]]; then\n  startx\nfi' >> "/home/$(logname)/.profile"
chown "$(logname)":"$(logname)" "/home/$(logname)/.xinitrc" "/home/$(logname)/.profile"

echo 'Setup is complete, rebooting in 3 seconds'
sleep 3
reboot
