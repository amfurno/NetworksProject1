//
// Created by amfur on 10/21/2020.
//

#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdint>
#include <unistd.h>
#include "../unp.h"

using namespace std;

bool isThereError(uint8_t expectedHash, uint8_t *data);

int main () {
    int n;
    uint8_t expectedPacketNumber = 0;
    struct sockaddr_in server;
    uint8_t packet[128];

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(SERVPORT);

    int sd = socket(AF_INET, SOCK_DGRAM, 0);

    bind(sd, (struct sockaddr *) &server, sizeof(server));

    for (;;) {

        n = recv(sd, packet, sizeof(packet), 0);

        int8_t* hash = &packet[2];
        int8_t* data = &packet[4];

        if (!(isThereError(hash, data))) {
            cout << "There is no error." << endl;
        }
        else {
            cout << "There is an error." << endl;
        }

        if (packet[0] == '\0') {
            break;
        }

        if (packet[1] != expectedPacketNumber) {
            cout << "There is a packet missing." << endl;
        }

        expectedPacketNumber++;

    }
    close(sd);
    return 0;
}

bool isThereError(uint8_t expectedHash, uint8_t *data) {

    uint8_t hash = checksum(data);

    if (hash == expectedHash) {
        return false;
    }
    return true;
}