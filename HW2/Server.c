#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for bzero()
#include <unistd.h>	//for close()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define PortNumber 5555

int main(int argc, char *argv[])
{	
	clock_t start_time, end_time;
	struct sockaddr_in server_addr, client_addr;
	int sock, byte_recv, server_addr_length = sizeof(server_addr), client_addr_length = sizeof(client_addr), recfd;
	char buffer[50];
	int i;
	int count = 1;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		printf("Error creating socket\n");
	}

	bzero(&server_addr, server_addr_length);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PortNumber);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sock, (struct sockaddr *)&server_addr, server_addr_length) == -1)
	{
		printf("error binding\n");
		close(sock);
	}
	if(listen(sock, 20) == -1)
	{
		printf("listen failed!\n");
		close(sock);
	}

	for(;;)
	{
		recfd = accept(sock, (struct sockaddr *)&client_addr, &client_addr_length);
		if(recfd == -1)
		{
			printf("accept failed\n");
			close(sock);
		}

		byte_recv = recv(recfd, buffer, sizeof(buffer), 0);

		while(byte_recv)
		{	
			if (count == 1)
			{
				start_time = clock();
			}
			if(byte_recv < 0){
				printf("Error recving packet\n");				
			}
			printf("Datagram %d: %s\n",count, buffer);			
			byte_recv = 0;	
			byte_recv = recv(recfd, buffer, sizeof(buffer), 0);
			count++;
		}
		end_time = clock();
		int total_bits = 13*8*count;		

		printf("Client send completed!\n");
		printf("Datagram number : %d\n",count-1);		
		printf("Time Interval : %f sec\n",((float)(end_time - start_time)/CLOCKS_PER_SEC) + (float)count);
		printf("Throughput = %f Mbps\n", total_bits/((float)(end_time - start_time)/CLOCKS_PER_SEC)/1000000);

		count = 1;
	}
	return 0;
}
