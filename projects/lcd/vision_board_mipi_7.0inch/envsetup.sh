#!/bin/bash

export RTT_ROOT=${PWD}/rt-thread
export ENV_ROOT=${HOME}/.env
export BSP_ROOT=${PWD}
export RTT_CC='gcc'
export RTT_EXEC_PATH='/usr/bin'

rtt_dir='../../../rt-thread'
lib_dir='../../../libraries'

if [ ! -L "rt-thread" ]; then
	if [ -d $rtt_dir ]; then
		ln -s $rtt_dir ./rt-thread
	fi
fi

if [ ! -L "libraries" ]; then
	if [ -d $lib_dir ]; then
		ln -s $lib_dir ./libraries
	fi
fi
