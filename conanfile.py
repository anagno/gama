from conans import ConanFile, CMake
import platform

# cmake /home/anagno/Documents/projects/gama -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=TRUE -DUSE_SQLITE3=TRUE -DCONAN_EXPORTED=TRUE
# ctest
# message (STATUS "language_files == ${language_files}")
# message (FATAL_ERROR "DEBUG: stopping CMake")

# cmake . -LAH
# node --experimental-modules --experimental-wasm-modules --experimental-wasm-threads --experimental-wasm-bulk-memory bin/gama-local.js /home/anagno/Documents/projects/gama/tests/gama-local/input/azimuth-angle.gkf --angles 360
# nodejs bin/gama-local.js /home/anagno/Documents/projects/gama/tests/gama-local/input/azimuth-angle.gkf --angles 360

#https://stackoverflow.com/questions/55635294/how-to-create-packages-with-cmake

class GaMa(ConanFile):
   name = "GaMa"
   version = "2.7"
   settings = "os", "compiler", "build_type", "arch",
   generators = ["cmake_find_package", "virtualrunenv"]

   options = {
       "sqlite3": [True, False],
       "libxml2": [True, False],
       "shared": [True, False],
       "native_compiler": "ANY",
       }

   default_options = {
       "sqlite3": False,
       "libxml2": False,
       "shared": False,
       "native_compiler": None,
       }

   def requirements(self):
       self.requires("expat/2.2.9")
       self.requires("boost/1.73.0")

       if self.options.sqlite3:
           self.requires("sqlite3/3.29.0")

       if self.options.libxml2:
           self.requires("libxml2/2.9.9")

   def imports(self):
      self.copy("*.dll", "bin", "bin")

   def _configure_cmake(self):
      cmake = CMake(self)

      if self.settings.os == "Emscripten":
         if self.options.native_compiler:
            cmake.definitions["BUILD_TOOS_CMAKE_CXX_COMPILER"] = self.options.native_compiler
         else:
            raise ConanInvalidConfiguration("A native compiler was not specified")

      cmake.definitions["CONAN_EXPORTED"] = True

      if self.options.sqlite3:
         cmake.definitions["USE_SQLITE3"] = "ON"

      if self.options.libxml2:
         cmake.definitions["USE_LIBXML2"] = "ON"
         cmake.definitions["BUILD_TESTING"] = "ON"

      #cmake.verbose = True
      cmake.configure()
      return cmake

   def build(self):
      cmake = self._configure_cmake()
      cmake.build()

   def package(self):
      cmake = self._configure_cmake()
      cmake.install()


# sudo apt-get install clang nodejs
