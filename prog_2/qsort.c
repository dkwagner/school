/*----------------------------------------------------------------------------*/
/*NAME: Daniel Wagner                                                         */
/*DUE DATE: 03/02/2018                                                        */
/*PROGRAM ASSIGNMENT 2                                                        */
/*FILE NAME: qsort.c                                                          */
/*PROGRAM PURPOSE:                                                            */
/*  Given input array, perform a concurrent quicksort on said array           */
/*  and display the results                                                   */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256

/*----------------------------------------------------------------------------*/
/*FUNCTION: partion(int left, int right, int *shmptr)                         */
/*    Function uses hoares partitioning algorithm to partition the array      */
/*    and return the pivot point                                              */
/*PARAMETER USAGE:                                                            */
/*    int left: left bound of array to partition                              */
/*    int right: right bound of array to partition                            */
/*FUNCTION CALLED:                                                            */
/*    none                                                                    */
/*----------------------------------------------------------------------------*/
int partition(int left, int right, int *shmptr);

int main(int argc, char **argv){

  /* Variable Declarations */
  char buf[BUFFER_SIZE];  /* Character buffer for printing */
  pid_t pid = getpid();  /* Get the PID */
  pid_t pidL, pidR;
  int shmid, left, right, size, pivot, i, status;
  int *shmptr;
  key_t shKey;

  /* Declare execvp strings, reuse size array for both */
  char qprog[] = {"./qsort"};
  char qLLeft[12], qLRight[12], qRLeft[12], qRRight[12], qSize[12];
  char *qargvL[5], *qargvR[5];

  /* Perform check to make sure arguments are correct */
  if(argc > 4 || argc < 4){
    sprintf(buf, "   ### QPROC(%d): ERROR, incorrect number of command line arguments\n", (int)pid);
    write(1, buf, strlen(buf));
  }

  /* Grab left and right indicies from command line arguments */
  left = atoi(argv[1]);
  right = atoi(argv[2]);
  size = atoi(argv[3]);

  /* Inform user that program has started */
  sprintf(buf, "   ### QPROC(%d): entering with a[%d..%d]\n", (int)pid, left, right);
  write(1, buf, strlen(buf));

  /* Check if array is too small */
  /* if so, exit before attaching memory to save time */
  if(left >= right){
    sprintf(buf, "   ### QPROC(%d): exits\n", (int)pid);
    write(1, buf, strlen(buf));
    exit(0);
  }

  /* Get and attach shared memory */
  shKey = ftok("./", 'a');
  shmid = shmget(shKey, size * sizeof(int), 0666);
  shmptr = (int *) shmat(shmid, NULL, 0);

  /* Partition the array */
  pivot = partition(left, right, shmptr);
  sprintf(buf, "   ### QPROC(%d): pivot element is a[%d] = %d\n", (int)pid, pivot, shmptr[pivot]);
  write(1, buf, strlen(buf));

  /* Build command line arguments for execvp()*/
  sprintf(qLLeft, "%d", left);
  sprintf(qLRight, "%d", pivot);
  sprintf(qRLeft, "%d", pivot + 1);
  sprintf(qRRight, "%d", right);
  sprintf(qSize, "%d", size);

  /* Build left partition arguments */
  qargvL[0] = qprog;
  qargvL[1] = qLLeft;
  qargvL[2] = qLRight;
  qargvL[3] = qSize;
  qargvL[4] = '\0';

  /* Build right partition arguments */
  qargvR[0] = qprog;
  qargvR[1] = qRLeft;
  qargvR[2] = qRRight;
  qargvR[3] = qSize;
  qargvR[4] = '\0';

  /* Fork child processes and wait */
  if((pidL = fork()) == 0){  /* We are child process */
    if(execvp(qprog, qargvL) < 0){
      sprintf(buf, "!!! ERROR: Quicksort Left(%d) execvp() failed!\n", (int)pidL);
      write(1, buf, strlen(buf));
      exit(1);
    }
  }
  if((pidR = fork()) == 0){  /* We are child process */
    if(execvp(qprog, qargvR) < 0){
      sprintf(buf, "!!! ERROR: Quicksort Right(%d) execvp() failed!\n", (int)pidL);
      write(1, buf, strlen(buf));
      exit(1);
    }
  }
  /* Wait for child processes to exit */
  for(i = 0; i < 2; i ++){
    wait(&status);
  }

  /* DETACH FROM SHARED MEMORY */
  shmdt(shmptr);
}

int partition(int left, int right, int *shmptr){
  int pivot = shmptr[left];
  int i = left - 1;
  int j = right + 1;

  while(1){
    do{
      i ++;
    } while(shmptr[i] < pivot);

    do{
      j --;
    } while(shmptr[j] > pivot);

    if( i >= j){
      return j;
    }
    /* If not done partitioning, swap A[i] with A[j] */
    else{
      int temp = shmptr[i];
      shmptr[i] = shmptr[j];
      shmptr[j] = temp;
    }
  }
}
