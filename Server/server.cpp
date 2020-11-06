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

bool isThereError(uint16_t expectedHash, uint8_t *data);

int main () {
    int n;
    struct sockaddr_in server, client;
    int slen = sizeof(client);
    char startRequest[128];
    uint8_t packet[128];
    uint16_t expectedPacketNumber = 0;

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
    // waiting for start request "PUT filename"
    for (;;) {

        n = recv(sd, startRequest, sizeof(startRequest), 0);

        if (startRequest[0] != '\0') {
        	cout << "received PUT" << endl;
            break;
        }
    }

    // receiving packet loop
    for (;;) {

        n = recvfrom(sd, packet, sizeof(packet), 0, (struct sockaddr*) &client, (socklen_t*)&slen);

        uint8_t firstByte = packet[0];
        uint8_t secondByte = packet[1];
        uint8_t thirdByte = packet[2];
        uint8_t fourthByte = packet[3];

        uint16_t packetNumber = (uint16_t)(secondByte << (uint8_t) 8) | firstByte;
        uint16_t hash = (uint16_t)(fourthByte << (uint8_t) 8) | thirdByte;
        uint8_t *data = &packet[4];

        if (n == 1) {
            break;
        }

        if (packetNumber != expectedPacketNumber) {
            cout << "There is a packet missing: " << expectedPacketNumber << endl;
            expectedPacketNumber = packetNumber;
        }

        if (!(isThereError(hash, data))) {
        	cout << "There is no error: " << packetNumber << endl;
			for (int i = 4; i < 52; i++) {
				cout << (char) packet[i];
			}
        }
        else {
            cout << "There is an error: " << packetNumber << endl;
        }

        for (int i = 4; i < n; i++) {
        	cout << (char) packet[i];
			if (packet[i] != 0)
        		outfile << (char) packet[i];
		}
        //outfile.write((char*)data, sizeof(data));

        expectedPacketNumber++;
    }

    // sending final PUT message and closing the socket and file
    cout << "sending awk packet" << endl;
    cout << "ip: " << inet_ntoa(client.sin_addr) << ":" << ntohs(server.sin_port);
    sendto(sd, "PUT successfully completed", 26, 0, (struct sockaddr *) &client, sizeof(client));

    close(sd);

    outfile.close();

    return 0;
}
/*
void stackoverflow() {
	struct sockaddr_in si_me, si_other;
	int s, i, blen, slen = sizeof(si_other);
	char buf[BUFLEN];

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == -1)
	die("socket");

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(1234);
	si_me.sin_addr.s_addr = htonl(192.168.1.1);

	if (bind(s, (struct sockaddr*) &si_me, sizeof(si_me))==-1)
	die("bind");

	blen = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*) &si_other, &slen);
	if (blen == -1)
	diep("recvfrom()");

	printf("Data: %.*s \nReceived from %s:%d\n\n", blen, buf, inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

	//send answer back to the client
	if (sendto(s, buf, blen, 0, (struct sockaddr*) &si_other, slen) == -1)
	diep("sendto()");

	close(s);
	return 0;
}*/

bool isThereError(uint16_t expectedHash, uint8_t *data) {

    uint16_t hash = checksum(data, 124);

    if (hash == expectedHash) {
        return false;
    }
    return true;
}