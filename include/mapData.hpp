// Copyright 2014 Alastair Knowles
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
#ifndef MAP_DATA_HPP
#define MAP_DATA_HPP

// Standard C++ includes
#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Standard C includes
#include <cmath>

// Library includes
#include <png.h>
#include "common.hpp"


template< size_t N >
class mapData {
    std::vector<png_byte> pixelArray;
    
    //unsigned int width;
    //unsigned int height;
    unsigned int widthTerm;
    unsigned int heightTerm;
    unsigned int resolution;
    bool wrap;

    const png_byte bitDepth = 8;

    png_byte colourType;

    void setColourType();

  public:
    std::array<png_byte, N> get( int X, int Y ) const;

    void set( unsigned int X, unsigned int Y, std::array<png_byte, N> );

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    unsigned int getRealWidth() const;
    unsigned int getRealHeight() const;

    unsigned int getWidthTerm() const;
    unsigned int getHeightTerm() const;

    unsigned int getBlockWidth() const;
    unsigned int getBlockHeight() const;

    unsigned int getRes() const;
    unsigned int getWrap() const;

    void writeImage( const std::string & );

    png_byte& at( unsigned int );

    // Constructor/Destructor
    mapData(
            unsigned int BlkWidth, unsigned int BlkHeight,
            unsigned int Res, bool Wrap = true
            );
    mapData( mapData<N> &copiedData );
    ~mapData();
};

typedef mapData<1> monoMapData;
typedef mapData<3> colourMapData;


// Constructor
template< size_t N >
mapData<N>::mapData( unsigned int BlkWidth, unsigned int BlkHeight, unsigned int ResPwr, bool Wrap ) {
    unsigned int num = BlkWidth;
    unsigned int denom = BlkHeight;
    unsigned int gcf = 0;

    // Get the greatest common denomenator of the block height and width.
    while( !gcf ){
        unsigned int numTmp = denom;
        unsigned int denomTmp = num % denom;
        if( denomTmp == 0 ) {
            gcf = denom;
        } else {
            num = numTmp;
            denom = denomTmp;
        }
    }
    widthTerm = BlkWidth / gcf;
    heightTerm = BlkHeight / gcf;
    resolution = pow( 2, ResPwr );


    // Set colour type of the PNG based off of the template size.
    setColourType();

    // Set if map is to wrap
    wrap = Wrap;

    // Create array with size multiplied by No. of subpixels
    // Create array with width and height as multiples of 2 and with N subpixels
    if( wrap ) {
        pixelArray.resize( widthTerm * heightTerm * pow( resolution, 2 ) * N );
    } else {
        pixelArray.resize( ( widthTerm * resolution + 1 ) * ( heightTerm * resolution + 1) * N );
    }
}

// Destructor
template< size_t N >
mapData<N>::~mapData() {
}

// Default colourType to RGBA when any other size is chosen as it is the largest type
template< size_t N >
void mapData<N>::setColourType() {
    colourType = PNG_COLOR_TYPE_RGBA;
}

// Need to add other colour types later

// Overload colourType to greyscale when using 1 byte
template<>
void mapData<1>::setColourType() {
    colourType = PNG_COLOR_TYPE_GRAY;
}

// Overload colourType to RGB when using 3 bytes
template<>
void mapData<3>::setColourType() {
    colourType = PNG_COLOR_TYPE_RGB;
}


// Get a byte array representing a pixel of N subpixels
template< size_t N >
std::array<png_byte, N> mapData<N>::get( int X, int Y ) const { 
    std::array<png_byte, N> pixels = {0};
    unsigned long position = 0;
    unsigned int width = getRealWidth();
    unsigned int height = getRealHeight();

    if( wrap ) {
        position = ( mod( X, width ) + mod( Y, height ) * width ) * N;
    } else {
        if( X >= width ) {
            std::cerr << "X: " << X << ", width: " << width << std::endl;
            throw std::runtime_error( "mapData<N>::get(): X too large for get statement" );
        }
        if( Y >= height ) {
            std::cerr << "Y: " << Y << ", height: " << height << std::endl;
            throw std::runtime_error( "mapData<N>::get(): Y too large for get statement" );
        }
        position = ( X + Y * width ) * N;
    }

    for( size_t i = 0; i < N; i++ ) {
        if( i != 0 ) {
            std::cout << i << std::endl;
        }
        pixels[i] = pixelArray[ position + i ];
    };
    return pixels;
}

template< size_t N >
void mapData<N>::set( unsigned int X, unsigned int Y, std::array<png_byte, N> data ) {
    unsigned int width = getRealWidth();
    unsigned int height = getRealHeight();

    if( wrap ) {
        for( size_t i = 0; i < N; i++ ) {
            pixelArray[ mod( X, width ) * N + mod( Y, height ) * width + i ] = data[i];
        }
    } else {
        if( X >= width ) {
            std::cerr << "X: " << X << ", width: " << width << std::endl;
            throw std::runtime_error( "mapData<N>::set(): X too large for set statement" );
        }
        if( Y >= height ) {
            std::cerr << "Y: " << Y << ", height: " << height << std::endl;
            throw std::runtime_error( "mapData<N>::set(): Y too large for set statement" );
        }
        for( unsigned int i = 0; i < N; i++ ) {
            pixelArray[ X * N + Y * width + i ] = data[i];
        }
    }
}

template< size_t N >
unsigned int mapData<N>::getWidth() const {
    return widthTerm * resolution + 1;
}

template< size_t N >
unsigned int mapData<N>::getHeight() const {
    return heightTerm * resolution + 1;
}

template< size_t N >
unsigned int mapData<N>::getRealWidth() const {
    if( wrap ) {
        return widthTerm * resolution;
    } else {
        return widthTerm * resolution + 1;
    }
}

template< size_t N >
unsigned int mapData<N>::getRealHeight() const {
    if( wrap ) {
        return heightTerm * resolution;
    } else {
        return heightTerm * resolution + 1;
    }
}

template< size_t N >
unsigned int mapData<N>::getWidthTerm() const {
    return widthTerm;
}

template< size_t N >
unsigned int mapData<N>::getHeightTerm() const {
    return heightTerm;
}

template< size_t N >
unsigned int mapData<N>::getRes() const {
    return resolution;
}

template< size_t N >
unsigned int mapData<N>::getWrap() const {
    return wrap;
}

template< size_t N >
png_byte& mapData<N>::at( uint32_t i ) {
    return pixelArray.at( i );
}

template< size_t N >
void mapData<N>::writeImage( const std::string &name ) {
    // Get width and height
    unsigned int width = getRealWidth();
    unsigned int height = getRealHeight();

    // Create C usable variables for image
    png_structp png_ptr =  png_create_write_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
    png_infop info_ptr = png_create_info_struct( png_ptr );
    png_bytep rowPointer = pixelArray.data();

    // Generate filename
    std::string fileName = name + ".png";


    // Open file for write
    FILE *filePointer = fopen( fileName.data(), "wb" );
    if( filePointer == NULL) {
        throw std::runtime_error( "[writeImage] Unable to open file for write" );
    }
    
    // Initialise file
    if( setjmp( png_jmpbuf( png_ptr ) ) ) {
        png_free_data( png_ptr, info_ptr, PNG_FREE_ALL, -1 );
        throw std::runtime_error( "[writeImage] init_i was unsucessful" );
    }
    png_init_io( png_ptr, filePointer );
    
    // Write header
    if( setjmp( png_jmpbuf( png_ptr ) ) ) {
        png_free_data( png_ptr, info_ptr, PNG_FREE_ALL, -1 );
        throw std::runtime_error( "[writeImage] Unable to write header" );
    }
    png_set_IHDR( png_ptr, info_ptr, width, height, bitDepth,
                  colourType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                  PNG_FILTER_TYPE_BASE );
    png_write_info( png_ptr, info_ptr );

    // Set jump point for writing bytes
    if( setjmp( png_jmpbuf( png_ptr ) ) ) {
        png_free_data( png_ptr, info_ptr, PNG_FREE_ALL, -1 );
        throw std::runtime_error( "[writeImage] Unable to write data" );
    }

    //Write bytes
    if( colourType == PNG_COLOR_TYPE_RGB ) {
        for( int i = 0; i < height; i++ ){
            // Iterate through and write each row
            png_write_row( png_ptr, rowPointer + ( i * width * 3 ) );
        }

    }
    else {
        for( int i = 0; i < height; i++ ){
            // Iterate through and write each row
            png_write_row( png_ptr, rowPointer + ( i * width ) );
        }
    }

    // Write end
    if( setjmp( png_jmpbuf( png_ptr ) ) ) {
        png_free_data( png_ptr, info_ptr, PNG_FREE_ALL, -1 );
        throw std::runtime_error( "[writeImage] Unable to write ending" );
    }
    
    // Cleanup
    fclose( filePointer );
    png_free_data( png_ptr, info_ptr, PNG_FREE_ALL, -1 );

    return;
}

#endif
