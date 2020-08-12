/*
    GNU Gama --- Geodesy and Mapping C++ library
    Copyright (C) 1999, 2003, 2007, 2018  Ales Cepek <cepek@gnu.org>

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
    along with GNU Gama.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "UtilitiesBusinessDLL.h"

#include <string>

namespace GNU_gama {

  UtilitiesBusinessAPI std::string GNU_gama_version();
  UtilitiesBusinessAPI std::string GNU_gama_compiler();

  extern std::string GNU_gama_year;

  UtilitiesBusinessAPI int version(const char* program, const char* copyright_holder);
}
