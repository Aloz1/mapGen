// Copyright (C) 2013-2016 Alastair Knowles
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
#ifndef HEIGHT_NODE_HPP
#define HEIGHT_NODE_HPP

#include <cstdint>

struct biomeNode {
    float height = 0;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
};

#endif
