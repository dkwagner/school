/*----------------------------------------------------------------------------*/
/*NAME: Daniel Wagner                                                         */
/*DUE DATE: 03/02/2018                                                        */
/*PROGRAM ASSIGNMENT 2                                                        */
/*FILE NAME: main.c                                                           */
/*PROGRAM PURPOSE:                                                            */
/*  Read the input from a file, and spawn the correct child processes         */
/*  to sort the data. Then wait until the child processes exit.               */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256

int main(int argc, char **argv){
  /* Create the character buffer for sprintf */
  char buf[BUFFER_SIZE];
  pid_t pid, qpid, mpid;
  int status;
  /* Declare array variables */
  int arrASize, arrXSize, arrYSize, arrMSize, i, temp;
  /* Declare SHMID variables */
  int shmidA, shmidX, shmidY, shmidM;
  /* Declare shared memory pointers */
  int *shmptrA;
  int *shmptrX;
  int *shmptrY;
  int *shmptrM;
  /* Declare shared memory keys */
  key_t aKey, xKey, yKey, mKey;
  /* Declare command line arguments */
  /* Use size of 12 since that is max length of int plus 1 */
  char qprog[] = {"./qsort"};
  char mprog[] = {"./merge"};
  char qLeft[12], qRight[12], qSize[12];
  char mXSize[12], mYSize[12];
  char *qargv[5];
  char *margv[4];

  /* Array a[] creation */
  /*--------------------------------------------------------------------------*/

  scanf("%d", &arrASize);  /* Read in the size of the array */

  /* Create shared memory for array A */
  /* Create key */
  aKey = ftok("./", 'a');
  sprintf(buf, "*** MAIN: shared memory key: %d\n", (int) aKey);
  write(1, buf, strlen(buf));
  /* Create shared memory */
  shmidA = shmget(aKey, arrASize * sizeof(int), IPC_CREAT | 0666);
  sprintf(buf, "*** MAIN: shared memory created\n");
  write(1, buf, strlen(buf));
  /* Attach Shared memory */
  shmptrA = (int *) shmat(shmidA, NULL, 0);
  sprintf(buf, "*** MAIN: shared memory attached and ready to use\n");
  write(1, buf, strlen(buf));

  /* Print out array size*/
  sprintf(buf, "Input array for qsort has %d elements:\n\t", arrASize);
  write(1, buf, strlen(buf));

  /* Read array into shared memory */
  /* and print array */
  for(i = 0; i < arrASize; i ++){
    scanf("%d", &temp);
    shmptrA[i] = temp;
    printf("%d ", shmptrA[i]);
  }
  printf("\n");

  /* Array x[] creation */
  /*--------------------------------------------------------------------------*/

  scanf("%d", &arrXSize);  /* Read in the size of the array */

  /* Create shared memory for array X */
  /* Create key */
  xKey = ftok("./", 'x');
  sprintf(buf, "*** MAIN: shared memory key: %d\n", (int) xKey);
  write(1, buf, strlen(buf));
  /* Create shared memory */
  shmidX = shmget(xKey, arrXSize * sizeof(int), IPC_CREAT | 0666);
  sprintf(buf, "*** MAIN: shared memory created\n");
  write(1, buf, strlen(buf));
  /* Attach Shared memory */
  shmptrX = (int *) shmat(shmidX, NULL, 0);
  sprintf(buf, "*** MAIN: shared memory attached and ready to use\n");
  write(1, buf, strlen(buf));

  /* Print out array size*/
  sprintf(buf, "Input array x[] for merge has %d elements:\n\t", arrXSize);
  write(1, buf, strlen(buf));

  /* Read array into shared memory */
  /* and print array */
  for(i = 0; i < arrXSize; i ++){
    scanf("%d", &temp);
    shmptrX[i] = temp;
    printf("%d ", shmptrX[i]);
  }
  printf("\n");

  /* Array y[] creation */
  /*--------------------------------------------------------------------------*/

  scanf("%d", &arrYSize);  /* Read in the size of the array */

  /* Create shared memory for array y */
  /* Create key */
  yKey = ftok("./", 'y');
  sprintf(buf, "*** MAIN: shared memory key: %d\n", (int) yKey);
  write(1, buf, strlen(buf));
  /* Create shared memory */
  shmidY = shmget(yKey, arrYSize * sizeof(int), IPC_CREAT | 0666);
  sprintf(buf, "*** MAIN: shared memory created\n");
  write(1, buf, strlen(buf));
  /* Attach Shared memory */
  shmptrY = (int *) shmat(shmidY, NULL, 0);
  sprintf(buf, "*** MAIN: shared memory attached and ready to use\n");
  write(1, buf, strlen(buf));

  /* Print out array size*/
  sprintf(buf, "Input array y[] for merge has %d elements:\n\t", arrYSize);
  write(1, buf, strlen(buf));

  /* Read array into shared memory */
  /* and print array */
  for(i = 0; i < arrYSize; i ++){
    scanf("%d", &temp);
    shmptrY[i] = temp;
    printf("%d ", shmptrY[i]);
  }
  printf("\n");

  /* Array m[] creation */
  /*--------------------------------------------------------------------------*/
  arrMSize = arrXSize + arrYSize;
  /* Creake key */
  mKey = ftok("./", 'm');
  sprintf(buf, "*** MAIN: shared memory key: %d\n", (int)mKey);
  write(1, buf, strlen(buf));
  /* Create shared memory */
  shmidM = shmget(mKey, (arrMSize) * sizeof(int), IPC_CREAT | 0666);
  sprintf(buf, "*** MAIN: Shared memory created\n");
  write(1, buf, strlen(buf));
  /* Attach Shared memory */
  shmptrM = (int *) shmat(shmidM, NULL, 0);
  sprintf(buf, "*** MAIN: shared memory attached and ready to use\n");
  write(1, buf, strlen(buf));


  /*--------------------------------------------------------------------------*/
  /* Build command line arguments */
  /* Build quicksort arguments */
  sprintf(qLeft, "%d", 0);
  sprintf(qRight, "%d", arrASize - 1);
  sprintf(qSize, "%d", arrASize);
  qargv[0] = qprog;
  qargv[1] = qLeft;
  qargv[2] = qRight;
  qargv[3] = qSize;
  qargv[4] = '\0';

  /* Build mergesort arguments */
  sprintf(mXSize, "%d", arrXSize);
  sprintf(mYSize, "%d", arrYSize);
  margv[0] = mprog;
  margv[1] = mXSize;
  margv[2] = mYSize;
  margv[3] = '\0';

  /* Fork qsort processs and wait */
  if((qpid = fork()) == 0){ /* We are a child process */
    if(execvp(qprog, qargv) < 0){
      sprintf(buf, "!!! ERROR: Quicksort execvp() failed!\n");
      write(1, buf, strlen(buf));
      exit(1);
    }
  }

  /* Fork merge process and wait */
  if((mpid = fork()) == 0){  /* We are a child process */
    if(execvp(mprog, margv) < 0){
      sprintf(buf, "!!! ERROR: Merge execvp() failed!\n");
      write(1, buf, strlen(buf));
      exit(1);
    }
  }


  for(i = 0; i < 2; i ++){
    wait(&status);
  }

  /* Print Quicksorted Array */
  sprintf(buf, "*** MAIN: sorted array by qsort\n\t");
  write(1, buf, strlen(buf));

  for(i = 0; i < arrASize; i ++){
    sprintf(buf, "%d ", shmptrA[i]);
    write(1, buf, strlen(buf));
  }
  printf("\n");
  /* Print Quicksorted Array */
  sprintf(buf, "*** MAIN: merged array\n\t");
  write(1, buf, strlen(buf));

  for(i = 0; i < arrMSize; i ++){
    sprintf(buf, "%d ", shmptrM[i]);
    write(1, buf, strlen(buf));
  }
  printf("\n");

  /* REMOVE ALL SHARED MEMORY SEGMENTS! */
  shmdt(shmptrA);
  sprintf(buf, "*** MAIN: Shared memory A successfully detached\n");
  write(1, buf, strlen(buf));
  shmctl(shmidA, IPC_RMID, NULL);
  sprintf(buf, "*** MAIN: Shared memory A successfully removed\n");
  write(1, buf, strlen(buf));

  shmdt(shmptrX);
  sprintf(buf, "*** MAIN: Shared memory X successfully detached\n");
  write(1, buf, strlen(buf));
  shmctl(shmidX, IPC_RMID, NULL);
  sprintf(buf, "*** MAIN: Shared memory X successfully removed\n");
  write(1, buf, strlen(buf));

  shmdt(shmptrY);
  sprintf(buf, "*** MAIN: Shared memory Y successfully detached\n");
  write(1, buf, strlen(buf));
  shmctl(shmidY, IPC_RMID, NULL);
  sprintf(buf, "*** MAIN: Shared memory Y successfully removed\n");
  write(1, buf, strlen(buf));

  shmdt(shmptrM);
  sprintf(buf, "*** MAIN: Shared memory M successfully detached\n");
  write(1, buf, strlen(buf));
  shmctl(shmidM, IPC_RMID, NULL);
  sprintf(buf, "*** MAIN: Shared memory M successfully removed\n");
  write(1, buf, strlen(buf));

}
