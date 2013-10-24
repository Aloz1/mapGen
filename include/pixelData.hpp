// Copyright 2013 Alastair Knowles
//
// This file is part of mapGen.
//
// mapGen is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Foobar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with mapGen.  If not, see <http://www.gnu.org/licenses/>.

#ifndef PIXEL_DATA_HPP
#define PIXEL_DATA_HPP

#include <string>
#include <vector>

#include <cstdint>

#include <png.h>

class pixelData {
    std::vector<png_byte> pixelArray;
    std::string name;
    
    uint32_t width;
    uint32_t height;

    png_byte bitDepth;
    png_byte colourType;
    png_structp png_ptr;
    png_infop info_ptr;

  public:
    png_byte get( uint32_t X, uint32_t Y ) const;
    png_byte wrapGet( int32_t X, int32_t Y ) const;

    png_byte getColour() const;

    void set( uint32_t X, uint32_t Y, png_byte );
    void wrapSet( int32_t X, int32_t Y, png_byte );

    uint16_t getWidth() const;
    uint16_t getHeight() const;

    void writeImage();

    png_bytep raw();

    // Constructor/Destructor
    pixelData( uint32_t width, uint32_t height, png_byte bDepth, png_byte clrType, const std::string &title );
    ~pixelData();
};
#endif
