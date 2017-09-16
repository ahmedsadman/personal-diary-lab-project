/*
THIS HEADER FILE CONTAINS NECESSARY SIDE-FUNCTIONS
THAT ARE NEEDED IN THE MAIN PROGRAM
*/

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <unistd.h>

bool fileExist(char fname[])
{
	// checks if a file exists in the working directory
	if (access(fname, F_OK) != -1)
		return true;
	else
		return false;
}

char *reverseDate(char d[])
{
	// converts dd/mm/yyyy to yyyy/mm/dd
	static char buffer[20];
	int day, month, year;
	sscanf(d, "%d/%d/%d", &day, &month, &year);
	sprintf(buffer, "%d/%d/%d", year, month, day);
	return buffer;
}

struct tm *getCurrentTime()
{
	// get the instant date-time from the system
	time_t rawtime;
	struct tm *info;
	time( &rawtime );
	info = localtime( &rawtime );
	return info;
}

int generateToken(char key[])
{
	// generates a token that is needed for the encryption
	int token = strlen(key) * 2;
	int i;

	for (i = 0; i < strlen(key); i++)
	{
		if (i % 2)
			token -= key[i] / 2;
		else
			token += key[i] / 2;
	}

	// if in any rare case, token exceeds ascii limit
	if (token < 0) token = 1;
	if (token > 127) token = 126;
	return token;
}

void encrypt(char text[], char key[])
{
	int i = 0;
	int token = generateToken(key);

	while (1)
	{
		if (text[i] == '\0')
			break;

		text[i] = (i % 2 == 0) ? text[i] + token : text[i] - token;
		i++;
	}
}


void decrypt(char text[], char key[])
{
	int i = 0;
	int token = generateToken(key);
	while (1)
	{
		if (text[i] == '\0')
			break;

		text[i] = (i % 2 == 0) ? text[i] - token : text[i] + token;
		i++;
	}
}


void inputpass(char pass[])
{
	// asks user to input password
	// also implements traditional password input style (show star signs)
	int i = 0;
	char ch;

	while (1)
	{
		if (i < 0)
			i = 0;
		ch = getch();

		if (ch == 13) // ENTER key
			break;
		if (ch == 8) // BACKSPACE key (not character)
		{
			putch('\b'); // backspace character
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


void SetPass(char pass[])
{
	// writes the password to the file
	FILE *fp;
	fp = fopen("userinfo.dat", "w");
	// 2nd argument is a random encryption key, should be kept secret
	encrypt(pass, "_1a2d_");
	fprintf(fp, pass);
	fclose(fp);
	puts("\nPassword set successfully");
	getch();
}

void GetPass(char mypass[])
{
	// Reads the password stored in the file
	FILE *fp;
	char pass[50];
	fp = fopen("userinfo.dat", "r");
	fgets(pass, 255, fp);
	decrypt(pass, "_1a2d_");
	fclose(fp);
	strcpy(mypass, pass);
}

bool checkPass(char userinput[])
{
	// checks if the user input pass is correct
	char pass[50];
	GetPass(pass);
	if (!strcmp(userinput, pass))
		return true;
	else
		return false;
}

void changePass(char password[])
{
	// changes current user password
	char pass[50];
	puts("Type your new password: ");
	inputpass(pass);
	strcpy(password, pass);
	SetPass(pass);
	puts("# Password set successfully #");
}
