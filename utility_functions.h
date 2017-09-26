/*
THIS HEADER FILE CONTAINS NECESSARY SIDE-FUNCTIONS
THAT ARE NEEDED IN THE MAIN PROGRAM
*/

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>

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

void formatDate(char d[])
{
	// formats a date properly using dd/mm/yyyy style
	// for example, 9/12/2016 will be converted to 09//12/2016, similarly 1/1/1996 would be 01/01/1996
	int day, month, year;
	char f_date[50];
	sscanf(d, "%d/%d/%d", &day, &month, &year);
	sprintf(f_date, "%02d/%02d/%d", day, month, year);
	strcpy(d, f_date);
}

void formatTime(char t[])
{
	// formats time properly using "hh:mm meridian" format
	// 2:30 am will be converted to 02:30 AM
	int h, m, i;
	char f_time[20];
	char meridian[5];
	sscanf(t, "%d:%d %s", &h, &m, meridian);

	for (i = 0; i < strlen(meridian); i++)
		meridian[i] = toupper(meridian[i]);

	sprintf(f_time, "%02d:%02d %s", h, m, meridian);
	strcpy(t, f_time);
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

int checkTime(char t1[], char t2[])
{
	// return -1 if t1 is less than t2, 0 if both are same, 1 otherwise
	int greater;
	char *ret1, *ret2;
	if (strcmp(t1, t2) == 0) // both same
		return 0;
	else if (strcmp(t1, t2) < 0) // t1 is smaller
		greater = 2;
	else // t2 is smaller
		greater = 1;

	ret1 = strstr(t1, "AM");
	ret2 = strstr(t2, "AM");

	// printf("greater: %d\n", greater);
	if (ret1 && ret2 && greater == 1 && strcmp(t2, "12:00 AM") <= 0 && strcmp(t1, "12:00 AM") >=0)
		return -1;
	else if (ret1 && ret2 && greater == 2 && strcmp(t1, "12:00 AM") <= 0 && strcmp(t2, "12:00 AM") >= 0)
		return 1;
	else if ((ret1 && ret2 && greater == 1) || (!ret1 && !ret2 && greater == 1) || (!ret1 && ret2))
		return 1;
	else if ((ret1 && ret2 && greater == 2) || (!ret1 && !ret2 && greater == 2) || (ret1 && !ret2))
		return -1;
}

char generateKey(char pass[])
{
	// generate an encyrption key based on password
	int i;
	char base = '!';
	for (i = 0; i < strlen(pass); i++)
		base += pass[i];
	return '!' + ((strlen(pass)+base) % 92);
}

void encryptDecrypt(char text[], char pass[])
{
	// encrypt/decrypt using XOR encryption
	char keys[2];
	keys[0] = generateKey(pass);
	keys[1] = pass[strlen(pass)/2];
	int i;

	for (i = 0; i < strlen(text); i++)
		text[i] = text[i] ^ keys[i % 2];
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
	encryptDecrypt(pass, "_1a2d_");
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
	encryptDecrypt(pass, "_1a2d_");
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
