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
    char result[SIZE];

    struct sockaddr_in server_addr;
    socklen_t server_len;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter new-generation English sentence:\n");

    // Read complete sentence
    fgets(str, SIZE, stdin);

    // Remove newline
    str[strcspn(str, "\n")] = '\0';

    server_len = sizeof(server_addr);

    // Send sentence to server
    sendto(sockfd, str, strlen(str) + 1, 0,
           (struct sockaddr *)&server_addr,
           server_len);

    // Receive translated sentence
    recvfrom(sockfd, result, SIZE, 0,
             (struct sockaddr *)&server_addr,
             &server_len);

    printf("\nFormal English:\n%s\n", result);

    close(sockfd);

    return 0;
}