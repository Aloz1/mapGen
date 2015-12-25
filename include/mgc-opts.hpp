// Copyright (C) 2015-2016 Alastair Knowles
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

#ifndef CLI_MAPGEN_OPTS
#define CLI_MAPGEN_OPTS

#include <vector>
#include <cstdint>

#include "heightNode.hpp"

class CLI_mapGen_opts {
    uint8_t res;
    uint8_t width;
    uint8_t height;
    bool wrapX;
    bool wrapY;
    float randomFactor;
    std::vector<biomeNode> biomes;

  public:
    CLI_mapGen_opts( int argc = 0, char **argv = nullptr );
    void parseOptions( int argc, char **argv );
    void parseBiomes( char *biomeText );

    uint8_t getRes();
    uint8_t getWidth();
    uint8_t getHeight();
    bool getWrapX();
    bool getWrapY();
    float getRandFact();
    std::vector<biomeNode>* getBiome();
};

#endif
