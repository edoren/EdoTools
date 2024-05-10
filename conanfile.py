from pathlib import Path

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout


class EdoToolsConan(ConanFile):
    name = "edotools"
    version = "0.1.0"
    package_type = "library"

    # Optional metadata
    license = "MIT"
    author = "Manuel Sabogal <mfer32@gmail.com>"
    url = "https://github.com/edoren/EdoTools"
    description = "Personal tools to ease development process"
    # topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Options configuration
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "fmt_support": [True, False],
        "nlohmann_json_support": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "fmt_support": True,
        "nlohmann_json_support": True
    }

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        variables = {
            "FMT_SUPPORT":
                "ON" if self.options.fmt_support else "OFF",
            "NLOHMANN_JSON_SUPPORT":
                "ON" if self.options.nlohmann_json_support else "OFF"
        }
        cmake.configure(variables=variables)
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def requirements(self):
        if self.options.fmt_support:
            self.requires("fmt/8.1.1", transitive_headers=True)
        if self.options.nlohmann_json_support:
            self.requires("nlohmann_json/3.10.4", transitive_headers=True)

    def package_info(self):
        self.cpp_info.libs = [self.name]
        if not self.options.shared:
            self.cpp_info.defines.append("EDOTOOLS_STATIC")
        if self.options.fmt_support:
            self.cpp_info.defines.append("EDOTOOLS_FMT_SUPPORT")
        if self.options.nlohmann_json_support:
            self.cpp_info.defines.append("EDOTOOLS_NLOHMANN_JSON_SUPPORT")
