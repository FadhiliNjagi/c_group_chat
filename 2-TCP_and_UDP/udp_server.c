/*
  UDP SERVER:
  Simple UDP server program to demo creating socket, sending/receiving data, and closing socket
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Socket libraries
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // Socket addresses
#include <unistd.h> // Close function

int main() {
  char request[256], response[] = "Hello, I'm the server :)";

  // Creating socket
  int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

  // Specifying address
  struct sockaddr_in server_address, client_address;
  server_address.sin_family = AF_INET; // IPv4
  server_address.sin_port = htons(9003); // Port 9002 to correct byte order
  server_address.sin_addr.s_addr = INADDR_ANY; // Any interface IP on local machine

  // Bind socket to port
  if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
    printf("Failed to bind socket to port 9003.\n");
    exit(1);
  }

  // Receive request
  recvfrom(server_socket, request, sizeof(request), MSG_WAITALL, (struct sockaddr *) &client_address, sizeof(client_address));
  printf("[+] Request: %s\n", request);

  // Send response
  sendto(server_socket, response, strlen(response), 0, (struct sockaddr *) &client_address, sizeof(client_address);
  printf("[+] Sending response.\n");

  // Closing server socket
  close(server_socket)
}