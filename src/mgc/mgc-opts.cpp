#include <vector>
#include <iostream>
#include <stdexcept>

#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <unistd.h>
#include <getopt.h>

#include "mgc-opts.hpp"
#include "heightNode.hpp"

using namespace std;

const char *VERSION = "0.1.1";

void printHelpMessage( const char *name ) {
    cerr << "mapGenCli " << VERSION << ", random map generator" << endl;
    cerr << "Copyright (C) Alastair Knowles 2013-2016" << endl;
    cerr << endl;

    cerr << "Usage: " << name << " [OPTION]\n"
            "\n"
            "OPTIONS:\n"
            "  -b, --biomes=PATTERN     Used to setup biomes as per what is described in"
            "                           the BIOME PATTERN section below\n"
            "  -r, --res=NUM            Resolution of image (NUM^2)\n"
            "      --resolution=NUM\n"
            "  -R, --randomness=NUM     The amount of randomness between each pixel\n"
            "  -w, --width=NUM          Defines the width component of the images aspect ratio\n"
            "  -h, --height=NUM         Defines the height component of the images aspect ratio\n"
            "  -x, --wrapX              Enables map wrapping in the X direction\n"
            "  -x, --wrapY              Enables map wrapping in the Y direction\n"
            "  -?, --help               Shows this help message\n"
            "\n"
            "BIOME PATTERN\n"
            "  Biome patterns are as follows: HEIGHT1:COLOUR1;HEIGHT2:COLOUR2;...;HEIGHT3:COLOUR3\n"
            "\n"
            "  HEIGHT: A number between 0 and 1 which represents the starting height of the\n"
            "          biome.\n"
            "\n"
            "  COLOUR: A 3 or 6 digit hexadecimal number representing the biomes colour.\n"
            "          All 3 digit numbers are expanded such that XYZ becomes XXYYZZ\n"
            "\n"
            "EXAMPLES\n"
            "  " << name << " -r 4 -w 5 -h 6\n"
            "\n";
}

CLI_mapGen_opts::CLI_mapGen_opts( int argc, char **argv ):
    res( 8 ),
    width( 3 ),
    height( 2 ),
    wrap( true ),
    randomFactor( 1.0 )
{
    if( argc > 0 ) {
        parseOptions( argc, argv );
    }
}

void CLI_mapGen_opts::parseOptions( int argc, char *argv[] ) {
    int optn = 0;

    static option long_options[] = {
        { "biomes",     required_argument,  0,  'b'  },
        { "res",        required_argument,  0,  'r'  },
        { "resolution", required_argument,  0,  'r'  },
        { "randomness", required_argument,  0,  'R'  },
        { "width",      required_argument,  0,  'w'  },
        { "height",     required_argument,  0,  'h'  },
        { "wrapX",      no_argument,        0,  'x'  },
        { "wrapY",      no_argument,        0,  'y'  },
        { "help",       no_argument,        0,  '?'  },
        { 0,            0,                  0,   0,  }
    };
    int option_index = 0;

    while( (optn = getopt_long( argc, argv, "b:r:w:h:xy?", long_options, &option_index)) != -1 ) {
        switch( optn ) {
            case 'r':
                res = atoi( optarg );
                break;
            case 'R':
                randomFactor = atoi( optarg );
                break;
            case 'w':
                width = atoi( optarg );
                break;
            case 'h':
                height = atoi( optarg );
                break;
            case 'x':
                wrap = true;
                break;
            case 'y':
                wrap = true;
                break;
            case 'b':
                parseBiomes( optarg );
                break;
            case '?':
                printHelpMessage( argv[0] );
                exit( 1 );
                break;
            default:
                cerr << "Error: " << optarg << "is not a valid argument" << endl;

                printHelpMessage( argv[0] );
                exit( 1 );
        }
        option_index = 0;
    }
}

vector<string> splitString( string toSplit, string delimiter ) {
    vector<string> result;
    size_t prevPos = 0;
    size_t nextPos = 0;

    while( nextPos != string::npos ) {
        nextPos = toSplit.find( delimiter, prevPos );
        if( nextPos == string::npos ) {
            result.push_back( toSplit.substr( prevPos ) );
            break;
        }
        result.push_back( toSplit.substr( prevPos, nextPos - prevPos ) );
        prevPos = nextPos + delimiter.length();
    }

    return result;
}

void CLI_mapGen_opts::parseBiomes( char *biomeText ) {
    vector<string> parameters = splitString( string( biomeText ), string(";") );
    for( size_t i = 0; i < parameters.size(); i++ ) {
        cout << parameters[i] << endl;
    }
}
