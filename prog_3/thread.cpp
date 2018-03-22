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
Pass::Pass(int i, int a[], bool s, int n){
  index = i;
  arr = a;
  swapped = s;
  ThreadName = n;

}

//------------------------------------------------------------------------------
// FUNCTION: Pass::ThreadFunc():
//  Perform a comparison on x[i] and x[i-1], if they are out of order, swap
//  them. IMPORTANT: This does not check for array out of bounds whatsoever.
//  Remember to do these error checks on thread-main.cpp
// PARAMETER USAGE:
//  n/a
// FUNCTION CALLED:
//
//------------------------------------------------------------------------------
void Pass::ThreadFunc(){
  Thread::ThreadFunc();  // Thread Mentor required line

  // Variable Declarations
  char spacing[9] = "        ";

  // Declare thread as created
  cout << spacing << "Thread " << ThreadName << " created\n";

  // Declare what the thread is handling
  cout << spacing << "Thread " << ThreadName << " compares x[" << index - 1 << "] and x[" << index << "]\n";

  // Check to see if the elements are out of order
  // If so, swap them
  if(arr[index - 1] > arr[index]){
    temp = arr[index - 1];
    arr[index - 1] = arr[index];
    arr[index] = temp;
    swapped = true;
    cout << spacing << "Thread " << ThreadName << " swaps x[" << index - 1 << "] and x[" << index << "]\n";
  }

  // Alert user to thread exiting
  cout << spacing << "Thread " << ThreadName << " exits\n";
  Exit();
}
