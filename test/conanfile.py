from conans import ConanFile, CMake, tools
import os


class TestUtilConan(ConanFile):
    settings = {"os": None, "compiler": None, "build_type": None, "arch": ["x86_64"]}
    generators = ["cmake", "txt"]
    requires = "catch2/2.2.2@bincrafters/stable"

    def generate_testlist(self):
        print "Working Directory: " + os.getcwd()
        print "Source Folder: " + self.source_folder
        print "Generating testlist.txt in " + os.getcwd() + " from " + self.source_folder
        f = open("testlist.txt", "w")

        for (path, dirs, files) in os.walk(self.source_folder):
            if (path.startswith("build") or path.startswith("./build") or "CMakeFiles" in path):
                continue

            for filename in files:
                if (filename.endswith("cpp") or filename.endswith("hpp")):
                    f.write(os.path.join(path, filename).replace("\\", "/") + "\n")

        f.close()

    def build(self):
        self.generate_testlist()

        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')

    def test(self):
        if not tools.cross_building(self.settings):
            os.chdir("bin")
            self.run(".%stests" % os.sep)
