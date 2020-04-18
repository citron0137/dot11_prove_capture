#!/bin/bash

if [ $# -ne 1 ];then
	echo "usage : $0 <network_interface>"
	exit 1
fi

sudo ifconfig $1 down 
sudo ip link set $1 name mon0
sudo iwconfig mon0 mode monitor
sudo ifconfig mon0 up 
