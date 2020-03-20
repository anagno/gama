/*
  GNU Gama C++ library
  Copyright (C) 1999, 2018  Ales Cepek <cepek@fsv.cvut.cz>

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

#ifndef gama_local_Bod_Mer_BMFCE_H
#define gama_local_Bod_Mer_BMFCE_H

#include <cmath>
#include <gnu_gama/local/gamadata.h>

namespace GNU_gama { namespace local {

void bearing_distance(const LocalPoint& a, const LocalPoint& b,
                      double& br, double& d);
void bearing_distance(double ya, double xa, double yb, double xb,
                      double& br, double& d);

double bearing (const LocalPoint& a,  const LocalPoint& b);
double bearing (double ya, double xa, double yb, double xb);

double distance(const LocalPoint& a, const LocalPoint& b);

}}   // namespace GNU_gama::local

#endif
