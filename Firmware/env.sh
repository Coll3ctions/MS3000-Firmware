#!/bin/sh
alias flashit="python ../../esptool/esptool.py -p /dev/ttyUSB0 write_flash 0x00 /tmp/MS3KOS_BUILD/MS3KOS.bin"
