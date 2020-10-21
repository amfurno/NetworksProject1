//
// Created by amfur on 10/21/2020.
//

#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "../unp.h"


int main () {
    int expectedPacket = 1;
    return 0;
}

bool errorCheck(char[] expectedChecksum, char[] packet) {
    char[16] hash;
    checksum(packet, hash);
}