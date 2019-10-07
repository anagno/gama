from conans import ConanFile

class Gama(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   generators = "cmake_find_package"

   options = { "sqlite3" : [True, False],}

   default_options = {"sqlite3": False}

   def requirements(self):
       self.requires("Expat/2.2.9@pix4d/stable")

       if self.options.sqlite3:
           self.requires("sqlite3/3.29.0@bincrafters/stable")
