#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>

#include "pageTable.hpp"

using namespace std;

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