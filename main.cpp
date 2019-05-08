#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>

using namespace std;

int main(){
    //taking in data from file 
    
    int next;
    char status;
    ifstream in_stream;
    in_stream.open("memory.dat");
    in_stream >> next;
    in_stream >> status;
    in_stream>> next;
}
