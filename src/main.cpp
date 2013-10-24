// Copyright 2013 Alastair Knowles
//
// This file is part of mapGen.
//
// mapGen is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// mapGen is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with mapGen.  If not, see <http://www.gnu.org/licenses/>.

// Standard C++ includes
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <stdexcept>

// Standard C includes
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cmath>

// Library includes
#include <png.h>

// Definition includes
#include "mapGenerate.hpp"

// Namespace Definitions
namespace {
    uint8_t size = 10;
    uint8_t randomFactor = 255;
    float water = 0.9;
}

using namespace std;

// Main Function
int main( int argc, char *argv[] ) {
    try {
        // Create image
        mapGenerator map( size, 8, PNG_COLOR_TYPE_GRAY, "file" );
        
        // Generate Image
        map.dsGenerate( randomFactor );
        
        // Colour map
        unsigned int totalSize = map.pixel.getWidth() * map.pixel.getHeight();
        vector<unsigned int> counter( 256, 0 );
        unsigned int countSize;
        unsigned int waterHeight = 0;

        for( unsigned int i = 0; i < totalSize; i++ ) {
            ++counter[ map.pixel.raw()[i] ];
        }

        if( water > 0.5 ){
            uint32_t i, waterTotal = 0;
            countSize = ( 1 - water ) * totalSize; 
            for( i = 255;  waterTotal <= countSize; i-- ) {
                waterTotal += counter[i];
            }
            waterHeight = i + 1;
        }
        else {
            uint32_t i, waterTotal = 0;
            countSize = water * totalSize; 
            for( i = 0;  waterTotal <= countSize; i++ ) {
                waterTotal += counter[i];
                cout << i << " " << waterTotal << " " << counter[i] << endl;
            }
            waterHeight = i - 1;
        }
        
        for( unsigned int i; i < totalSize; i++ ) {
            if( map.pixel.raw()[i] <= waterHeight ) {
                map.pixel.raw()[i] = 0;
            }
        }

        
        // Write image
        map.pixel.writeImage();
        
        return 0;
    }
    catch( exception &x ) {
        cerr << "Error: " << x.what() << endl;
        return 1;
    }
    catch( ... ) {
        cerr << "Error: Unexpected error" << endl;
        return 2;
    }
}



// While the length of the side of the squares is greater than zero {
//     Pass through the array and perform the diamond step for each square present.
//     Pass through the array and perform the square step for each diamond present.
//     Reduce the random number range.
// }

