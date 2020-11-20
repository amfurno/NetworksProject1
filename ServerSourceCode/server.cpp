//  Anthony Furno
//  William Canup
//  Basil Wells

#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdint>
#include <unistd.h>
#include "../unp.h"
#include <fstream>

using namespace std;

ifstream openFile(char* fileName);

int main () {
    int n;
    struct sockaddr_in server, client;
    int slen = sizeof(client);
    char startRequest[PACKETSIZE];
    uint8_t packet[PACKETSIZE];
    uint16_t PacketNumber = 0;

    ofstream outfile;
    outfile.open("../output.txt", ios::out);
    if (outfile.is_open() and outfile.good())
    	cout << "file opened correctly" << endl;



    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVPORT);

    int sd = socket(AF_INET, SOCK_DGRAM, 0);

    if (bind(sd, (struct sockaddr *) &server, sizeof(server)) < 0)
    	cerr << "error binding socket" << endl;

    listen(sd, 5);
    // waiting for start request "PUT/GET filename"
    for (;;) {

        n = recv(sd, startRequest, sizeof(startRequest), 0);

        if (startRequest[0] != '\0') {
        	cout << "received PUT" << endl;
            break;
        }
        char* fileName = &startRequest[4]


    }

    uint8_t ch;
    vector<uint8_t> packetData;
    int sd;
    struct sockaddr_in server;
    struct hostent *hp;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
        cerr << "ERROR opening socket" << endl;

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVPORT);

    hp = gethostbyname("192.168.1.64");
    if (hp == NULL) {
        cerr << "ERROR no such host" << endl;
        exit(5);
    }
    bcopy(hp->h_addr, &(server.sin_addr), hp->h_length);


    if (bind(sd, (struct sockaddr *) &server, sizeof(server)) < 0)
        cerr << "error binding socket" << endl;

    string request = ("PUT" + (string)fileName);
    cout << "sending PUT packet " << endl;
    sendto(sd, request.c_str(), request.size(), 0, (struct sockaddr *) &server, sizeof(server));

    while (inputFile >> noskipws >> ch) {
        packetData.push_back(ch);
        if (packetData.size() == PACKETSIZE - 4) {
            makePacket(packet, packetNumber, packetData);
            if (gremlin(packet, damage, drop)) {
                cout << "sending packet: " << packetNumber << endl;
                for (int i = 4; i < 52; i++) {
                    cout << (int) packet[i];
                }
                cout << endl;
                sendto(sd, packet, PACKETSIZE, 0, (struct sockaddr *) &server, sizeof(server));
            } else
                cout << "Dropping packet: " << packetNumber << endl;
            packetNumber++;
            packetData.clear();
        }
    }
    if (!packetData.empty()) {
        while (packetData.size() < packetSize) {
            packetData.push_back(0);
        }

        makePacket(packet, packetNumber, packetData);
        if (gremlin(packet, damage, drop)) {
            cout << "sending final packet: " << packetNumber << endl;
            for (int i = 4; i < 52; i++) {
                cout << (int) packet[i];
            }
            cout << endl;
            sendto(sd, packet, PACKETSIZE, 0, (struct sockaddr *) &server, sizeof(server));
        }
    }
    cout << "sending termination packet" << endl;
    sendto(sd, "/0", 1, 0, (struct sockaddr *) &server, sizeof(server));

    return 0;
}




ifstream openFile(char* fileName) {
    ifstream inputFile;
    inputFile.open (fileName, ios::in);

    if (!inputFile.is_open()) {
        cout << "Unable to open file, exiting";
        exit(1);
    }
    else {
        cout << "file opened successfully.\n";
    }
    return inputFile;
}