#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>

#include "pageTable.hpp"

using namespace std;

Process::Process(){
    PID = 0;
    isCreated = false;
    isTerminated = false;
    pages = NULL;

}
pageTable::pageTable(){
    processID = 0;
    dirty = false;
    isAllocated = false;
    accessed = 0;
    physicalAddress = 0;
    virtualAddress = 0;
    isFreed = false;
    inSwapSpace = false;

}

istream &operator>>(istream &in_stream, pageTable &page){
    in_stream >> page.processID;
    in_stream >> page.virtualAddress;
    in_stream >> page.physicalAddress;
    return in_stream;
}

ostream &operator<<(ostream &out_stream, pageTable &page){
    out_stream << page.processID<<"\t";
    out_stream << page.virtualAddress<<"\t";
    out_stream << page.physicalAddress<<"\t";
    return out_stream;
}


void pageTable::operator=(pageTable &page){
    processID = page.processID;
    dirty = page.dirty;
    isAllocated = page.isAllocated;
    accessed = page.accessed;
    physicalAddress = page.physicalAddress;
    virtualAddress = page.virtualAddress;
}
int pageTable::get_processID(){
    return processID;
}
bool pageTable::isDirty(){
    return dirty;
}
bool pageTable::makeDirty(){
    dirty = true;
    return dirty;
}
bool pageTable::setFree(){
    isAllocated = false;
    return isAllocated;
}
