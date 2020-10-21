//
// Created by amfur on 10/21/2020.
//

#include <fstream>
#include <iostream>
#include <vector>
#include "../unp.h"
#include "gremlin.h"

using namespace std;

void sendPacket(); //TODO write this
void makePacket(); //TODO write this

int main (int argc, char *argv[]) {
//arguments expected [filename, errorChance]
    ifstream inputFile;
    inputFile.open (/*innput file*/, ios::in);

    if (!inputFile.is_open()) {
        cout << "Unable to open file, exiting";
        return 1;
    }
    else {
        cout << "file opened successfully.\n";
    }

    char ch;
    vector<char> packetData;
    while (inputFile >> noskipws >> ch) {
        packetData.push_back(ch);
        if (packetData.size() == 110) {
            makePacket();
            gremlin();//TODO add proper function call
            sendPacket();
            packetData.clear();
        }
    }
    packetData.resize(110, '\0');
    makePacket();
    gremlin();//TODO add proper function call
    sendPacket();

    //TODO send 1 Byte packet


    return 0;
}