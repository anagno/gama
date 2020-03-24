/*
    GNU Gama --- Geodesy and Mapping C++ library
    Copyright (C) 2004  Ales Cepek <cepek@gnu.org>

    This file is part of the GNU Gama C++ library.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

namespace GNU_gama {

inline constexpr double PI = 3.141592653589793238462643383279502884197169399375105820;

inline constexpr double RAD_TO_GON = 200.0/PI;

inline constexpr double GON_TO_RAD = PI/200.0;

inline constexpr double RAD_TO_CC = 200.0e4/PI;

inline constexpr double CC_TO_RAD = PI/200.0e4;

inline constexpr double RAD_TO_DEG = 180.0/PI;

inline constexpr double DEG_TO_RAD = PI/180.0;

inline constexpr double RAD_TO_SS = 648000.0/PI;

inline constexpr double SS_TO_RAD = PI/648000.0;

} // namespace GNU_gama