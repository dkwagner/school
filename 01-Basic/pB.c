#include  <stdio.h>

int  main(void)
{
     int   i, LIMIT;
     char  input[100];
   
     gets(input);   // read a complete input line
     LIMIT = atoi(input); // convert to integer
     for (i = 1; i <= LIMIT; i++) {  // repeat
          gets(input);    // read a complete input line
          printf("     From B: %s\n", input);
     }
}

