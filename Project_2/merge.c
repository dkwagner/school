//-----------------------------------------------
//NAME: Daniel Wagner		User ID: dkwagner
//DUE DATE: 10/14/2016
//PROGRAM ASSIGNMENT 2
//FILE NAME: merge.c
//PROGRAM PURPOSE:
//	Perform merge sort on two sorted integer
// 	arrays referred to as "x[]" and "y[]". 
//	Create a child process for each element
//	in array x and perform a binary search on
//	y to find its correct place in the array.
//-----------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//-----------------------------------------------
//FUNCTION searchAndMerge:
//	Use parameters to perform modified binary search 
//	on output array to determine placement of given index
// 	from input array. Then place that value into the array.
//PARAMETER USAGE:
//	index: index of shmPtrX you are currently placing
//	sizeX: size of array x
//	sizeY: size of array y
//	pid: pid of this process
//	shmPtrX: shared memory array x
//	shmPtrY: shared memory array y
//FUNCTION CALLED:
//	fprintf(), exit()
//-----------------------------------------------
void searchAndMerge(int index, int sizeX, int sizeY, pid_t pid, int* shmPtrX, int* shmPtrY);

//-----------------------------------------------
//FUNCTION main:
//	take in command line arguments and 
//	perform a merge sort on arrays
//	x and y, using child processes
//PARAMTER USAGE:
//	argc: number of cli arguments
//	argv: array of strings representing cli arguments
//FUNCTION CALLED:
//	atoi(), shmat(), fork(), fprintf(), searchAndMerge(),
//	wait(), exit(), shmdt()
//-----------------------------------------------
int main(int argc, char** argv){
	// declare required variables
	int sizeX, sizeY, shmIDX, shmIDY, i, status;
	
	int *shmPtrX;
	int *shmPtrY;
	
	pid_t pid;

	// Read in variables
	sizeX = atoi(argv[1]);
	sizeY = atoi(argv[2]);
	shmIDX = atoi(argv[3]);
	shmIDY = atoi(argv[4]);

	shmPtrX = (int*) shmat(shmIDX, NULL, 0);
	shmPtrY = (int*) shmat(shmIDY, NULL, 0);
	
	// Create child processes to merge arrays
	for(i = 0; i < sizeX; i ++){
		if((pid = fork()) < 0){
			fprintf(stdout, "fork() failed\n");
		}
		else if(pid > 0){
			searchAndMerge(i, sizeX, sizeY, pid, shmPtrX, shmPtrY);
		}
		else{
			wait(&status);
			sizeY ++;
			continue;
		}
	}
	// Detach shared memory
	shmdt(shmPtrX);
	shmdt(shmPtrY);
	
	exit(0);
}

void searchAndMerge(int index, int sizeX, int sizeY, pid_t pid, int* shmPtrX, int* shmPtrY){
	int left, right, insertIndex, middle, search, i;	

	fprintf(stdout, "      $$$ M-PROC(%d): handling x[%d] = %d\n", (int) pid, index, shmPtrX[index]); 
	search = shmPtrX[index];
	
	// Begin modified binary search
	if(shmPtrX[index] < shmPtrY[0]){
		fprintf(stdout,"      $$$ M-PROC(%d): x[%d] = %d is found to be smaller than y[0] = %d\n", (int) pid, index, shmPtrX[index], shmPtrY[0]); 
		insertIndex = 0;
	}	
	else if(shmPtrX[index] > shmPtrY[sizeY-1]){
		fprintf(stdout,"      $$$ M-PROC(%d): x[%d] = %d is found to be larger than y[%d] = %d\n", (int) pid, index, shmPtrX[index], sizeY - 1, shmPtrY[sizeY - 1]);
		insertIndex = sizeY;
	}
	// Begin binary search
	else{
		// Initialize needed variables
		left = 0; 
		right = sizeY - 1;
		middle = (left + right) / 2;
		
		while(left <= right){
			if(shmPtrY[middle] < search){
				left = middle + 1;
			}
			else if(shmPtrY[middle] > search){
				right = middle - 1;
			}

			middle = (left+right) / 2;
		}
		
		insertIndex = left;
		fprintf(stdout, "      $$$ M-PROC(%d): x[%d] = %d is found between y[%d] = %d and y[%d] = %d\n", (int) pid, index, shmPtrX[index], right, shmPtrY[right],
															left, shmPtrY[left]);
	}

	// Insert value into array
	fprintf(stdout, "      $$$ M-PROC(%d): about to write x[%d] = %d into position %d of the output array\n", (int) pid, index, shmPtrX[index], insertIndex);
	if(insertIndex == sizeY){
		shmPtrY[sizeY] = shmPtrX[index];
	}
	else{
		for(i = sizeY; i >= insertIndex; i --){
			shmPtrY[i] = shmPtrY[i-1];
		}
		shmPtrY[insertIndex] = search;
	}
	exit(0);
}
