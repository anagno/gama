/*  
    GNU Gama -- adjustment of geodetic networks
    Copyright (C) 2003  Ales Cepek <cepek@fsv.cvut.cz>

    This file is part of the GNU Gama library.
    
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
 *  $Id: outstream.h,v 1.4 2003/03/15 21:22:52 cepek Exp $
 */

#include <iostream>
#include <string>
#include <expat/xmlparse/xmlparse.h>
#include <gamalib/xml/encoding.h>

#ifndef GNU_gama__outstream__h____output_stream__outstreamh
#define GNU_gama__outstream__h____output_stream__outstreamh

namespace GNU_gama {

  class OutStream {
  public:
    
    enum { utf_8, iso_8859_2, iso_8859_2_flat, cp_1250 }; 
    
    OutStream(std::ostream& str);
    
    OutStream& operator << (const char* c)
      {
        ostr << recode(c);
        return *this;
      }
    OutStream& operator << (const std::string& s)
      {
        ostr << recode(s.c_str());
        return *this;
      }
    
    template<class T> OutStream& operator << (const T& t)
      {
        ostr << t;
        return *this;
      }
    
    std::ostream& std_stream() { return ostr; }
    
    void setf (std::ios::fmtflags t, std::ios::fmtflags v) { ostr.setf(t, v); }
    void width     (int t)  { ostr.width(t);     }
    void precision (int t)  { ostr.precision(t); }
    void flush     ()       { ostr.flush();      }
    
    void set_encoding(int e) { encoding = e; }
    
  private:
    
    std::ostream& ostr;
    int           encoding;
    std::string   text;
    
    const char* recode(const char* s);  
  };

}

#endif
