#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TARGET_IP "8.8.8.8"
#define TARGET_PORT 80
#define NUM_PACKETS 10000
#define NUM_ITERATIONS 100

// Function to send a TCP SYN packet to the specified IP and port
void send_tcp_syn(char *ip, int port, int *sock) {
  // Create a socket
  *sock = socket(AF_INET, SOCK_STREAM, 0);

  // Set the IP and port to connect to
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  // Connect to the target IP and port
  connect(*sock, (struct sockaddr *)&addr, sizeof(addr));

  // Close the socket
  close(*sock);
}

int main(int argc, char *argv[]) {
  // Set the name of the file to store the SYN requests in
  char *file_name = "syn_requests.txt";

  // Open the file for writing
  FILE *file = fopen(file_name, "w");

  // Keep track of the total time it takes to send the SYN packets
  clock_t total_time = 0;

  // Send the SYN packets in 100 iterations
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    // Keep track of the time it takes to send all the SYN packets in this iteration
    clock_t start_time = clock();

    // Send NUM_PACKETS SYN packets
    for (int j = 0; j < NUM_PACKETS; j++) {
      // Send a SYN packet
      int sock;
      send_tcp_syn(TARGET_IP, TARGET_PORT, &sock);

      // Record the time it took to send the packet
      clock_t packet_time = clock() - start_time;

      // Write the packet index and time to the file
      fprintf(file, "%d, %ld\n", i * NUM_PACKETS + j + 1, packet_time);
    }

    // Record the total time it took to send all the SYN packets in this iteration
    clock_t iteration_time = clock() - start_time;
    total_time += iteration_time;
  }

  // Calculate the average time it took to send a SYN packet
  double average_time = (double)total_time / (NUM_ITERATIONS * NUM_PACKETS);

  // Write the total time and average time to the file
  fprintf(file, "Total time: %ld\n", total_time);
  fprintf(file, "Average time: %ld\n" , average_time);