#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/stat.h>

typedef struct packet{
    char data[1024];
}Packet;

typedef struct frame{
    int sq_no;
    int ack;
    int frame_kind;
    Packet packet;
}Frame;

int main(int argc,char **argv){
    if(argc!=2){
        return 1;
    }
    int port = atoi(argv[1]);
    int sockfd;
    int frame_id=0;
    struct sockaddr_in serverAddr,newAddr;
    char buffer[1024];
    socklen_t addr_size;
    Frame frame_send;
    Frame frame_recv;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&serverAddr,'\0',sizeof(serverAddr));

    serverAddr.sin_family=AF_INET;
    serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serverAddr.sin_port=htons(port);

    bind(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    addr_size = sizeof(newAddr);
    while(1){
        int f_recv_size = recvfrom(sockfd,&frame_recv,sizeof(Frame),0,(struct sockaddr *)&newAddr,&addr_size);
        if(f_recv_size>0&&frame_recv.frame_kind==1&&frame_recv.sq_no==frame_id){
            frame_send.ack=frame_recv.sq_no+1;
            frame_send.sq_no=0;
            frame_send.frame_kind=0;
            sendto(sockfd,&frame_send,sizeof(frame_send),0,(struct sockaddr *)&newAddr,addr_size);
            printf("Ack sent");
        }else{
            printf("frame not recieved");
        }
        frame_id++;

    }
    close(sockfd);
    return 0;
}