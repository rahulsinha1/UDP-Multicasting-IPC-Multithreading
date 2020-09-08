#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>
#include <sstream>

using namespace std;
int ports[20];
string container_ids[20];
int *arrSize;
char *message ="default";
std::ifstream infile("config");

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
     serv.sin_port = htons(50000);
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
     //int recvPort =
     cout<<"RECEIVED FROM "<< ntohs(client.sin_port);
     if(rc<0) {
        cout<<"ERROR READING FROM SOCKET";
     }
     else {
         for(int x = 0; x < *arrSize; x++) {
             if(strcmp(buffer, container_ids[x].c_str()) == 0) {
                 cout << " Successfully Received from container ID[ "<<x<<"]" << container_ids[x];
                 container_ids[x].clear();
                int rp= sendto(sockfd,"hi",2,0,(struct sockaddr *)&client,l);
                if(rp<0)
                {
                cout<<"ERROR writing to SOCKET";
                }
             }
         }
         bool flag = true;
         for(int x = 0; x < *arrSize; x++) {
             if (!container_ids[x].empty()) {
                 flag = false;
             }
         }
         if(flag) {
             break;
         }
     }
     //cout<<"\n the message received is : "<<buffer<<endl;
     }
     cout << "\n***********READY**********" << endl;
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
     //int newports[] ={50000};
     //int portsSize = sizeof(newports)/sizeof(newports[0]);
     while(true)
     {
     for(int i=0;i<*arrSize;i++)
      {
        cout<<"\nSending to" <<ports[i]<<"\n";
        serv.sin_port = htons(ports[i]);
        sendto(sockfd,message,strlen(message) + 1,0,(struct sockaddr *)&serv,m);

      
     int rc =  recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&serv,&m);
     if(rc>0)
     {
         cout<<"\n Server says : "<<buffer<<endl;
         ports[i]= -1;
     }
      }
      bool flag = true;
      for(int x=0;x<*arrSize;x++)
      {
          if (ports[x]!=-1)
          {
              flag = false;
          }
      }
      if(flag) {
          break;
      }
      }
      cout<< "\nho gya send\n";
      
}


int getPorts(char file_name[], string container_ids[], int ports[]) {
	int size = 0;

	std::string line;
	while (std::getline(infile, line))
	{
	    std::istringstream iss(line);
	    if (!(iss >> container_ids[size] >> ports[size])) { break; } // error
	    size++;
	}

	return size;
}


int main(int argc, char *argv[])
{
    char filename[] = "config";

    if(argc >= 2) {
        message = argv[1];
    }
    cout<<"message is : "<<message<<"\n";
    cout<<sizeof(message);
    arrSize = new int;

    *arrSize = getPorts(filename, container_ids, ports);
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