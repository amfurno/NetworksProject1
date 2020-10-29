//
// Created by bhold on 10/21/2020.
//

#ifndef NETWORKSPROJECT1_UNP_H
#define NETWORKSPROJECT1_UNP_H
#include <cstdint>

const int SERVPORT = 21;
const int MAXLINE = 4096;
const int BUFFSIZE = 8192;

const int packetSize = 10;



uint16_t checksum(uint8_t *packet);

#endif //NETWORKSPROJECT1_UNP_H
