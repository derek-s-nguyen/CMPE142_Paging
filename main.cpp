#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include "pageTable.hpp"


using namespace std;

pageTable table[20];
pageTable swap[20];
int main(){
    //taking in data from file 
    
	int PID;//Process ID
	int VA; //Virtual Address
	char status; //Process Action
	int processActionCounter = 0;//# of instructions
	int newProcessIndex = 0;//index where new process will be created 
	int accessTimeStamp = 0;//determine when page was accessed
	Process Processes[50];//array of processes

	
    	ifstream in_stream;
    	in_stream.open("memory.dat");

	while(!in_stream.eof()){
    		in_stream >> PID;
		cout << PID;
    		in_stream >> status;
		cout << status;
		if(status == 'A' || status == 'W' || status == 'F' || status == 'R'){
    			in_stream >> VA;
			cout << VA;
		}
 		cout << "\t";
		processActionCounter++;
		cout << processActionCounter;
		cout << "\n";
		
		if(status == 'C'){//create the process in processes array 
			Processes[newProcessIndex].PID = PID;
			Processes[newProcessIndex].isCreated = true; 
		}
		if(status == 'A'){//allocate page for PID
			//find process with PID
			//1(a).If Process has no pages
			//use pageTable ptr in process object to make dynamic pagetable array
			//assign VA and PA
			//
			//1(b).If Process already has pages 
			//find free space in page table and assign VA and PA
			//
			//2. place page in physical PageTable (PA and VA)
			//3. change index to next free page in the physical pages 	
		}
		if(status == 'W'){//write into page for PID
			//locate process with PID in the processes array 
			//find page in process page table 
			//make dirty teehee ;)
		}
		if(status == 'R'){//Read from page
			//locate process with PID in process array
			//find page in process page table 
			//set accessTimeStamp 
		}
		if(status == 'F'){//free page 
			//locate process with PID in process array
			//find page in process page table
			//free page in process page table 
			//free page in physical pages (using PA found page)
		}
		if(status == 'T'){//Terminate process
			//locate process with PID in process array
			//set isTerminated to true 
			//delete array (deallocate)
			//remove physical pages (reset them or whatever)
		}
		
		
	}

	
}
