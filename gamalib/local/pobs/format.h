/*  
    Geodesy and Mapping C++ Library (GNU GaMa / GaMaLib)
    Copyright (C) 1999  Ales Cepek <cepek@fsv.cvut.cz>

    This file is part of the GNU GaMa / GaMaLib C++ Library.
    
    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
 *  $Id: format.h,v 1.1 2001/12/07 12:54:43 cepek Exp $
 */

#ifndef GaMaLib_Bod_Mer_FORMAT_H
#define GaMaLib_Bod_Mer_FORMAT_H

namespace GaMaLib {

class Format {

  static int coordinates_p;
  static int centesimal_degrees_p;
  static int standard_deviations_p;

public:

  static int coord_p(int n = coordinates_p)
             {
                int p = coordinates_p;
                coordinates_p = n;
                return p;
             }
  static int gon_p(int n = centesimal_degrees_p)
             {
                int p = centesimal_degrees_p;
                centesimal_degrees_p = n;
                return p;
             }
  static int stdev_p(int n = standard_deviations_p)
             {
                int p = standard_deviations_p;
                standard_deviations_p = n;
                return p;
             }

};

}
#endif