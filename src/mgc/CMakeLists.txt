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

# The executable and which files are required to build it
add_executable( ${MGC_NAME}
    main.cpp
    mgc-opts.cpp
    ../common.cpp
)

set_property( TARGET ${MGC_NAME} PROPERTY CXX_STANDARD 11 )

# Add libraries that need to be linked
target_link_libraries( ${MGC_NAME} ${LIBMG_NAME} png )
