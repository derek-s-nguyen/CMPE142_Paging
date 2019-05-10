#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include "pageTable.hpp"


using namespace std;

pageTable physicalPages[20];
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
	int AllocateIndex = 0;
	int PA;
	

	
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
			newProcessIndex++;
			//Processes[newProcessIndex].pages = NULL;
			cout<<Processes[AllocateIndex].pages<<endl;
		
		}
		
		if(status == 'A'){//allocate page for PID
			//find process with PID
			PA = 0;
			cout<<Processes[AllocateIndex].pages<<endl;
			while(physicalPages[PA].isAllocated) PA++; //find free page
			cout<<"PA found"<<endl;
			for(int i = 0; i < 50; i++){
				if(Processes[i].PID == PID){
					cout<< "Process found"<<endl;
					if(Processes[AllocateIndex].pages == NULL){ 
						Processes[i].pages = new pageTable[20];
						cout<<"page Table pointed successfully"<<endl;
						Processes[i].pages[VA].virtualAddress = VA;
						Processes[i].pages[VA].physicalAddress = PA;
						Processes[i].pages[VA].isAllocated = true;
				
					}
					else{	
						
						cout<<"process has already page table so adding PTE"<<endl;
						Processes[i].pages[VA].virtualAddress = VA;
						Processes[i].pages[VA].physicalAddress = PA;
						Processes[i].pages[VA].isAllocated = true;		
					}
	
					physicalPages[PA].processID = PID;
					physicalPages[PA].virtualAddress = VA;
					physicalPages[PA].physicalAddress = PA;
					break;
				}
			}

			
		
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
			for(int i = 0; i < 50; i++){
				if(Processes[i].PID == PID){
					Processes[i].pages[VA].dirty = true;
					physicalPages[Processes[i].pages[VA].physicalAddress].dirty = true;	
					break;
				}
			}

			//find page in process page table 
			//make dirty teehee ;)
		}
		if(status == 'R'){//Read from page
			//locate process with PID in process array
			for(int i = 0; i < 50; i++){
				if(Processes[i].PID == PID){
					Processes[i].pages[VA].accessed = accessTimeStamp;
					physicalPages[Processes[i].pages[VA].physicalAddress].accessed = accessTimeStamp;
					accessTimeStamp++;	
					break;
				}
			}
			//find page in process page table 
			//set accessTimeStamp 
		
		}
		if(status == 'F'){//free page 
			//locate process with PID in process array
			for(int i = 0; i < 50; i++){
				if(Processes[i].PID == PID){
					Processes[i].pages[VA].isAllocated = false;
					physicalPages[Processes[i].pages[VA].physicalAddress].isAllocated = false;
					physicalPages[Processes[i].pages[VA].physicalAddress].virtualAddress = 0;
					physicalPages[Processes[i].pages[VA].physicalAddress].dirty = false;
					physicalPages[Processes[i].pages[VA].physicalAddress].accessed = 0;

					break;
				}
			}
			//find page in process page table
			//free page in process page table 
			//free page in physical pages (using PA found page)
			cout<<physicalPages[PA];
		}
		if(status == 'T'){//Terminate process
			//locate process with PID in process array
			//set isTerminated to true 
			//delete array (deallocate)
			//remove physical pages (reset them or whatever)
		}
		
		
	}
	
	
	
}
