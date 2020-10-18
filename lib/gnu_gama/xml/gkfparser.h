/*
    GNU Gama -- adjustment of geodetic networks
    Copyright (C) 2000, 2002, 2013, 2014, 2018  Ales Cepek <cepek@gnu.org>

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

#include <gnu_gama/GaMaDLL.h>

#include <gnu_gama/local/network.h>
#include <gnu_gama/exception.h>
#include <iostream>

namespace GNU_gama
{
namespace local
{
namespace GKFparser
{

using ParserException = GNU_gama::Exception::parser;

GaMaAPI std::istream& operator>>(std::istream& input, GNU_gama::local::LocalNetwork& network);

}  // namespace GKFparser
}  // namespace local
}  // namespace GNU_gama
