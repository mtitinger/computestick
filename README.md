# ComputeStick Debug #

## Buildroot / Serial console debug setup ##

Kernel command Line override:

CONFIG_CMDLINE="console=ttyS0,115200n8 root=/dev/mmcblk1p2 nomodeset acpi_enforce_resources=lax debug initcall_debug ignore_loglevel no_console_suspend"

Also see https://01.org/blogs/rzhang/2015/best-practice-debug-linux-suspend/hibernate-issues (thanks Frode)

## Create SDCARD with EFI partition ##

using gdisk (not fdisk)

* create a new empty GUID partition table (GPT)
* create EFI partition 34 MiB   => will be mmcblk1p1 for the kernel
* create ext3 partition		=> will be mmcblk1p2 for the kernel

You may set buildroot to compile the kernel from the kernel_source, using the symlinked .config

* make -C buildroot
* make -C kernel_source

insert the prepared SDCard

* copy buildroot/output/images/efi_part/* to EFI partition
* copy buildroot/output/images/bzimage to EFI parition /boot (or according to buildroot settings)


## Wifi Driver ##

Installing the RTL8723BS Wifi device

> https://github.com/hadess/rtl8723bs



A working internet connection is required for the first two steps below:

    sudo apt-get install build-essential linux-headers-generic git
    git clone https://github.com/codeTom/rtl8723bs.git
    cd rtl8723bs
    make
    sudo make install
    sudo depmod -a
    sudo modprobe r8723bs

## Enabling wowlan ##

> iw phy0 wowlan enable any

> echo mem > /sys/power/state

Tips for using 'iw', see http://processors.wiki.ti.com/index.php/WoWLAN_-_Linux

## Misc ##

sudo hdparm -W 0 /dev/sdc
