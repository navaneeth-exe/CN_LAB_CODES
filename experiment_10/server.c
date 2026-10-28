
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

#define SIZE 65536

void printMAC(unsigned char *mac)
{
    printf("%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2],
           mac[3], mac[4], mac[5]);
}

int main()
{
    int sockfd, n;
    unsigned char *buffer = malloc(SIZE);
    struct sockaddr saddr;
    socklen_t len;

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (sockfd < 0)
    {
        perror("Socket");
        free(buffer);
        return 1;
    }

    printf("RAW SOCKET PACKET CAPTURE\n");
    printf("Press Ctrl+C to stop...\n");

    while (1)
    {
        len = sizeof(saddr);
        n = recvfrom(sockfd, buffer, SIZE, 0,
                     &saddr, &len);

        if (n < (int)sizeof(struct ethhdr))
            continue;

        struct ethhdr *eth = (struct ethhdr *)buffer;

        printf("\nPacket Length: %d Bytes\n", n);
        printf("Destination MAC: ");
        printMAC(eth->h_dest);
        printf("\nSource MAC: ");
        printMAC(eth->h_source);
        printf("\nEthernet Protocol: 0x%04X\n",
               ntohs(eth->h_proto));

        if (ntohs(eth->h_proto) != ETH_P_IP ||
            n < (int)(sizeof(struct ethhdr) +
                      sizeof(struct iphdr)))
            continue;

        struct iphdr *ip =
            (struct iphdr *)(buffer + sizeof(struct ethhdr));

        int ip_len = ip->ihl * 4;

        if (ip_len < (int)sizeof(struct iphdr) ||
            n < (int)(sizeof(struct ethhdr) + ip_len))
            continue;

        struct sockaddr_in src = {0}, dst = {0};
        src.sin_addr.s_addr = ip->saddr;
        dst.sin_addr.s_addr = ip->daddr;

        printf("\nIP Header\n");
        printf("Source IP: %s\n", inet_ntoa(src.sin_addr));
        printf("Destination IP: %s\n", inet_ntoa(dst.sin_addr));
        printf("TTL: %d\n", ip->ttl);
        printf("Protocol: %d\n", ip->protocol);

        unsigned char *data =
            buffer + sizeof(struct ethhdr) + ip_len;
        int remaining = n - sizeof(struct ethhdr) - ip_len;

        if (ip->protocol == IPPROTO_TCP &&
            remaining >= (int)sizeof(struct tcphdr))
        {
            struct tcphdr *tcp = (struct tcphdr *)data;
            printf("\nTCP Header\n");
            printf("Source Port: %u\n", ntohs(tcp->source));
            printf("Destination Port: %u\n", ntohs(tcp->dest));
        }
        else if (ip->protocol == IPPROTO_UDP &&
                 remaining >= (int)sizeof(struct udphdr))
        {
            struct udphdr *udp = (struct udphdr *)data;
            printf("\nUDP Header\n");
            printf("Source Port: %u\n", ntohs(udp->source));
            printf("Destination Port: %u\n", ntohs(udp->dest));
        }
        else if (ip->protocol == IPPROTO_ICMP &&
                 remaining >= (int)sizeof(struct icmphdr))
        {
            struct icmphdr *icmp = (struct icmphdr *)data;
            printf("\nICMP Header\n");
            printf("Type: %d\n", icmp->type);
            printf("Code: %d\n", icmp->code);
        }
        else
            printf("\nOther Protocol\n");
    }

    close(sockfd);
    free(buffer);
    return 0;
}
