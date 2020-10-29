//
// Created by amfur on 10/21/2020.
//

#include "gremlin.h"
#include <random>
#include <functional>


bool gremlin(uint8_t *packet, float damage, float drop) {
	std::random_device g;
	std::uniform_real_distribution<float> dist(0,1);

	float result = dist(g);
	if (result > drop) {
		return false;
	}

	result = dist(g);
	if (result > damage) { //damage packet
		//TODO: Damage packet
	}
	return true;
}

