//
// Created by bhold on 10/21/2020.
//

#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdint>
#include "unp.h"

//#include <iostream>
//using namespace std;


uint16_t checksum(uint8_t *packet) {
	uint16_t sum = 0;
	uint16_t value;
	uint8_t partial;
	int repeat = 1;
	size_t index = 0;
	for (int i = 0; i < repeat; i++) {
		while (index < packetSize) {
			partial = packet[index];
			value = partial << 8;
			//cout << "value: " << value << endl;
			index++;
			if (index < packetSize) {
				value = value | packet[index];
			} else {
				value = value | (uint8_t)0;
			}
			//cout << "value: " << value << endl;

			sum = sum ^ value;

			//cout << "sum: " << sum << endl;
			index++;
		}
	}
	return sum;
}