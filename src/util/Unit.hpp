#ifndef UNIT_HPP
#define UNIT_HPP

namespace Util {

/**
 * @brief The Unit class is a type that has only one value.
 */
class Unit
{
public:
	Unit();
	bool operator==(const Unit& other) const;
};

}

#endif // UNIT_HPP
