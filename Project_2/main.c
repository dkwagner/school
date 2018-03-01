//-----------------------------------------------
//NAME: Daniel Wagner		User ID: dkwagner
//DUE DATE: 10/14/2016
//PROGRAM ASSIGNMENT 2
//FILE NAME: main.c
//PROGRAM PURPOSE:
//	Read three input arrays from a file, and
//	perform two types of sort algorithms
//	independently from each other in seperate
//	child processes qsort.c and merge.c
//	using the execvp() system call.
//-----------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

//-----------------------------------------------
//FUNCTION main:
//	Read three input arrays from a file, and
//	perform two types of sort algorithms
//	independently from each other in seperate
//	child processes qsort.c and merge.c
//	using the execvp() system call.
//PAREMETER USAGE:
//	argc: number of command line arguments
// 	argv: array of strings representing cli arguments
//FUNCTION CALLED:
//	printf(), scanf(), ftok(), shmget(), shmat()
//	getpid(), fork(), execvp(), exit(), shmctl()
//	shmdt()
//-----------------------------------------------
int main(int argc, char** argv){
	// Declare required array variables
	int aSize, xSize, ySize, i, temp, status;

	// Declare shared memory id variables
	int shmIDA, shmIDX, shmIDY;

	//Declare shared memory pointers
	int  *shmPtrA;
	int  *shmPtrX;
	int  *shmPtrY;

	// Declare required key variables
	key_t aKey, xKey, yKey;

	// Print title of output
	printf("\nQuicksort and Binary Merge with Multiple Processes:\n\n");

	// Take in size of a
	scanf("%d", &aSize);

	// Get and attach shared memory segment for a
	aKey = ftok("./",'a');
	printf("*** MAIN: shared memory key = %d\n",(int) aKey);
	shmIDA = shmget(aKey,(sizeof(int)) * aSize, IPC_CREAT | 0666);
	printf("*** MAIN: shared memory created\n");
	shmPtrA = (int*) shmat(shmIDA, NULL, 0);
	printf("*** MAIN: shared memory is attached and ready to use\n");

	printf("Input array for qsort has %d elements:\n   ", aSize);
	// Populate shared memory with array a
	for(i = 0; i < aSize; i ++){
		scanf("%d", &temp);
		shmPtrA[i] = temp;
		printf("%d ", shmPtrA[i]);
	}
	printf("\n\n");

	// Take in size of x
	scanf("%d", &xSize);

	// Get and attch shared memory segment for x
	xKey = ftok("./", 'x');
	printf("*** MAIN: shared memory key = %d\n", (int) xKey);
	shmIDX = shmget(xKey, (sizeof(int))*xSize, IPC_CREAT | 0666);
	printf("*** MAIN: shared memory created\n");
	shmPtrX = (int*) shmat(shmIDX, NULL, 0);
	printf("*** MAIN: shared memory is attached and ready to use\n");

	printf("Input array x[] for merge has %d elements\n   ", xSize);
	// Populate shared memory with array x
	for(i = 0; i < xSize; i ++){
		scanf("%d", &temp);
		shmPtrX[i] = temp;
		printf("%d ", shmPtrX[i]);
	}
	printf("\n\n");

	// Take in size of y
	scanf("%d", &ySize);

	// Get and attach shared memory segment for y
	yKey = ftok("./", 'y');
	printf("*** MAIN: shared memory key = %d\n", (int) yKey);
	shmIDY = shmget(yKey, (sizeof(int))*(ySize + xSize), IPC_CREAT | 0666);
	printf("*** MAIN: shared memory created\n");
	shmPtrY = (int*) shmat(shmIDY, NULL, 0);
	printf("*** MAIN: shared memory is attached and ready to use\n");

	printf("Input array y[] for merge has %d elements:\n   ", ySize);
	// Populate shared memory with array y
	for(i = 0; i < ySize; i ++){
		scanf("%d", &temp);
		shmPtrY[i] = temp;
		printf("%d ", shmPtrY[i]);
	}
	printf("\n\n");

	// Create and execute the child processes
	// Create required variables for qsort process

	// Declare execvp() arguments for qsort and merge
	char qprog[] = {"./qsort"};
	char left[10];   // We use a size of 10 since that is the most digits an int can be
	char right[10];
	char size[10];
	char sharedIDA[10];
	char *qargv[6];

	char mprog[] = {"./merge"};
	char xarrSize[10];
	char yarrSize[10];
	char sharedIDX[10];
	char sharedIDY[10];
	char *margv[6];

	// Prepare qsort arguments
	sprintf(left, "%d", 0);
	sprintf(right, "%d", aSize - 1);
	sprintf(size, "%d", aSize);
	sprintf(sharedIDA, "%d", shmIDA);

	// Create qargv[]
	qargv[0] = qprog;
	qargv[1] = left;
	qargv[2] = right;
	qargv[3] = size;
	qargv[4] = sharedIDA;
	qargv[5] = '\0';

	// Prepare merge arguments
	sprintf(xarrSize, "%d", xSize);
	sprintf(yarrSize, "%d", ySize);
	sprintf(sharedIDX, "%d", shmIDX);
	sprintf(sharedIDY, "%d", shmIDY);

	// Create margv[]
	margv[0] = mprog;
	margv[1] = xarrSize;
	margv[2] = yarrSize;
	margv[3] = sharedIDX;
	margv[4] = sharedIDY;
	margv[5] = '\0';

	// Fork and create qsort process
	pid_t pid;

	printf("*** MAIN: about to spawn the process for qsort\n");
	if((pid = fork()) < 0){
		printf("fork() failed\n");
		exit(1);
	}
	else if(pid == 0){
		if((execvp(qprog, qargv)) < 0){
			printf("execvp() failed\n");
			exit(1);
		}
	}
	else{
		if((pid = fork()) < 0){
			printf("fork() failed\n");
			exit(1);
		}
		else if(pid == 0){
			if((execvp(mprog, margv)) < 0){
				printf("execvp() failed\n");
				exit(1);
			}
		}
		else{
			// Wait for both processes to complete
			for(i = 0; i < 2; i ++){
				wait(&status);
			}
			printf("*** MAIN: sorted array by qsort:\n");
			for(i = 0; i < aSize; i ++){
				if(i == 0){
					fprintf(stdout, "   %d ", shmPtrA[i]);
				}
				else{
					fprintf(stdout, "%d ", shmPtrA[i]);
				}
			}

			printf("\n");

			printf("*** MAIN: sorted array by merge:\n");
			for(i = 0; i < xSize + ySize; i ++){
				if(i == 0){
					printf("   %d ", shmPtrY[i]);
				}
				else{
					printf("%d ", shmPtrY[i]);
				}
			}
			printf("\n");

			// DO THIS LAST!!
			// Detach and remove shared memory segments
			shmdt(shmPtrA);
			printf("*** MAIN: qsort shared memory successfully detached\n");
			shmdt(shmPtrX);
			shmdt(shmPtrY);
			printf("*** MAIN: merge shared memory successfully detached\n");
			shmctl(shmIDA, IPC_RMID, NULL);
			printf("*** MAIN: qsort shared memory successfully removed\n");
			shmctl(shmIDX, IPC_RMID, NULL);
			shmctl(shmIDY, IPC_RMID, NULL);
			printf("*** MAIN: merge shared memeory successfully removed\n");

			// Exit the process
			exit(0);
		}
	}
}
