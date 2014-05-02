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
#include <vector>
#include <stdexcept>
#include <iostream>

// Library includes
#include <png.h>

// Definition includes
#include "pixelData.hpp"

using namespace std;

pixelData::pixelData( uint32_t wdth, uint32_t hght, png_byte bDepth, png_byte clrType ):
    width( wdth ),
    height( hght ),
    bitDepth( bDepth ),
    colourType( clrType ),
    png_ptr( png_create_write_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0) ),
    info_ptr( png_create_info_struct( png_ptr ) )
{
    // Create array, if array is colour, make it 3 times larger
    if( clrType == PNG_COLOR_TYPE_GRAY ) pixelArray.resize( wdth * hght );
    else if( clrType == PNG_COLOR_TYPE_RGB ) pixelArray.resize( wdth * hght * 3 );
    else throw runtime_error( "Unknown colour type" );
}

pixelData::~pixelData() {
}

png_byte pixelData::get( uint32_t X, uint32_t Y ) const {
    if( X >= width || Y >= height ) {
        throw runtime_error( "X or Y too large" );
    }
    return pixelArray[ X + Y * height ];
}

png_byte pixelData::wrapGet( int32_t X, int32_t Y ) const {
    return pixelArray[ ( X & ( width - 1 ) ) + ( Y & ( height - 1 ) ) * height ];
}

png_byte pixelData::getColour() const {
    return colourType;
}

void pixelData::set( uint32_t X, uint32_t Y, png_byte data ) {
    if( X >= width || Y >= height ) {
        throw runtime_error( "X or Y too large" );
    }
    pixelArray[ X + Y * height ] = data;
    return;
}

void pixelData::wrapSet( int X, int Y, png_byte data ) {
    pixelArray[ ( X & ( width - 1 ) ) + ( Y & ( height - 1 ) ) * height ] = data;
    return;
}

uint16_t pixelData::getWidth() const {
    return width;
}

uint16_t pixelData::getHeight() const {
    return height;
}

png_byte& pixelData::at( uint32_t i ) {
    return pixelArray.at( i );
}

void pixelData::writeImage( const string &name ) {
    // Create C usable variables for image
    png_bytep rowPointer = pixelArray.data();

    // Generate file name
    string fileName = name + ".png";

    // Open file for write
    FILE *filePointer = fopen( fileName.data(), "wb" );
    if( filePointer == NULL) {
        throw runtime_error( "[writeImage] Unable to open file for write" );
    }
    
    // Initialise file
    if( setjmp( png_jmpbuf( png_ptr ) ) ) {
        png_free_data( png_ptr, info_ptr, PNG_FREE_ALL, -1 );
        throw runtime_error( "[writeImage] init_i was unsucessful" );
    }
    png_init_io( png_ptr, filePointer );
    
    // Write header
    if( setjmp( png_jmpbuf( png_ptr ) ) ) {
        png_free_data( png_ptr, info_ptr, PNG_FREE_ALL, -1 );
        throw runtime_error( "[writeImage] Unable to write header" );
    }
    png_set_IHDR( png_ptr, info_ptr, width, height, bitDepth,
                  colourType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                  PNG_FILTER_TYPE_BASE );
    png_write_info( png_ptr, info_ptr );

    // Write bytes
    if( setjmp( png_jmpbuf( png_ptr ) ) ) {
        png_free_data( png_ptr, info_ptr, PNG_FREE_ALL, -1 );
        throw runtime_error( "[writeImage] Unable to write data" );
    }
    for( int i = 0; i < height; i++ ){
        // Iterate through and write each row
        png_write_row( png_ptr, rowPointer + ( i * width ) );
    }

    // Write end
    if( setjmp( png_jmpbuf( png_ptr ) ) ) {
        png_free_data( png_ptr, info_ptr, PNG_FREE_ALL, -1 );
        throw runtime_error( "[writeImage] Unable to write ending" );
    }
    
    // Cleanup
    fclose( filePointer );
    png_free_data( png_ptr, info_ptr, PNG_FREE_ALL, -1 );

    return;
}
