#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define PortNumber 5555

int main(int argc, char *argv[])
{
	clock_t start_time, end_time;
	struct sockaddr_in address;
	int sock, byte_recv, address_length = sizeof(address);
	char buffer[50];
	int count = 1;
	int start = 0;	

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		printf("Error creating socket\n");
	}
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(PortNumber);
	address.sin_addr.s_addr = INADDR_ANY;

	if(bind(sock, (struct sockaddr *)&address, sizeof(address)) == -1)
	{
		printf("error binding\n");
		close(sock);
	}
	for(;;)
	{	
		byte_recv = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&address, &address_length);
		if(byte_recv < 0)
		{
			printf("Error recving packet\n");		
		}

		if(strcmp(buffer,"1") == 0)
		{	
			int total_bits = 12*8*count+2;
			printf("Client send completed!\n");
			printf("Datagram number : %d\n",count-1);
			end_time = clock();
			printf("Time Interval : %f sec\n",(float)(end_time - start_time)/CLOCKS_PER_SEC);
			printf("Throughput = %f Mbps\n", total_bits/((float)(end_time - start_time)/CLOCKS_PER_SEC)/10000000);			

			count = 1;
			start = 0;
		}else{
			if(start == 0);
			{
				start_time = clock();
				start = 1;
			}
			printf("Datagram %d: %s\n", count, buffer);
			count++;
		}


	}
	
	return 0;
}
