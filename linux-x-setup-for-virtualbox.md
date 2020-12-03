# Linux X setup for VirtualBox

Note: The same process is valid for bare metal benchmarking except there's no need for vm setup or dkms. Disk shrinking/trimming/defrag can still be valueable when making a minimal ISO for benchmarking quickly.

1. Download and install VirtualBox
2. Download [Debian NetInst](https://www.debian.org/CD/netinst/)
3. Download any other [live boot Distro](https://www.debian.org/CD/live/#live-install-stable)
4. Set VirtualBox prefered resolution
  - File > Preferences > Display > Maximum Guest Screen Size > Hint
  - Set Width to 1920 and Height to 1080
5. Create a new virtual machine
  - 4096MB RAM
  - 8GB fixed size (pre-allocated) disk
6. Configure the virutal machine (right click > Settings)
  - System > Motherboard > disable Floppy
  - System > Motherboard > set cores to 4
  - Display > Screen > set Video Memory to 128MB
  - Display > Screen > Set Graphics Controller to VBoxSVGA
  - Display > Screen > Enable 3D Acceleration
  