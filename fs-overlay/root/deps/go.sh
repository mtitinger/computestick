#!/bin/sh

export LD_LIBRARY_PATH=`pwd`

./powertop --debug -r result.htm -q -t 30

