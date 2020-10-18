/*
  GNU Gama -- testing version numbers defined in configure.ac and version.cpp
  Copyright (C) 2018, 2019  Ales Cepek <cepek@gnu.org>

  This file is part of the GNU Gama C++ library

  GNU Gama is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  GNU Gama is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with GNU Gama.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <fstream>
// #include <config.h>
#include <Utilities/Business/version.h>

using std::string;

int main(int /*argc*/, char* argv[])
{
  int error = 0;

  string version_cpp = GNU_gama::GNU_gama_version(); /* gnu_gama/version.cpp */
  std::cout << version_cpp << " version_cpp\n";

  std::ifstream inpf(argv[1]);

  /* We expect the fix format of CMakeLists.txt project directive:
   *
   * project (gnu-gama VERSION MAJOR.MINOR)
   */
  const string pattern = "project (GaMa VERSION ";

  string version_cmake;
  while (std::getline(inpf, version_cmake))
    {
      auto indx = version_cmake.find(pattern, 0);
      if (indx > 0) continue;

      version_cmake.erase(0, pattern.size());
      version_cmake.pop_back();

      if (version_cmake != version_cpp)
        {
          std::cout
            << "\nPackage version " << version_cpp
            << ", defined in configure.ac, "
            << "\nis different from version "
            << version_cmake << ", defined in CMakeLists.txt\n\n";

          error++;
        }

      break;
    }
    std::cout << version_cmake << " version_cmake\n";


  return error;
}
