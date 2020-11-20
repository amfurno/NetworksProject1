//
// Created by bhold on 10/21/2020.
//

#ifndef NETWORKSPROJECT1_UNP_H
#define NETWORKSPROJECT1_UNP_H
#include <cstdint>
#include <cstddef>

const int SERVPORT = 12345;
const int MAXLINE = 4096;
const int BUFFSIZE = 8192;

const int PACKETSIZE = 512;



uint16_t checksum(const uint8_t *packet, size_t size);

#endif //NETWORKSPROJECT1_UNP_H
