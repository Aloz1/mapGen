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

#ifndef MAP_GENERATOR_HPP
#define MAP_GENERATOR_HPP

#include <string>
#include <cstdint>
#include "pixelData.hpp"

// Class Definitions
class mapGenerator {
    // Variables
    unsigned int size;
    

  public:
    // Variables
    pixelData pixel;

    // Functions
    uint8_t getSize();
    void dsGenerate( int randomFactor, int seed = 0 );
    int writeImage();
    void convColour();
    
    // Constructor/Destructor
    mapGenerator( uint8_t size, png_byte bDepth, png_byte clrType, const std::string &outName );
    ~mapGenerator();
};
#endif
