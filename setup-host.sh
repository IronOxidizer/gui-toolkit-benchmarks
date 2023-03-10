#!/bin/sh

if [ `whoami` != 'root' ];
then
    echo "This program needs to be run using 'sudo'"
    exit
fi

apt-get update && apt-get upgrade

# Install dependencies
apt-get install --no-install-recommends ca-certificates curl gnupg lsb-release xorg openbox dbus-user-session

# Install docker
mkdir -m 0755 -p /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/debian/gpg | gpg --dearmor -o /etc/apt/keyrings/docker.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/debian $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list > /dev/null
apt-get update
apt-get install --no-install-recommends docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

# Return to normal user
su "$(logname)"

# Setup and start window manager
echo 'exec openbox' > ~/.xinitrc
startx