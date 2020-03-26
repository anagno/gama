from conans import ConanFile, CMake
import platform

# conan install /home/anagno/Documents/projects/gama -pr cmake -o sqlite3=True
# cmake /home/anagno/Documents/projects/gama -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=TRUE -DUSE_SQLITE3=TRUE -DCONAN_EXPORTED=TRUE
# ctest
# message (STATUS "language_files == ${language_files}")
# message (FATAL_ERROR "DEBUG: stopping CMake")

# conan install /home/anagno/Documents/projects/gama -pr wasm.profile
# conan build /home/anagno/Documents/projects/gama --source-folder=/home/anagno/Documents/projects/gama --build-folder=.
# cmake . -LAH
# nodejs gama-local.js /home/anagno/Documents/projects/gama/tests/gama-local/input/azimuth-angle.gkf --angles 360

#https://stackoverflow.com/questions/55635294/how-to-create-packages-with-cmake

class GaMa(ConanFile):
   name = "GaMa"
   version = "2.7"
   settings = "os", "compiler", "build_type", "arch",
   generators = ["cmake_find_package", "cmake_paths", "virtualenv"]

   options = {
       "sqlite3": [True, False],
       "libxml2": [True, False],
       "shared": [True, False],
       }

   default_options = {
       "sqlite3": False,
       "libxml2": False,
       "shared": False,
       }

   def requirements(self):
       self.requires("Expat/2.2.9@pix4d/stable")

       if self.options.sqlite3:
           self.requires("sqlite3/3.29.0@bincrafters/stable")

       if self.options.libxml2:
           self.requires("libxml2/2.9.9@bincrafters/stable")

   def _configure_cmake(self):
      cmake = CMake(self)

      if self.settings.os == "Emscripten":
         if platform.system() == "Windows":
            cmake.definitions["BUILD_TOOS_CMAKE_CXX_COMPILER"] = "Visual Studio"
         if platform.system() == "Linux":
            cmake.definitions["BUILD_TOOS_CMAKE_CXX_COMPILER"] = "g++"

      cmake.definitions["CONAN_EXPORTED"] = True
      cmake.definitions["BUILD_TESTING"] = "ON"

      if self.options.sqlite3:
         cmake.definitions["USE_SQLITE3"] = "ON"

      if self.options.libxml2:
         cmake.definitions["USE_LIBXML2"] = "ON"

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
