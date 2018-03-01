/*----------------------------------------------------------------------------*/
/*NAME: Daniel Wagner                                                         */
/*DUE DATE: 03/02/2018                                                        */
/*PROGRAM ASSIGNMENT 2                                                        */
/*FILE NAME: merge.c                                                          */
/*PROGRAM PURPOSE:                                                            */
/*  Given two input arrays, perform a concurrent merge sort into a new array  */
/*  and display the results                                                   */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define BUFFER_SIZE 256

/*----------------------------------------------------------------------------*/
/*FUNCTION: int merge(int index, int array, int xSize, int ySize, int *shmptrX*/
/*                      int *shmPtrY, int *shmPtrM)                           */
/*    Function performs a modified binary search to place the given element   */
/*      correctly in the output array m[]                                     */
/*PARAMETER USAGE:                                                            */
/*    int index: index of element to place in output array                    */
/*    int array: selector for which array to use                              */
/*    int xSize: size of x[] array                                            */
/*    int ySize: size of y[] array                                            */
/*    int *shmPtrX: shared memory array for x[]                               */
/*    int *shmPtrY: shared memory array for y[]                               */
/*    int *shmPtrM: shared memory array for m[]                               */
/*FUNCTION CALLED:                                                            */
/*    getpid(), sprintf(), write(), strlen(), floor(), exit()                 */
/*----------------------------------------------------------------------------*/
int merge(int index, int array, int xSize, int ySize, int *shmPtrX, int *shmPtrY, int *shmPtrM);

/*----------------------------------------------------------------------------*/
/*FUNCTION: int main(int argc, char **argv)                                   */
/*PARAMETER USAGE:                                                            */
/*    int argc: number of command line arguments                              */
/*    char **argv: command line arguments                                     */
/*FUNCTION CALLED:                                                            */
/*    getpid(), sprintf(), write(), strlen(), atoi(), ftok(), shmget(),       */
/*    shmat(), fork(), wait(), exit(), shmdt(), merge()                       */
/*----------------------------------------------------------------------------*/
int main(int argc, char **argv){

  /* Variable Declarations */
  char buf[BUFFER_SIZE];  /* Character buffer for printing */
  pid_t pid = getpid();  /* Get the PID */
  pid_t fork_pid;
  int shmidX, shmidY, shmidM, xSize, ySize, mSize, i, status;
  key_t xKey, yKey, mKey;
  int *shmPtrX, *shmPtrY, *shmPtrM;

  /* Arugment sanity checking */
  if(argc > 3 || argc < 3){
    sprintf(buf, "      $$$ MPROC(%d): ERROR, wrong number of command line arguments\n", (int)pid);
    write(1, buf, strlen(buf));
  }

  /* Snag command line variables */
  xSize = atoi(argv[1]);
  ySize = atoi(argv[2]);
  mSize = xSize + ySize;

  /* Attach shared memories */
  xKey = ftok("./", 'x');
  yKey = ftok("./", 'y');
  mKey = ftok("./", 'm');
  shmidX = shmget(xKey, xSize * sizeof(int), 0666);
  shmidY = shmget(yKey, ySize * sizeof(int), 0666);
  shmidM = shmget(mKey, mSize * sizeof(int), 0666);
  shmPtrX = (int *) shmat(shmidX, NULL, 0);
  shmPtrY = (int *) shmat(shmidY, NULL, 0);
  shmPtrM = (int *) shmat(shmidM, NULL, 0);

  /* Fork child processes */
  /* Fork x[] child processes */
  for(i = 0; i < xSize; i ++){
    if((fork_pid = fork()) == 0){  /* We are the child process */
      sprintf(buf, "      $$$ MPROC(%d): Writing for x[%d]\n", (int)pid, i);
      write(1, buf, strlen(buf));
      merge(i, 0, xSize, ySize, shmPtrX, shmPtrY, shmPtrM);
      exit(0);
    }
    else{  /* We are the parent process */
      continue;
    }
  }
  for(i = 0; i < ySize; i ++){
    if((fork_pid = fork()) == 0){  /* We are the child process */
      merge(i, 1, xSize, ySize, shmPtrX, shmPtrY, shmPtrM);
      exit(0);
    }
    else{  /* We are the parent process */
      continue;
    }
  }
  /* Wait for child processes to finish */
  for(i = 0; i < xSize + ySize; i ++){
    wait(&status);
  }

  /* Detach shared memories */
  shmdt(shmPtrX);
  shmdt(shmPtrY);
  shmdt(shmPtrM);

  /* Exit gracefully */
  exit(0);
}

int merge(int index, int array, int xSize, int ySize, int *shmPtrX, int *shmPtrY, int *shmPtrM){
  int mSize = xSize + ySize;
  int mid, left, right;
  char buf[BUFFER_SIZE];
  pid_t pid = getpid();

  /* Handle the x[i] in y[] situation */
  if(array == 0){  /* We are handling an x array index */
    sprintf(buf, "      $$$ MPROC(%d): handling x[%d]\n", (int)pid, index);
    write(1, buf, strlen(buf));

    if(shmPtrX[index] < shmPtrY[0]){  /* Check to see if x[index] is less all elements in y[] */
      sprintf(buf, "      $$$ MPROC(%d): x[%d] = %d is found to be smaller than y[0] = %d\n",
        (int)pid, index, shmPtrX[index], shmPtrY[0]);
      write(1, buf, strlen(buf));
      sprintf(buf, "      $$$ MPROC(%d): about to write x[%d] = %d into position %d of the output array\n",
        (int)pid, index, shmPtrX[index], 0);
      write(1, buf, strlen(buf));
      shmPtrM[0] = shmPtrX[index];
    }
    else if(shmPtrX[index] > shmPtrY[ySize-1]){  /* Check to see if x[index] is larger than all elements in y[] */
      sprintf(buf, "      $$$ MPROC(%d): x[%d] = %d is found to be larger than y[%d] = %d\n",
        (int)pid, index, shmPtrX[index], ySize - 1,  shmPtrY[ySize - 1]);
      write(1, buf, strlen(buf));
      sprintf(buf, "      $$$ MPROC(%d): about to write x[%d] = %d into position %d of the output array\n",
        (int)pid, index, shmPtrX[index], mSize - 1);
      write(1, buf, strlen(buf));
      shmPtrM[mSize -1] = shmPtrX[index];
    }
    else{  /* Run a binary search to find the location of x[index] in m */
      left = 0;
      right = ySize - 1;
      while(1){
        mid = floor((left + right) / 2);
        if(shmPtrY[mid] > shmPtrX[index] && shmPtrY[mid - 1] < shmPtrX[index]){
          sprintf(buf, "      $$$ MPROC(%d): x[%d] = %d is found between y[%d] = %d and y[%d] = %d\n",
            (int)pid, index, shmPtrX[index], mid, shmPtrY[mid], mid - 1, shmPtrY[mid - 1]);
          write(1, buf, strlen(buf));
          sprintf(buf, "      $$$ MPROC(%d): about to write x[%d] = %d into position %d of the output array\n",
            (int)pid, index, shmPtrX[index], index + mid);
          write(1, buf, strlen(buf));
          shmPtrM[index + mid] = shmPtrX[index];
          exit(0);
        }
        else if(shmPtrY[mid] > shmPtrX[index]){
          right = mid - 1;
        }
        else{
          left = mid + 1;
        }
      }
    }
  }
  /* Handle the y[i] in x[] situation */
  else if(array == 1){  /* We are handling a y array index */
    sprintf(buf, "      $$$ MPROC(%d): handling y[%d]\n", (int)pid, index);
    write(1, buf, strlen(buf));

    if(shmPtrY[index] < shmPtrX[0]){
      sprintf(buf, "      $$$ MPROC(%d): x[%d] = %d is found to be smaller than y[0] = %d\n",
        (int)pid, index, shmPtrY[index], shmPtrX[0]);
      write(1, buf, strlen(buf));
      shmPtrM[0] = shmPtrY[index];
    }
    else if(shmPtrY[index] > shmPtrX[xSize-1]){
      sprintf(buf, "      $$$ MPROC(%d): x[%d] = %d is found to be larger than y[%d] = %d\n",
        (int)pid, index, shmPtrY[index], xSize - 1, shmPtrX[xSize - 1]);
      write(1, buf, strlen(buf));
      shmPtrM[mSize -1] = shmPtrY[index];
    }
    else{
      left = 0;
      right = xSize - 1;
      while(1){
        mid = floor((left + right) / 2);
        if(shmPtrX[mid] > shmPtrY[index] && shmPtrX[mid - 1] < shmPtrY[index]){
          sprintf(buf, "      $$$ MPROC(%d): y[%d] = %d is found between x[%d] = %d and x[%d] = %d\n",
            (int)pid, index, shmPtrY[index], mid, shmPtrX[mid], mid - 1, shmPtrX[mid - 1]);
          write(1, buf, strlen(buf));
          sprintf(buf, "      $$$ MPROC(%d): about to write x[%d] = %d into position %d of the output array\n",
            (int)pid, index, shmPtrY[index], index + mid);
          write(1, buf, strlen(buf));
          shmPtrM[index + mid] = shmPtrY[index];
          exit(0);
        }
        else if(shmPtrX[mid] > shmPtrY[index]){
          right = mid - 1;
        }
        else{
          left = mid + 1;
        }
      }
    }
  }
  else{  /* Incorrect array designation */
    sprintf(buf, "Incorrect array designation\n");
    write(1, buf, strlen(buf));
    return -1;
  }
}
