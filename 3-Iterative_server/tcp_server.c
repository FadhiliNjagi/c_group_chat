/*
  TCP SERVER GROUP CHAT
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

typedef struct Message {
  char group_name[30];
  char sender[30];
  char sent_at[30];
  char message[160];
} Message;

typedef struct User {
  char username[30];
  char password[30];
} User;

typedef struct Group {
  char name[30];
  char members[10][30];
  int messages_no;
  Message messages[100];
} Group;

// Global variables
User users[20];
Message messages[100];
Group groups[20];
int users_no = 0, messages_no = 0, groups_no = 0;
int server_socket;
struct sockaddr_in server_address;
char request[256], response[256];

// Function protoypes
void load_data();
void send_response();
int login(char *username, char *password);
int signup(char *username, char *password);
void load_group_messages();
void update_users();
void update_messages();
void update_groups();

int main() {
  int client_socket, i, j;
  char *token;
  const char s[2] = "\n";

  // Load data
  load_data();
  load_group_messages();

  // Create a socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  // Specifying address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET; // IPv4
  server_address.sin_port = htons(9002); // Port 9002 to correct byte order
  server_address.sin_addr.s_addr = INADDR_ANY; // Any interface on local machine

  // Bind socket to address
  bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

  // Listen for connections
  listen(server_socket, 10); // Queue capacity of 10
  printf("[+] Server is listening...\n");

  // Iterative server
  while (1) {
    // Accept incoming client connections
    client_socket = accept(server_socket, NULL, NULL);
    printf("[+] Incoming connection.\n");
    // Receive request
    recv(client_socket, &request, sizeof(request), 0);
    // Process request
    printf("[+] Request: %s\n", request);
    // Get request type
    token = strtok(request, s);
    if (strcmp("/login", token) == 0) {
      login(strtok(NULL, s), strtok(NULL, s));
    }
    // Close connection
    close(server_socket);
  }

  return 0;
}

void load_data() {
  /* 
    1. Load user data (username, password) from users.txt
    2. Load messages data (group_name, sender, sent_at, message) from messages.txt
    3. Load groups data (name, members) from groups.txt
  */
  // Load user data
  FILE *fp;
  char buffer[200];
  int line_no = 0, item_no = 0, i = 0;
  const char s[2] = ",";
  char *token;
  fp = fopen("users.txt", "r");
  if (fp == NULL) {
    printf("[Error] The file users.txt could not be found.");
    exit(1);
  }
  while (fgets(buffer, 200, fp)) {
    if (isspace(*buffer))
      continue;
    buffer[strcspn(buffer, "\n")] = 0;
    if (line_no == 0) {
      strcpy(users[item_no].username, buffer);	
      line_no++;
    } else if (line_no == 1) {
      strcpy(users[item_no].password, buffer);
      line_no = 0;
      item_no++;
    }
  }
  users_no = item_no;
  fclose(fp);

  // Load messages
  line_no = 0;
  item_no = 0;
  fp = fopen("messages.txt", "r");
  if (fp == NULL) {
    printf("[Error] The file messages.txt could not be found.");
    exit(1);
  }
  while (fgets(buffer, 50, fp)) {
    if (isspace(*buffer))
      continue;
    buffer[strcspn(buffer, "\n")] = 0;
    if (line_no == 0) {
      strcpy(messages[item_no].group_name, buffer);
      line_no++;
    } else if (line_no == 1) {
      strcpy(messages[item_no].sender, buffer);
      line_no++;
    } else if (line_no == 2) {
      strcpy(messages[item_no].sent_at, buffer);	
      line_no++;
    } else if (line_no == 3) {
      strcpy(messages[item_no].message, buffer);
      line_no = 0;
      item_no++;
    }
  }
  messages_no = item_no;
  fclose(fp);

  // Load groups
  line_no = 0;
  item_no = 0;
  fp = fopen("groups.txt", "r");
  if (fp == NULL) {
    printf("[Error] The file groups.txt could not be found.");
    exit(1);
  }
  while (fgets(buffer, 50, fp)) {
    if (isspace(*buffer))
      continue;
    buffer[strcspn(buffer, "\n")] = 0;
    if (line_no == 0) {
      strcpy(groups[item_no].name, buffer);
      line_no++;
    } else if (line_no == 1) {
      // Get comma-separated member list
      token = strtok(buffer, s);

      for (i = 0; token != NULL; i++) {
        strcat(groups[item_no].members[i], token);
        token = strtok(NULL, s);
      }
      line_no = 0;
      item_no++;
    }
  }
  groups_no = item_no;
  fclose(fp);
}

void load_group_messages() {
  /* Load messages into the group structs they belong to */
  int i, j;
  for (i = 0; i < groups_no; i++) {
    groups[i].messages_no = 0;
    for (j = 0; j < messages_no; j++) {
      if (strcmp(messages[j].group_name, groups[i].name) == 0) {
        groups[i].messages[groups[i].messages_no] = messages[j];
        groups[i].messages_no++;
      }
    }
  } 
}

void update_users() {
  /* Update user records in users.txt */
  FILE *fp;
  int i = 0;
  fp = fopen("users.txt", "w");
  for (i = 0; i < users_no; i++) {
    fprintf(fp, "%s\n", users[i].username);
    fprintf(fp, "%s\n\n", users[i].password);
  }
  fclose(fp);
}

void update_messages() {
  /* Update messages records in users.txt */
  FILE *fp;
  int i = 0;
  fp = fopen("messages.txt", "w");
  for (i = 0; i < messages_no; i++) {
    fprintf(fp, "%s\n", messages[i].group_name);
    fprintf(fp, "%s\n", messages[i].sender);
    fprintf(fp, "%s\n", messages[i].sent_at);
    fprintf(fp, "%s\n\n", messages[i].message);
  }
  fclose(fp);
}

void update_groups() {
  /* Update group records in users.txt */
  FILE *fp;
  int i, j;
  fp = fopen("groups.txt", "w");
  for (i = 0; i < groups_no; i++) {
    fprintf(fp, "%s\n,", groups[i].name);
    for (j = 0; j < 10; j++) {
      if (strlen(groups[i].members[j]) > 0) {
        if (j == 0)
          fprintf(fp, "%s", groups[i].members[j]);
        else
          fprintf(fp, ",%s", groups[i].members[j]);
      }
    }
    fprintf(fp, "\n\n");
  }
  fclose(fp);
}

void send_response() {
  // Sending response
  send(network_socket, request, sizeof(request), 0);
}

int login(char *username, char *password) {
  /* Check for matching username, password pair */
  int i;
  for (i = 0; i < users_no; i++) {
    if ((strcmp(users[i].username, username) == 0) && (strcmp(users[i].password, password) == 0)) {
      snprintf(response, sizeof(response), "OK\n%s", username);
      send_response();
      return 0;
    }
  }
  snprintf(response, sizeof(response), "FAIL");
  send_response();
  return 0;
}

int signup(char *username, char *password) {
  int i;
  // Check if username is available
  for (i = 0; i < users_no; i++) {
    if (strcmp(users[i].username, username) == 0) {
      snprintf(response, sizeof(response), "FAIL");
      send_response();
      return 0;
    }
  }
  strcpy(users[users_no].username, username);
  strcpy(users[users_no].password, password);
  users_no++;
  update_users();
  snprintf(response, sizeof(response), "OK\n%s", username);
  send_response();
  return 0;
}