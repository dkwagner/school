//------------------------------------------------------------------------------
// NAME: Daniel Wagner                       User ID: dkwagner
// DUE DATE: 03/23/2018
// PROGRAM ASSIGNMENT #3
// FILE NAME: thread-main.cpp
// PROGRAM PURPOSE:
//    To learn the basics of thread mentor, and create a multithreaded
//    approach to the even odd sorting algorithm
//------------------------------------------------------------------------------

#include <iostream>
#include "thread.h"

using namespace std;

//------------------------------------------------------------------------------
// FUNCTION: main():
//    Main program, read in array, and then create multiple threads to
//    do the even odd sorting algorithm concurrently
// PARAMETER USAGE:
//    n/a
// FUNCTION CALLED:
//------------------------------------------------------------------------------
int main(){
  int length, temp, i;
  cin >> length;  // Read in the length of the array
  int arr[length];  // Define the array

  for(i = 0; i < length; i ++){
    cin >> arr[i];
    cout << arr[i] << " ";
  }
  cout << "\n";

}
