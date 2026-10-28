#include <stdio.h>
#include <string.h>
#include <time.h>
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
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server_addr,
         sizeof(server_addr));

    printf("Time server waiting...\n");

    while (1)
    {
        client_len = sizeof(client_addr);

        recvfrom(sockfd, str, SIZE, 0,
                 (struct sockaddr *)&client_addr, &client_len);

        time_t t = time(NULL);
        snprintf(str, SIZE, "%s", ctime(&t));

        sendto(sockfd, str, strlen(str) + 1, 0,
               (struct sockaddr *)&client_addr, client_len);
    }

    close(sockfd);
    return 0;
}