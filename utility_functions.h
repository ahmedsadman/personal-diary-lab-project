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
	sprintf(buffer, "%d/%02d/%02d", year, month, day);
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

char *get24(char str[])
{
    // convert the given time in 24h format
    static char newtime[20];
    int i, j;
    int h1 = (int)str[1] - '0';
    int h2 = (int)str[0] - '0';
    int hh = (h2 * 10 + h1 % 10);

    // If time is in "AM"
    if (str[6] == 'A')
    {
        if (hh == 12)
        {
            strcpy(newtime, "00");
            for (i=2; i <= 4; i++)
                newtime[i] = str[i];
        }
        else
        {
            for (i=0; i <= 4; i++)
                newtime[i] = str[i];
        }
    }

    // If time is in "PM"
    else
    {
        if (hh == 12)
        {
            strcpy(newtime, "12");
            for (i=2; i <= 4; i++)
                newtime[i] = str[i];
        }
        else
        {
            hh = hh + 12;
            sprintf(newtime, "%02d", hh);
            for (i=2; i <= 4; i++)
                newtime[i] = str[i];
        }
    }
    newtime[5] = '\0';
    return newtime;
}

int checkTime(char t1[], char t2[])
{
	// return -1 if t1 is less than t2, 0 if both are same, 1 otherwise
	char time1[30];
	char time2[30];

	strcpy(time1, get24(t1));
	strcpy(time2, get24(t2));

	if (strcmp(time1, time2) == 0) // both same
		return 0;
	else if (strcmp(time1, time2) < 0) // t1 is smaller
		return -1;
	else // t1 is greater OR t2 is smaller
		return 1;
}

char *getPastTime(int x)
{
	struct tm *t;
	static char buffer[30];
	t = getCurrentTime();

	struct tm xmas = { 0, 0, 0, t->tm_mday, t->tm_mon, t->tm_year };
	struct tm *mytime;

	time_t rawtime = mktime(&xmas);
	rawtime -= x*86400;
	mytime = localtime(&rawtime);

	sprintf(buffer, "%02d/%02d/%d", mytime->tm_mday, mytime->tm_mon+1, mytime->tm_year+1900);
	return buffer;
}

int monthNumber(char month[])
{
	int i;
	for (i = 0; i < strlen(month); i++)
		month[i] = tolower(month[i]);

	if (!strcmp(month, "january")) return 1;
	else if (!strcmp(month, "february")) return 2;
	else if (!strcmp(month, "march")) return 3;
	else if (!strcmp(month, "april")) return 4;
	else if (!strcmp(month, "may")) return 5;
	else if (!strcmp(month, "june")) return 6;
	else if (!strcmp(month, "july")) return 7;
	else if (!strcmp(month, "august")) return 8;
	else if (!strcmp(month, "september")) return 9;
	else if (!strcmp(month, "october")) return 10;
	else if (!strcmp(month, "november")) return 11;
	else if (!strcmp(month, "december")) return 12;
	else return 0;
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

void encryptDecrypt(char text[], char pass[], int length)
{
	// encrypt/decrypt using XOR encryption
	char keys[2];
	keys[0] = generateKey(pass);
	keys[1] = pass[strlen(pass)/2];
	int i;

	for (i = 0; i < length; i++)
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
	encryptDecrypt(pass, "_1a2d_", 6);
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
	encryptDecrypt(pass, "_1a2d_", 6);
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
