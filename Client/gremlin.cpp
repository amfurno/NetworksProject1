//
// Created by amfur on 10/21/2020.
//

#include "gremlin.h"
#include <random>
//#include <functional>


bool gremlin(uint8_t *packet, float damage, float drop) {
	std::random_device g;
	std::uniform_real_distribution<float> dist(0,1);

	int corruptBytes;

	float result = dist(g);
	if (result > drop) {
		return false;
	}

	result = dist(g);
	if (result > damage) { //damage packet

		result = dist(g);
		if (result > .1)
			corruptBytes = 3;
		else if (result > .2)
			corruptBytes = 2;
		else
			corruptBytes = 1;

		while (corruptBytes > 0) {
			size_t byte = std::uniform_real_distribution<float>(4, packetSize)(g);

			packet[byte] = packet[byte] | (uint8_t) 0xe5; //bitwise or it with a random number I chose

			corruptBytes--;
		}
	}
	return true;
}

