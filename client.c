// Program 1: client.c
// Programmer: Anuja Sahu
// Professor Goodrum, CS 4348.002

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

// Define max length and magic number variables
#define MAXLENGTH 100
#define MAGICNUMBER 9

// Method declarations
bool send_string(FILE *fp, char *str);
bool receive_string(FILE *fp, char *newStr);

int main(int argc, char* argv[])
{
	// Create char arrays that will store file name, pipe name and magic square results
	char filename[MAXLENGTH], clientFifo[MAXLENGTH], result[MAXLENGTH];
	char magicSquare[MAGICNUMBER + 1];

	// Create client pipe and connect to server
	sprintf(filename, "/tmp/%s", "axs180258");
	sprintf(clientFifo, "/tmp/%s-%d", "axs180258", getpid());
	mkfifo(clientFifo, 0666);

	// Write client pipe to server
	FILE *fp = fopen(filename, "w");
	send_string(fp, clientFifo);
	fclose(fp);

	// Prompt user to enter nine-digit value
	printf("Enter 9 integer values: \n");

	// Read in numbers entered by user and store in array
	for(int i = 0; i < MAGICNUMBER; i++)
	{
		scanf("%c",&magicSquare[i]);
		scanf("%c");
	}

	// Display entered magic square
	printf("Magic Square: %s\n", magicSquare);

	// Open client pipe, send over array and receive string
	fp = fopen(clientFifo, "w");
	send_string(fp, magicSquare);
	fclose(fp);
	fp = fopen(clientFifo, "r");
	receive_string(fp, result);
	fclose(fp);

	// Display whether value is a magic square or not
	if(result[0] == '1')
	{
		printf("A proper magic square was entered.\n");
	}
	else
	{
		printf("The values entered do not make a magic square.\n");
	}
}

// Method that sends string input to client and lets user know if it did
bool send_string(FILE *fp, char *str)
{
	if (fprintf(fp, "%s\n", str))
	{
		printf("Sent: %s\n", str);
		fflush(fp);
		return true;
	}
	return false;
}

// Method that receives string input and lets user know if it did
bool receive_string(FILE *fp, char *newStr)
{
	char line[MAXLENGTH];
	if (fgets(line, MAXLENGTH, fp) == NULL)
	{
		return false;
	}
	strcpy(newStr, strtok(line, "\n"));
	printf("Received: %s\n", newStr);
	return true;
}
