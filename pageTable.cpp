#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>

#include "pageTable.hpp"

using namespace std;

pageTable::pageTable(){}

istream &operator>>(istream &in_stream, &pageTable page){
    in_stream >> pageTable.processID;
    in_stream >> pageTable.processAction;
    in_stream >> pageTable.pageNumber; 
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