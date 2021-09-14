#!/bin/bash
#Author: Kuan-Ju Chen
#Date: 2021/04/12
#ID: 0711529
#Describe:
#		To open the ip_forward function
#		We can use "cat /proc/sys/net/ipv4/ip_forward" to check 
echo 1 >  /proc/sys/net/ipv4/ip_forward