#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5000
#define SIZE 1000

int main()
{
    int sockfd;
    char filename[100], result[SIZE];
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server_addr,
            sizeof(server_addr));

    printf("Enter filename: ");
    scanf("%99s", filename);

    send(sockfd, filename, strlen(filename) + 1, 0);

    int n = recv(sockfd, result, SIZE - 1, 0);
    if (n > 0)
    {
        result[n] = '\0';
        printf("\nServer response:\n%s\n", result);
    }

    close(sockfd);
    return 0;
}