#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#define DEFAULT_PORT 45952
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
     serv.sin_port = htons(49152);
     serv.sin_addr.s_addr = INADDR_ANY;
     bind(sockfd,(struct sockaddr *)&serv,sizeof(serv));
     char buffer[256];
     socklen_t l = sizeof(client);
     //socklen_t m = client;
     cout<<"\ngoing to recv\n";
     int rc;
     while(true)
     {
     rc= recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&client,&l);
     if(rc<0)
     {
     cout<<"ERROR READING FROM SOCKET";
     }
     cout<<"\n the message received is : "<<buffer<<endl;
     int rp= sendto(sockfd,"hi",2,0,(struct sockaddr *)&client,l);
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
     serv.sin_addr.s_addr = inet_addr("127.0.0.1");
     char buffer[256];
     socklen_t l = sizeof(client);
     socklen_t m = sizeof(serv);
     //socklen_t m = client;
     cout<<"\ngoing to send\n";
     for(int i=0;i<*arrSize;i++)
      {
        cout<<"Sending to" <<ports[i]<<"\n";
        serv.sin_port = htons(ports[i]);
     sendto(sockfd,"Hey server",sizeof(buffer),0,(struct sockaddr *)&serv,m);
      }
     recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&serv,&m);
      cout<<"\n Server says : "<<buffer<<endl;
      
    //  for(int i=0;i<*arrSize;i++)
    //  {
    //  serv.sin_port = htons(ports[i]);
    // //  cout << ports[i];
    // //  cout << serv.sin_port;
    //  socklen_t l = sizeof(client);
    //  socklen_t m = sizeof(serv);
    //  cout<<"\ngoing to send"<<ports[i]<<"\n";
    //  sendto(sockfd,"Hi dude",sizeof(buffer),0,(struct sockaddr *)&serv,m);
    //   cout<<"sent on"<<ports[i];
    //  }
    //  cout<<"Hello";
      //recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&serv,&m);
      //cout<<"\n Server says : "<<buffer<<endl;
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


int main(int argc, char const *argv[])
{
    int port = DEFAULT_PORT;
    if(argc >= 2) {
        port = atoi(argv[1]);
    }
    cout<<"Using Port : "<<port;
    
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