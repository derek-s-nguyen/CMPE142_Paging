#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include "pageTable.hpp"

using namespace std;

pageTable physicalPages[20];
pageTable swapSpace[40];
Process Processes[50]; //array of processes

int main()
{
    //taking in data from file

    int PID; //Process ID
    int VA; //Virtual Address
    char status; //Process Action
    int processActionCounter = 0; //# of instructions
    int accessTimeStamp = 1; //determine when page was accessed
    int PA;
    int swapSpaceIndex = 0;
    int 
    ifstream in_stream;
    in_stream.open("memory.dat");

    while (in_stream >> PID)
    {

        cout << PID<<"\t";
        in_stream >> status;
        cout << status<<"\t";
        if (status == 'A' || status == 'W' || status == 'F' || status == 'R')
        {
            in_stream >> VA;
            cout << VA;
        }
        cout << "\t";
        processActionCounter++;
        cout << processActionCounter;
        cout << "\n";

        if (status == 'C')
        { //create the process in processes array
	    int newProcessIndex; //index where new process will be created
	    bool terminatedProcessFound = false;
	    for(newProcessIndex = 0; newProcessIndex < 50; newProcessIndex++){

		if(Processes[newProcessIndex].PID == PID && Processes[newProcessIndex].isTerminated){terminatedProcessFound = true; break;}
		

	    }

	    if(!terminatedProcessFound){
		for(newProcessIndex = 0; newProcessIndex < 50; newProcessIndex++){

			if(!Processes[newProcessIndex].isCreated) break;
		}

	    }
            Processes[newProcessIndex].PID = PID;
            Processes[newProcessIndex].isCreated = true;
	    Processes[newProcessIndex].isTerminated = false;
            //Processes[newProcessIndex].pages = NULL;

        }

        if (status == 'A')
        { //allocate page for PID
          //find process with PID
            PA = 0;
            swapSpaceIndex = 0;
            bool needSwapAlgo = false;
            bool swapHappens = false;

            for(PA = 0; PA < 20; PA++)
            {
                //find free page
		if(!physicalPages[PA].isAllocated) break;//found free page then break and pass PA to allocation
		if(PA == 19 && physicalPages[PA].isAllocated) swapHappens = true;//all pages are taken

            }

	    if(swapHappens){//need to swap
		for(PA = 0; PA < 20; PA++){//cycle through physical pages
		if(!physicalPages[PA].dirty) break;//if page clean, pass PA to allocation
		if(PA == 19 && physicalPages[PA].dirty) needSwapAlgo = true;//all pages are dirty
		}
	    
	    }
	    
            if(needSwapAlgo){
	     cout<<"need swap algo"<<endl;
	     //will find PA using the swap algo
            }
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID)
                {

                    if (swapHappens)
                    {
                        //find an empty page in swap space
                        while (swapSpace[swapSpaceIndex].isAllocated)
                        {
                            swapSpaceIndex++;
                        }
                        //copy page into swap space
                        swapSpace[swapSpaceIndex].processID =
                                physicalPages[PA].processID;
                        swapSpace[swapSpaceIndex].virtualAddress =
                                physicalPages[PA].virtualAddress;
                        swapSpace[swapSpaceIndex].isAllocated = true;
                        swapSpace[swapSpaceIndex].dirty =
                                physicalPages[PA].dirty;
                        swapSpace[swapSpaceIndex].accessed =
                                physicalPages[PA].accessed;

                        for (int processToSwap = 0; processToSwap < 50;
                                processToSwap++)
                        { //set the PTE to 'S'
                            if (Processes[processToSwap].PID
                                    == physicalPages[PA].processID)
                            {
                                Processes[processToSwap].pages[physicalPages[PA].virtualAddress].inSwapSpace =
                                        true;
                                break;
                            }
                        }
                        //indicate if in swap space in page table
                    }
                    if (Processes[i].pages == NULL)
                    {
                        Processes[i].pages = new pageTable[1000];
                        Processes[i].pages[VA].virtualAddress = VA;
                        Processes[i].pages[VA].physicalAddress = PA;
                        Processes[i].pages[VA].isAllocated = true;

                    }
                    else
                    {
                    
                        Processes[i].pages[VA].virtualAddress = VA;
                        Processes[i].pages[VA].physicalAddress = PA;
                        Processes[i].pages[VA].isAllocated = true;
                    }

                    physicalPages[PA].processID = PID;
                    physicalPages[PA].virtualAddress = VA;
                    physicalPages[PA].physicalAddress = PA;
                    physicalPages[PA].isAllocated = true;
                    break;
                }
            }

            //not sure where to include this, but i added some code that will consider the case where process tries to access a page that is not allocated
//           i commented this out because the output wouldn't show up, so needs some debugging
            //for (int i = 0; i < 50; i++)
//            {
//                if (Processes[i].PID == PID
//                        && !physicalPages[PA].isAllocated == false
//                        && Processes[i].pages[VA].isAllocated == false)
//                {
//                    delete Processes[i].pages;
//                    for (int i = 0; i < 20; i++)
//                    {
//
//                        if (physicalPages[i].processID == PID)
//                        {
//                            physicalPages[i].isAllocated = false;
//                            physicalPages[i].virtualAddress = 0;
//                            physicalPages[i].dirty = false;
//                            physicalPages[i].accessed = 0;
//                            physicalPages[i].processID = 0;
//
//                        }
//                        break;
//                    }
//                }

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
        if (status == 'W')
        { //write into page for PID
            bool writeSwap = false;
	    bool writeSwapWithAlgo = false;
	    swapSpaceIndex = 0;
	    int pageFoundInSwap = 0;

            //locate process with PID in the processes array and write into that page
	    
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID){   
		   if(!Processes[i].pages[VA].isAllocated){
                           //page trying to write to is not allocated
		     for (int killIndex = 0; killIndex < 20; killIndex++){
                             //search for process pages in memory to kill
               		 if (physicalPages[killIndex].processID == PID){
                    		physicalPages[killIndex].isAllocated = false;
                    		physicalPages[killIndex].virtualAddress = 0;
                    		physicalPages[killIndex].dirty = false;
                    		physicalPages[killIndex].accessed = 0;
                    		physicalPages[killIndex].processID = 0;
                	}
            	     }
		     for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){
                             //search for process pages in swap space to kill
			if(swapSpace[killSwapIndex].processID == PID){
				swapSpace[killSwapIndex].isAllocated = false;
                    		swapSpace[killSwapIndex].virtualAddress = 0;
                    		swapSpace[killSwapIndex].dirty = false;
                    		swapSpace[killSwapIndex].accessed = 0;
                    		swapSpace[killSwapIndex].processID = 0;
			}
		     }
               	     //finds Process
                    Processes[i].isCreated = false; //Not allocated anymore
                    Processes[i].isTerminated = true; //deemed terminated
                    Processes[i].pages = NULL; //page table ptr points to NULL
                    break;
		   }

		   //if page wants to be writen to but is located in swap
		   if(Processes[i].pages[VA].isAllocated && Processes[i].pages[VA].inSwapSpace){
			 //find process in swap space
			for(pageFoundInSwap = 0; pageFoundInSwap < 40; pageFoundInSwap++){
				if(swapSpace[pageFoundInSwap].processID == PID && swapSpace[pageFoundInSwap].virtualAddress == VA)break;
			}
			 //find free space in swap
			while (swapSpace[swapSpaceIndex].isAllocated && swapSpaceIndex < 40)
                        {
                            swapSpaceIndex++;
                        }
			
                        //look for clean pages in physical memory
			for(int swapPageIndex = 0; swapPageIndex < 20; swapPageIndex++){
				if(!physicalPages[swapPageIndex].dirty){ 
					writeSwapWithAlgo = false;
                                        //tried implementing the swap here before breaking the loop
                                        
                                        //adds the contents of page from physical memory into the free page in swap
                                        swapSpace[pageFoundInSwap].dirty =true;
                                        swapSpace[pageFoundInSwap].processID = physicalPages[swapPageIndex].processID;
                                        swapSpace[pageFoundInSwap].physicalAddress = physicalPages[swapPageIndex].physicalAddress;
                                        swapSpace[pageFoundInSwap].virtualAddress = physicalPages[swapPageIndex].virtualAddress;
                                        swapSpace[pageFoundInSwap].inSwapSpace = true;

                                        //copying desired process into physical page of memory
                                        physicalPages[swapPageIndex].dirty = true;
                                        physicalPages[swapPageIndex].processID = swapSpace[swapSpaceIndex].processID; 
                                        physicalPages[swapPageIndex].inSwapSpace = false;
                                        physicalPages[swapPageIndex].isAllocated = true;
                                        physicalPages[swapPageIndex].physicalAddress = swapSpace[swapSpaceIndex].physicalAddress;
                                        physicalPages[swapPageIndex].virtualAddress = swapSpace[swapSpaceIndex].virtualAddress;
                                        
                                        //cleaning out one page -- not to confuse ourselves and the swap is done
                                        swapSpace[swapSpaceIndex].isAllocated = false;
                    		        swapSpace[swapSpaceIndex].virtualAddress = 0;
                    		        swapSpace[swapSpaceIndex].dirty = false;
                    		        swapSpace[swapSpaceIndex].accessed = 0;
                    		        swapSpace[swapSpaceIndex].processID = 0;

					break;
				}
			}

 			if(writeSwapWithAlgo){
				cout<<"need swap policy"<<endl;
		
		   	}
			/*	//copying physical page into swap space 
				  swapSpace[swapSpaceIndex].processID =
                                physicalPages[swapPageIndex].processID;
                        swapSpace[swapSpaceIndex].virtualAddress =
                                physicalPages[swapPageIndex].virtualAddress;
                        swapSpace[swapSpaceIndex].isAllocated = true;
                        swapSpace[swapSpaceIndex].dirty =
                                physicalPages[swapPageIndex].dirty;

                        physicalPages[swapPageIndex].processID = swapSpace[pageFoundInSwap].processID;
			physicalPages[swapPageIndex].virtualAddress = swapSpace[pageFoundInSwap].virtualAddress;
			physicalPages[swapPageIndex].inSwapSpace = true;
			physicalPages[swapPageIndex].virtualAddress = swapSpace[pageFoundInSwap].virtualAddress;
			*/



			
		   }
		  

		   else{
                    	Processes[i].pages[VA].dirty = true; //make dirty teehee ;)
                    	physicalPages[Processes[i].pages[VA].physicalAddress].dirty =
                            true;
                    	physicalPages[Processes[i].pages[VA].physicalAddress].accessed =
                            accessTimeStamp;
                    	accessTimeStamp++;
                   	break;
		   }
                }
            }
            
        }

        if (status == 'R')
        { //Read from page
          //locate process with PID in process array

          bool needSwapForReadAlgo = false;
          int pageFoundInSwap = 0;
            for (int i = 0; i < 50; i++)
            {
                   if(Processes[i].PID == PID){   
		   	if(!Processes[i].pages[VA].isAllocated){
                                //trying read a page that is not allocated
		     		for (int killIndex = 0; killIndex < 20; killIndex++){
                                        //search for process pages in memory to kill
               		 		if (physicalPages[killIndex].processID == PID){
                    				physicalPages[killIndex].isAllocated = false;
                    				physicalPages[killIndex].virtualAddress = 0;
                    				physicalPages[killIndex].dirty = false;
                    				physicalPages[killIndex].accessed = 0;
                    				physicalPages[killIndex].processID = 0;
                			}
            	     		}
		     		for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){
                                             //search for process pages in swap space to kill
					if(swapSpace[killSwapIndex].processID == PID){
						swapSpace[killSwapIndex].isAllocated = false;
                    				swapSpace[killSwapIndex].virtualAddress = 0;
                    				swapSpace[killSwapIndex].dirty = false;
                    				swapSpace[killSwapIndex].accessed = 0;
                    				swapSpace[killSwapIndex].processID = 0;
					}
		     		}
               	     		//finds Process
                    		Processes[i].isCreated = false; //Not allocated anymore
                    		Processes[i].isTerminated = true; //deemed terminated
                    		Processes[i].pages = NULL; //page table ptr points to NULL
                    		break;
			}

                        //if page wants to be read to but is located in swap
		   if(Processes[i].pages[VA].isAllocated && Processes[i].pages[VA].inSwapSpace){
			 //find process in swap space
			for(pageFoundInSwap = 0; pageFoundInSwap < 40; pageFoundInSwap++){
				if(swapSpace[pageFoundInSwap].processID == PID && swapSpace[pageFoundInSwap].virtualAddress == VA)break;
			}
			 //find free space in swap
			while (swapSpace[swapSpaceIndex].isAllocated && swapSpaceIndex < 40)
                        {
                            swapSpaceIndex++;
                        }
			
                        //look for clean pages in physical memory
			for(int swapPageIndex = 0; swapPageIndex < 20; swapPageIndex++){
				if(!physicalPages[swapPageIndex].dirty){ 
					needSwapForReadAlgo = false;
                                        //tried implementing the swap here before breaking the loop
                                        swapSpace[pageFoundInSwap].dirty = true;
                                        swapSpace[pageFoundInSwap].processID = physicalPages[swapPageIndex].processID;
                                        swapSpace[pageFoundInSwap].physicalAddress = physicalPages[swapPageIndex].physicalAddress;
                                        swapSpace[pageFoundInSwap].virtualAddress = physicalPages[swapPageIndex].virtualAddress;
                                        swapSpace[pageFoundInSwap].inSwapSpace = true;

                                        physicalPages[swapPageIndex].dirty = true;
                                        physicalPages[swapPageIndex].processID = swapSpace[swapSpaceIndex].processID; 
                                        physicalPages[swapPageIndex].inSwapSpace = false;
                                        physicalPages[swapPageIndex].isAllocated = true;
                                        physicalPages[swapPageIndex].physicalAddress = swapSpace[swapSpaceIndex].physicalAddress;
                                        physicalPages[swapPageIndex].virtualAddress = swapSpace[swapSpaceIndex].virtualAddress;
					

                                        //cleaning out one page -- not to confuse ourselves and the swap is done
                                        swapSpace[swapSpaceIndex].isAllocated = false;
                    		        swapSpace[swapSpaceIndex].virtualAddress = 0;
                    		        swapSpace[swapSpaceIndex].dirty = false;
                    		        swapSpace[swapSpaceIndex].accessed = 0;
                    		        swapSpace[swapSpaceIndex].processID = 0;


                                        break;
				}
			}

 			if(needSwapForReadAlgo){
				cout<<"need swap policy"<<endl;
		
		   	}
			else{
                    		Processes[i].pages[VA].accessed = accessTimeStamp;
                    		physicalPages[Processes[i].pages[VA].physicalAddress].accessed =
                            	accessTimeStamp;
                    		accessTimeStamp++;
                    		break;
		   	}

		   }
		   
                
            }
            }

        }

        if (status == 'F')
        { //free page
          //locate process with PID in process array
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID
                        && Processes[i].pages[VA].isAllocated == true)
                {
                    Processes[i].pages[VA].isAllocated = false;
                    Processes[i].pages[VA].isFreed = true;
                    physicalPages[Processes[i].pages[VA].physicalAddress].isAllocated =
                            false;
                    physicalPages[Processes[i].pages[VA].physicalAddress].virtualAddress =
                            0;
                    physicalPages[Processes[i].pages[VA].physicalAddress].dirty =
                            false;
                    physicalPages[Processes[i].pages[VA].physicalAddress].accessed =
                            0;
		     physicalPages[Processes[i].pages[VA].physicalAddress].processID =
                            0;

                    break;
                }
                
                else if(Processes[i].PID == PID && !Processes[i].pages[VA].isAllocated){
                       
                                //trying to free a page that is not allocated
		     		for (int killIndex = 0; killIndex < 20; killIndex++){
                                        //search for process pages in memory to kill
               		 		if (physicalPages[killIndex].processID == PID){
                    				physicalPages[killIndex].isAllocated = false;
                    				physicalPages[killIndex].virtualAddress = 0;
                    				physicalPages[killIndex].dirty = false;
                    				physicalPages[killIndex].accessed = 0;
                    				physicalPages[killIndex].processID = 0;
                			}
            	     		}
		     		for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){
                                             //search for process pages in swap space to kill
					if(swapSpace[killSwapIndex].processID == PID){
						swapSpace[killSwapIndex].isAllocated = false;
                    				swapSpace[killSwapIndex].virtualAddress = 0;
                    				swapSpace[killSwapIndex].dirty = false;
                    				swapSpace[killSwapIndex].accessed = 0;
                    				swapSpace[killSwapIndex].processID = 0;
					}
		     		}
               	     		//finds Process
                    		Processes[i].isCreated = false; //Not allocated anymore
                    		Processes[i].isTerminated = true; //deemed terminated
                    		Processes[i].pages = NULL; //page table ptr points to NULL
                    		break;
			
                }
        }
         //need to consider if user wants to free process in swap space
        for(int freeSwap = 0; freeSwap < 40; freeSwap++){
                if(swapSpace[freeSwap].processID==PID){
                        swapSpace[freeSwap].isAllocated = false;
                        swapSpace[freeSwap].isFreed = true;
                        swapSpace[freeSwap].accessed = 0;
                        swapSpace[freeSwap].processID = 0;
                        swapSpace[freeSwap].virtualAddress = 0;
                        swapSpace[freeSwap].physicalAddress = 0;
                        break;
                }



//                    else if (Processes[i].PID == PID
//                            && Processes[i].pages[VA].isAllocated == false)
//                    {
//                        delete Processes[i].pages;
//                        break;
//                    } //deletes the page table assigned to the process since it tries to free an allocated page
            }

            //find page in process page table
            //free page in process page table
            //free page in physical pages (using PA found page)

        }

        if (status == 'T')
        {
            //Terminate process
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID
                        && Processes[i].pages[VA].isAllocated == true)
                {
                    //finds Process
                    Processes[i].isCreated = false; //Not allocated anymore
                    Processes[i].isTerminated = true; //deemed terminated
                    Processes[i].pages = NULL; //page table ptr points to NULL
                    break;
                }
//                    else if (Processes[i].PID == PID
//                            && Processes[i].pages[VA].isAllocated == false)
//                    {
//                        delete Processes[i].pages;
//                        break;
//                    } //deletes the page table assigned to the process since it tries to free an allocated page and reset everything
            }

            //dealing with the physical memory
            for (int i = 0; i < 20; i++){

                if (physicalPages[i].processID == PID)
                {
                    physicalPages[i].isAllocated = false;
                    physicalPages[i].virtualAddress = 0;
                    physicalPages[i].dirty = false;
                    physicalPages[i].accessed = 0;
                    physicalPages[i].processID = 0;

                }
            }
	    //dealing with swap space
	    for(int i = 0; i < 40; i++){
		if(swapSpace[0].processID == PID){

	            physicalPages[i].isAllocated = false;
                    physicalPages[i].virtualAddress = 0;
                    physicalPages[i].dirty = false;
                    physicalPages[i].accessed = 0;
                    physicalPages[i].processID = 0;

		}


	    

            //locate process with PID in process array
            //set isTerminated to true
            //delete array (deallocate)
            //remove physical pages (reset them or whatever)

            //now trying to do swap by looking for allocated pages

            //case: if a process tries to swap with an allocated page
        }

    }
    //Print out physical memory
    cout<<"PHYSICAL MEMORY"<<endl;
    cout << "PID\t" << "VA\t" << "PA\t" << "Dirty?\t" << "Access\t" << endl;
    cout << "___________________________________________________________"
            << endl;
    for (int i = 0; i < 20; i++)
    {

        cout << physicalPages[i].processID << "\t"
                << physicalPages[i].virtualAddress << "\t" << i << "\t";
        if (physicalPages[i].dirty == true)
            cout << "Yes" << "\t" << physicalPages[i].accessed << endl;
        if (physicalPages[i].dirty == false)
            cout << "No" << "\t" << physicalPages[i].accessed << endl;

    }
    //Print out swap
    cout<<"SWAP SPACE"<<endl;
    cout << "PID\t" << "VA\t" << "Dirty?\t" << "Access\t" << endl;
    cout << "___________________________________________________________"<<endl;
    for (int i = 0; i < 20; i++)
    {
        cout << swapSpace[i].processID << "\t"
                << swapSpace[i].virtualAddress << "\t";
        if (swapSpace[i].dirty == true)
            cout << "Yes" << "\t" << physicalPages[i].accessed << endl;
        if (swapSpace[i].dirty == false)
            cout << "No" << "\t" << physicalPages[i].accessed << endl;
    }

}

