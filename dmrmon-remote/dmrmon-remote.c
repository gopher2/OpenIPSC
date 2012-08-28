/*
dmrmon-remote - monitor DMR repeater and send to server 
Copyright (C) 2012 David Kierzokwski (kd8eyf@digitalham.info)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "/usr/include/pcap/pcap.h" 	// NEED TO FIX THIS
#include<sys/socket.h>
#include<arpa/inet.h>
#include<net/ethernet.h>
#include<netinet/udp.h>
#include<netinet/ip.h>
#include<getopt.h>
struct UDP_hdr {
	u_short	uh_sport;		/* source port */
	u_short	uh_dport;		/* destination port */
	u_short	uh_ulen;		/* datagram length */
	u_short	uh_sum;			/* datagram checksum */
};
int  debug = 0;
char *devname = NULL;

void usage( int8_t e );
void processPacket(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char * packet)
{
	struct ip * ip;
	struct UDP_hdr * udp;
	unsigned int IP_header_length;	
	unsigned int capture_len = pkthdr->len;
	unsigned int SourceID;
	unsigned int PacketType;
	unsigned int UserID;
	int i=0, *counter = (int *)arg;
	
	packet += sizeof (struct ether_header);
	capture_len -= sizeof(struct ether_header);
	ip = (struct ip*) packet;
	IP_header_length = ip->ip_hl *4;
	packet += IP_header_length;
	capture_len -= IP_header_length;	
	udp = (struct UDP_hdr*) packet;
	packet += sizeof (struct UDP_hdr);
	if (	
	if (debug == 1){
		printf("Packet Count:\t%d\n", ++(*counter));	
		printf("Packet Size:\t%d\n", capture_len);
		printf("Size ETH Header\t%lu\n",sizeof(struct ether_header));
		printf("Size IP Header\t%u\n",IP_header_length);
		printf("UDP Src Port\t%d\n", ntohs(udp->uh_sport));
		printf("IDP Dst Port\t%d\n", ntohs(udp->uh_dport));
	}
        for (i=0; i<capture_len; i++) {
	        printf("%x ", packet[i]);
	
}                        
printf("\n");
        return;
}

int main(int argc, char *argv[] )
{
	char packet_filter[] = "ip and udp";
	struct bpf_program fcode;
	u_int netmask;
	pcap_t *descr = NULL;
        int32_t c;
        while ((c = getopt(argc, argv, "dVhi:")) != EOF) {
                switch (c) {
                case 'd': 
			debug = 1;
			break;
		case 'V':
                        version();
                        break;
                case 'i':
                        devname = optarg;
                        break;
                case 'h':
                        usage(-1);
                        break;
                }
        }
        if (devname == NULL) {
                usage(-1);
        }
        if (debug == 1) {
		 printf("USING CAPTURE DEVICE: %s\n", devname); }

        pcap_if_t *alldevsp , *device;
        pcap_t *handle;
        char errbuf[100] , devs[100][100];
        int count = 1 , n;
        handle = pcap_open_live(devname , 65536 , 1 , 0 , errbuf);
        if (handle == NULL) {
                fprintf(stderr, "Couldn't open device %s : %s\n" , devname , errbuf);
                exit(1);
        }
        pcap_compile(handle, &fcode, packet_filter, 1, netmask);

	if ( pcap_loop(handle, -1, processPacket, (u_char *)&count) == -1) {
                fprintf(stderr, "ERROR: %s\n", pcap_geterr(descr) );
                exit(1);
        }


        return 0;
}
void usage(int8_t e)
{
        printf(	"Usage: dmrmon-remotei [OPTION]... [REMOTE SERVER]...\n"
		"Listen send DMR data for remote server for processing\n" 
		"\n"
		"   -i, --interface	Interface to listen on\n"
		"   -h, --help		This Help\n"
		"   -V, --version	Version Information\n"
		"   -d, --debug		Show verbose information\n"
		"\n"
		"With no REMOTE SERVER or REMOTE SERVER is -, output to standard output\n"
		"\n"
		"Examples:\n"
		"   dmrmon-remote -i eth0 192.168.10.20 50000\n"	
		"   Send the DMR data heard on eth0 to remote server\n"      
		"\n"
		"Report cat bugs to kd8eyf@digitalham.info\n");
        exit(e);
}

int version ( void )
{
        printf ("dmrmon 0.01\n");
        exit(1);
}
