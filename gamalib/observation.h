/*  
    Geodesy and Mapping C++ Library (GNU GaMa / GaMaLib)
    Copyright (C) 2000  Ales Cepek <cepek@fsv.cvut.cz>

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
 *  $Id: observation.h,v 1.1 2001/12/07 12:22:42 cepek Exp $
 */

#ifndef GaMaLib_Bod_Mer_Mereni_H
#define GaMaLib_Bod_Mer_Mereni_H

#ifdef _MSC_VER
      #define CLONE(ptr) Observation*
#else
      #define CLONE(ptr) ptr
#endif

#include <gamalib/float.h>
#include <gamalib/pointid.h>
#include <gamalib/revision.h>
#include <gamalib/linearization.h>
#include <gamalib/exception.h>
#include <gamalib/language.h>

#include <iostream>
#include <vector>

namespace GaMaLib {

  class Cluster;
  class PointData;
  class Observation;


  typedef std::vector<Observation*> ObservationList ;
  
  class Observation 
    {
    protected:
    
      Cluster* cluster;
      int      cluster_index;
      friend   Cluster;
    
    public:
    
      Observation(const PointID& s, const PointID& c, Double m)
        : cluster(0), from_(s), to_(c), value_(m), active_(true) 
        {
          if (s == c) throw GaMaLib::Exception(T_GaMa_from_equals_to);
        }
      virtual ~Observation() {}

      virtual Observation* clone() const = 0; 

      const Cluster* ptr_cluster() const     { return cluster; }
      Cluster*       ptr_cluster()           { return cluster; }
      void           set_cluster(Cluster* c) { cluster = c; }
      int&           tag() const;

      const PointID& from() const { return from_;    }
      const PointID& to()   const { return to_;      }
      Double value()        const { return value_;   }
      Double stdDev()       const ;
      bool   active()       const { return active_;  }
      void   set_active ()  const { active_ = true;  }
      void   set_passive()  const { active_ = false; }

      bool revision(const Revision* rev) const 
        { 
          return rev->revision(this); 
        }
      void linearization(const Linearization* lin) const
        {
          lin->linearization(this);
        }

      virtual void write(std::ostream&, bool print_at) const = 0;

      // function objects to be used with ObservationData::for_each()

      class CopyTo {
        ObservationList& OL;
      public:
        CopyTo(ObservationList& ol) : OL(ol) {}
        void operator()(Observation* obs) { OL.push_back(obs); }
      };
      
      class CopyActiveTo {
        ObservationList& OL;
      public:
        CopyActiveTo(ObservationList& ol) : OL(ol) {}
        void operator()(Observation* obs) { 
          if(obs->active()) OL.push_back(obs); 
        }
      }; 

      class CopyHorizontalTo {    // directions, angles, distances
        ObservationList& OL;
      public:
        CopyHorizontalTo(ObservationList& ol) : OL(ol) {}
        void operator()(Observation* obs);
      }; 

    private:
    
      const PointID from_;
      const PointID to_;
      Double        value_;        // observed value
      mutable bool  active_;       // set false for unused observation
    
    protected:
    
      void norm_rad_val() 
        {
          while (value_ >= 2*M_PI) value_ -= 2*M_PI;
          while (value_ <   0    ) value_ += 2*M_PI;
        };

    };
        

  class Distance : public Observation 
    {
    public:
      Distance(const PointID& s, const PointID& c, Double d)
        : Observation(s, c, d) 
        { 
          if (d <= 0) 
            throw GaMaLib::Exception(T_POBS_zero_or_negative_distance);
        }
      ~Distance() {}

      CLONE(Distance*) clone() const { return new Distance(*this); }

      void write(std::ostream&, bool print_at) const;
    };
 

  class Direction : public Observation 
    {
    public:
      Direction(const PointID& s, const PointID& c,  Double d)
        : Observation(s, c, d) 
        { 
          norm_rad_val(); 
        }
      ~Direction() {}

      CLONE(Direction*) clone() const { return new Direction(*this); }

      void write(std::ostream&, bool print_at) const;
     
      Double orientation() const; 
      void   set_orientation(Double p);
      bool   test_orientation() const; 
      void   delete_orientation();     
      void   index_orientation(int n); 
      int    index_orientation() const;
    };
 

  class Angle : public Observation 
    {
    private:
      PointID rs_;
    public:
      Angle(const PointID& s, const PointID& c,  const PointID& c2,
            Double d) : Observation(s, c, d), rs_(c2) 
        { 
          if (s == c2 || c == c2) 
            throw GaMaLib::Exception(T_GaMa_from_equals_to);
          norm_rad_val(); 
        }
      ~Angle() {}

      CLONE(Angle*) clone() const { return new Angle(*this); }

      const PointID& rs() const { return rs_; }     // right sight station
      void write(std::ostream&, bool print_at) const;
    };


  // height differences 

  class H_Diff : public Observation 
    {
    private:
      Double dist_;
    public:
      H_Diff(const PointID& s, const PointID& c, Double dh, Double d=0)
        : Observation(s, c, dh), dist_(d) 
        { 
          if (d < 0)   // zero distance is legal in H_Diff 
            throw GaMaLib::Exception(T_POBS_zero_or_negative_distance);
        }
      ~H_Diff() {}

      CLONE(H_Diff*) clone() const { return new H_Diff(*this); }

      void write(std::ostream&, bool print_at) const;

      void   set_dist(Double d) 
        { 
          if (d < 0)    // zero distance is legal in H_Diff 
            throw GaMaLib::Exception(T_POBS_zero_or_negative_distance);
          dist_ = d;    
        }
      Double dist() const { return dist_; }
    };
 

  // coordinate differences (vectors)  dx, dy, dz

  class Xdiff : public Observation
    {
    public:
      Xdiff(const PointID& from, const PointID& to, Double dx) 
        : Observation(from, to, dx) {}
      ~Xdiff() {}

      CLONE(Xdiff*) clone() const { return new Xdiff(*this); }

      void write(std::ostream&, bool print_at) const;
    };

  class Ydiff : public Observation
    {
    public:
      Ydiff(const PointID& from, const PointID& to, Double dy) 
        : Observation(from, to, dy) {}
      ~Ydiff() {}

      CLONE(Ydiff*) clone() const { return new Ydiff(*this); }

      void write(std::ostream&, bool) const;
    };

  class Zdiff : public Observation
    {
    public:
      Zdiff(const PointID& from, const PointID& to, Double dz) 
        : Observation(from, to, dz) {}
      ~Zdiff() {}

      CLONE(Zdiff*) clone() const { return new Zdiff(*this); }

      void write(std::ostream&, bool print_at) const;
    };

 
  // coordinate observations (observed coordinates)  x, y, z

  class X : public Observation
    {
    public:
      X(const PointID& point, Double x) : Observation(point, "", x) {}
      ~X() {}

      CLONE(X*) clone() const { return new X(*this); }

      void write(std::ostream&, bool print_at) const;
    };

  class Y : public Observation
    {
    public:
      Y(const PointID& point, Double y) : Observation(point, "", y) {}
      ~Y() {}

      CLONE(Y*) clone() const { return new Y(*this); }

      void write(std::ostream&, bool) const;
    };

  class Z : public Observation
    {
    public:
      Z(const PointID& point, Double z) : Observation(point, "", z) {}
      ~Z() {}

      CLONE(Z*) clone() const { return new Z(*this); }

      void write(std::ostream&, bool print_at) const;
    };

 
  // slope observations (slope distances and zenith angles)

  class S_Distance : public Observation 
    {
    public:
      S_Distance(const PointID& s, const PointID& c, Double d)
        : Observation(s, c, d) 
        { 
          if (d <= 0) 
            throw GaMaLib::Exception(T_POBS_zero_or_negative_distance);
        }
      ~S_Distance() {}

      CLONE(S_Distance*) clone() const { return new S_Distance(*this); }

      void write(std::ostream&, bool print_at) const;
    };
 

  class Z_Angle : public Observation 
    {
    public:
      Z_Angle(const PointID& s, const PointID& c, Double d)
        : Observation(s, c, d) 
        { 
          if (d <= 0) 
            throw GaMaLib::Exception(T_POBS_zero_or_negative_distance);
        }
      ~Z_Angle() {}

      CLONE(Z_Angle*) clone() const { return new Z_Angle(*this); }

      void write(std::ostream&, bool print_at) const;
    };
 

}   // namespace GaMaLib

#undef CLONE

#endif







