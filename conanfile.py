from conans import ConanFile, CMake
from conans import tools
import os

# cmake /home/anagno/Documents/projects/gama -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=TRUE -DUSE_SQLITE3=TRUE -DCONAN_EXPORTED=TRUE
# ctest
# message (STATUS "language_files == ${language_files}")
# message (FATAL_ERROR "DEBUG: stopping CMake")

# cmake . -LAH
# node --experimental-modules --experimental-wasm-modules --experimental-wasm-threads --experimental-wasm-bulk-memory bin/gama-local.js /home/anagno/Documents/projects/gama/tests/gama-local/input/azimuth-angle.gkf --angles 360
# nodejs LocalNetwork/Application/gama-local.js /home/anagno/Documents/projects/gama/tests/gama-local/input/azimuth-angle.gkf --angles 360

#https://stackoverflow.com/questions/55635294/how-to-create-packages-with-cmake


class GaMa(ConanFile):
   name = "GaMa"
   version = "2.9.4"
   settings = "os", "compiler", "build_type", "arch",
   generators = ["cmake_find_package", "virtualrunenv"]
   exports_sources = {
       "*",
       "!.git/*",
       "!.travis/*",
       "!.var/*",
       "!.vscode/*"
   }

   options = {
       "sqlite3": [True, False],
       "shared": [True, False],
       "fPIC": [True, False]
   }

   default_options = {
       "sqlite3": False,
       "shared": False,
       "fPIC": True
   }


   def configure(self):
      tools.check_min_cppstd(self, "17")
      # https://github.com/conan-io/conan-center-index/pull/2274
      # if self.should_test and not tools.cross_building(self.settings):
      #   if self.options.libxml2 and self.options["libxml2"].include_utils == False:
      #      raise ConanInvalidConfiguration(
      #          "LibXml2 should contain the tools")

   def requirements(self):
      self.requires("expat/2.2.9")
      self.requires("boost/1.73.0")

      if self.options.sqlite3:
         self.requires("sqlite3/3.29.0")

   def build_requirements(self):

      if tools.cross_building(self):
         self.build_requires("expat/2.2.9")

      if self.should_test:
        self.build_requires("libxml2/2.9.9")

   def imports(self):
      self.copy("*.dll", "bin", "bin")
      self.copy("*.dll", "lib", "bin")

   def _configure_cmake(self):
      cmake = CMake(self)

      if tools.cross_building(self):
        if self.settings_build.compiler == "gcc":
            cmake.definitions["BUILD_TOOLS_CMAKE_CXX_COMPILER"] = "g++"
        if self.settings_build.compiler == "clang":
            cmake.definitions["BUILD_TOOLS_CMAKE_CXX_COMPILER"] = "clang++"

      cmake.definitions["CONAN_EXPORTED"] = True

      # We test in the test phase of conan
      cmake.definitions["BUILD_TESTING"] = False

      if self.options.sqlite3:
         cmake.definitions["USE_SQLITE3"] = "ON"

      #cmake.verbose = True
      if self.should_configure:
         cmake.configure()

      return cmake

   def build(self):
      cmake = self._configure_cmake()

      if self.should_build:
         cmake.build()

   def package(self):
      cmake = self._configure_cmake()

      if self.should_install:
         cmake.install()


   def package_info(self):
      bin_path = os.path.join(self.package_folder, "bin")
      self.output.info("Appending PATH env var with : {}".format(bin_path))
      self.env_info.PATH.append(bin_path)

      self.cpp_info.names["cmake_find_package"] = "GaMa"

      self.cpp_info.components["Service_Math"].names["cmake_find_package"] = "Service_Math"
      self.cpp_info.components["Service_Math"].includedirs = ["include/GaMa"]

      self.cpp_info.components["Service_Parsing"].names["cmake_find_package"] = "Service_Parsing"
      self.cpp_info.components["Service_Parsing"].includedirs = ["include/GaMa"]
      self.cpp_info.components["Service_Parsing"].libs = ["Service_Parsing"]
      self.cpp_info.components["Service_Parsing"].requires = ["expat::expat"]

      self.cpp_info.components["Business_Parsing"].names["cmake_find_package"] = "Business_Parsing"
      self.cpp_info.components["Business_Parsing"].includedirs = ["include/GaMa"]
      self.cpp_info.components["Business_Parsing"].libs = ["Business_Parsing"]
      self.cpp_info.components["Business_Parsing"].requires = ["Service_Parsing"]

      self.cpp_info.components["Service_Utilities"].names["cmake_find_package"] = "Service_Utilities"
      self.cpp_info.components["Service_Utilities"].includedirs = ["include/GaMa"]
      self.cpp_info.components["Service_Utilities"].libs = ["Service_Utilities"]

      self.cpp_info.components["Business_Utilities"].names["cmake_find_package"] = "Business_Utilities"
      self.cpp_info.components["Business_Utilities"].includedirs = ["include/GaMa"]
      self.cpp_info.components["Business_Utilities"].libs = ["Business_Utilities"]

      self.cpp_info.components["Business_MathCore"].names["cmake_find_package"] = "Business_MathCore"
      self.cpp_info.components["Business_MathCore"].includedirs = ["include/GaMa"]
      self.cpp_info.components["Business_MathCore"].libs = ["Business_MathCore"]

      self.cpp_info.components["Business_MathAdjustment"].names["cmake_find_package"] = "Business_MathAdjustment"
      self.cpp_info.components["Business_MathAdjustment"].includedirs = ["include/GaMa"]
      self.cpp_info.components["Business_MathAdjustment"].libs = ["Business_MathAdjustment"]
      self.cpp_info.components["Business_MathAdjustment"].requires = ["Service_Math"]

      self.cpp_info.components["libgama"].names["cmake_find_package"] = "libgama"
      self.cpp_info.components["libgama"].includedirs = ["include/GaMa"]
      self.cpp_info.components["libgama"].libs = ["libgama"]
      self.cpp_info.components["libgama"].resdirs = ['share']
      self.cpp_info.components["libgama"].requires = ["expat::expat", "boost::boost", "Service_Math", "Business_Utilities",
                                                      "Business_Parsing", "Service_Parsing", "Service_Utilities", "Business_MathCore"]

      if self.options.sqlite3:
         self.cpp_info.components["libgama"].requires.append("sqlite3::sqlite3")

# sudo apt-get install clang nodejs
