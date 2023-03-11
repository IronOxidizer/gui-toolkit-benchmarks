1. use the latest Debian net inst from https://www.debian.org/CD/netinst/
1. username: user
1. guided, use entire partition or disk (beware, this will wipe your disk)
1. all files in one partition
1. software selection
    - no desktop environment
    - no window manager
    - no web or ssh server
    - no standard system utilities
    - nothing
1. reboot
1. run the following commands
    ```
    # login to root
    apt install -y --no-install-recommends sudo git ca-certificates 
	groupadd docker
    usermod -aG sudo,video,docker user
    exit
	# login to user
    git clone https://github.com/IronOxidizer/gui-toolkit-benchmarks.git
    cd gui-toolkit-benchmarks
    sudo ./setup-host.sh
    ```
