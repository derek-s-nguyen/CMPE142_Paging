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
    int swapPageIndex = 0;
    int pageFoundInSwap = 0;
    int swapPolicy = 0;
    int FIFOcounter = 1;
    ifstream in_stream;
    in_stream.open("memory.dat");

    while(swapPolicy != 1 || swapPolicy != 2 || swapPolicy != 3){
	cout<<"Select One of The Policies"<<endl;
	cout<<"Type '1' for Random"<<endl;
	cout<<"Type '2' for FIFO"<<endl;
	cout<<"Type '3' for LRU"<<endl;
	cin >> swapPolicy;
	if(swapPolicy == 1 || swapPolicy == 2 || swapPolicy == 3) break;

    }
    while (in_stream >> PID)
    {
	//cout<<PID<<"\t";

        in_stream >> status;
	//cout<<status<<"\t";

        if (status == 'A' || status == 'W' || status == 'F' || status == 'R')
        {
            in_stream >> VA;
	    //cout<<"VA"<<endl;

        }


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

		//random
	     	if(swapPolicy == 1){
			PA = rand() % 20 + 0;
		}
		//FIFO
		if(swapPolicy == 2){
			int pageWithLeastAccessed = 0;
			for(int i = 0; i < 20; i++){
				if(physicalPages[i].FIFOcount < physicalPages[pageWithLeastAccessed].FIFOcount){
					pageWithLeastAccessed = i;
					break;
				}
			}
			swapPageIndex = pageWithLeastAccessed;
		}
		//LRU
		if(swapPolicy == 3){
			int pageWithLeastAccessed = 0;
			for(int i = 0; i < 20; i++){
				if(physicalPages[i].accessed < physicalPages[pageWithLeastAccessed].accessed){
					pageWithLeastAccessed = i;
					break;
				}
			}
			swapPageIndex = pageWithLeastAccessed;
		}


            }
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID)
                {

                    if(swapHappens)
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
			Processes[i].pages[VA].FIFOcount = FIFOcounter;
			FIFOcounter++;
                    }

                    physicalPages[PA].processID = PID;
                    physicalPages[PA].virtualAddress = VA;
                    physicalPages[PA].physicalAddress = PA;
                    physicalPages[PA].isAllocated = true;
		    physicalPages[PA].FIFOcount = FIFOcounter;
		    FIFOcounter++;
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
        if (status == 'W')
        { //write into page for PID
            bool writeSwap = false;
	    bool writeSwapWithAlgo = true;
	    swapSpaceIndex = 0;
	    int pageFoundInSwap = 0;

            //locate process with PID in the processes array and write into that page

            for (int i = 0; i < 50; i++)
            {
                 if(Processes[i].PID == PID){//find process
		   //check if process tries to write to unallocated page
		   if(!Processes[i].pages[VA].isAllocated){

                           //page trying to write to is not allocated
		     for(int killIndex = 0; killIndex < 20; killIndex++){
                             //search for process pages in memory to kill
               		 if(physicalPages[killIndex].processID == PID){
                    		physicalPages[killIndex].isAllocated = false;
                    		physicalPages[killIndex].virtualAddress = 0;
                    		physicalPages[killIndex].dirty = false;
                    		physicalPages[killIndex].accessed = 0;
                    		physicalPages[killIndex].processID = 0;
                	}//if-search for process pages in memory to kill
            	     }//for - search for process pages in memory to kill

		     for(int killSwapIndex = 0; killSwapIndex < 40; killSwapIndex++){
                             //search for process pages in swap space to kill
			if(swapSpace[killSwapIndex].processID == PID){
				swapSpace[killSwapIndex].isAllocated = false;
                    		swapSpace[killSwapIndex].virtualAddress = 0;
                    		swapSpace[killSwapIndex].dirty = false;
                    		swapSpace[killSwapIndex].accessed = 0;
                    		swapSpace[killSwapIndex].processID = 0;
			}//if - //search for process pages in swap space to kill
		     }//for - search for process pages in swap space to kill
               	     //finds Process
                    Processes[i].isCreated = false; //Not allocated anymore
                    Processes[i].isTerminated = true; //deemed terminated
                    Processes[i].pages = NULL; //page table ptr points to NULL
                    break;
		   }//if - check if process tries to write to unallocated page


		   //if page wants to be writen to but is located in swap
		   else if(Processes[i].pages[VA].isAllocated && Processes[i].pages[VA].inSwapSpace){
			 //find process in swap space
			for(pageFoundInSwap = 0; pageFoundInSwap < 40; pageFoundInSwap++){
				if(swapSpace[pageFoundInSwap].processID == PID && swapSpace[pageFoundInSwap].virtualAddress == VA)break;
			}//for loop

			 //find free space in swap
			while (swapSpace[swapSpaceIndex].isAllocated && swapSpaceIndex < 40)
                        {
                            swapSpaceIndex++;
                        }//while loop

                        //look for clean pages in physical memory
			for(swapPageIndex = 0; swapPageIndex < 20; swapPageIndex++){
				if(!physicalPages[swapPageIndex].dirty){
					writeSwapWithAlgo = false;
                                        //tried implementing the swap here before breaking the loop
					break;
				}//if
			}//for


			if(writeSwapWithAlgo){
				cout<<"need swap policy"<<endl;

	     			//will find PA using the swap algo

				//random
	     			if(swapPolicy == 1){
					swapPageIndex = rand() % 20 + 0;
				}
				//FIFO
				if(swapPolicy == 2){
					int pageWithLeastAccessed = 0;
					for(int i = 0; i < 20; i++){
						if(physicalPages[i].accessed < physicalPages[pageWithLeastAccessed].accessed){
							pageWithLeastAccessed = i;
							break;
						}
					}
					swapPageIndex = pageWithLeastAccessed;
				}
				//LRU
				if(swapPolicy == 3){
					int pageWithLeastAccessed = 0;
					for(int i = 0; i < 20; i++){
						if(physicalPages[i].accessed < physicalPages[pageWithLeastAccessed].accessed){
							pageWithLeastAccessed = i;
						}
					}
					swapPageIndex = pageWithLeastAccessed;
				}



		   	}//if

			 //adds the contents of page from physical memory into the free page in swap

                        //swapSpace[swapSpaceIndex].dirty = true;
                        swapSpace[swapSpaceIndex].processID = physicalPages[swapPageIndex].processID;
                        swapSpace[swapSpaceIndex].physicalAddress = physicalPages[swapPageIndex].physicalAddress;
                        swapSpace[swapSpaceIndex].virtualAddress = physicalPages[swapPageIndex].virtualAddress;
                        swapSpace[swapSpaceIndex].inSwapSpace = true;
			    swapSpace[swapSpaceIndex].FIFOcount = 0;

			 for(int k = 0; k < 50; k++){
			 	if(Processes[k].PID == physicalPages[swapPageIndex].processID){

			 			if(Processes[k].pages[physicalPages[swapPageIndex].virtualAddress].virtualAddress == physicalPages[swapPageIndex].virtualAddress){
			 				Processes[k].pages[physicalPages[swapPageIndex].virtualAddress].inSwapSpace = true;
			 				break;

			 		}
			 	}

			}


                         //copying desired process into physical page of memory
                        physicalPages[swapPageIndex].dirty = true;
                        physicalPages[swapPageIndex].processID = swapSpace[pageFoundInSwap].processID;
                        physicalPages[swapPageIndex].inSwapSpace = false;
                        physicalPages[swapPageIndex].isAllocated = true;
                        physicalPages[swapPageIndex].physicalAddress = swapSpace[pageFoundInSwap].physicalAddress;
                        physicalPages[swapPageIndex].virtualAddress = swapSpace[pageFoundInSwap].virtualAddress;
			physicalPages[swapPageIndex].FIFOcount = FIFOcounter;



			for(int i = 0; i < 50; i++){
				if(Processes[i].PID == swapSpace[pageFoundInSwap].processID){
					for(int j = 0; j < 100; j++){
						if(Processes[i].pages[j].virtualAddress == swapSpace[pageFoundInSwap].virtualAddress){
							Processes[i].pages[j].inSwapSpace = false;
							Processes[i].pages[j].physicalAddress = swapPageIndex;
							break;
						}
					}
				}

			}
		        FIFOcounter++;


                                        //cleaning out one page -- not to confuse ourselves and the swap is done
                        swapSpace[pageFoundInSwap].isAllocated = false;
                   	swapSpace[pageFoundInSwap].virtualAddress = 0;
                   	swapSpace[pageFoundInSwap].dirty = false;
                  	swapSpace[pageFoundInSwap].accessed = 0;
                   	swapSpace[pageFoundInSwap].processID = 0;

		   }//else if page wants to be writen to but is located in swap


		   else{
                    	Processes[i].pages[VA].dirty = true; //make dirty teehee ;)
                    	physicalPages[Processes[i].pages[VA].physicalAddress].dirty =
                            true;
                    	physicalPages[Processes[i].pages[VA].physicalAddress].accessed =
                            accessTimeStamp;
                    	accessTimeStamp++;
                   	break;
		   }//else
                }//for
            }//if - write to page


        if (status == 'R')
        { //Read from page
          //locate process with PID in process array

          bool needSwapForReadAlgo = true;
          int pageFoundInSwap = 0;
	  pageFoundInSwap = 0;
	  swapPageIndex = 0;

            for (int i = 0; i < 50; i++)
            {
                   if(Processes[i].PID == PID){
		   	if(!Processes[i].pages[VA].isAllocated){
                                //trying read a page that is not allocated
		     		for(int killIndex = 0; killIndex < 20; killIndex++){
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
			}//if - trying to read a ra

                        //if page wants to be read to but is located in swap
		   else if(Processes[i].pages[VA].isAllocated && Processes[i].pages[VA].inSwapSpace){
			 //find process in swap space
			for(pageFoundInSwap = 0; pageFoundInSwap < 40; pageFoundInSwap++){
				if(swapSpace[pageFoundInSwap].processID == PID && swapSpace[pageFoundInSwap].virtualAddress == VA)break;
			}//for
			 //find free space in swap
			while (swapSpace[swapSpaceIndex].isAllocated && swapSpaceIndex < 40)
                        {
                            swapSpaceIndex++;
                        }//while

                        //look for clean pages in physical memory
			for(int swapPageIndex = 0; swapPageIndex < 20; swapPageIndex++){
				if(!physicalPages[swapPageIndex].dirty){
					needSwapForReadAlgo = false;

                                        break;
				}//if
			}//for

 			if(needSwapForReadAlgo){

				cout<<"need swap policy"<<endl;

	     			//will find PA using the swap algo

				//random
	     			if(swapPolicy == 1){
					swapPageIndex = rand() % 20 + 0;
				}
				//FIFO
				if(swapPolicy == 2){
					int pageWithLeastAccessed = 0;
					for(int i = 0; i < 20; i++){
						if(physicalPages[i].accessed < physicalPages[pageWithLeastAccessed].accessed){
							pageWithLeastAccessed = i;
							break;
						}
					}
					swapPageIndex = pageWithLeastAccessed;
				}
				//LRU
				if(swapPolicy == 3){
					int pageWithLeastAccessed = 0;
					for(int i = 0; i < 20; i++){
						if(physicalPages[i].accessed < physicalPages[pageWithLeastAccessed].accessed){
							pageWithLeastAccessed = i;
						}
					}
					swapPageIndex = pageWithLeastAccessed;
				}

		   	}//if
					//adds the contents of page from physical memory into the free page in swap
			 		//swapSpace[swapSpaceIndex].dirty =true;
                                        swapSpace[swapSpaceIndex].processID = physicalPages[swapPageIndex].processID;
                                        swapSpace[swapSpaceIndex].physicalAddress = physicalPages[swapPageIndex].physicalAddress;
                                        swapSpace[swapSpaceIndex].virtualAddress = physicalPages[swapPageIndex].virtualAddress;
                                        swapSpace[swapSpaceIndex].inSwapSpace = true;
					for(int k = 0; k < 50; k++){
						if(Processes[k].PID == physicalPages[swapPageIndex].processID){

							if(Processes[k].pages[physicalPages[swapPageIndex].virtualAddress].virtualAddress == physicalPages[swapPageIndex].virtualAddress){
								Processes[k].pages[physicalPages[swapPageIndex].virtualAddress].inSwapSpace = true;
								break;

							}
						}

					}
                                        //copying desired process into physical page of memory
                                        //physicalPages[swapPageIndex].dirty = true;
                                        physicalPages[swapPageIndex].processID = swapSpace[pageFoundInSwap].processID;
                                        physicalPages[swapPageIndex].inSwapSpace = false;
                                        physicalPages[swapPageIndex].isAllocated = true;
                                        physicalPages[swapPageIndex].physicalAddress = swapSpace[pageFoundInSwap].physicalAddress;
                                        physicalPages[swapPageIndex].virtualAddress = swapSpace[pageFoundInSwap].virtualAddress;
					physicalPages[swapPageIndex].FIFOcount = FIFOcounter;
					for(int i = 0; i < 50; i++){
						if(Processes[i].PID == swapSpace[pageFoundInSwap].processID){
							for(int j = 0; j < 100; j++){
								if(Processes[i].pages[j].virtualAddress == swapSpace[pageFoundInSwap].virtualAddress){
									Processes[i].pages[j].inSwapSpace = false;
									Processes[i].pages[j].physicalAddress = swapPageIndex;
									break;
								}
							}
						}

					}
		        		FIFOcounter++;


                                        //cleaning out one page -- not to confuse ourselves and the swap is done
                                        swapSpace[pageFoundInSwap].isAllocated = false;
                    		        swapSpace[pageFoundInSwap].virtualAddress = 0;
                    		        swapSpace[pageFoundInSwap].dirty = false;
                    		        swapSpace[pageFoundInSwap].accessed = 0;
                    		        swapSpace[pageFoundInSwap].processID = 0;

			}//else if- page wants to be read to but is located in swap
			else{
                    		Processes[i].pages[VA].accessed = accessTimeStamp;
                    		physicalPages[Processes[i].pages[VA].physicalAddress].accessed =
                            	accessTimeStamp;
                    		accessTimeStamp++;
                    		break;
		   	}//else

		   }


            }
            }

        }//if - read from page

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
                if (Processes[i].PID == PID)
                {
                    //finds Process
                    Processes[i].isTerminated = true; //deemed terminated
	            Processes[i].isCreated = false; //Not allocated anymore
                    Processes[i].pages = NULL; //page table ptr points to NULL
                    break;
                }

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
		if(swapSpace[i].processID == PID){

	            swapSpace[i].isAllocated = false;
                    swapSpace[i].virtualAddress = 0;
                    swapSpace[i].dirty = false;
                    swapSpace[i].accessed = 0;
                    swapSpace[i].processID = 0;

		}
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
   //Print out processes
   for(int i = 0; i < 50; i++){
	if(Processes[i].isCreated){
   	cout<<"Process:"<<Processes[i].PID<<endl;
		if(Processes[i].pages != NULL){
			for(int j = 0; j < 100; j++){
				if(Processes[i].pages[j].isAllocated){
					cout <<"VA:"<<Processes[i].pages[j].virtualAddress << "\t";
					if(Processes[i].pages[j].inSwapSpace) cout<<"IN SWAP"<<endl;
					else{cout <<"PA:"<<Processes[i].pages[j].physicalAddress << endl;}
				}
			}

		}
		
   	}
   }
   //Print out terminated processes
   for(int i = 0; i < 50; i++){
	if(Processes[i].isTerminated){
		cout<<"Terminated Process:"<<Processes[i].PID<<endl;
	}
   }


}

