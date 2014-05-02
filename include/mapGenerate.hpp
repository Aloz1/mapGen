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

// Include guard
#ifndef MAP_GENERATOR_HPP
#define MAP_GENERATOR_HPP

// Standard C++ includes
#include <string>

// Standard C includes
#include <cstdint>

// Definition includes
#include "pixelData.hpp"
#include "heightNode.hpp"

// Class Definitions
class mapGenerator {
    // Variables
    unsigned int size;
    pixelData *heightMap, *colourMap;

  public:
    // Functions
    uint8_t getSize();
    void dsGenHeight( int randomFactor, int seed = 0 );
    void simplexGenHeight( int randomFactor, int seed = 0 );
    void convColour( std::vector<biomeNode> &biomes, float shadingFactor = 1.0 );
    void writeImage( const std::string &name );
    void writeImage( const std::string &heightName, const std::string &colourName );
    
    // Constructor/Destructor
    mapGenerator( uint8_t size, png_byte bDepth, png_byte clrType );
    ~mapGenerator();
};
#endif
