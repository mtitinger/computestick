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

> dyndbg="file suspend.c +p"


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

### generate the config file for wpa_supplicant ###

$ wpa_passphrase myrouter > wpa.conf
mypassphrase

You should end up with a file looking like this:

network={
    ssid="myrouter"
    #psk="mypassphrase"
    psk=8ada1f8dbea59704ac379538b4d9191f6a72390581b4cd7a72864cea685b1a7f
}

wpa_supplicant -D nl80211,wext -i wlan0 -d -c/etc/wpa_supplicant.conf


## Enabling wowlan ##

> iw phy0 wowlan enable any

> echo mem > /sys/power/state

Tips for using 'iw', see http://processors.wiki.ti.com/index.php/WoWLAN_-_Linux

## Misc ##

sudo hdparm -W 0 /dev/sdc
echo +10 > /sys/class/rtc/rtc0/wakealarm
cat /proc/interrupts | grep rtc0

## loaded modules with XXXX ##

nfc                    66887  0
bluetooth             342121  2
usb_f_mtp              11842  2
uio                     9845  0
libcomposite           50894  10 usb_f_mtp
iwlwifi               115692  0
configfs               25298  3 usb_f_mtp,libcomposite
axp288_fuel_gauge      12638  0
axp20x_pek              3690  0
rtl8723bs             763604  0
axp288_adc              3739  0
industrialio           49103  2 axp288_adc,axp288_fuel_gauge
i915                 1097887  0
cfg80211              548959  2 iwlwifi,rtl8723bs
rfkill                 13261  5 nfc,cfg80211,bluetooth
drm_kms_helper        106820  1 i915
efivars                 9661  0
dw_dmac                 2819  0
dw_dmac_core           16675  1 dw_dmac
video                  14883  1 i915
i2c_designware_platform     7248  1
spi_pxa2xx_platform    16350  0
