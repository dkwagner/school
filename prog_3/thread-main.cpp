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
#include "ThreadClass.h"

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
  cout << "Concurrent Even-Odd Sort\n\n";  // Signal Program beginning

  // Define variables
  int length, temp, i, count, iteration;
  bool swapped = true;
  char spacing[] = "    ";
  cin >> length;  // Read in the length of the array

  int arr[length];  // Define the array
  cout << "Number of input data = " << length << "\n";
  cout << "Input Array: \n  ";

  Pass* passes[length];

  // Read in array
  for(i = 0; i < length; i ++){
    cin >> arr[i];
    cout << arr[i] << " ";
  }
  cout << "\n";

  iteration = 0;
  while(swapped){
    swapped = false;
    count = 0;
    iteration += 1;
    cout << "Iteration " << iteration << ": \n";

    cout << spacing << "Odd Pass: \n";
    // Perform Odd Pass
    for(i = 1; i < length; i += 2){
      if(i >= length){  // Break if out of bounds
        break;
      }
      passes[count] = new Pass(i, arr, &swapped, i); // Create new passes
      passes[count]->Begin();
      count ++;  // For sanity checks when joining threads
    }

    // Join (wait) for all threads
    for(i = 0; i < count; i ++){
      passes[i]->Join();
    }

    cout << spacing << "Even Pass: \n";
    // Perform Even pass
    count = 0;
    for(i = 2; i < length; i += 2){
      passes[count] = new Pass(i, arr, &swapped, i); // Create new passes
      passes[count]->Begin();
      count ++;  // For sanity checks when joining threads
    }

    // Join (wait) for all threads
    for(i = 0; i < count; i ++){
      passes[i]->Join();
    }

    // Print new array
    cout << "Result after iteration " << iteration << ": \n  ";
    for(i = 0; i < length; i ++){
      cout << arr[i] << " ";
    }
    cout << "\n";

  }

  // Exit function for ThreadMentor purposes
  Exit();
}
