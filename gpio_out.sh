#!/bin/sh

if [ $# != 2 ]; then
    echo "Usage: $0 gpio_num value"
    echo "    gpio_num: 0~31, value: 0-Low/1-High"
    echo "    e.g.: $0 15 1     set GPIO15=1"
	exit 1
fi

if [ $2 -ne 0 -a $2 -ne 1 ]; then
    echo "value should be 0 or 1"
    exit 1
fi

echo $1 > /sys/class/gpio/export
# Set Output Mode
echo out > /sys/class/gpio/gpio$1/direction
# Set Value
echo $2 > /sys/class/gpio/gpio$1/value

echo GPIO$1 $(cat /sys/class/gpio/gpio$1/direction) $(cat /sys/class/gpio/gpio$1/value)

echo $1 > /sys/class/gpio/unexport

