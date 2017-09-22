#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "utility_functions.h"

// holds diary record
struct entry
{
	char date[20];
	char time[20];
	char content[1000];
	struct entry *next;
};

// function prototypes
void intro_screen(int choice);
void addEntry(struct entry **head);
void write_to_file(struct entry *head, char pass[]);
struct entry *load(struct entry *head, char pass[]);
void printelements(struct entry *head);
void delete_record(struct entry **head);
void print_record(struct entry *head);

int main(void)
{
	struct entry *head;
	head = NULL;
	int choice = 1;
	char key_press, password[50];

	// check if password is set
	if (fileExist("userinfo.dat"))
	{
		GetPass(password);
		head = load(head, password);

		// ask user for password and check it
		while (1)
		{
			printf("Password: \n");
			inputpass(password);
			if (checkPass(password))
				break;
			puts("\nWrong password, please try again");
		}
	} else
	{
		// prompt the user to set a new password
		puts("Please set a password");
		puts("Type your new password:");
		inputpass(password);
		SetPass(password);
		// 2nd argument is a random key used for encryption, should be kept secret
		encryptDecrypt(password, "_1a2d_");
	}

	//head = load(head, password);
	while (1)
	{
		intro_screen(choice);
		key_press = getche();
		if (choice > 5 || choice < 1) choice = 1;

		if (key_press == 72) // UP Arrow
		{
			choice--;
			if (choice > 5 | choice < 1) choice = 1;
			intro_screen(choice);
		}
		else if (key_press == 80) // Down Arrow
		{
			choice++;
			if (choice > 5 || choice < 1) choice = 1;
			intro_screen(choice);
		}
		else if (key_press == 13) // ENTER Key
		{
			if (choice == 1)
				addEntry(&head);
			else if (choice == 2)
				printelements(head);
			else if (choice == 3)
				delete_record(&head);
			else if (choice == 4)
			{
				changePass(password);
			}
			else if (choice == 5)
			{
				write_to_file(head, password);
				break;
			}
		}
	}

	return 0;
}

void delete_record(struct entry **head) {
	struct entry *previous, *current, *current2, *previous2;
	char d[50];
	char choice;
	int i, n = 0;
	fflush(stdout);
	printf("-------\n");
	printf("Date: ");
	fflush(stdin);
	scanf("%s", &d);
	formatDate(d);

	previous = NULL;
	current = *head;
	current2 = previous2 = NULL;

	while (current != NULL) {
		if (strcmp(current->date, d) == 0)
		{
			if (n == 0)
			{
				current2 = current;
				previous2 = previous;
			}
			printf("Record No: %d\n", n);
			print_record(current);
			printf("\n");
			n++;
		}
		previous = current;
		current = current->next;
	}

	if (current2 == NULL)
	{
		puts("Record not found");
		getch();
		intro_screen(1);
		return;
	}
	printf("Type record no to delete: ");
	scanf("%d", &n);

	for (i = 0; i < n; i++)
	{
		previous2 = current2;
		current2 = current2->next;
	}

	if (current2 == NULL && previous2 == NULL)
		printf("Not found\n");
	else if (previous2 == NULL)
	{
		*head = current2->next;
		free(current2);
	}
	else {
		previous2->next = current2->next;
		free(current2);
	}

	puts("\nRecord deleted successfully");
	getch();

}

void intro_screen(int choice)
{
	// interactive menu system
	system("cls");
	printf("Personal Digital Diary\n\n");
	printf("Choose a menu:\n");
	if (choice == 1)
		printf("==>1. Create a entry\n2. View all entry\n3. Delete record\n4. Change password\n5. Exit");
	else if (choice == 2)
		printf("1. Create a entry\n==>2. View all entry\n3. Delete record\n4. Change password\n5. Exit");
	else if (choice == 3)
		printf("1. Create a entry\n2. View all entry\n==>3. Delete record\n4. Change password\n5. Exit");
	else if (choice == 4)
		printf("1. Create a entry\n2. View all entry\n3. Delete record\n==>4. Change password\n5. Exit");
	else if (choice == 5)
		printf("1. Create a entry\n2. View all entry\n3. Delete record\n4. Change password\n==>5. Exit");
}


void addEntry(struct entry **head)
{
	struct entry *n, *current, *previous;
	char newDate[50], currentDate[50];
	n = malloc(sizeof(struct entry));

	// -- asks for input from the user --
	fflush(stdin);
	printf("\n\nDate (dd/mm/yyyy): ");
	gets(n->date);
	n->time[0] = '\0';

	// auto-fills current date and time when user input is "now"
	if (!strcmp(n->date, "now"))
	{
		struct tm *t;
		char buffer[30];
		t = getCurrentTime();

		strftime(buffer,30,"%d/%m/%Y", t);
		strcpy(n->date, buffer);

		strftime(buffer,30,"%I:%M %p", t);
		strcpy(n->time, buffer);

		printf("Date: %s\n", n->date);
		printf("Time: %s\n", n->time);
	} else formatDate(n->date);

	// checks if n->time is already filled up (by previous IF)
	if (!strcmp(n->time, ""))
	{
		printf("Time: ");
		gets(n->time);
	}
	formatTime(n->time);

	printf("Text: ");
	gets(n->content);
	// --------- User input ends -----------------------
	n->next = NULL;

	if (*head == NULL)
		*head = n;
	else
	{
		current = *head;
		previous = NULL;
		strcpy(newDate, reverseDate(n->date));
		int track = 0; // keeps track of whether record with same date is found, so that new record can be SORTED by time

		while (current != NULL)
		{
			// finds where to put the record (added in sorted order, by date)
			strcpy(currentDate, reverseDate(current->date));

			if (strcmp(newDate, currentDate) == 0)
			{
				// sorted by time
				track = 1;
				while (current != NULL && strcmp(newDate, currentDate) == 0)
				{
					if (checkTime(n->time, current->time) < 0 || checkTime(n->time, current->time) == 0)
					{
						// n->time should be smaller to return negative value
						break;
					}

					previous = current;
					current = current->next;
					if (current != NULL) strcpy(currentDate, reverseDate(current->date));
				}
			}
			else if (strcmp(newDate, currentDate) < 0)
				break;

			if (track == 0)
			{
				previous = current;
				current = current->next;
			} else break;
		}

		// places the record and updates the linked list
		if (previous == NULL)
		{
			*head = n;
			n->next = current;
		} else if (current == NULL)
		{
			previous->next = n;
			n->next = NULL;
		} else
		{
			previous->next = n;
			n->next = current;
		}
	}

	puts("# Entry Added Successfully #");
	getch();
}


void write_to_file(struct entry *head, char pass[])
{
	// save the entries in the database file
	FILE *fp;
	struct entry *current;
	if (head == NULL)
	{
		fp = fopen("DataBase.dat", "wb");
		fclose(fp);
		return;
	}
	current = head;

	encryptDecrypt(current->date, pass);
	encryptDecrypt(current->time, pass);
	encryptDecrypt(current->content, pass);

	fp = fopen("DataBase.dat", "wb");
	if (fp == NULL)
		printf("Error occured\n");

	while (fwrite(current, sizeof(struct entry), 1, fp) == 1)
	{
		current = current->next;
		if (current != NULL)
		{
			encryptDecrypt(current->date, pass);
			encryptDecrypt(current->time, pass);
			encryptDecrypt(current->content, pass);
		}
	}
	fclose(fp);
}


struct entry *load(struct entry *head, char pass[])
{
	// loads entries from the database and connects with the linked list
	FILE *fp;
	fp = fopen("DataBase.dat", "rb");

	struct entry *record;
	struct entry *current;
	record = malloc(sizeof(struct entry));

	if (head == NULL)
	{
		if(fread(record, sizeof(struct entry), 1, fp) == 1)
		{
			record->next = NULL;
			encryptDecrypt(record->date, pass);
			encryptDecrypt(record->time, pass);
			encryptDecrypt(record->content, pass);
			head = record;
			current = head;
		} else // no records exist
			return NULL;
	}

	while(1)
	{
		record = malloc(sizeof(struct entry));
		if (fread(record, sizeof(struct entry), 1, fp) != 1)
			break;

		record->next = NULL;
		encryptDecrypt(record->date, pass);
		encryptDecrypt(record->time, pass);
		encryptDecrypt(record->content, pass);
		current->next = record;
		current = current->next;
	}
	fclose(fp);
	return head;
}


void printelements(struct entry *head) {
	// print all the entries
	struct entry *current;
	current = head;
	printf("\n\n");

	while (current != NULL) {
		puts(current->date);
		puts(current->time);
		puts(current->content);
		current = current->next;
		puts("----------------------");
		printf("\n");
	}
	getch();
}

void print_record(struct entry *e)
{
	printf("Date: %s\n", e->date);
	printf("Time: %s\n", e->time);
	printf("Content: %s\n", e->content);
}
