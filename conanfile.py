from conans import ConanFile, CMake, tools
import os


class UtilConan(ConanFile):
    name = "Util"
    version = "0.1"
    license = "MIT"
    url = "dev@null.com"
    description = "C++ Utility Library"
    settings = {"os": None, "compiler": None, "build_type": None, "arch": ["x86_64"]}
    options = {"shared": [False]}
    default_options = "shared=False"
    generators = ["cmake", "txt"]
    exports_sources=["src/*", "CMakeLists.txt", "cmake/*"]

    def generate_filelist(self):
        print("Generating filelist.txt")
        f = open("filelist.txt", "w")

        for (path, dirs, files) in os.walk("src"):
            for filename in files:
                f.write(os.path.join(path, filename).replace("\\", "/") + "\n")

        f.close()	

    def build(self):
        self.generate_filelist()

        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="src")
        self.copy("*.hpp", dst="include", src="src")
        self.copy("*.a", dst="lib", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="bin", keep_path=False)
        self.copy("*.dylib", dst="bin", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["Util"]
