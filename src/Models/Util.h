#ifndef UTIL_H
#define UTIL_H
#include <bitset>
#include <vector>
#include <iostream>

const unsigned int MAX_COMPONENTS = 32;

/////////////////
/// SIGNATURE
/// Used to keep track of which components an entity has
/// also of which entities a system is interested in
////////////////
typedef std::bitset<MAX_COMPONENTS> SIGNATURE;

#endif // UTIL_H