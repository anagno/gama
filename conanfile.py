from conans import ConanFile

class Gama(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   generators = "cmake_find_package"

   def requirements(self):
       self.requires("Expat/2.2.9@pix4d/stable")
