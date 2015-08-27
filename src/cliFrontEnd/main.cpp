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
#include <cstring>

// Library includes
#include <png.h>

// Definition includes
#include "mapGenerate.hpp"
#include "heightNode.hpp"
#include "mapData.hpp"

// Prototypes
void setHeights( std::vector<biomeNode> & );

// Namespace Definitions
namespace {
    uint8_t res = 8;
    uint8_t width = 3;
    uint8_t height = 2;
    bool wrap = true;
    float randomFactor = 1.0;
    std::vector<biomeNode> biomes;
}

using namespace std;


//@ To Do: Get different width and height working!

// Main Function
int main( int argc, char *argv[] ) {

    int count = 1;
    while( count < argc ) {
        if( strcmp( argv[count], "--res") == 0 ) {
            count++;
            if( count >= argc ) {
                cerr << "Error: Not enough parameters specified" << endl;
                return -1;
            }
            res = atol( argv[count] );
        } else if( strcmp( argv[count], "--width" ) == 0 ) {
            count++;
            if( count >= argc ) {
                cerr << "Error: Not enough parameters specified" << endl;
                return -1;
            }
            width = atol( argv[count] );
        } else if( strcmp( argv[count], "--height" ) == 0 ) {
            count++;
            if( count >= argc ) {
                cerr << "Error: Not enough parameters specified" << endl;
                return -1;
            }
            height = atol( argv[count] );
        } else if( strcmp( argv[count], "--wrap" ) == 0 ) {
            count++;
            if( count >= argc ) {
                cerr << "Error: Not enough parameters specified" << endl;
                return -1;
            }
            if( strcmp( argv[count], "true" ) == 0 ) {
                wrap = true;
            } else {
                wrap = false;
            }
        }
        count++;
    }

    try {
        // Set biomes
        setHeights( biomes );

        // Create image
        monoMapData heightMap( width, height, res, wrap );
        colourMapData colourMap( width, height, res, wrap );

        mapGenerator map;
        
        // Generate Image
        map.dsGenerate( heightMap );
        
        // Colour map
        map.colourise( heightMap, colourMap, biomes );

        // Write image
        heightMap.writeImage( "heightMap" );
        colourMap.writeImage( "colourMap" );

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
    // Ocean
    height.at( 0 ).height = 0.7;
    height.at( 0 ).blue = 255;
    //Earth
    height.at( 1 ).height = 0.99;
    height.at( 1 ).green = 255;
    //Snow
    height.at( 2 ).height = 0.9;
    height.at( 2 ).red = 255;
    height.at( 2 ).green = 255;
    height.at( 2 ).blue = 255;
}
