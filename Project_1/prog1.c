//-------------------------------------------------------
//NAME: Daniel Wagner 			User ID: dkwagner
//PROGRAM ASSIGNMENT 1
//FILE NAME: prog1.c
//PROGRAM PURPOSE:
//	Read in 4 command line arguments
//	create 4 child processes, wait for them
// 	to complete, then exit.
//-------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

//------------------------------------------------------
//FUNCTION sortHeap:
//	Create random array and send it to heapsort
//	functions then send array to print function
//PARAMETER USAGE:
//	numElements: Number of elements to generate
//		and then sort
//FUNCTION CALLED:
//	srand(),printf(),rand(),heapsort(),printArr()
//------------------------------------------------------
void sortHeap(int numElements);

//------------------------------------------------------
//FUNCTION fibonacci:
//	Calculate nth iteration of the fibonacci
//	sequence and print result
//PARAMETER USAGE:
//	nth: The nth iteration of the sequence
//		to calculate
//FUNCTION CALLED:
//	printf(),recFib()
//------------------------------------------------------
void fibonacci(int nth);

//------------------------------------------------------
//FUNCTION needleToss:
//	Simulate the Buffon's Needle Toss
//	problem and print the results
//PARAMETER USAGE:
//	numTosses: Number of tosses to
//		simulate
//FUNCTION CALLED:
//	acos(),srand(),prinft(),rand()
//-----------------------------------------------------
void needleToss(int numTosses);

//-----------------------------------------------------
//FUNCTION integrate:
//	Estimate the integral of sin(x)
//	and print results
//PARAMETER USAGE:
//	numPoints: Number of points to 
//	use in estimation of integral
//FUNCTION CALLED:
//	acos(),srand(),rand(),prinft()
//----------------------------------------------------
void integrate(int numPoints);

//----------------------------------------------------
//FUNCTION waitAndExit:
//	Helper funtion for main 
//	function to wait for child
// 	processes to complete, then 
// 	inform user main process has
//	ended
//PARAMETER USAGE:
//	N/A
//FUNCTION CALLED:
//	wait(),printf()
//----------------------------------------------------
void waitAndExit();

//----------------------------------------------------
//FUNCTION printArr:
//	Helper function for printing
//	arrays in correct format.
//	Mainly for use with sortHeap
//	function in particular
//PARAMTER USAGE:
//	arr[]: Array to print
//	size: Length of arr[]
//FUNCTION CALLED:
//	printf()
//----------------------------------------------------
void printArr(int arr[], int size);

//----------------------------------------------------
//FUNCTION recFib:
//	Computes the nth iteration
// 	or the fibonacci sequence
//	recursively
//PARAMETER USAGE:
//	n: Nth iteration to compute
//FUNCTION CALLED:
//	recFib()
//----------------------------------------------------
int recFib(long n);

//----------------------------------------------------
//FUNCTION max:
//	Find the maximum value in a subtree of 
//	arr[] rooted at i. j and k are the 
//	children of i.
//PARAMETER USAGE:
//	arr[]: Array containing heap(max)
//	size: Total size of heap
//	i: root of subtree to find max of
//	j: left child of i
//	k: right child of i
//FUNCTION CALLED:
//	N/A
//----------------------------------------------------
int max(int arr[], int size, int i, int j, int k);

//----------------------------------------------------
//FUNCTION downheap:
//	Place element at arr[i] in
// 	proper position in heap
//PARAMETER USAGE:
//	arr[]: Array containing heap
//	size: Size of heap
//	i: index of element to place
//		in correct position
//FUNCTION CALLED:
//	max()
//----------------------------------------------------	
void downheap(int arr[], int size, int i);

//----------------------------------------------------
//FUNCTION heapsort:
//	Use inplace heapsort algorithm
//	to sort a given array of ints
//PARAMETER USAGE:
//	arr[]: Array of ints to sort
//	size: size of arr[]
//FUNCTION CALLED:
//	downheap()
//----------------------------------------------------
void heapsort(int arr[], int size);

//----------------------------------------------------
//FUNCTION main:
//	Take 4 integers from cli
//	and use them to start 4 
//	child processes. Then
//	wait for those processes
//	to complete before exiting.
//PARAMETER USAGE:
//	argc: number of arguments
//	char** argv: array of cli arguments
//FUNCTION CALLED:
//	printf(),fork(),atoi(),sortHeap,fibonacci(),
//	needleToss(),integrate(),waitAndExit()
//----------------------------------------------------
int main(int argc, char** argv){
	int heapSort, fibNum, buffonNeedle, integration;
	int argNum = argc;
	pid_t pid;
	int i;

	// Begin Main Process
	printf("Main Process Starts\n");

	if(argc < 5){
		printf("Incorrect Input: <int1> <int2> <int3> <int4> \n");
		printf("Num arguments: %d", argNum);
	}
	else if(argc > 5){
		printf("Incorrect Input: <int1> <int2> <int3> <int4> \n");
		printf("Num arguments: %d", argNum);
	}
	else{
		heapSort = atoi(argv[1]);
		fibNum = atoi(argv[2]);
		buffonNeedle = atoi(argv[3]);
		integration = atoi(argv[4]);
	}

	// Print info to cli
	printf("Number of Random Numbers 	= %d\n", heapSort);
	printf("Fibonacci Input 		= %d\n", fibNum);
	printf("Buffon's Needle Iterations	= %d\n", buffonNeedle);
	printf("Integration Iterations		= %d\n", integration);

	// Create four child processes, switch to next process for each iteration
	for(i = 0; i < 4; i ++){
		pid = fork();
		if(pid < 0){
			printf("Failed to fork process %d", (i+1));
		}
		else if(pid == 0){	// Create process based on position in loop
			switch(i){
				case 0:
					printf("Heap Sort Process Created\n");
					sortHeap(heapSort);
					break;
				case 1:
					printf("Fibonacci Process Created\n");
					fibonacci(fibNum);
					break;
				case 2:
					printf("Buffon's Needle Process Created\n");
					needleToss(buffonNeedle);
					break;
				case 3:
					printf("Integration Process Created\n");
					integrate(integration);
					break;
				default:
					printf("Default used, possible bug");
			}
			break;
		}
	}
	if(pid > 0){	// if parent process, use waitAndExit to wait for child processes to complete
		waitAndExit();
	}
}

void sortHeap(int numElements){
	// Create variables needed for function
	int randNum[numElements];
	int i;
	srand(time(NULL));
	
	// Inform user that process is started
	printf("   Heap Sort Process Started\n");

	// Create numElements random integers between 0 - 99
	for (i = 0; i < numElements; i ++){
		randNum[i] = rand()%100;
	}

	// Report to user the random numbers generated
	printf("   Random Numbers Generated:\n");
	printArr(randNum, numElements);
	
	heapsort(randNum, numElements);

	// Report to user output and that process is exiting
	printf("   Sorted Sequence:\n");
	printArr(randNum, numElements);
	printf("   Heap Sort Process Exits\n");
	
}

void fibonacci(int nth){
	long fibNum;
	// Inform user process has started, and display nth number
	printf("      Fibonacci Process Started\n      Input Number %d\n", nth);
	
	fibNum = recFib(nth);	

	// Report to user output and that process is exiting
	printf("      Fibonacci Number f(%d) is %ld\n", nth, fibNum);
	printf("      Fibonacci Process Exits\n");
}

void needleToss(int numTosses){
	int i;
	int t = 0;
	float pi = acos(-1.0);	// Define Pi 
	float a, d, toss, result;
	srand(time(NULL));
	
	// Inform user of process beginning
	printf("         Buffon's Needle Process Started\n");
	printf("	 Input Number %d\n", numTosses);

	// Perform needle tosses and count number of successes
	for(i = 0; i < numTosses; i ++){
		d = ((float)rand())/(float)RAND_MAX;
		a = ((float)rand())/(float)(RAND_MAX)*(2*pi);
		toss = d + (1*sin(a));
		if(toss < 0 || toss > 1){
			t ++;
		}		
	}
	
	// Calculate result
	result = (float)t/numTosses;

	// Report to user output and that process is exiting
	printf("	 Estimated Probability is %.5f\n", result);
	printf("	 Buffon's Needle Process Exits\n");
}

void integrate(int numPoints){
	int i;
	int hits = 0;
	float pi = acos(-1.0);
	float a, b, result;
	srand(time(NULL));
	
	// Inform user of process beginning
	printf("             Integration Process Started\n");
	printf("             Input Number %d\n", numPoints);
	
	for(i = 0; i < numPoints; i ++){
		a = (float)rand()/(float)RAND_MAX * pi;
		b = (float)rand()/(float)RAND_MAX;
		if(b <= sin(a)){
			hits ++;
		} 
	}
	
	// Calculate result
	result = ((float) hits/numPoints)*pi;

	// Report to user output and that process is exiting
	printf("	     Estimated Area is %f\n", result);
	printf("	     Integration Process Exits\n");
}

void waitAndExit(){
	int i, status;
	printf("Main Process Waits\n");

	for (i = 0; i < 4; i ++){
		wait(&status);
	}

	printf("Main Process Exits\n");
}

void printArr(int arr[], int size){
	int i, elemPrinted;
	elemPrinted = 0;
	
	for(i = 0; i < size; i ++){
		if(elemPrinted == 0){
			printf("   %4d", arr[i]);
		}
		else if(elemPrinted == 20){
			elemPrinted = 0;
			printf("\n   %4d", arr[i]);
		}
		else{
			printf(" %4d", arr[i]);
		}
		elemPrinted ++;
	}
	printf("\n");
}

int recFib(long n){
	// Check for base case
	if(n <= 1){
		return n;
	}
	
	// Return next steps in sequence
	return recFib(n-1) + recFib(n-2);
}

int max(int arr[], int size, int i, int j, int k){
	int max = i;   // Start max at root of subtree
	if(j < size && arr[j] > arr[max]){  
		max = j;
	}	
	if(k < size && arr[k] > arr[max]){
		max = k;
	}
	return max;
}

void downheap(int arr[], int size, int i){
	int j, t;
	
	while(1){
		j = max(arr, size, i, 2 * i + 1, 2 * i + 2);// Find max of sub heap
		if(j == i){
			break;
		}
		// perform required swaps
		t = arr[i];
		arr[i] = arr[j];
		arr[j] = t;
		i = j; 	
	}
}


void heapsort(int arr[], int size){
	int i, t;
	// Perform downheap until arr is heapified
	for(i = (size - 2) / 2; i >= 0; i --){
		downheap(arr, size, i);
	}
	// Place elements in order
	for(i = 0; i < size; i ++){
		t = arr[size - i - 1];
		arr[size - i - 1] = arr[0];
		arr[0] = t;
		downheap(arr, size - i - 1, 0);
	}
}
