#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>

char *reverseDate(char d[])
{
	// converts dd/mm/yyyy to yyyy/mm/dd
	static char buffer[80];
	int day, month, year;
	sscanf(d, "%d/%d/%d", &day, &month, &year);
	sprintf(buffer, "%d/%d/%d", year, month, day);
	// puts(buffer);
	return buffer;
}

void encrypt(char text[], char key[])
{
	int i = 0;
	int token = strlen(key) + key[0] - key[1];
	while (1)
	{
		if (text[i] == '\0')
			break;

		text[i] = text[i] + token;
		i++;
	}
}


void decrypt(char text[], char key[])
{
	int i = 0;
	int token = strlen(key) + key[0] - key[1];
	while (1)
	{
		if (text[i] == '\0')
			break;

		text[i] = text[i] - token;
		i++;
	}
}


void getpass(char pass[])
{
	int i = 0;
	char ch;
	printf("Password:\n");

	while (1)
	{
		if (i < 0)
			i = 0;
		ch = getch();

		if (ch == 13)
			break;
		if (ch == 8)
		{
			putch('\b');
			putch('\0');
			putch('\b');
			i--;
			continue;
		}

		pass[i++] = ch;
		ch = '*';
		putch(ch);
	}

	pass[i] = '\0';
}


int main()
{
	// char text[1000], pass[50];
	// printf("Enter text to encrypt: ");
	// gets(text);
	// getpass(pass);
	// encrypt(text, pass);
	// printf("\nEncrypted: ");
	// puts(text);
	// getpass(pass);
	// printf("\nDecrypted: ");
	// decrypt(text, pass);
	// puts(text);
	char some[100];
	strcpy(some, reverseDate("21/1/1999"));
	puts(some);
	return 0;
}
