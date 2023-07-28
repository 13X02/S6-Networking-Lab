#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char buf[100];
    int k;
    int sock_desc;
    struct sockaddr_in client;
    
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1) {
        printf("Error in socket creation");
        return 1;
    }
    
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_port = htons(3005); // Use htons() to convert the port number to network byte order
    
    k = connect(sock_desc, (struct sockaddr *)&client, sizeof(client));
    if (k == -1) {
        printf("Error in connecting to server");
        return 1;
    }
    
    printf("Enter data to be sent: ");
    fgets(buf, sizeof(buf), stdin);
    
    k = send(sock_desc, buf, strlen(buf) + 1, 0); // Include the null terminator in the message size
    if (k == -1) {
        printf("Error in sending");
        return 1;
    }
    
    close(sock_desc);
    return 0;
}
