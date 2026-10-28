#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT 5000
#define SIZE 1000

int main()
{
    int sockfd, newsockfd;
    char filename[100], result[SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server_addr,
         sizeof(server_addr));

    listen(sockfd, 5);
    printf("File server waiting...\n");

    while (1)
    {
        len = sizeof(client_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);

        if (fork() == 0)
        {
            FILE *fp;
            close(sockfd);

            recv(newsockfd, filename, sizeof(filename), 0);
            filename[sizeof(filename) - 1] = '\0';

            fp = fopen(filename, "r");

            if (fp == NULL)
                snprintf(result, SIZE, "PID: %d\nFile not found", getpid());
            else
            {
                snprintf(result, SIZE, "PID: %d\n", getpid());
                int n = strlen(result);
                fread(result + n, 1, SIZE - n - 1, fp);
                result[SIZE - 1] = '\0';
                fclose(fp);
            }

            send(newsockfd, result, strlen(result) + 1, 0);
            close(newsockfd);
            exit(0);
        }

        close(newsockfd);
    }

    close(sockfd);
    return 0;
}