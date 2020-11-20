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
#include <netdb.h>
#include <vector>
#include <strings.h>
#include "../ClientSourceCode/gremlin.h"

using namespace std;

void makePacket(uint8_t packet[], uint16_t packetNumber, const vector<uint8_t>& data);
ifstream openFile(char* fileName);

int main (int argc, char *argv[]) {
	int n;
    struct sockaddr_in server, client;
    int slen = sizeof(client);
    char startRequest[PACKETSIZE];
    uint8_t packet[PACKETSIZE];
    uint16_t packetNumber = 0;
	char* fileName;



	float damage = stof(argv[1]);
	float drop = stof(argv[2]);


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVPORT);

    int sd = socket(AF_INET, SOCK_DGRAM, 0);

    if (bind(sd, (struct sockaddr *) &server, sizeof(server)) < 0)
    	cerr << "error binding socket" << endl;

    listen(sd, 5);
    // waiting for start request "PUT/GET filename"
    cout << "waiting for packet" << endl;
    for (;;) {

        n = recvfrom(sd, startRequest, sizeof(startRequest), 0, (struct sockaddr*) &client, (socklen_t*)&slen);
        if (startRequest[0] != '\0') {
        	cout << "received GET" << endl;
			fileName = &startRequest[4];
            break;
        }
    }

    uint8_t ch;
    vector<uint8_t> packetData;

	cout << fileName << endl;
    ifstream inputFile = openFile(fileName);

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
                sendto(sd, packet, PACKETSIZE, 0, (struct sockaddr *) &client, sizeof(client));
            } else
                cout << "Dropping packet: " << packetNumber << endl;
            packetNumber++;
            packetData.clear();
        }
    }
    if (!packetData.empty()) {
        while (packetData.size() < PACKETSIZE) {
            packetData.push_back(0);
        }

        makePacket(packet, packetNumber, packetData);
        if (gremlin(packet, damage, drop)) {
            cout << "sending final packet: " << packetNumber << endl;
            for (int i = 4; i < 52; i++) {
                cout << (int) packet[i];
            }
            cout << endl;
            sendto(sd, packet, PACKETSIZE, 0, (struct sockaddr *) &client, sizeof(client));
        }
    }
    cout << "sending termination packet" << endl;
    sendto(sd, "\0", 1, 0, (struct sockaddr *) &client, sizeof(client));

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

void makePacket(uint8_t packet[], uint16_t packetNumber, const vector<uint8_t>& data) {
	int packetLocation = 4;
	for (unsigned char i : data) {
		packet[packetLocation] = i;
		packetLocation++;
	}

	//while (packetLocation < packetSize) {

	//}

	uint8_t packNum1 = (packetNumber >> (8*0)) & 0xff;
	uint8_t packNum2 = (packetNumber >> (8*1)) & 0xff;
	packet[0] = packNum1;
	packet[1] = packNum2;

	uint16_t checksumValue = checksum(packet, PACKETSIZE);
	uint8_t checkSum1 = (checksumValue >> (8*0)) & 0xff;
	uint8_t checkSum2 = (checksumValue >> (8*1)) & 0xff;


}