#include <stdio.h>
#include <stdlib.h>
#include <string.h>	//for bzero()
#include <unistd.h>	//for close()
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
	struct sockaddr_in server_addr;
	int sock, byte_sent, server_addr_length = sizeof(server_addr);
	char buffer[13] = "hello world!\0";

	int i;
	int total_bits = 13*8*input;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		printf("Error creating socket\n");
	}

	bzero(&server_addr, server_addr_length);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PortNumber);
	server_addr.sin_addr.s_addr = inet_addr(Address);

	if(connect(sock, (struct sockaddr *)&server_addr, server_addr_length) == -1 )
	{
		printf("connect failed!");
		close(sock);
	}	

	start_time = clock();
	for( i=0 ; i<input ; i++ )
	{	
		byte_sent = send(sock, buffer, sizeof(buffer), 0);
		if(byte_sent < 0){
			printf("Error sending packet\n");
		}
		sleep(1);
	}
	end_time = clock();	
	printf("Server IP : %s\n", Address);
	printf("Datagram number : %d\n", input);
	printf("Time Interval : %f sec\n",((float)(end_time - start_time)/CLOCKS_PER_SEC)+(float)input);
	printf("Throughput = %f Mbps\n", total_bits/((float)(end_time - start_time)/CLOCKS_PER_SEC)/1000000);

	close(sock);
	return 0;
}

