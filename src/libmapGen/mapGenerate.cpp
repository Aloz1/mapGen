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
#include <sstream>

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

using namespace std;

namespace {
    uint8_t maxSize = 14;
}

// Function Definitions

mapGenerator::mapGenerator( int Seed, float SmoothFactor, float ShadingFactor ):
    smoothFactor( SmoothFactor ),
    shadingFactor( ShadingFactor ),
    rEng( Seed )
{
    if( Seed == 0 ) {
         rEng.seed( chrono::high_resolution_clock::now().time_since_epoch().count() );
    }
}


int mapGenerator::foldback( int range, int position ) {
    range--;
    return abs( mod( ( position + range ), ( range * 2 )) - range );
}

int mapGenerator::partialDiamond( monoMapData &heightMap, int stepSize, int xOffset, int yOffset ) {
    int topCorner, bottomCorner, leftCorner, rightCorner;
    int width = heightMap.getWidth();
    int height = heightMap.getHeight();

    if( !heightMap.getWrap() ) {
        topCorner = heightMap.get(
                foldback( width, xOffset ),
                foldback( height, yOffset )
                )[0];
        bottomCorner = heightMap.get(
                foldback( width, xOffset ),
                foldback( height, yOffset + stepSize )
                )[0];
        leftCorner = heightMap.get(
                foldback( width, xOffset - stepSize / 2 ),
                foldback( height, yOffset + stepSize / 2 )
                )[0];
        rightCorner = heightMap.get(
                foldback( width, xOffset + stepSize / 2 ),
                foldback( height, yOffset + stepSize / 2 )
                )[0];
    } else {
        topCorner = heightMap.get(
                xOffset,
                yOffset
                )[0];
        bottomCorner = heightMap.get(
                xOffset,
                yOffset + stepSize
                )[0];
        leftCorner = heightMap.get(
                xOffset - stepSize / 2,
                yOffset + stepSize / 2
                )[0];
        rightCorner = heightMap.get(
                xOffset + stepSize / 2,
                yOffset + stepSize / 2
                )[0];
    }

    int pixel = (
            topCorner +
            bottomCorner +
            leftCorner +
            rightCorner
            ) / 4;
    return pixel;
}

void mapGenerator::squareStep( monoMapData &heightMap, int stepSize, float randFact ) {
    int width = heightMap.getWidth();
    int height = heightMap.getHeight();
    unsigned int topLeftCorner, topRightCorner, bottomLeftCorner, bottomRightCorner;

    std::array<png_byte, 1> pixel;
    uniform_real_distribution< float > uniDist( -1, 1 );


    for( int yOffset = 0; yOffset < height - 1; yOffset += stepSize ) {
        for( int xOffset = 0; xOffset < width - 1; xOffset += stepSize ) {
            topLeftCorner = heightMap.get( xOffset, yOffset )[0];
            topRightCorner = heightMap.get( xOffset + stepSize, yOffset )[0];
            bottomLeftCorner = heightMap.get( xOffset, yOffset + stepSize )[0];
            bottomRightCorner = heightMap.get( xOffset + stepSize, yOffset + stepSize )[0];

            pixel[0] = (
                    topLeftCorner +
                    topRightCorner +
                    bottomLeftCorner +
                    bottomRightCorner
                    ) / 4 + uniDist( rEng ) * randFact;
            heightMap.set( xOffset + stepSize / 2, yOffset + stepSize / 2, pixel );
        }
    }
}

void mapGenerator::diamondStep( monoMapData &heightMap, int stepSize, float randFact ) {
    int width = heightMap.getWidth();
    int height = heightMap.getHeight();

    std::array<png_byte, 1> pixel;
    uniform_real_distribution< float > uniDist( -1, 1 );

    // New Code
    for( int yOffset = 0; yOffset < height - 1; yOffset += stepSize ) {
        for( int xOffset = 0; xOffset < width; xOffset += stepSize ) {
            pixel[0] = partialDiamond( heightMap, stepSize, xOffset, yOffset ) + uniDist( rEng ) * randFact;
            heightMap.set( xOffset, yOffset + stepSize / 2, pixel );
        }
    }
    for( int yOffset = 0 - stepSize / 2; yOffset < height - 1; yOffset += stepSize ) {
        for( int xOffset = stepSize / 2; xOffset < width; xOffset += stepSize ) {
            pixel[0] = partialDiamond( heightMap, stepSize, xOffset, yOffset ) + uniDist( rEng ) * randFact;
            heightMap.set( xOffset, yOffset + stepSize / 2, pixel );
        }
    }
}

void mapGenerator::dsGenerate( monoMapData &heightMap ) {
    // Setup well used variables
    unsigned int width = heightMap.getWidth();
    unsigned int height = heightMap.getHeight();
    unsigned int stepInit = (width - 1) / heightMap.getWidthTerm();
    unsigned int stepSize = stepInit;
    unsigned int res = heightMap.getRes();
    unsigned int resPwr = heightMap.getResPwr();
    float rand;

    std::array<png_byte, 1> pixel;
    uniform_real_distribution< float > uniDist( -1, 1 );

    cout << "Generating" << endl;
    cout << "  Width Term: " << heightMap.getWidthTerm() << endl;
    cout << "  Height Term: " << heightMap.getHeightTerm() << endl;
    cout << "  Width: " << width << endl;
    cout << "  Height: " << height << endl;
    cout << "  Resolution: " << res << endl;
    cout << "  Wrap: " << ( heightMap.getWrap() ? "true" : "false" ) << endl;
//    seed = 5;

    
    // Set initial values
    int extra = 0;
    if( !heightMap.getWrap() ) {
        extra = 1;
    }

    for( unsigned int countY = 0; countY < heightMap.getHeightTerm() + extra; countY++ ) {
        for( unsigned int countX = 0; countX < heightMap.getWidthTerm() + extra; countX++ ) {
            pixel[0] = round( uniDist( rEng ) * 128 + 127 );
            heightMap.set( countX * res, countY * res, pixel );
        }
    }

    //stringstream ss;
    //double debugNum = 0;
    rand = 256 * resPwr / 14;
    while( stepSize > 1 ) {
        /*
        cout << "stepSize: " << stepSize << endl;
        cout << "randomness: " << randomFactor << endl;
        cout << "stepSize: " << stepSize << endl;
        cout << "rand: " << rand << endl;
        */
        //ss.str("");
        //ss << "heightMap" << debugNum;
        //debugNum += 0.5;
        //heightMap.writeImage(ss.str());
        squareStep( heightMap, stepSize, rand );

        //ss.str("");
        //ss << "heightMap" << debugNum;
        //debugNum += 0.5;
        //heightMap.writeImage(ss.str());
        diamondStep( heightMap, stepSize, rand );

        stepSize /= 2;
        rand /= 2;
    }
}

void mapGenerator::colourise(
        monoMapData &heightMap, colourMapData &colourMap,
        vector<biomeNode> &biomes
        ) {
    unsigned int pixelsTotal = heightMap.getRealWidth() * heightMap.getRealHeight();
    unsigned int biomeCount = biomes.size();
    unsigned int biomeHeight = 0;
    unsigned int prevHeight = 0;
    vector<unsigned int> counter( 256, 0 ); // Container to count pixels

    cout << "Colouring" << endl;

    if( shadingFactor > 1.0 ) shadingFactor = 1.0;

    for( unsigned int i = 0; i < pixelsTotal; i++ ) {
        ++counter[ heightMap.at( i ) ];
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
            if( heightMap.at( j ) <= biomeHeight && heightMap.at( j ) >= prevHeight ) {
                float colourFactor =
                    1.0 - shadingFactor + (float)heightMap.at( j ) / ( biomeHeight ) * shadingFactor;
                colourMap.at( j * 3 ) = biomes.at(i).red * colourFactor;
                colourMap.at( j * 3 + 1 ) = biomes.at(i).green * colourFactor;
                colourMap.at( j * 3 + 2 ) = biomes.at(i).blue * colourFactor;
            }
        }
        cout << prevHeight << " " << biomeHeight << endl;
        prevHeight = biomeHeight;
    }
    cout << "Colouring finished" << endl;
}
