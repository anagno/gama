/*  
    Geodesy and Mapping C++ Library (GNU GaMa / GaMaLib)
    Copyright (C) 2001  Ales Cepek <cepek@fsv.cvut.cz>

    This file is part of the GNU GaMa / GaMaLib C++ Library.
    
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

#ifndef GaMaLib_Acord___accord___header___h
#define GaMaLib_Acord___accord___header___h

#include <gnu_gama/local/local/gamadata.h>
#include <gnu_gama/local/local/acord/reduce_observations.h>
#include <gnu_gama/local/local/acord/reduce_to_ellipsoid.h>
#include <fstream>
#include <algorithm>
#include <list>
#include <set>

namespace GaMaLib {


  class Acord 
    {
    public: 
      
      PointData&          PD;
      ObservationData&    OD;
      ReducedObservations RO;
	
      Acord(PointData& b, ObservationData& m);
      void execute();

      int  observations;
      int  given_xy, given_z, given_xyz;
      int  computed_xy, computed_z, computed_xyz;
      int  total_xy, total_z, total_xyz;
      bool missing_coordinates;

    private:
      std::set<PointID> set_xyz, set_xy, set_z;
    };

}   // namespace GaMaLib

#endif

