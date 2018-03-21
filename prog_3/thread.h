//------------------------------------------------------------------------------
// NAME: Daniel Wagner                       User ID: dkwagner
// DUE DATE: 03/23/2018
// PROGRAM ASSIGNMENT #3
// FILE NAME: thread.h
// PROGRAM PURPOSE:
//    Define the derived class for thread mentor and functions/variables
//    for thread.cpp
//------------------------------------------------------------------------------

#include "ThreadClass.h"

class Pass : public Thread
{
  public:
    Pass(int l, int r, int a[], bool s, int n);
  private:
    int left, right;
    int ThreadName;
    int *arr;
    bool swapped;
    void ThreadFunc();
};
