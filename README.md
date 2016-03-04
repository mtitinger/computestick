# ComputeStick Debug #

## Buildroot / Serial console debug setup ##

* Kernel command Line setup with grub.cfg

```
set default="0"
set timeout="5"

menuentry "pm-debug" {
        linux /bzImage console=ttyS0,115200n8 root=/dev/mmcblk1p2 nomodeset ignore_loglevel no_console_suspend rootdelay=10
}
```

* In buildroot, setup BR2_TARGET_GRUB2_BUILTIN_CONFIG to reference the custom grub.cfg in this git.
* Also see https://01.org/blogs/rzhang/2015/best-practice-debug-linux-suspend/hibernate-issues (thanks Frode)

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

> https://github.com/hadess/rtl8723bs

## Enabling wowlan ##

> iw phy0 wowlan enable any

> echo mem > /sys/power/state

Tips for using 'iw', see http://processors.wiki.ti.com/index.php/WoWLAN_-_Linux

## Misc ##

sudo hdparm -W 0 /dev/sdc
