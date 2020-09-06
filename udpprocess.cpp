#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;
int *ports;
int *arrSize;

void error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void *udpserver(void *vargp)
{
     int sockfd;
     sockfd = socket(AF_INET,SOCK_DGRAM,0);
     struct sockaddr_in serv,client;
     serv.sin_family = AF_INET;
     serv.sin_addr.s_addr = INADDR_ANY;
     serv.sin_port = htons(1000);
     bind(sockfd,(struct sockaddr *)&serv,sizeof(serv));
     char buffer[1000];
     socklen_t l = sizeof(client);
     //socklen_t m = client;
     cout<<"\ngoing to recv\n";
     while(true)
     {
     int rc= recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&client,&l);
     if(rc<0)
     {
     cout<<"ERROR READING FROM SOCKET";
     }
     cout<<"\n the message received is : "<<buffer<<endl;
     int rp= sendto(sockfd,"message received dude",2,0,(struct sockaddr *)&client,l);
     if(rp<0)
     {
     cout<<"ERROR writing to SOCKET";
     }
     }
}

void *udpclient(void *vargp)
{
    int sockfd;
     sockfd = socket(AF_INET,SOCK_DGRAM,0);
     struct sockaddr_in serv,client;
     serv.sin_family = AF_INET;
     serv.sin_addr.s_addr = INADDR_ANY;
     char buffer[256];
     cout<< ports[1];
     for(int i=0;i<*arrSize;i++)
     {
     serv.sin_port = htons(ports[i]);
    //  cout << ports[i];
    //  cout << serv.sin_port;
     socklen_t l = sizeof(client);
     socklen_t m = sizeof(serv);
     cout<<"\ngoing to send\n";
     sendto(sockfd,"Hi dude",sizeof(buffer),0,(struct sockaddr *)&serv,m);
     }
    //  recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&serv,&m);
    //   cout<<"\n Server says : "<<buffer<<endl;
}


int* getPorts(int *ports, int *arrSize)
{
    char const* fileName = "./data";//argv[1]; /* should check that argc > 1 */
    FILE* file = fopen(fileName, "r"); /* should check the result */
    char line[256];
    //int arr[100];
    int size_of_array = 0;
    int i;

    while (fgets(line, sizeof(line), file)) {
    	int num = atoi(line);
    	ports[size_of_array++] = num;
    }

    // for(i=0; i<size_of_array; i++) {
    // 	printf("Number %d : %d\n", i, ports[i]);
    // }
    *arrSize = size_of_array;
    fclose(file);
    return ports;
} 


int main()
{
    ports = new int [100];
    arrSize = new int;
    getPorts(ports,arrSize);
    cout << *arrSize;
    pthread_t server_thread;
    cout<< "Before server Thread\n";
    pthread_create(&server_thread, NULL, udpserver, NULL);
    cout<<"After server thread\n";
    pthread_t client_thread;
    cout<< "Before client Thread\n";
    pthread_create(&client_thread, NULL, udpclient, NULL);
    cout<<"After client thread\n";
    pthread_join(server_thread, NULL);
    pthread_join(client_thread, NULL);
    exit(0);
}