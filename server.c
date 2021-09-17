// Program 1: server.c
// Programmer: Anuja Sahu
// Professor Goodrum, CS 4348.002

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

// Define max length and magic number variables
#define MAXLENGTH 100
#define MAGICNUMBER 9

// Method declarations
bool isMagicSquare(char magicSquare[]);
bool send_string(FILE *fp, char *str);
bool receive_string(FILE *fp, char *newStr);

int main(int argc, char *argv[])
{
	// Create char array that stores names
	size_t len = HOST_NAME_MAX + 1;
	char name[len];

	// Struct for user name
	struct utsname buffer;

	// Get and display domain name
	getdomainname(name, len);
	printf("\n");
	printf("The domain name is: %s", name);

	// Get and display host name
	gethostname(name, len);
	printf("\n");
	printf("The host name is: %s", name);

	// Display struct
	printf("\n");
	uname(&buffer);
	printf("\n");
	printf("Uname system: %s\n", buffer.sysname);
	printf("Uname node name: %s\n", buffer.nodename);
	printf("Uname release: %s\n", buffer.release);
	printf("Uname version: %s\n", buffer.version);
	printf("Uname machine: %s\n", buffer.machine);
	printf("Uname domain name: %s\n", buffer.__domainname);

	// Char arrays that holds file names for client, server and magic square
	char filename[MAXLENGTH], clientFifo[MAXLENGTH], magicSquare[MAGICNUMBER + 1];

	// Create server pipe
	sprintf(filename, "/tmp/%s", "axs180258");
	mkfifo(filename, 0666);

	// Open file in read mode using pointer
	FILE *fp = fopen(filename, "r");

	// Look for client
	while(1)
	{
		if(receive_string(fp, clientFifo))
		{
			// Open client pipe
			FILE *client = fopen(clientFifo, "r");

			// Continue waiting for client
			while(1)
			{
				// Store magic number in array if client sends it
				if(receive_string(client, magicSquare))
				{
					for(int i = 0; i < MAGICNUMBER; i++)
					{
						printf("%c ", magicSquare[i]);
					}
					printf("\n");

					// Send result to client pipe
					fclose(client);
					client = fopen(clientFifo, "w");
					char result[MAXLENGTH];

					// Check if value is magic square. If so, result is true/false (1 or 0) and pass to isMagicSquare function
					if(isMagicSquare(magicSquare))
					{
						sprintf(result, "1");
					}
					else
					{
						sprintf(result, "0");
					}

					// Send result to client and close client pipe
					send_string(client, result);
					break;
					fclose(client);
				}

				// Close server pipe
				fclose(fp);

				// Open server pipe again to look for other clients
				fp = fopen(filename, "r");
			}
		}
	}
}

// Method that checks if nine-digit value is a proper magic square
bool isMagicSquare(char *magicSquare)
{
	// Variables to track sum of rows, columns, diagonals and total sum 
	int sumRow = 0, sumCol = 0, sumDiag = 0, sum = 0;
	// Create an array for magic square
	int square[3][3];
	// Counter variable
	int counter = 0;

	// Create a magic square and store received values in it
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			square[i][j] = (magicSquare[counter] - '0');
			printf("%d%d: %d\n", i,j, square[i][j]);
			counter++;
		}
	}

	// Set sum to first row 
	for(int i = 0; i < 3; i++)
	{
		sum += square[i][0];
	}

	// Calculate sum of rows and columns
	for(int i = 0; i < 3; i++)
	{
		sumRow = 0;
		sumCol = 0;
 		for(int j = 0; j < 3; j++)
		{
			sumRow += square[i][j];
			sumCol += square[j][i];
 		}

		// Check if sum of row and col is equal to sum. If not, it is not a magic square
		if(sumRow != sum)
		{
			return false;
		}
		if(sumCol != sum)
		{
			return false;
		}
	}
	// Calculate sum of first diagonal
	for(int i = 0; i < 3; i++)
	{
		sumDiag += square[i][i];
	}

	// Check if sum of diagonal is equal to sum. If not, it is not a magic square
	if(sumDiag != sum)
	{
		return false;
	}
	sumDiag = 0;

	// Calculate sum of second diagonal
	for(int i = 0; i < 3; i++)
	{
		int j = 2;
		sumDiag += square[i][j--];
	}

	// Check if sum of diagonal is equal to sum. If not, it is not a magic square
	if(sumDiag != sum)
	{
		return false;
	}

	return true;
}

// Method that sends string input to client and lets user know if it did
bool send_string(FILE *fp, char *str) {
	if (fprintf(fp, "%s\n", str)) {
		printf("Sent: %s\n", str);
		fflush(fp);
		return true;
	}
	return false;
}

// Method that receives string input and lets user know if it did
bool receive_string(FILE *fp, char *newStr) {
	char line[MAXLENGTH];
	if (fgets(line, MAXLENGTH, fp) == NULL)
		return false;
	strcpy(newStr, strtok(line, "\n"));
	printf("Received: %s\n", newStr);
	return true;
}
