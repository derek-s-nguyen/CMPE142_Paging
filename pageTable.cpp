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
    processAction = NULL;
    pageNumber = 0;

}

istream &operator>>(istream &in_stream, pageTable &page){
    in_stream >> page.processID;
    in_stream >> page.processAction;
    in_stream >> page.pageNumber; 
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
bool pageTable::checkStatus(char status){
    if(status == 'C' || status == 'T'){
        pageStatus = false;
        pageNumber = 0;
    }
    else if(status == 'A' || status == 'R' || status == 'W' || status == 'F'){
        pageStatus = true;
    }
    return pageStatus;
}

