#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include "pageTable.hpp"


using namespace std;

pageTable table[20];

int main(){
    //taking in data from file 
    
	int PID;//Process ID
	int VA; //Virtual Address
	char status; //Process Action
	
	
	VA = 12;
    	ifstream in_stream;
    	in_stream.open("memory.dat");
	while(!in_stream.eof()){
    		in_stream >> PID;
		cout << PID;
    		in_stream >> status;
		cout << status;
		if(status == 'A' || status == 'W' || status == 'F' || status == 'R'){
    			in_stream >> VA;
			cout << VA;
		}
    		cout << "\n";
	}

	
}
