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
