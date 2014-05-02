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
//#include <cstdlib>
#include <cstdint>
#include <cmath>

// Library includes
#include <png.h>

// Definition includes
#include "mapGenerate.hpp"
#include "heightNode.hpp"

// Prototypes
void setHeights( std::vector<biomeNode> & );

// Namespace Definitions
namespace {
    uint8_t size = 10;
    uint8_t randomFactor = 255;
    std::vector<biomeNode> heights;
}

using namespace std;

// Main Function
int main( int argc, char *argv[] ) {

    try {
        // Set biomes
        setHeights( heights );

        // Create image
        mapGenerator map( size, 8, PNG_COLOR_TYPE_GRAY );
        
        // Generate Image
        map.dsGenHeight( randomFactor );
        
        // Colour map
        //map.convColour( heights );
        
        // Write image
        map.writeImage( "heightMap" );
        
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

void setHeights( std::vector<biomeNode> &height ) {
    height.resize( 3 );
    height.at( 0 ).height = 0.4;
    height.at( 0 ).blue = 255;
    height.at( 1 ).height = 0.7;
    height.at( 1 ).red = 255;
    height.at( 2 ).height = 0.9;
    height.at( 2 ).green = 255;
}
