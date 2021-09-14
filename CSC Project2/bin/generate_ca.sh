#!/bin/bash 
#Author: Kuan-Ju Chen
#Date: 2021/04/12
#ID: 0711529
#Describe:                                                            
#To generate the certificate                                           
#Output:                                                         
#		- ca.key                                                
#		- ca.crt                                                
#			this file is going to install at the victim's host            
# 			transfer to victim: scp <PATH>/ca.crt cs2021@<IP address>:~/Desktop/
#                                                                       
#How to instll certificate in victim                                   
#Step with command:                                              
#		cd /usr/local/share/ca-certificates/                                                    
# 		suod cp <PATH>/ca.crt /usr/local/share/ca-certificates/ca.crt
# 		sudo chmod 755 /usr/local/share/ca-certificates/        
# 		sudo chmod 644 /usr/local/share/ca-certificates/ca.crt  
# 		sudo update-ca-certificates                                                                                       
openssl genrsa -out ca.key 4096                                       
openssl req -new -x509 -days 1826 -key ca.key -out ca.crt