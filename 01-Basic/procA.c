#include <stdio.h>
#include <stdlib.h>

#define  LIMIT  (20)
int main(void)
{
     int i, j, x, y;

     srand(time(NULL));
     for (j = 1; j <= LIMIT; j++) {
          x = rand()/10;
          for (i = 1; i <= x; i++)
               y = rand();
          printf("Hi, A here!  Random number = %d\n", x);
     }
     printf("A completes\n");
}

