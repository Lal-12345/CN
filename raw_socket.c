
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

#define BUFFER_SIZE 65536

int main()
{
    int sockfd, data_size;
    unsigned char *buffer;

    struct sockaddr saddr;
    socklen_t saddr_len;

    // Allocate memory
    buffer = (unsigned char *)malloc(BUFFER_SIZE);

    // Create raw socket
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (sockfd < 0)
    {
        perror("Socket");
        return 1;
    }

    printf("\nRAW SOCKET PACKET CAPTURE\n");
    printf("Press Ctrl+C to stop\n");

    // Receive packets continuously
    while (1)
    {
        saddr_len = sizeof(saddr);

        data_size = recvfrom(sockfd, buffer, BUFFER_SIZE,
                             0, &saddr, &saddr_len);

        if (data_size < 0)
        {
            perror("recvfrom");
            break;
        }

        // Extract Ethernet header
        if (data_size < sizeof(struct ethhdr))
            continue;

        struct ethhdr *eth = (struct ethhdr *)buffer;

        printf("\n-----------------------------\n");
        printf("Packet Length : %d Bytes\n", data_size);

        printf("\nEthernet Header\n");

        printf("Destination MAC : ");
        printf("%02X:%02X:%02X:%02X:%02X:%02X",
               eth->h_dest[0], eth->h_dest[1],
               eth->h_dest[2], eth->h_dest[3],
               eth->h_dest[4], eth->h_dest[5]);

        printf("\nSource MAC : ");
        printf("%02X:%02X:%02X:%02X:%02X:%02X",
               eth->h_source[0], eth->h_source[1],
               eth->h_source[2], eth->h_source[3],
               eth->h_source[4], eth->h_source[5]);

        printf("\nProtocol : 0x%04X\n",
               ntohs(eth->h_proto));

        // Check IPv4
        if (ntohs(eth->h_proto) == ETH_P_IP)
        {
            if (data_size < sizeof(struct ethhdr) + sizeof(struct iphdr))
                continue;

            // Extract IP header
            struct iphdr *iph =
                (struct iphdr *)(buffer + sizeof(struct ethhdr));

            int iplen = iph->ihl * 4;

            if (iph->version != 4 ||
                iplen < sizeof(struct iphdr) ||
                data_size < sizeof(struct ethhdr) + iplen)
                continue;

            struct sockaddr_in src, dst;

            memset(&src, 0, sizeof(src));
            memset(&dst, 0, sizeof(dst));

            src.sin_addr.s_addr = iph->saddr;
            dst.sin_addr.s_addr = iph->daddr;

            printf("\nIP Header\n");

            printf("Source IP : %s\n",
                   inet_ntoa(src.sin_addr));

            printf("Destination IP : %s\n",
                   inet_ntoa(dst.sin_addr));

            printf("TTL : %d\n", iph->ttl);

            printf("Protocol : %d\n", iph->protocol);

            // Locate transport header
            unsigned char *transport =
                buffer + sizeof(struct ethhdr) + iplen;

            int remaining = data_size - sizeof(struct ethhdr) - iplen;

            // TCP Header
            if (iph->protocol == IPPROTO_TCP)
            {
                if (remaining < sizeof(struct tcphdr))
                    continue;

                struct tcphdr *tcp = (struct tcphdr *)transport;

                printf("\nTCP Header\n");

                printf("Source Port : %u\n",
                       ntohs(tcp->source));

                printf("Destination Port : %u\n",
                       ntohs(tcp->dest));
            }

            // UDP Header
            else if (iph->protocol == IPPROTO_UDP)
            {
                if (remaining < sizeof(struct udphdr))
                    continue;

                struct udphdr *udp = (struct udphdr *)transport;

                printf("\nUDP Header\n");

                printf("Source Port : %u\n",
                       ntohs(udp->source));

                printf("Destination Port : %u\n",
                       ntohs(udp->dest));
            }

            // ICMP Header
            else if (iph->protocol == IPPROTO_ICMP)
            {
                if (remaining < sizeof(struct icmphdr))
                    continue;

                struct icmphdr *icmp =
                    (struct icmphdr *)transport;

                printf("\nICMP Header\n");

                printf("Type : %d\n", icmp->type);

                printf("Code : %d\n", icmp->code);
            }

            else
            {
                printf("\nOther Protocol\n");
            }
        }
    }

    // Close socket and release memory
    close(sockfd);
    free(buffer);

    return 0;
}

