#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Message {
	int sender;
	int recipient;
	char sent_at[20];
	char message[200];
} Message;

typedef struct User {
	int id;
	char username[50];
	char password[20];
} User;

typedef struct Conversation {
	int converser;
	int messages_no;
	Message messages[100];
} Conversation;

// Global variables
User users[20];
Message messages[50];
Conversation conversations[20];
int logged_in_user, users_no, messages_no, conversations_no = 0;
char separator[34] = "--------------------------------\n";

void load_data();
void new_user();
int login(char *username, char *password);
void send_message(int recipient, char *message);
void load_user_conversations();
void update_users();
void update_messages();

int main() {
	int choice, i, j, chatted;
	char username[50], password[20], temp, choice_str[4];
	load_data();
	printf("--------Chat Application--------\n");
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
			if (login(username, password)) {
				printf("\nLogin successful. Welcome %s!\n", users[logged_in_user].username);
				load_user_conversations();
				printf(separator);
			}	else {
				printf("\nInvalid credentials. Please try again.\n");
				goto login;
			}
			break;
		case 2:
			signup:
			printf(separator);
			printf("Enter new user details below.\n\nUsername: ");
			scanf(" %[^\n]s", username);
			printf("Password: ");
			scanf(" %[^\n]s", password);
			for (i = 0; i < users_no; i++) {
				if (strcmp(users[i].username, username) == 0) {
					printf("\nUsername is already taken.\n");
					goto signup;
				}
			}
			users_no++;
			users[users_no].id = users_no;
			strcpy(users[users_no].username, username);
			strcpy(users[users_no].password, password);
			update_users();
			logged_in_user = users_no;
			printf("User created successfully. Welcome %s!\n", username);
			printf(separator);
			break;
		default:
			printf("Invalid choice.\n");
			goto start;
	}
	printf("Select an option to continue.\n\n0. New conversation\n");
	for (i = 0; i < conversations_no; i++) {
		printf("%d. %s\n", i + 1, users[conversations[i].converser].username);
	}
	printf("\nChoice: ");
	scanf(" %[^\n]s", choice_str);
	choice = atoi(choice_str);
	if (choice == 0) {
		printf(separator);
		printf("Select a user.\n\n");
		for (i = 0; i < users_no; i++) {
			chatted = 0;
			// Exclude users already in conversations with the logged in user
			for (j = 0; j < conversations_no; j++) {
				if (i == conversations[j].converser) {
					chatted = 1;
					break;
				}
			}
			if (chatted == 0)
				printf("(%d) %s\n", users[i].id, users[i].username);
		}
	}
	return 0;
}

void load_data() {
	FILE *fp;
	char buffer[200];
	int line_no = 0, item_no = 0;
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
			users[item_no].id = atoi(buffer);	
			line_no++;
		} else if (line_no == 1) {
			strcpy(users[item_no].username, buffer);	
			line_no++;
		} else if (line_no == 2) {
			strcpy(users[item_no].password, buffer);
			line_no = 0;
			item_no++;
		}
	}
	users_no = item_no;
  fclose(fp);
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
			messages[item_no].sender = atoi(buffer);
			line_no++;
		} else if (line_no == 1) {
			messages[item_no].recipient = atoi(buffer);
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
}

int login(char *username, char *password) {
	int i = 0;
	for (i = 0; i < users_no; i++) {
		if ((strcmp(users[i].username, username) == 0) && (strcmp(users[i].password, password) == 0)) {
			logged_in_user = i;
			return 1;
		}
	}
	return 0;
}

void load_user_conversations() {
	int i, j;
	for (i = 0; i < messages_no; i++) {
		// If the logged in user was the sender (converser => recipient)
		if (messages[i].sender == logged_in_user) {
			for (j = 0; j < conversations_no; j++) {
				// If conversation exists, append message to conversation's message list
				if (conversations[j].converser == messages[i].recipient) {
					conversations[j].messages[conversations[j].messages_no + 1] = messages[i];
					conversations[j].messages_no++;
				}
			}
			conversations[conversations_no + 1].converser = messages[i].recipient;
			conversations[conversations_no + 1].messages[0] = messages[i];
			conversations[conversations_no + 1].messages_no = 0;
		} else if (messages[i].recipient == logged_in_user) {
			// If the logged in user was the recipient (converser => sender)
			for (j = 0; j < conversations_no; j++) {
				// If conversation exists, append message to conversation's message list
				if (conversations[j].converser == messages[i].sender) {
					conversations[j].messages[conversations[j].messages_no + 1] = messages[i];
					conversations[j].messages_no++;
				}
			}
			conversations[conversations_no + 1].converser = messages[i].sender;
			conversations[conversations_no + 1].messages[0] = messages[i];
			conversations[conversations_no + 1].messages_no = 0;
		}
	}
}

void update_users() {
	FILE *fp;
	int i = 0;
	fp = fopen("users.txt", "w");
	for (i = 0; i < users_no; i++) {
		fprintf(fp, "%d\n", users[i].id);
		fprintf(fp, "%s\n", users[i].username);
		fprintf(fp, "%s\n\n", users[i].password);
	}
	fclose(fp);
}

void update_messages() {
	FILE *fp;
	int i = 0;
	fp = fopen("messages.txt", "w");
	for (i = 0; i < messages_no; i++) {
		fprintf(fp, "%s\n", messages[i].sender);
		fprintf(fp, "%s\n", messages[i].recipient);
		fprintf(fp, "%s\n", messages[i].sent_at);
		fprintf(fp, "%s\n\n", messages[i].message);
	}
	fclose(fp);
}
