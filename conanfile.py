from conans import ConanFile, CMake, tools


class PubConan(ConanFile):
    name = "pub"
    version = "1.0"
    license = "MIT"
    author = "Denis Kotov, redradist@gmail.com"
    url = "https://github.com/redradist/PUB.git"
    description = "P.U.B. - Pack Unpack Buffers, C++ library that is used for packing and unpacking messages between " \
                  "components (serializer, deserializer) "
    topics = ("serialization", "deserialization", "packing", "unpacking")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def source(self):
        self.run("git clone git@github.com:redradist/PUB.git")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="/home/redra/Projects/PUB")
        cmake.build()

    def package(self):
        self.copy("*.h", dst="src/pub", src="pub")
        self.copy("*.hpp", dst="src/pub", src="pub")
        self.copy("*pub.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        pass
        # self.cpp_info.libs = ["pub"]
