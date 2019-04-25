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
    int getPageNumber(); //returns page number
    bool checkStatus(char status); //checks the status for paging in process
    void outputPageTable();

    private:
    int processID;
    int pageNumber; //what page are we on
    bool pageStatus; //checks if the page exists
}
