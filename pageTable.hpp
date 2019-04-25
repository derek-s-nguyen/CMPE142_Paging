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
    istream &operator >>(istream &in_stream, pageTable &page);
    ostream &operator <<(ostream &out_stream, pageTable &page);

    private:
    int processID;
    char processAction;
    int pageNumber; //what page are we on
    bool pageStatus; //checks if the page exists
}
