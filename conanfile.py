from conans import ConanFile, CMake


class EdoToolsConan(ConanFile):
    name = "edotools"
    license = "MIT"
    url = "https://github.com/edoren/EdoTools"
    description = "Personal tools to ease development process"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"
    build_requires = (
        "catch2/2.13.7"
    )
    requires = (
        "fmt/8.0.1"
    )
    exports_sources = "include/*", "src/*", "CMakeLists.txt"

    def build(self):
        cmake = CMake(self)
        if not self.in_local_cache:
            cmake.definitions["BUILD_TESTS"] = "ON"
        cmake.configure()
        cmake.build()
        if not self.in_local_cache:
            cmake.test()

    def package(self):
        self.copy("*.cpp", dst="src", src="src")
        self.copy("*.hpp", dst="include", src="include")
        self.copy("*.inl", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
