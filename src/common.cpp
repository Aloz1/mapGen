#include <cmath>
#include "common.hpp"

int mod( int num, int denom ) {
    int const result = num % denom;
    return ( result < 0 ? result + std::abs( denom ) : result );
}
