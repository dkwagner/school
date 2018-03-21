//------------------------------------------------------------------------------
// NAME: Daniel Wagner                       User ID: dkwagner
// DUE DATE: 03/23/2018
// PROGRAM ASSIGNMENT #3
// FILE NAME: thread.cpp
// PROGRAM PURPOSE:
//    Serve as a derived thread. Used to perform basic swaps for the even odd
//    sorting algorithm
//------------------------------------------------------------------------------

#include <iostream>
#include "thread.h"

using namespace std;

//------------------------------------------------------------------------------
// FUNCTION:
//
// PARAMETER USAGE:
//
// FUNCTION CALLED:
//
//------------------------------------------------------------------------------
Pass::Pass(int l, int r, int a[], bool s, int n){
  left = l;
  right = r;
  arr = a;
  swapped = s;
  ThreadName = n;

}

//------------------------------------------------------------------------------
// FUNCTION:
//
// PARAMETER USAGE:
//
// FUNCTION CALLED:
//
//------------------------------------------------------------------------------
void Pass::ThreadFunc(){
  Thread::ThreadFunc();  // Thread Mentor required line

  // Variable Declarations
  char spacing[9] = "        ";

  // Declare thread as created
  cout << spacing << "Thread " << ThreadName << " created\n";

  Exit();
}
