from conans import ConanFile, CMake


class EdoToolsConan(ConanFile):
    name = "edotools"
    license = "MIT"
    url = "https://github.com/edoren/EdoTools"
    description = "Personal tools to ease development process"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fmt_json_support": [True, False],
        "nlohmann_json_support": [True, False],
    }
    default_options = {
        "shared": False,
        "fmt_json_support": True,
        "nlohmann_json_support": True
    }
    generators = "cmake"
    requires = tuple()
    exports_sources = "include/*", "src/*", "CMakeLists.txt"

    def build_requirements(self):
        if not self.in_local_cache:
            self.build_requires("catch2/2.13.7")

    def requirements(self):
        if self.options.fmt_json_support:
            self.requires("fmt/8.1.1")
        if self.options.nlohmann_json_support:
            self.requires("nlohmann_json/3.10.4")

    def build(self):
        cmake = CMake(self)
        if not self.in_local_cache:
            cmake.definitions["BUILD_TESTS"] = "ON"
        cmake.definitions["FMT_SUPPORT"] = "ON" if self.options.fmt_json_support else "OFF"
        cmake.definitions["NLOHMANN_JSON_SUPPORT"] = "ON" if self.options.nlohmann_json_support else "OFF"
        cmake.configure()
        cmake.build()
        if not self.in_local_cache:
            cmake.test(output_on_failure=True)

    def package(self):
        self.copy("*.hpp", dst="include", src="include")
        self.copy("*.inl", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = [self.name]
        if not self.options.shared:
            self.cpp_info.defines.append("EDOTOOLS_STATIC")
        if self.options.fmt_json_support:
            self.cpp_info.defines.append("EDOTOOLS_FMT_SUPPORT")
        if self.options.nlohmann_json_support:
            self.cpp_info.defines.append("EDOTOOLS_NLOHMANN_JSON_SUPPORT")
