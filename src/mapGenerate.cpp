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
mapGenerator::mapGenerator( uint8_t size, png_byte bDepth, png_byte clrType ):
    size( size ),
    colourMap( nullptr )
{
    heightMap = new pixelData( pow( 2, size ), pow( 2, size ), bDepth, clrType );
    if( size > maxSize )
        throw runtime_error( "Size can not be larger than 14" );
}

mapGenerator::~mapGenerator() {
}

uint8_t mapGenerator::getSize() {
    return size;
}

void mapGenerator::dsGenHeight( int randomFactor, int seed ) {
    if( seed == 0 ) {
        seed = std::chrono::system_clock::now().time_since_epoch().count();
    }
    cout << "Seed: " << seed << endl;

    // Create random number generator
    float rand = ( randomFactor * size ) / 14;
    default_random_engine Generator( seed );
    uniform_real_distribution< float > randomness( -1, 1 );
    
    // Set initial value
    heightMap->set( 0, 0, randomness( Generator ) * 255 );

    // Generate map
    
    // Setup well used variables
    unsigned int width = heightMap->getWidth();
    unsigned int height = heightMap->getHeight();
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
                    heightMap->wrapGet( xOffset,            yOffset ) +
                    heightMap->wrapGet( xOffset + edge,     yOffset) +
                    heightMap->wrapGet( xOffset,            yOffset + edge ) +
                    heightMap->wrapGet( xOffset + edge,     yOffset + edge )
                ) / 4 + randomness( Generator ) * rand;
                
                heightMap->wrapSet( xOffset + halfEdge, yOffset + halfEdge, mean );
            }
        }
        for( yOffset = 0; yOffset < height; yOffset += edge ) {
            for( xOffset = 0; xOffset < width; xOffset += edge ) {
            // Square step
                
                // First diamond
                mean = (
                         // X position                      Y position
                    heightMap->wrapGet( xOffset + halfEdge, yOffset - halfEdge ) +
                    heightMap->wrapGet( xOffset,            yOffset ) +
                    heightMap->wrapGet( xOffset + edge,     yOffset ) +
                    heightMap->wrapGet( xOffset + halfEdge, yOffset + halfEdge )
                ) / 4 + randomness( Generator ) * rand;
                
                heightMap->wrapSet( xOffset + halfEdge, yOffset, mean);
                
                // Second diamond
                                 // X position                  Y position
                mean =
                (
                    heightMap->wrapGet( xOffset,                yOffset ) +
                    heightMap->wrapGet( xOffset - halfEdge,     yOffset + halfEdge ) +
                    heightMap->wrapGet( xOffset + halfEdge,     yOffset + halfEdge ) +
                    heightMap->wrapGet( xOffset,                yOffset + edge ) 
                ) / 4 + randomness( Generator ) * rand;
                
                heightMap->wrapSet( xOffset, yOffset + halfEdge, mean );
            }
        }
        edge = halfEdge;
        // Reduce random number
        rand /= 2 ;
    }
}

void mapGenerator::convColour( vector<biomeNode> &biomes, float shadingFactor ) {
    unsigned int pixelsTotal = heightMap->getWidth() * heightMap->getHeight();
    unsigned int biomeCount = biomes.size();
    unsigned int biomeHeight = 0;
    unsigned int prevHeight = 0;
    vector<unsigned int> counter( 256, 0 ); // Container to count pixels

    if( colourMap == nullptr )
        colourMap = new pixelData(
                heightMap->getWidth(), heightMap->getHeight(),
                8, PNG_COLOR_TYPE_RGB
        );
    if( shadingFactor > 1.0 ) shadingFactor = 1.0;

    for( unsigned int i = 0; i < pixelsTotal; i++ ) {
        ++counter[ heightMap->at( i ) ];
    }

    for( unsigned int i = 0; i < biomeCount; i++ ) {
        if( i == biomeCount - 1 ) {
            biomeHeight = 255;
        }
        else {
            float height = biomes.at( i ).height;
            
            // Find pixel area to colour
            // Count up all pixels till correct quantity is reached
            if( height > 0.5 ){
                unsigned short j = 255;
                unsigned int biomeTotal = 0;
                unsigned int maxPixels = ( 1 - height ) * pixelsTotal; 

                while( biomeTotal <= maxPixels ) {
                   biomeTotal += counter[j];
                   j--;
                }
                biomeHeight = j + 1;
            }
            else {
                unsigned short j = 0;
                unsigned int biomeTotal = 0;
                unsigned int maxPixels = height * pixelsTotal; 

                while( biomeTotal <= maxPixels ) { 
                    biomeTotal += counter[j];
                    j++;
                }
                biomeHeight = j - 1;
            }
        }

        // Write biome to new image
        for( unsigned int j = 0; j < pixelsTotal; j++ ) {
            if( heightMap->at( j ) <= biomeHeight && heightMap->at( j ) >= prevHeight ) {
                float colourFactor =
                    1.0 - shadingFactor + (float)heightMap->at( j ) / ( biomeHeight ) * shadingFactor;
                colourMap->at( j * 3 ) = biomes.at(i).red * colourFactor;
                colourMap->at( j * 3 + 1 ) = biomes.at(i).green * colourFactor;
                colourMap->at( j * 3 + 2 ) = biomes.at(i).blue * colourFactor;
            }
        }
        cout << prevHeight << " " << biomeHeight << endl;
        prevHeight = biomeHeight;
    }
}

void mapGenerator::writeImage( const string &name ) {
    heightMap->writeImage( name );
}

void mapGenerator::writeImage( const string &heightName, const string &colourName ) {
    heightMap->writeImage( heightName );
    if( colourMap != nullptr )
        colourMap->writeImage( colourName );
}
