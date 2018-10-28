#ifndef RANDOM_HPP
#define RANDOM_HPP

namespace Util {
namespace Math {

/**
 * @brief drand
 * @return Random double in [0.0, 1.0].
 */
double drand();
double drand(double min, double max);
int irand(int min, int max);

}
}

#endif // RANDOM_HPP
