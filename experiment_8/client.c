#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5000
#define SIZE 1000

int main()
{
    int sockfd;
    char str[SIZE];
    struct sockaddr_in server_addr;
    socklen_t server_len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Requesting server time...\n");

    sendto(sockfd, "TIME", 5, 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    server_len = sizeof(server_addr);

    recvfrom(sockfd, str, SIZE, 0,
             (struct sockaddr *)&server_addr, &server_len);

    printf("Server time: %s", str);

    close(sockfd);
    return 0;
}