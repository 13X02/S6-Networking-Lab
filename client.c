#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
int main(int argc,char *argv[]){
    int n,s,t;
    struct sockaddr_in servaddr,local;
    char buffer[1024];
    if((s=socket(AF_INET,SOCK_DGRAM,0))==-1){
        perror("error in socket creation");
        exit(0);
    }
    bzero((char*)&local,sizeof(local));
    local.sin_family=AF_INET;
    local.sin_port=htons(6677);
    local.sin_addr.s_addr=inet_addr(argv[1]);
    if(bind(s,(struct sockaddr*)&local,sizeof(local))==-1){
        perror("bind error");
        exit(1);
    }
    bzero((char*)&servaddr,sizeof(local));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons((short)atoi(argv[2]));
    servaddr.sin_addr.s_addr=inet_addr(argv[1]);
    strcpy(buffer,"TIME");
    if(sendto(s,buffer,sizeof(buffer),0,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
        perror("error in sendto");
        exit(0);
    }
    t=sizeof(servaddr);
    printf("the current time is:");
    if((n=recvfrom(s,buffer,1024,0,(struct sockaddr*)&servaddr,&t))>0){
        buffer[n]='\0';
        fputs(buffer,stdout);
    }
    else{
        if(n<0){
            perror("error in recvfrom");
            exit(0);
        }
        else{
            printf("server closed\n");
        }
    }
    memset(buffer,0,sizeof(buffer));
    close(s);
    return 0;

}