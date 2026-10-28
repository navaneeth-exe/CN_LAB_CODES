#include <stdio.h>
#include <string.h>
#include <strings.h>
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

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    bind(sockfd, (struct sockaddr *)&server_addr,
         sizeof(server_addr));

    printf("Server waiting for message...\n");

    client_len = sizeof(client_addr);

    // Receive sentence from client
    recvfrom(sockfd, str, SIZE, 0,
             (struct sockaddr *)&client_addr,
             &client_len);

    str[SIZE - 1] = '\0';

    printf("\nReceived sentence:\n%s\n", str);

    /*
       Replace abbreviations with formal English.
       Do longer/specific replacements first where needed.
    */

    char result[SIZE] = "";
    char word[100];

    // Tokenize sentence
    char *token = strtok(str, " ");

    while (token != NULL)
    {
        if (strcmp(token, "tbh") == 0)
            strcpy(word, "to be honest");
        else if (strcmp(token, "ig") == 0)
            strcpy(word, "I guess");
        else if (strcmp(token, "tbf") == 0)
            strcpy(word, "to be fair");
        else if (strcmp(token, "atm") == 0)
            strcpy(word, "at the moment");
        else if (strcmp(token, "irl") == 0)
            strcpy(word, "in real life");
        else if (strcmp(token, "lol") == 0)
            strcpy(word, "laughing out loud");
        else if (strcmp(token, "asap") == 0)
            strcpy(word, "as soon as possible");
        else if (strcmp(token, "omg") == 0)
            strcpy(word, "oh my god");
        else if (strcmp(token, "ttyl") == 0)
            strcpy(word, "talk to you later");
        else if (strcmp(token, "idk") == 0)
            strcpy(word, "I don't know");
        else if (strcmp(token, "nvm") == 0)
            strcpy(word, "never mind");
        else
            strcpy(word, token);

        strcat(result, word);
        strcat(result, " ");

        token = strtok(NULL, " ");
    }

    // Send translated sentence back
    sendto(sockfd, result, strlen(result) + 1, 0,
           (struct sockaddr *)&client_addr, client_len);

    printf("\nTranslated sentence:\n%s\n", result);

    close(sockfd);
    return 0;
}