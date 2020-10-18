from conans import ConanFile, CMake, tools, RunEnvironment


class GaMaTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch",
    generators = ["cmake_find_package", "virtualrunenv"]


    def configure(self):
      tools.check_min_cppstd(self, "17")

    def build_requirements(self):
        self.build_requires("libxml2/2.9.9")

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.so", dst="bin", src="bin")

        self.copy("*.dll", dst="lib", src="bin")
        self.copy("*.so", dst="lib", src="bin")

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["CONAN_EXPORTED"] = True
        cmake.configure()

        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def test(self):
        if not tools.cross_building(self.settings):
            env_build = RunEnvironment(self)
            with tools.environment_append(env_build.vars):
                cmake = self._configure_cmake()
                cmake.test()
