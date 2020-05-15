/* GNU Gama C++ library
   Copyright (C) 1999, 2002, 2003, 2010, 2011, 2012, 2014, 2018, 2020
                 Ales Cepek <cepek@gnu.org>

   This file is part of the GNU Gama C++ library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

#include <Math/Business/Core/radian.h>

#ifdef   GNU_GAMA_LOCAL_SQLITE_READER
#include <gnu_gama/local/sqlitereader.h>
#endif

#include <Parsing/Business/outstream.h>

#include <cstring>
#include <gnu_gama/version.h>
#include <Math/Business/Core/intfloat.h>
#include <gnu_gama/xml/localnetworkoctave.h>
#include <gnu_gama/xml/localnetworkxml.h>
#include <gnu_gama/xml/gkfparser.h>

#include <gnu_gama/local/language.h>
#include <gnu_gama/local/gamadata.h>
#include <gnu_gama/local/network.h>
#include <gnu_gama/local/acord/acord2.h>
#include <gnu_gama/local/acord/acordstatistics.h>
#include <gnu_gama/local/svg.h>
#include <gnu_gama/local/html.h>

#include <gnu_gama/local/results/text/approximate_coordinates.h>
#include <gnu_gama/local/results/text/network_description.h>
#include <gnu_gama/local/results/text/general_parameters.h>
#include <gnu_gama/local/results/text/fixed_points.h>
#include <gnu_gama/local/results/text/adjusted_observations.h>
#include <gnu_gama/local/results/text/adjusted_unknowns.h>
#include <gnu_gama/local/results/text/outlying_abs_terms.h>
#include <gnu_gama/local/results/text/reduced_observations_to_ellipsoid.h>
#include <gnu_gama/local/results/text/residuals_observations.h>
#include <gnu_gama/local/results/text/error_ellipses.h>
#include <gnu_gama/local/test_linearization_visitor.h>
#include <gnu_gama/local/xmlerror.h>

#include <boost/program_options.hpp>


namespace GNU_gama{
namespace local{

std::istream& operator >>(std::istream& in, GNU_gama::local::LocalNetwork::Algorithm& algorithm);
std::istream& operator >>(std::istream& in, GNU_gama::local::gama_language& language);
} // namespace local

std::istream& operator >>(std::istream& in, GNU_gama::OutStream::Encoding& encoding);
} // namespace GNU_gama

enum class Angle {Gon, Degree};
std::istream& operator >>(std::istream& in, Angle& angles);
GNU_gama::local::XMLerror xmlerr;


int main(int argc, char *argv[])
  try {

    auto argv_algo = GNU_gama::local::LocalNetwork::Algorithm::envelope;
    auto argv_lang = GNU_gama::local::en;
    auto argv_enc  = GNU_gama::OutStream::utf_8;
    auto argv_angles = Angle::Gon;
    auto argv_iterations = int{5};

    auto option_description = boost::program_options::options_description( "\n"
      "Adjustment of local geodetic network version: " + GNU_gama::GNU_gama_version() +
      " / " + GNU_gama::GNU_gama_compiler() + "\n"
      "************************************\n"
      "https://www.gnu.org/software/gama/\n\n"
      "Usage: \n"
      "gama-local  input.xml  [options]\n"
#ifdef GNU_GAMA_LOCAL_SQLITE_READER
      "gama-local  input.xml  --sqlitedb sqlite.db"
       "  --configuration name  [options]\n"
       "gama-local  --sqlitedb sqlite.db  --configuration name  [options]\n"
       "gama-local  --sqlitedb sqlite.db"
       "  --readonly-configuration name  [options]\n"
#endif
      "\nOptions");

    option_description.add_options()
      ("help,h", "Display this help message")
      ("version,v", "Display the version number")
      ("input-file", boost::program_options::value<std::string>(), "The input xml that will be parsed")
      (
        "algorithm", 
        boost::program_options::value<GNU_gama::local::LocalNetwork::Algorithm>(&argv_algo), 
        "gso | svd | cholesky | envelope"
      )
      (
        "language", 
        boost::program_options::value<GNU_gama::local::gama_language>(&argv_lang), 
        "en | ca | cz | du | es | fi | fr | hu | ru | ua | zh"
      )
      (
        "encoding", 
        boost::program_options::value<GNU_gama::OutStream::Encoding>(&argv_enc), 
        "utf-8 | iso-8859-2 | iso-8859-2-flat | cp-1250 | cp-1251"
      )
      (
        "angles", 
        boost::program_options::value<Angle>(&argv_angles),
        "400 | 360"
      )
      (
        "ellipsoid", 
        boost::program_options::value<std::string>(), 
        "<ellipsoid name>"
      )
      (
        "latitude", 
        boost::program_options::value<std::string>(), 
        "<latitude>"
      )
      (
        "text", 
        boost::program_options::value<std::string>(), 
        "adjustment_results.txt"
      )
      (
        "html", 
        boost::program_options::value<std::string>(), 
        "adjustment_results.html"
      )
      (
        "xml", 
        boost::program_options::value<std::string>(), 
        "adjustment_results.xml"
      )
      (
        "octave", 
        boost::program_options::value<std::string>(), 
        "adjustment_results.m"
      )
      (
        "svg", 
        boost::program_options::value<std::string>(), 
        "network_configuration.svg"
      )
      (
        "obs", 
        boost::program_options::value<std::string>(), 
        // TODO
        "UNDOCUMENTED"
      )
      (
        "cov-band", 
        boost::program_options::value<int>(), 
        "covariance matrix of adjusted parameters in XML output\n"
        "n  = -1 \tfor full covariance matrix (implicit value)\n"
        "n >=  0 \tcovariances are computed only for bandwidth n\n"
      )
      (
        "iterations", 
        boost::program_options::value<int>(&argv_iterations),
        "maximum number of iterations allowed in the linearized least squares algorithm (implicit value is 5)"
      )
      (
        "updated-xml", 
        boost::program_options::value<std::string>(), 
        // TODO
        "UNDOCUMENTED"
      )
#ifdef GNU_GAMA_LOCAL_SQLITE_READER
      (
        "sqlitedb", 
        boost::program_options::value<std::string>(), 
        "sqlitedb sqlite.db"
      )
      (
        "configuration", 
        boost::program_options::value<std::string>(), 
        "name"
      )
      (
        "readonly-configuration", 
        boost::program_options::value<std::string>(), 
        "name"
      )
#endif
    ;

    auto positional_options = boost::program_options::positional_options_description{};
    positional_options.add("input-file", 1);

    auto option_variables = boost::program_options::variables_map{};

    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv)
            .options(option_description)
            .positional(positional_options)
            .run(),
        option_variables);


    if (option_variables.count("help"))
    {
      std::cout << option_description << std::endl;
      std::cout << "Report bugs to: https://github.com/anagno/gama\n";
      return EXIT_SUCCESS;
    }

    if (option_variables.count("version"))
    {
      return GNU_gama::version("gama-local", "Ales Cepek et al.");
    }

    set_gama_language(argv_lang);

    // implicit output
    std::ostream* output = nullptr;

    if (!option_variables.count("text") && 
        !option_variables.count("html") &&
        !option_variables.count("xml"))
    {
        output = &std::cout;
    } 
    std::ofstream fcout;
    if (option_variables.count("text"))
    {
        auto argv_txtout = option_variables["text"].as<std::string>();
        fcout.open(argv_txtout);
        output = &fcout;
    }

#ifdef GNU_GAMA_LOCAL_SQLITE_READER
    if (option_variables.count("configuration") && option_variables.count("readonly-configuration"))
    {
      std::cout << option_description << std::endl;
      return EXIT_SUCCESS;
    } 
    
    if (!option_variables.count("input-file") && !option_variables.count("sqlitedb"))
    {
      std::cout << option_description << std::endl;
      return EXIT_SUCCESS;
    }
#endif

    GNU_gama::OutStream cout(output);
    cout.set_encoding(argv_enc);

    GNU_gama::local::LocalNetwork* IS = new GNU_gama::local::LocalNetwork;

#ifdef GNU_GAMA_LOCAL_SQLITE_READER
    if (option_variables.count("sqlitedb"))
      {
        auto argv_sqlitedb = option_variables["sqlitedb"].as<std::string>();
        GNU_gama::local::sqlite_db::SqliteReader reader(argv_sqlitedb);

        auto conf = std::string{};
        if (option_variables.count("configuration")) 
          conf = option_variables["configuration"].as<std::string>();
        if (option_variables.count("readonly-configuration")) 
          conf = option_variables["readonly-configuration"].as<std::string>();;

        reader.retrieve(IS, conf);
      }
    else
#endif
      {
        auto argv_1 = option_variables["input-file"].as<std::string>();
        std::ifstream soubor(argv_1);
        GNU_gama::local::GKFparser gkf(*IS);
        try
          {
            char c;
            int  n, konec = 0;
            std::string radek;
            do
              {
                radek = "";
                n     = 0;
                while (soubor.get(c))
                  {
                    radek += c;
                    n++;
                    if (c == '\n') break;
                  }
                if (!soubor) konec = 1;

                gkf.xml_parse(radek.c_str(), n, konec);
              }
            while (!konec);
          }
        catch (const GNU_gama::local::ParserException& v) {
          if (xmlerr.isValid())
            {
              xmlerr.setDescription(GNU_gama::local::T_GaMa_exception_2a);
              std::string t, s = v.what();
              for (std::string::iterator i=s.begin(), e=s.end(); i!=e; ++i)
                {
                  if      (*i == '<') t += "\"";
                  else if (*i == '>') t += "\"";
                  else                t += *i;
                }
              xmlerr.setDescription(t);
              xmlerr.setLineNumber (v.line);
              //xmlerr.setDescription(T_GaMa_exception_2b);
              return xmlerr.write_xml("gamaLocalParserError");
            }

          std::cerr << "\n" << GNU_gama::local::T_GaMa_exception_2a << "\n\n"
               << GNU_gama::local::T_GaMa_exception_2b << v.line << " : " << v.what() << std::endl;
          return 3;
        }
        catch (const GNU_gama::local::Exception& v) {
          if (xmlerr.isValid())
            {
              xmlerr.setDescription(GNU_gama::local::T_GaMa_exception_2a);
              xmlerr.setDescription(v.what());
              return xmlerr.write_xml("gamaLocalException");
            }

          std::cerr << "\n" <<GNU_gama::local::T_GaMa_exception_2a << "\n"
               << "\n***** " << v.what() << "\n\n";
          return 2;
        }
        catch (...)
          {
            std::cerr << "\n" << GNU_gama::local::T_GaMa_exception_2a << "\n\n";
            throw;
          }
      }

    IS->set_algorithm(argv_algo);

    switch (argv_angles)
    {
    case ::Angle::Gon:
      IS->set_gons();
      break;
    case ::Angle::Degree:
      IS->set_degrees();
      break;  
    }

    if (option_variables.count("cov-band"))
      {
        int band = option_variables["cov-band"].as<int>();

        if (band < -1)
        {
            std::cout << "band variable is invalid";
            return EXIT_SUCCESS;
        }

        IS->set_adj_covband(band);
      }

    if (argv_iterations < 0)
    {
        std::cout << "The number of iterations must be a positive number";
        return EXIT_SUCCESS;
    }

    IS->set_max_linearization_iterations(argv_iterations);  

    if (option_variables.count("latitude"))
      {
        auto argv_latitude = option_variables["latitude"].as<std::string>();
        double latitude;
        if (!GNU_gama::deg2gon(argv_latitude, latitude))
          {
            if (!GNU_gama::IsFloat(argv_latitude))
            {
                std::cout << "The latitude was not valid";
                return EXIT_SUCCESS;
            }

            latitude = atof(argv_latitude.c_str());
          }

        IS->set_latitude(latitude * GNU_gama::PI/200);
      }

    if (option_variables.count("ellipsoid"))
      {
        auto argv_ellipsoid = option_variables["ellipsoid"].as<std::string>();

        GNU_gama::gama_ellipsoid gama_el = GNU_gama::ellipsoid(argv_ellipsoid.c_str());
        if  (gama_el == GNU_gama::ellipsoid_unknown)
        {
            std::cout << "The defined ellipsoid is unknown.";
            return EXIT_SUCCESS;
        } 
        IS->set_ellipsoid(argv_ellipsoid);
      }


    {
      cout << GNU_gama::local::T_GaMa_Adjustment_of_geodetic_network << "        "
           << GNU_gama::local::T_GaMa_version << GNU_gama::GNU_gama_version()
           << "-" << IS->algorithm()
           << " / " << GNU_gama::GNU_gama_compiler() << "\n"
           << GNU_gama::local::underline(GNU_gama::local::T_GaMa_Adjustment_of_geodetic_network, '*') << "\n"
           << "http://www.gnu.org/software/gama/\n\n\n";
    }

    if (IS->PD.empty())
      throw GNU_gama::local::Exception(GNU_gama::local::T_GaMa_No_points_available);

    if (IS->OD.clusters.empty())
      throw GNU_gama::local::Exception(GNU_gama::local::T_GaMa_No_observations_available);

    try
      {
        // if (!GaMaConsistent(IS->PD))
        //   {
        //      cout << T_GaMa_inconsistent_coordinates_and_angles << "\n\n\n";
        //   }
        IS->remove_inconsistency();

        GNU_gama::local::AcordStatistics stats(IS->PD, IS->OD);

        /* Acord2 class for computing approximate values of unknown paramaters
         * (needed for linearization of project equations) superseded previous
         * class Acord.
         *
         * Acord2 apart from other algorithms relies on a class
         * ApproximateCoordinates (used also in Acord) for solving unknown xy
         * by intersections and local transformations. Original author
         * of ApproximateCoordinates is Jiri Vesely.
         *
         * Class Acord2 was introduced for better handling of traverses.
         */

        GNU_gama::local::Acord2 acord2(IS->PD, IS->OD);
        acord2.execute();

        if (IS->correction_to_ellipsoid())
          {
            GNU_gama::gama_ellipsoid elnum = GNU_gama::ellipsoid(IS->ellipsoid().c_str());
            GNU_gama::Ellipsoid el {};
            GNU_gama::set(&el, elnum);
            GNU_gama::local::ReduceToEllipsoid reduce_to_el(IS->PD, IS->OD, el, IS->latitude());
            reduce_to_el.execute();
            GNU_gama::local::ReducedObservationsToEllipsoidText(IS, reduce_to_el.getMap(), cout);
          }

        stats.execute();
        GNU_gama::local::ApproximateCoordinates(&stats, cout);


      }
    catch(GNU_gama::local::Exception& e)
      {
        if (xmlerr.isValid())
          {
            xmlerr.setDescription(e.what());
            return xmlerr.write_xml("gamaLocalException");
          }

        std::cerr << e.what() << std::endl;
        return 1;
      }
    catch(...)
      {
        if (xmlerr.isValid())
          {
            xmlerr.setDescription("Gama / Acord: "
                                  "approximate coordinates failed");
            return xmlerr.write_xml("gamaLocalApproximateCoordinates");
          }

        std::cerr << "Gama / Acord: approximate coordinates failed\n\n";
        return 1;
      }


    if (IS->sum_points() == 0 || IS->sum_unknowns() == 0)
      {
        throw GNU_gama::local::Exception(GNU_gama::local::T_GaMa_No_network_points_defined);
      }

    //else ... do not use else after throw
      {
        if (IS->huge_abs_terms())
          {
            OutlyingAbsoluteTerms(IS, cout);
            IS->remove_huge_abs_terms();
            cout << GNU_gama::local::T_GaMa_Observatios_with_outlying_absolute_terms_removed
                 << "\n\n\n";
          }

        if (!IS->connected_network())
          cout  << GNU_gama::local::T_GaMa_network_not_connected << "\n\n\n";

        bool network_can_be_adjusted;
        {
          std::ostringstream tmp_out;
          if (!(network_can_be_adjusted = GeneralParameters(IS, tmp_out)))
            {
              GNU_gama::local::NetworkDescription(IS->description, cout);
              cout << tmp_out.str();
            }
        }

        if (network_can_be_adjusted)
          {
            IS->clear_linearization_iterations();
            while (IS->next_linearization_iterations() &&
                   TestLinearization(IS))
              {
                IS->increment_linearization_iterations();
                IS->refine_approx();
              }

            if (IS->linearization_iterations() > 0)
              {
                cout << GNU_gama::local::T_GaMa_Approximate_coordinates_replaced << "\n"
                     << GNU_gama::local::underline(GNU_gama::local::T_GaMa_Approximate_coordinates_replaced,'*')
                     << "\n\n"
                     << GNU_gama::local::T_GaMa_Number_of_linearization_iterations
                     << IS->linearization_iterations() << "\n\n";
              }

            if (!TestLinearization(IS, cout)) cout << "\n";

            GNU_gama::local::NetworkDescription   (IS->description, cout);
            GNU_gama::local::GeneralParameters    (IS, cout);
            GNU_gama::local::FixedPoints          (IS, cout);
            GNU_gama::local::AdjustedUnknowns     (IS, cout);
            GNU_gama::local::ErrorEllipses        (IS, cout);
            GNU_gama::local::AdjustedObservations (IS, cout);
            GNU_gama::local::ResidualsObservations(IS, cout);
          }

        if (option_variables.count("svg"))
          {
            GNU_gama::local::GamaLocalSVG svg(IS);
            auto argv_svgout = option_variables["svg"].as<std::string>();
            std::ofstream file(argv_svgout);
            svg.draw(file);
          }

        if (option_variables.count("obs"))
          {
            auto argv_obsout = option_variables["obs"].as<std::string>();
            std::ofstream opr(argv_obsout);
            IS->project_equations(opr);
          }

        if (option_variables.count("html"))
          {
            GNU_gama::local::GamaLocalHTML html(IS);
            html.exec();

            auto argv_htmlout = option_variables["html"].as<std::string>();
            std::ofstream file(argv_htmlout);
            html.html(file);
          }

        if (option_variables.count("xml"))
          {
            auto argv_xmlout = option_variables["xml"].as<std::string>();
            xmlerr.setXmlOutput(argv_xmlout);

            // TODO: why do we override the choice from the user?
            IS->set_gons();

            GNU_gama::LocalNetworkXML xml(IS);

            std::ofstream file(argv_xmlout);
            xml.write(file);
          }

        if (option_variables.count("octave"))
          {

            auto argv_octaveout = option_variables["octave"].as<std::string>();
            // TODO: why do we override the choice from the user?
            IS->set_gons();

            GNU_gama::LocalNetworkOctave octave(IS);

            std::ofstream file(argv_octaveout);
            octave.write(file);
          }

        if (network_can_be_adjusted && option_variables.count("updated-xml"))
          {
            auto argv_updated_xml = option_variables["updated-xml"].as<std::string>();
            std::string xml = IS->updated_xml();
            std::ofstream file(argv_updated_xml);
            file << xml;
          }
      }

    delete IS;
    return 0;

  }
#ifdef GNU_GAMA_LOCAL_SQLITE_READER
  catch(const GNU_gama::Exception::sqlitexc& gamalite)
    {
      if (xmlerr.isValid())
        {
          xmlerr.setDescription(gamalite.what());
          return xmlerr.write_xml("gamaLocalSqlite");
        }

      std::cout << "\n" << "****** " << gamalite.what() << "\n\n";
      return 1;
    }
#endif
  catch(const GNU_gama::Exception::adjustment& choldec)
    {
      if (xmlerr.isValid())
        {
          xmlerr.setDescription(GNU_gama::local::T_GaMa_solution_ended_with_error);
          xmlerr.setDescription(choldec.str);
          return xmlerr.write_xml("gamaLocalAdjustment");
        }

      std::cout << "\n" << GNU_gama::local::T_GaMa_solution_ended_with_error << "\n\n"
                << "****** " << choldec.str << "\n\n";
      return 1;
    }
  catch (const GNU_gama::local::Exception& V)
    {
      if (xmlerr.isValid())
        {
          xmlerr.setDescription(GNU_gama::local::T_GaMa_solution_ended_with_error);
          xmlerr.setDescription(V.what());
          return xmlerr.write_xml("gamaLocalException");
        }

      std::cout << "\n" << GNU_gama::local::T_GaMa_solution_ended_with_error << "\n\n"
                << "****** " << V.what() << "\n\n";
      return 1;
    }
  catch (std::exception& stde) {
    if (xmlerr.isValid())
      {
        xmlerr.setDescription(stde.what());
        return xmlerr.write_xml("gamaLocalStdException");
      }

    std::cout << "\n" << stde.what() << "\n\n";
  }
  catch(...) {

    if (xmlerr.isValid())
      {
        return xmlerr.write_xml("gamaLocalUnknownException");
      }

    std::cout << "\n" << GNU_gama::local::T_GaMa_internal_program_error << "\n\n";
    return 1;
}

std::istream& GNU_gama::local::operator >>(std::istream& in, GNU_gama::local::LocalNetwork::Algorithm& algorithm)
{
  std::string token;
  in >> token;

  if(token == "gso") algorithm = GNU_gama::local::LocalNetwork::Algorithm::gso;
  else if(token == "svd") algorithm = GNU_gama::local::LocalNetwork::Algorithm::svd;
  else if(token == "cholesky") algorithm = GNU_gama::local::LocalNetwork::Algorithm::cholesky;
  else if(token == "envelope") algorithm = GNU_gama::local::LocalNetwork::Algorithm::envelope;
  else in.setstate(std::ios_base::failbit);

  return in;
}

std::istream& GNU_gama::local::operator >>(std::istream& in, GNU_gama::local::gama_language& language)
{
  std::string token;
  in >> token;

  if(token == "en") language = GNU_gama::local::en;
  else if(token == "ca") language = GNU_gama::local::ca;
  else if(token == "cs") language = GNU_gama::local::cz;
  else if(token == "cz") language = GNU_gama::local::cz;
  else if(token == "du") language = GNU_gama::local::du;
  else if(token == "es") language = GNU_gama::local::es;
  else if(token == "fr") language = GNU_gama::local::fr;
  else if(token == "fi") language = GNU_gama::local::fi;
  else if(token == "hu") language = GNU_gama::local::hu;
  else if(token == "ru") language = GNU_gama::local::ru;
  else if(token == "ua") language = GNU_gama::local::ua;
  else if(token == "zh") language = GNU_gama::local::zh;
  else in.setstate(std::ios_base::failbit);

  return in;
}

std::istream& GNU_gama::operator >>(std::istream& in, GNU_gama::OutStream::Encoding& encoding)
{
  std::string token;
  in >> token;

  if(token == "utf-8") encoding = GNU_gama::OutStream::utf_8;
  else if(token == "iso-8859-2") encoding = GNU_gama::OutStream::iso_8859_2;
  else if(token == "iso-8859-2-flat") encoding = GNU_gama::OutStream::iso_8859_2_flat;
  else if(token == "cp-1250") encoding = GNU_gama::OutStream::cp_1250;
  else if(token == "cp-1251") encoding = GNU_gama::OutStream::cp_1251;
  else in.setstate(std::ios_base::failbit);

  return in;
}

std::istream& operator >>(std::istream& in, Angle& angles)
{
  std::string token;
  in >> token;

  if (token == "400") angles = ::Angle::Gon;
  else if (token == "360") angles = ::Angle::Degree;
  else in.setstate(std::ios_base::failbit);

  return in;
}