#include "Random.hpp"

#include <cstdlib>

namespace Util {
namespace Math {

/**
 * @brief drand
 * @return Random double in [0.0, 1.0].
 */
double drand() {
	return ((double)rand()) / ((double)RAND_MAX);
}

double drand(double min, double max) {
	return drand() * (max - min) + min;
}

int irand(int min, int max) {
	return (rand() % (max - min)) + min;
}

}
}
