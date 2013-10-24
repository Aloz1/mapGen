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
#include <stdexcept>
#include <random>

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

namespace {
    uint8_t maxSize = 14;
}

using namespace std;

// Function Definitions
mapGenerator::mapGenerator( uint8_t size, png_byte bDepth, png_byte clrType, const std::string &outName ):
    size( size ),
    pixel( pow( 2, size ), pow( 2, size ), bDepth, clrType, outName )
{
    if( size > maxSize )
        throw runtime_error( "Size can not be larger than 14" );
}

mapGenerator::~mapGenerator() {
}

uint8_t mapGenerator::getSize() {
    return size;
}

void mapGenerator::dsGenerate( int randomFactor, int seed ) {
    if( seed == 0 ) {
        seed = std::chrono::system_clock::now().time_since_epoch().count();
    }
    cout << "Seed: " << seed << endl;

    // Create random number generator
    float rand = ( randomFactor * size ) / 14;
    default_random_engine Generator( seed );
    uniform_real_distribution< float > randomness( -1, 1 );
    
    // Set initial value
    pixel.set( 0, 0, randomness( Generator ) * 255 );

    // Generate map
    
    // Setup well used variables
    unsigned int width = pixel.getWidth();
    unsigned int height = pixel.getHeight();
    unsigned int edge = width;
    unsigned int mean;
    
    while( edge > 1 ) {
        // Setup recalculated well used values
        int halfEdge = edge / 2;
        int xOffset, yOffset;

        for( yOffset = 0; yOffset < height; yOffset += edge ) {
            for( xOffset = 0; xOffset < width; xOffset += edge ) {
                
            // Diamond step
				mean = (
                                 // X positon           Y position
                    pixel.wrapGet( xOffset,            yOffset ) +
                    pixel.wrapGet( xOffset + edge,     yOffset) +
                    pixel.wrapGet( xOffset,            yOffset + edge ) +
                    pixel.wrapGet( xOffset + edge,     yOffset + edge )
                ) / 4 + randomness( Generator ) * rand;
                
                pixel.wrapSet( xOffset + halfEdge, yOffset + halfEdge, mean );
            }
        }
        for( yOffset = 0; yOffset < height; yOffset += edge ) {
            for( xOffset = 0; xOffset < width; xOffset += edge ) {
            // Square step
                
                // First diamond
                mean = (
                         // X position                      Y position
                    pixel.wrapGet( xOffset + halfEdge, yOffset - halfEdge ) +
                    pixel.wrapGet( xOffset,            yOffset ) +
                    pixel.wrapGet( xOffset + edge,     yOffset ) +
                    pixel.wrapGet( xOffset + halfEdge, yOffset + halfEdge )
                ) / 4 + randomness( Generator ) * rand;
                
                pixel.wrapSet( xOffset + halfEdge, yOffset, mean);
                
                // Second diamond
                                 // X position                  Y position
                mean =
                (
                    pixel.wrapGet( xOffset,                yOffset ) +
                    pixel.wrapGet( xOffset - halfEdge,     yOffset + halfEdge ) +
                    pixel.wrapGet( xOffset + halfEdge,     yOffset + halfEdge ) +
                    pixel.wrapGet( xOffset,                yOffset + edge ) 
                ) / 4 + randomness( Generator ) * rand;
                
                pixel.wrapSet( xOffset, yOffset + halfEdge, mean );
            }
        }
        edge = halfEdge;
        // Reduce random number
        rand /= 2 ;
    }
}

void mapGenerator::convColour() {
    if( pixel.getColour() == PNG_COLOR_TYPE_GRAY ) {
        
    }
}
