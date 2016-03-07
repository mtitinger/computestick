#!/bin/sh
set -x

echo 0 > /sys/power/pm_async

echo +10 > /sys/class/rtc/rtc0/wakealarm
echo freeze > /sys/power/state
