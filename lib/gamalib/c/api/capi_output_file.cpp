/*  
    Geodesy and Mapping C++ Library (GNU GaMa / GaMaLib)
    Copyright (C) 2001  Ales Cepek <cepek@fsv.cvut.cz>

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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*
 *  $Id: capi_output_file.cpp,v 1.2 2006/06/06 17:31:51 cepek Exp $
 */

#include <gnu_gama/outstream.h>

#include <gamalib/c/api/capi_output_file.h>
#include <gamalib/c/api/capi_private_exception.h>
#include <gnu_gama/version.h>
#include <gamalib/exception.h>

#include <gamalib/local/results/text/adjusted_observations.h>
#include <gamalib/local/results/text/adjusted_unknowns.h>
#include <gamalib/local/results/text/approximate_coordinates.h>
#include <gamalib/local/results/text/error_ellipses.h>
#include <gamalib/local/results/text/fixed_points.h>
#include <gamalib/local/results/text/general_parameters.h>
#include <gamalib/local/results/text/network_description.h>
#include <gamalib/local/results/text/outlying_abs_terms.h>
#include <gamalib/local/results/text/residuals_observations.h>
#include <gamalib/local/results/text/test_linearization.h>
 

using namespace std;
using namespace GaMaLib;

extern "C" {

  /* C API output file constructor */
  void* Cgama_output_file(void* local_network, const char* file_name)
  {
    try 
      {
        LocalNetwork* locnet = static_cast<LocalNetwork*>(local_network);
        return new C_API_output_file(locnet, new ofstream(file_name));
      }
    catch(...)
      {
        return 0;
      }
  }

  /* C API output file destructor */
  void  Cgama_output_file_close(void* object)
  {
    C_API_output_file* capiof = static_cast<C_API_output_file*>(object);

    delete capiof;
  }

  const char* Cgama_gamalib_version()
  {
    return GNU_gama::GNU_gama_version;
  }

  void Cgama_of_string(void* object, const char* string)
  {
    try
      {
        C_API_output_file* capiof = static_cast<C_API_output_file*>(object);
        std::ofstream* of = capiof->out;
        *of << string;
      }
    catch(const GaMaLib::Exception& e)
      {
        Cgama_private_set_exception(e);
      }       
    catch(...)
      {
        Cgama_private_set_unknown_exception();
      }
  }


  void Cgama_of_adjusted_observations(void* object)
  {
    try
      {
        C_API_output_file* capiof = static_cast<C_API_output_file*>(object);
        LocalNetwork*  ln = capiof->IS;
        std::ofstream* of = capiof->out;
        AdjustedObservations(ln, *of);
      }
    catch(const GaMaLib::Exception& e)
      {
        Cgama_private_set_exception(e);
      }       
    catch(...)
      {
        Cgama_private_set_unknown_exception();
      }
  }

  void Cgama_of_adjusted_unknowns(void* object)
  {
    try
      {
        C_API_output_file* capiof = static_cast<C_API_output_file*>(object);
        LocalNetwork*  ln = capiof->IS;
        std::ofstream* of = capiof->out;
        AdjustedUnknowns(ln, *of);
      }
    catch(const GaMaLib::Exception& e)
      {
        Cgama_private_set_exception(e);
      }       
    catch(...)
      {
        Cgama_private_set_unknown_exception();
      }
  }

  void Cgama_of_approximate_coordinates(void* object)
  {
    try
      {
        C_API_output_file* capiof = static_cast<C_API_output_file*>(object);
        LocalNetwork*  ln = capiof->IS;
        std::ofstream* of = capiof->out;

        Acord acord(ln->PD, ln->OD);
        acord.execute();
        ApproximateCoordinates(&acord, *of);
      }
    catch(const GaMaLib::Exception& e)
      {
        Cgama_private_set_exception(e);
      }       
    catch(...)
      {
        Cgama_private_set_unknown_exception();
      }
  }

  void Cgama_of_error_ellipses(void* object)
  {
    try
      {
        C_API_output_file* capiof = static_cast<C_API_output_file*>(object);
        LocalNetwork*  ln = capiof->IS;
        std::ofstream* of = capiof->out;
        ErrorEllipses(ln, *of);
      }
    catch(const GaMaLib::Exception& e)
      {
        Cgama_private_set_exception(e);
      }       
    catch(...)
      {
        Cgama_private_set_unknown_exception();
      }
  }

  void Cgama_of_fixed_points(void* object)
  {
    try
      {
        C_API_output_file* capiof = static_cast<C_API_output_file*>(object);
        LocalNetwork*  ln = capiof->IS;
        std::ofstream* of = capiof->out;
        FixedPoints(ln, *of);
      }
    catch(const GaMaLib::Exception& e)
      {
        Cgama_private_set_exception(e);
      }       
    catch(...)
      {
        Cgama_private_set_unknown_exception();
      }
  }

  int Cgama_of_general_parameters(void* object)
  {
    try
      {
        C_API_output_file* capiof = static_cast<C_API_output_file*>(object);
        LocalNetwork*  ln = capiof->IS;
        std::ofstream* of = capiof->out;

        GNU_gama::OutStream output(of);
        return GeneralParameters(ln, output);
      }
    catch(const GaMaLib::Exception& e)
      {
        Cgama_private_set_exception(e);
      }       
    catch(...)
      {
        Cgama_private_set_unknown_exception();
      }
    return 0;
  }

  void Cgama_of_network_description(void* object, char* text)
  {
    try
      {
        C_API_output_file* capiof = static_cast<C_API_output_file*>(object);
        std::ofstream* of = capiof->out;
        const std::string st(text);
        NetworkDescription(st, *of);
      }
    catch(const GaMaLib::Exception& e)
      {
        Cgama_private_set_exception(e);
      }       
    catch(...)
      {
        Cgama_private_set_unknown_exception();
      }
  }

  void Cgama_of_outlying_abs_terms(void* object)
  {
    try
      {
        C_API_output_file* capiof = static_cast<C_API_output_file*>(object);
        LocalNetwork*  ln = capiof->IS;
        std::ofstream* of = capiof->out;
        OutlyingAbsoluteTerms(ln, *of);
      }
    catch(const GaMaLib::Exception& e)
      {
        Cgama_private_set_exception(e);
      }       
    catch(...)
      {
        Cgama_private_set_unknown_exception();
      }
  }

  void Cgama_of_residuals_observations(void* object)
  {
    try
      {
        C_API_output_file* capiof = static_cast<C_API_output_file*>(object);
        LocalNetwork*  ln = capiof->IS;
        std::ofstream* of = capiof->out;
        ResidualsObservations(ln, *of);
      }
    catch(const GaMaLib::Exception& e)
      {
        Cgama_private_set_exception(e);
      }       
    catch(...)
      {
        Cgama_private_set_unknown_exception();
      }
  }

  int Cgama_of_test_linearization(void* object)
  {
    try
      {
        C_API_output_file* capiof = static_cast<C_API_output_file*>(object);
        LocalNetwork*  ln = capiof->IS;
        std::ofstream* of = capiof->out;
        return TestLinearization(ln, *of) ? 1 : 0;
      }
    catch(const GaMaLib::Exception& e)
      {
        Cgama_private_set_exception(e);
      }       
    catch(...)
      {
        Cgama_private_set_unknown_exception();
      }
    return 0;
  }


}