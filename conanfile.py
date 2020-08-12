from conans import ConanFile, CMake
from conans import tools
import platform

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
   version = "2.7"
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
       "native_compiler": "ANY"
   }

   default_options = {
       "sqlite3": False,
       "shared": False,
       "native_compiler": None,
   }

   #def configure(self):
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

      # This is a build_requiremes but for the moment we do not have a good
      # way of declaring it 
      # https://github.com/conan-io/conan/issues/4893
      # https://github.com/conan-io/conan/issues/7423
      if self.should_test and not tools.cross_building(self):
         self.requires("libxml2/2.9.9@#98c13df03fac70408756b07364074446")

   def build_requirements(self):

      if tools.cross_building(self):
         self.build_requires("expat/2.2.9")

   def imports(self):
      self.copy("*.dll", "bin", "bin")

   def _configure_cmake(self):
      cmake = CMake(self)

      if self.settings.os == "Emscripten":
         if self.options.native_compiler:
            cmake.definitions["BUILD_TOOS_CMAKE_CXX_COMPILER"] = self.options.native_compiler
         else:
            raise ConanInvalidConfiguration(
                "A native compiler was not specified")

      cmake.definitions["CONAN_EXPORTED"] = True

      if self.options.sqlite3:
         cmake.definitions["USE_SQLITE3"] = "ON"

      if self.should_test and not tools.cross_building(self.settings):
         cmake.definitions["BUILD_TESTING"] = "ON"

      #cmake.verbose = True
      if self.should_configure:
         cmake.configure()

      return cmake

   def build(self):
      cmake = self._configure_cmake()

      if self.should_build:
         cmake.build()

      if self.should_test and not tools.cross_building(self.settings):
         cmake.test()

   def package(self):
      cmake = self._configure_cmake()

      if self.should_install:
         cmake.install()


# sudo apt-get install clang nodejs
