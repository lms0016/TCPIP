#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define PortNumber 5555
#define Address "10.0.2.15"

int main(int argc, char *argv[])
{
	clock_t start_time, end_time;
	int input = atoi(argv[1]);
	struct sockaddr_in address;
	int sock, byte_sent, address_length = sizeof(address);
	char buffer[12] = "hello world\0";
	char completed[2] = "1\0";
	int i;
	int total_bits = 12*8*input+2;

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		printf("Error creating socket\n");	
	}

	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(PortNumber);
	address.sin_addr.s_addr = inet_addr(Address);

	printf("Server IP : %s\n", Address);
	start_time = clock();
	for( i=0 ; i<input ; i++ )
	{	
		byte_sent = sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&address, address_length);		
		if(byte_sent < 0)
		{
			printf("Error sending packet\n");
		}
	}
	if(i == input)
	{
		byte_sent = sendto(sock, completed, sizeof(completed), 0, (struct sockaddr *)&address, address_length);
		if(byte_sent < 0)
		{
			printf("Error sending packet\n");
		}
	}
	

	end_time = clock();

	printf("Datagram number : %d\n",i);
	printf("Time Interval : %f sec\n",(float)(end_time - start_time)/CLOCKS_PER_SEC);
	printf("Throughput = %f Mbps\n", total_bits/((float)(end_time - start_time)/CLOCKS_PER_SEC)/1000000);	

	close(sock);
	return 0;
}
