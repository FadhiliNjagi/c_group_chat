/*
  TCP CLIENT GROUP CHAT
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

// Global variables
int network_socket;
struct sockaddr_in server_address;
char request[256], response[256], logged_in_user[30];
char separator[] = "---------------------------------------------------\n";
const char s[2] = "\n";

// Function prototypes
void connect_to_server();
void send_request();

int main() {
  int i, j, flag;
  char username[30], password[30], temp[30], choice_str[4];
  char *token;
  printf("--------Chat Application--------\n");
  connect_to_server();
  start:
  printf("Select an option to continue.\n\n1. Log in\n2. Sign Up\n\nChoice: ");
  scanf(" %[^\n]s", choice_str);
  switch (atoi(choice_str)) {
    case 1:
      login:	// login label
      printf(separator);
      printf("Enter your credentials below.\n\nUsername: ");
      scanf(" %[^\n]s", username);
      printf("Password: ");
      scanf(" %[^\n]s", password);
      snprintf(request, sizeof(request), "/login\n%s\n%s", username, password);
      send_request();
      // Process response
      token = strtok(response, s);
      if (strcmp("OK", token) == 0) {
        logged_in_user = strtok(NULL, s);
        printf("Login successful. Welcome %s!", logged_in_user);
      }
      else {
        // If invalid login
        printf("\nInvalid credentials. Please try again.\n");
        printf(separator);
        goto start;
      }
      break;
    case 2:
      signup:
      printf(separator);
      printf("Enter new user details below.\n\nUsername: ");
      scanf(" %[^\n]s", username);
      printf("Password: ");
      scanf(" %[^\n]s", password);
      snprintf(request, sizeof(request), "/signup\n%s\n%s", username, password);
      send_request();
      // Process response
      token = strtok(response, s);
      if (strcmp("OK", token) == 0) {
        logged_in_user = strtok(NULL, s);
        printf("User created successfully. Welcome %s!\n", logged_in_user);
      } else {
        // If signup error
        printf("\nUsername is already taken.\n");
        goto signup;
      }
      break;
    default:
      printf("Invalid choice.\n");
      goto start;
  }
  Mainmenu:
  printf(separator);
  printf("Select an option to continue.\n\n1. Search for a group\n2. Groups list\n3. Create new group\n\nChoice: ");
  scanf(" %[^\n]s", choice_str);
  switch (atoi(choice_str)) {
    case 1:
      search:
      printf(separator);
      printf("Group search\nEnter name (0 to go back): ");
      scanf(" %[^\n]s", temp);
      if (strcmp(temp, "0") == 0)
        goto Mainmenu;
      break;
  }

  return 0;
}

void connect_to_server() {
  // Create socket
  network_socket = socket(AF_INET, SOCK_STREAM, 0);
  // Specify address and port
  server_address.sin_family = AF_INET; // IPv4
  server_address.sin_port = htons(9002); // Port 9002 to correct byte order
  server_address.sin_addr.s_addr = INADDR_ANY; // Any interface on local machine
  // Connect to a socket
  if (connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
    printf("Error connecting to server.\n");
    exit(1);
  }
  printf("[+] Connected to the server successfully.\n");
}

void send_request() {
  // Sending request
  send(network_socket, request, sizeof(request), 0);
  // Receive response
  recv(network_socket, &response, sizeof(response), 0);
  return 0;
}

// int chat_screen(char *group_name) {
//   // Check if group
// }