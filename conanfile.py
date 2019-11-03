from conans import ConanFile, CMake
import platform

class Gama(ConanFile):
   name = "GaMa"
   version = "2.7"
   settings = "os", "compiler", "build_type", "arch",
   generators = ["cmake_find_package", "cmake_paths", "virtualenv"]

   options = {
       "sqlite3": [True, False],
       "libxml2": [True, False],
       }

   default_options = {
       "sqlite3": False,
       "libxml2": False,
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
      cmake.definitions["USE_CONAN"] = "ON"
      cmake.definitions["BUILD_TESTING"] = "OFF"

      if self.options.sqlite3:
         cmake.definitions["USE_SQLITE3"] = "ON"

      cmake.verbose = True
      cmake.configure()
      return cmake

   def build(self):
      cmake = self._configure_cmake()
      cmake.build()

   def package(self):
      cmake = self._configure_cmake()
      cmake.install()


# sudo apt-get install clang nodejs
