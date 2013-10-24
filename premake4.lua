-- Copyright 2013 Alastair Knowles
--
-- This file is part of mapGen.
--
-- mapGen is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- Foobar is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with mapGen.  If not, see <http://www.gnu.org/licenses/>.

solution "mapGen"
    configurations { "Debug", "Release" }
    project "mapGen"
        kind "ConsoleApp"
        language "C++"
        links { "png" }
        includedirs { "include" }
        files { "include/**.hpp", "src/**.cpp" }
        location "build"
        configuration "windows"
            -- Windows properties

        configuration "non windows"
            -- Non windows properties

        configuration "linux"
            -- Linux properties
            buildoptions { "-std=c++11" }

        configuration "macosx"
            -- Mac OSx properties

        configuration "Debug"
            defines { "DEBUG" }
            flags { "Symbols" }
            targetdir "bin/Debug"

        configuration "Release"
            defines { "RELEASE" }
            flags { "Optimize" }
            targetdir "bin/Release"
