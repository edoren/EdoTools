### Create and upload Conan package
conan create . edotools/master@edoren/develop --build=missing
conan upload edotools/master@edoren/develop -r=remote
