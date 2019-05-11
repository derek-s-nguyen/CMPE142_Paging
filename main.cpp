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
    int newProcessIndex = 0; //index where new process will be created
    int accessTimeStamp = 1; //determine when page was accessed
    int PA;
    int swapSpaceIndex = 0;

    ifstream in_stream;
    in_stream.open("memory.dat");

    while (in_stream >> PID)
    {

        cout << PID;
        in_stream >> status;
        cout << status;
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

            Processes[newProcessIndex].PID = PID;
            Processes[newProcessIndex].isCreated = true;
            newProcessIndex++;
            //Processes[newProcessIndex].pages = NULL;

        }

        if (status == 'A')
        { //allocate page for PID
          //find process with PID
            PA = 0;
            swapSpaceIndex = 0;
            bool needSwapAlgo = false;
            bool swapHappens = false;
            while (physicalPages[PA].isAllocated)
            {
                PA++; //find free page
                if (PA > 19)
                {
                    //if no free page look for unmodified page
                    PA = 0;
                    swapHappens = true; //a swap will happen but not sure if it is clean page swap or swap algo
                    while (physicalPages[PA].dirty)
                    {
                        //find unmodified page
                        PA++;
                        if (PA > 19)
                        {
                            //if no clean page found
                            PA = 0;
                            needSwapAlgo = true; //need a swap algorithm
                            break;
                        }
                    }
                }

            }

            cout << "PA found:" << PA << endl;
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID)
                {
                    cout << Processes[i].pages << endl;
                    cout << "PID:" << PID << endl;
                    cout << "Process found" << endl;

                    if (swapHappens && !needSwapAlgo)
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
                        Processes[i].pages = new pageTable[20];
                        cout << "page Table pointed successfully" << endl;
                        Processes[i].pages[VA].virtualAddress = VA;
                        Processes[i].pages[VA].physicalAddress = PA;
                        Processes[i].pages[VA].isAllocated = true;

                    }
                    else
                    {
                        cout << "process has already page table so adding PTE"
                                << endl;
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

            //locate process with PID in the processes array and write into that page
            for (int i = 0; i < 50; i++)
            {

                if (Processes[i].PID == PID)
                {
                    Processes[i].pages[VA].dirty = true; //make dirty teehee ;)
                    physicalPages[Processes[i].pages[VA].physicalAddress].dirty =
                            true;
                    physicalPages[Processes[i].pages[VA].physicalAddress].accessed =
                            accessTimeStamp;
                    accessTimeStamp++;
                    break;
                }
            }
            //swap pages if index is out of range and look for free pages
            /*int index = 0;
             swapSpaceIndex = 0;
             bool needSwapAlgo = false;
             bool swapHappens = false;
             while(physicalPages[index].isAllocated){
             index++;//find free page
             if(index > 19){
             index = 0;
             swapHappens = true;
             while(physicalPages[index].dirty){
             //find unmodified page
             index++;
             if(index > 19){
             //if no clean page found
             index = 0;
             needSwapAlgo = true; //need a swap algorithm
             break;
             }
             }
             }
             }
             */
        }

        if (status == 'R')
        { //Read from page
          //locate process with PID in process array
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID)
                {
                    Processes[i].pages[VA].accessed = accessTimeStamp;
                    physicalPages[Processes[i].pages[VA].physicalAddress].accessed =
                            accessTimeStamp;
                    accessTimeStamp++;
                    break;
                }
            }
            //find page in process page table
            //set accessTimeStamp

            /*int index = 0;
             swapSpaceIndex = 0;
             bool needSwapAlgo = false;
             bool swapHappens = false;
             while(physicalPages[index].isAllocated){
             index++;//find free page
             if(index > 19){
             index = 0;
             swapHappens = true;
             while(physicalPages[index].dirty){
             //find unmodified page
             index++;
             if(index > 19){
             //if no clean page found
             index = 0;
             needSwapAlgo = true; //need a swap algorithm
             break;
             }
             }
             }

             }*/

        }
        if (status == 'F')
        { //free page
          //locate process with PID in process array
            for (int i = 0; i < 50; i++)
            {
                if (Processes[i].PID == PID && Processes[i].pages[VA].isAllocated == true)
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

                    break;
                }
                else if (Processes[i].processID == PID
                        && Processes[i].pages[VA].isAllocated == false)
                {
                    delete Processes[i].pages;
                } //deletes the page table assigned to the process since it tries to free an allocated page
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
                    Processes[i].isCreated = false; //Not allocated anymore
                    Processes[i].isTerminated = true; //deemed terminated
                    Processes[i].pages = NULL; //page table ptr points to NULL
                    break;
                }
            }

            for (int i = 0; i < 20; i++)
            {

                if (physicalPages[i].processID == PID)
                {
                    physicalPages[i].isAllocated = false;
                    physicalPages[i].virtualAddress = 0;
                    physicalPages[i].dirty = false;
                    physicalPages[i].accessed = 0;
                    physicalPages[i].processID = 0;

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

}
