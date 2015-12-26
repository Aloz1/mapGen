# Copyright (C) 2016 Alastair Knowles
#
# This file is part of mapGen.
#
# mapGen is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# mapGen is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with mapGen.  If not, see <http://www.gnu.org/licenses/>.


# State the minimum version of cmake required
cmake_minimum_required( VERSION 3.0 )

# Project info
set( PROJECT_NAME mapGen )
set( MAPGEN_VERSION_MAJOR 0 )
set( MAPGEN_VERSION_MINOR 1 )

project( ${PROJECT_NAME} )

#set( FULLBINNAME ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME} )

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Release CACHE string
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE )
endif( NOT CMAKE_BUILD_TYPE )

# File locations
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

include_directories( ${CMAKE_SOURCE_DIR}/include )

set( MGC_NAME mgc )
set( LIBMG_NAME mg )

# Other CMakeLists.txt
add_subdirectory( ${CMAKE_SOURCE_DIR}/src/mgc )
add_subdirectory( ${CMAKE_SOURCE_DIR}/src/libmapGen )
