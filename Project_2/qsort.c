//-----------------------------------------------
//NAME: Daniel Wagner		User ID: dkwagner
//DUE DATE: 10/14/2016
//PROGRAM ASSIGNMENT 2
//FILE NAME: qsort.c
//PROGRAM PURPOSE:
//	Take in an unsorted array of integers and
//	perform the quicksort algorithm on that
// 	array. Continually spawn new child
//	processes to handle each newly created 
//	array until it is sorted using execvp()
// 	system calls.
//-----------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

//-----------------------------------------------
//FUNCTION main:
//	Read in cli, attach shared memory, partition given section of
//	the array, then spawn two child processes to deal with the remaining
//	load
//PARAMETER USAGE: 
//	int argc: number of command line arguments
//	char** argv: array of strings denoting command line arguments
//FUNCTION CALLED:
//	getpid(), atoi(), fprintf(), shmat(), sprintf(), execvp(), fork(),
//	wait(), exit()
//------------------------------------------------
int main(int argc, char** argv){
	
	// Declare required variables
	int left, right, i, shmID, aSize, low, high, pivot, temp, status;
	int *shmPtrA;
	pid_t pid, cpid;
	key_t key;
	char qprog[] = {"./qsort"};
	char chleft[10];
	char chright[10];
	char size[10];
	char sharedIDA[10];
	char *qargv[6];

	// Get pid
	pid = getpid();
	
	// pull in needed command line arguments
	left = atoi(argv[1]);
	right = atoi(argv[2]);
	aSize = atoi(argv[3]);
	shmID = atoi(argv[4]);
	
	// inform user process has entered after required info has been gathered
	fprintf(stdout, "   ### Q-PROC(%d): entering with a[%d...%d}\n      ", (int) pid, left, right);

	// attach shared memory to this process
	shmPtrA = (int*) shmat(shmID, NULL, 0);

	// Print the shared memory
	for(i = left; i <= right; i ++){
		fprintf(stdout, "%d ", shmPtrA[i]);
	}
	fprintf(stdout, "\n");

	// Partition
	// Initialize variables for quick sort
	if(left < right){
		pivot = left;
		low = left;
		high = right;

		while(low < high){
			while((shmPtrA[low] <= shmPtrA[pivot]) && (low < right)){
				low ++;
			}
			while(shmPtrA[high] > shmPtrA[pivot]){
				high --;
			}
			if(low < high){
				// Swap low with high
				temp = shmPtrA[low];
				shmPtrA[low] = shmPtrA[high];
				shmPtrA[high] = temp;
			}
		}
		// Swap pivot with high value
		temp = shmPtrA[pivot];
		shmPtrA[pivot] = shmPtrA[high];
		shmPtrA[high] = temp;

		// print pivot element
		fprintf(stdout, "   ### Q-PROC(%d): pivot element is a[%d] = %d\n", pid, high, shmPtrA[high]);
	
		// check if right - left = 0, if so, exit
		if((right - left) <= 0){
			shmdt(shmPtrA);
			exit(0);	
		}
	
		// Create two child processes for next partitions
		// Prepare lower arguments
		sprintf(chleft, "%d", 0);
		sprintf(chright, "%d", high - 1);
		sprintf(size, "%d", aSize);
		sprintf(sharedIDA, "%d", shmID);
		qargv[0] = qprog;
		qargv[1] = chleft;
		qargv[2] = chright;
		qargv[3] = size;
		qargv[4] = sharedIDA;
		qargv[5] = '\0';
		
		// spawn lower child process
		if((cpid = fork()) < 0){
			fprintf(stdout, "fork() failed\n");
			exit(1);		
		}	
		else if(cpid > 0){
			if(execvp(qprog, qargv) < 0){
				fprintf(stdout, "execvp() failed\n");
			}	
		}
		else{
			// prepare upper arguments
			sprintf(chleft, "%d", high + 1);
			sprintf(chright, "%d", right);
			sprintf(size, "%d", aSize);
			sprintf(sharedIDA, "%d", shmID);
			qargv[0] = qprog;
			qargv[1] = chleft;	
			qargv[2] = chright;
			qargv[3] = size;
			qargv[4] = sharedIDA;
			qargv[5] = '\0';

			if((cpid = fork()) < 0){
				fprintf(stdout, "fork() failed\n");
				exit(1);
			}
			else if(cpid > 0){
				if(execvp(qprog, qargv) < 0){
					fprintf(stdout, "execvp() failed\n");
					exit(1);
				}
			}
			else{
				// wait for child processes to finish and detach pointer
				for(i = 0; i < 2; i ++){
					wait(&status);
				}
				shmdt(shmPtrA);
				exit(0);
			}
		}
	}

	return 0;
}
