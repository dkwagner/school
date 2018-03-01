/*------------------------------------------------*/
/*NAME: Daniel Wagner           User ID: dkwagner */
/*DUE DATE: 2/12/2018                             */
/*PROGRAM ASSIGNMENT: #1                          */
/*FILE NAME: prog1.c                              */
/*PROGRAM PURPOSE:                                */
/*  Perform various mathematical calculations     */
/*  in 4 seperate child processes in order to     */
/*  demonstrate an understanding of basic         */
/*  concurrent programming in c.                  */
/*------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define BUFFER_SIZE 256

/*------------------------------------------------*/
/*FUNCTION: fibonacci(int n)                      */
/*    This function is used to run the recursive  */
/*    fibonacci function while sending out print  */
/*    statements when necessary.                  */
/*PARAMETER USAGE:                                */
/*    int n:                                      */
/*      nth number in the fibonacci sequence to   */
/*      calculate.                                */
/*FUNCTION CALLED:                                */
/*    sprintf(),write(), recursiveFib(), strelen()*/
/*    exit()                                      */
/*------------------------------------------------*/
void fibonacci(int n);

/*------------------------------------------------*/
/*FUNCTION: recursiveFib(int n)                   */
/*    This function actually does the recursive   */
/*    fibonacci calculations.                     */
/*PARAMETER USAGE:                                */
/*    int n:                                      */
/*      nth number in the fibonacci sequence to   */
/*      calculate.                                */
/*FUNCTION CALLED:                                */
/*    recursiveFib()                              */
/*------------------------------------------------*/
long recursiveFib(int n);

/*------------------------------------------------*/
/*FUNCTION: buffonNeedle(int r)                   */
/*    This function simulates the buffon Needle   */
/*    toss problem, tossing the needle r times.   */
/*PARAMETER USAGE:                                */
/*    int r:                                      */
/*      Number of needle tosses to perform        */
/*FUNCTION CALLED:                                */
/*    acos(), srand(), time(), sprintf(), write() */
/*    strlen(), sin(), exit()                     */
/*------------------------------------------------*/
void buffonNeedle(int r);

/*------------------------------------------------*/
/*FUNCTION: integration(int s)                    */
/*    This function approximates the Integration  */
/*    of sin(x)                                   */
/*PARAMETER USAGE:                                */
/*    int s:                                      */
/*      Number of points to try in estimation     */
/*FUNCTION CALLED:                                */
/*    acos(), sprintf(), write(), strlen(), sin() */
/*    exit()                                      */
/*------------------------------------------------*/
void integration(int s);

/*------------------------------------------------*/
/*FUNCTION: bernoulli(unsigned long m)            */
/*    This function approximates the value of     */
/*    eulers number                               */
/*PARAMETER USAGE:                                */
/*    unsigned long m:                            */
/*      Largest exponent to use in calculations   */
/*FUNCTION CALLED:                                */
/*    exp(), sprintf(), write(), strlen(), pow()  */
/*    fabs(), exit()                              */
/*------------------------------------------------*/
void bernoulli(unsigned long m);

void fibonacci(int n){
  /* Variable declarations */
  char buffer[BUFFER_SIZE];
  long result = -1;

  sprintf(buffer, "      Fibonacci Process Started\n");
  write(1, buffer, strlen(buffer));

  sprintf(buffer, "      Input Number %d\n", n);
  write(1, buffer, strlen(buffer));

  /* Run the recursive fibonacci function */
  result = recursiveFib(n);

  sprintf(buffer, "      Fibonacci Number f(%d) is %ld \n", n, result);
  write(1, buffer, strlen(buffer));

  sprintf(buffer, "      Fibonacci Process Exits\n");
  write(1, buffer, strlen(buffer));

  exit(0);
}

long recursiveFib(int n){
  if(n <= 1){
    return 1;
  }
  else{
    return recursiveFib(n-1) + recursiveFib(n-2);
  }
}

void buffonNeedle(int r){
  /* Variable declarations */
  char buffer[BUFFER_SIZE];
  int i = 0;
  int t = 0;
  double pi = acos(-1.0);  /* Constant pi */
  int L = 1;  /* Length of our needle */
  int G = 1;  /* Gaps between lines on floor */
  double d, a, toss;
  double result = -1;  /* Result variable, return -1 if something goes wrong */
  srand(time(NULL));  /* Seed the random number */

  sprintf(buffer, "         Buffon's Needle Process Started\n");
  write(1, buffer, strlen(buffer));

  sprintf(buffer, "         Input Number %d\n", r);
  write(1, buffer, strlen(buffer));

  /* Perform tosses */
  for(i = 0; i < r; i ++){
    d = ((double)rand()/(double)RAND_MAX);
    a = ((double)rand()/(double)RAND_MAX)*(2*pi);
    toss = d + L*sin(a);  /* Save this toss */
    if(toss < 0 || toss > G){
      t ++;
    }
  }

  result = (double)t/r;

  sprintf(buffer, "         Estimated probability is %.5f\n", result);
  write(1, buffer, strlen(buffer));

  sprintf(buffer, "         Buffon's Needle Process Exits\n");
  write(1, buffer, strlen(buffer));
  exit(0);
}

void integration(int s){
  /* Variable Declarations */
  char buffer[BUFFER_SIZE];
  int i;
  int total = 0;
  double pi = acos(-1.0);  /* Pi constant */
  double a, b, result;

  sprintf(buffer, "            Integration Process Started\n");
  write(1, buffer, strlen(buffer));

  sprintf(buffer, "            Input Number %d\n", s);
  write(1, buffer, strlen(buffer));

  /* Perform area approximation */
  for(i = 0; i < s; i ++){
    a = ((double)rand()/(double)RAND_MAX)*pi;
    b = (double)rand()/(double)RAND_MAX;
    if(b <= sin(a)){
      total ++;
    }
  }
  result = (total/(float)s)*pi;

  sprintf(buffer, "            Total Hit %d\n", total);
  write(1, buffer, strlen(buffer));

  sprintf(buffer, "            Estimated Area is %.7f\n", result);
  write(1, buffer, strlen(buffer));

  sprintf(buffer, "            Integration Process Exits\n");
  write(1, buffer, strlen(buffer));
  exit(0);
}

void bernoulli(unsigned long m){
  /* Variable Declarations */
  char buffer[BUFFER_SIZE];
  unsigned long i = 1;
  double result, diff;
  double e = exp(1.0);

  sprintf(buffer, "   Approximation of e Process Started\n");
  write(1, buffer, strlen(buffer));

  sprintf(buffer, "   Maximum of the Exponent %lu\n", m);
  write(1, buffer, strlen(buffer));

  while(i <= m){
    result = 1 + (1/(double)i);  /* Do inner calculation */
    result = pow(result, (double)i); /* Raise to the ith power */
    diff = fabs(e - result);
    sprintf(buffer, "   %lu    %.15f    %.15f\n", i, result, diff);
    write(1, buffer, strlen(buffer));
    /*
    If i is less than 10, we calculate each i,
    if i is greater than 10, we switch to powers of 2 starting
    with 2^4
    */
    if(i < 10){
      i ++;
    }
    else if(i == 10){
      i = 16;
    }
    else{
      i = 2 * i;
    }
  }

  sprintf(buffer, "   Approximation of e Process Exits\n");
  write(1, buffer, strlen(buffer));
  exit(0);
}

/*------------------------------------------------*/
/*FUNCTION: main(int argc, char **argv)           */
/*    Main function, sets up the processes and    */
/*    waits for the the processes to complete     */
/*PARAMETER USAGE:                                */
/*    int argc:                                   */
/*      Number of arguments in stdin              */
/*    char **argv:                                */
/*      Array of arguments in stdin               */
/*FUNCTION CALLED:                                */
/*    exit(), printf(), sprintf(), write(),       */
/*    strlen(), atoi(), fork(), fibonacci(),      */
/*    buffonNeedle(), integration(), bernoulli()  */
/*    wait()
/*------------------------------------------------*/
int main(int argc, char **argv){
  /* Variable declaratons */
  int i, n, r, s, status;
  unsigned long m;
  pid_t pid;
  /* printf is not reliable when printing variables
  concurrently, so we will use write() and a character
  buffer instead */
  char buffer[BUFFER_SIZE];

  /* Sanity check for incorrect number of variables */
  if(argc > 5 || argc < 5){
    printf("Incorrect number of arguments!\n");
    exit(0);
  }

  /* Let user know that the main process has started */
  sprintf(buffer, "Main Process Started\n");
  write(1, buffer, strlen(buffer));

  /* Store arguments from cli */
  m = (unsigned long)atof(argv[1]);
  n = atoi(argv[2]);
  r = atoi(argv[3]);
  s = atoi(argv[4]);

  /* Print out the variables */
  sprintf(buffer, "Buffons Needle Iterations =  %d\n", r);
  write(1, buffer, strlen(buffer));
  sprintf(buffer, "Integration Iterations =     %d\n", s);
  write(1, buffer, strlen(buffer));
  sprintf(buffer, "Approx. e iterations =       %lu\n", m);
  write(1, buffer, strlen(buffer));

  /* Concurrent section */

  for(i = 0; i < 4; i ++){
    if((pid = fork()) == 0){  /* We are a child process */
      switch (i){
        case 0:  /* Fibonacci Process */
          sprintf(buffer, "Fibonacci Process Created\n");
          write(1, buffer, strlen(buffer));
          fibonacci(n);
          break;
        case 1:  /* Buffons Needle Process */
          sprintf(buffer, "Buffons Needle Process Created\n");
          write(1, buffer, strlen(buffer));
          buffonNeedle(r);
          break;
        case 2:  /* Integration Process */
          sprintf(buffer, "Integration Process Created\n");
          write(1, buffer, strlen(buffer));
          integration(s);
          break;
        case 3:  /* Approximation of e Process */
          sprintf(buffer, "Approximation of e Process Created\n");
          write(1, buffer, strlen(buffer));
          bernoulli(m);
          break;
      }
    }
    else if(pid < 0){
      sprintf(buffer, "***ERROR: PROCESS FAILED TO BE FORKED***\n");
      write(1, buffer, strlen(buffer));
    }
    else{  /* We are the parent process */
      continue;
    }
  }

  /* Wait for the chil processes to finish */
  sprintf(buffer, "Main Process Waits\n");
  write(1, buffer, strlen(buffer));
  for(i = 0; i < 4; i ++){
    wait(&status);
  }

  sprintf(buffer, "Main Process Exits\n");
  write(1, buffer, strlen(buffer));
  exit(0);
  return 0;
}
