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
#include <random>

// Standard C includes
#include <cstdint>

// Definition includes
#include "mapData.hpp"
#include "heightNode.hpp"

// Class Definitions
class mapGenerator {
    // Variables
    int seed;
    float randomFactor;
    float shadingFactor;

    // Private Functions
    void squareStep( monoMapData &heightMap, int stepSize, int rand );
    void diamondStep( monoMapData &heightMap, int stepSize, int rand1, int rand2 );
    int foldback( int range, int position );

    int partialDiamond( monoMapData &heightMap, int stepSize, int xOffset, int yOffset );

  public:
    // Functions
    uint8_t getSize();
    void dsGenerate( monoMapData &heightMap );
    void simplexGenerate( monoMapData &heightMap );

    void colourise( monoMapData &heightMap, colourMapData &colourMap, std::vector<biomeNode> &biomes );

    // Constructor/Destructor
    mapGenerator( int Seed = 0, float RandomFactor = 1.0, float ShadingFactor = 1.0 );
};
#endif
