#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>

using namespace std;

class pageTable{
public:
    pageTable();
    int get_processID();
    bool isDirty();
    bool makeDirty();
    bool setFree();
    friend istream &operator >>(istream &in_stream, pageTable &page);
    friend ostream &operator <<(ostream &out_stream, pageTable &page);
    void operator =(pageTable &page);
    int processID; //ID of the page
    int physicalAddress; //what page are we on
    int virtualAddress;
    bool dirty;
    int accessed;
    bool isFreed;
    bool isAllocated; //bool to check if the page is allocated or not
    char processAction; //create, allocate, write, read
    bool inSwapSpace;
};

class Process{
public:
    Process();
    int PID;
    bool isCreated;
    bool isTerminated;
    pageTable* pages;
};
