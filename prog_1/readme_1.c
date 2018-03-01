int main(int argc, char **argv)
{
     int i, n = atoi(argv[1]);

     for (i = 1; i < n; i++)
          if (fork())
               break;
     printf("Process %ld with parent %ld\n", getpid(), getppid());
     sleep(1);
     return 0;
}
