#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
typedef struct packet{
    char data[1024];
}Packet;
typedef struct frame{
    int frame_kind;
    int sq_no;
    int ack;
    Packet packet;
}Frame;
int main(int argc,char **argv){

    if(argc!=2){
        return 1;
    }
    int port = atoi(argv[1]);
    int sockfd;
    socklen_t addr_size;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    int frame_id=0;
    Frame frame_send;
    Frame frame_recv;
    int ack_recv =1;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd==-1){
        printf("error");
        return 1;
    }
    memset(&serverAddr,'\0',sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port=htons(port);
    while(1){
        if(ack_recv==1){
            frame_send.frame_kind=1;
            frame_send.sq_no=frame_id;
            frame_send.ack=0;
            printf("Enter data");
            scanf("%s",buffer);
            strcpy(frame_send.packet.data,buffer);
            if (sendto(sockfd,&frame_send,sizeof(Frame),0,(struct sockaddr *)&serverAddr,sizeof(serverAddr))==-1){
                printf("error sendinf");
            }
            
            printf("Frame sent");
        }
         addr_size = sizeof(serverAddr);
        int f_recv_size = recvfrom(sockfd,&frame_recv,sizeof(frame_recv),0,(struct sockaddr *)&serverAddr,&addr_size);
        if (f_recv_size>0&&frame_recv.sq_no==0&&frame_recv.ack==frame_id+1)
        {
            printf("ack recieved");
            ack_recv=1;
        }
        else{
            printf("ack not recieved");
            ack_recv=0;
        }
        frame_id++;
        
    }
    close(sockfd);
    return 0;

}