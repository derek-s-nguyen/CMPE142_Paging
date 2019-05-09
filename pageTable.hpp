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
    int get_processID();
    bool checkStatus(char status); //checks the status for paging in process
    bool isDirty();
    bool makeDirty(); 
    bool setFree();
    void outputPageTable();
    friend istream &operator >>(istream &in_stream, pageTable &page);
    friend ostream &operator <<(ostream &out_stream, pageTable &page);

private:
    int processID; //ID of the page
    int pageNumber; //what page are we on
    bool dirty;
    bool isAllocated; //bool to check if the page is allocated or not
    char processAction; //create, allocate, write, read
    bool pageStatus; //checks if the page exists
};
