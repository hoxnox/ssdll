#BUILD INSTRUCTIONS

##Windows

###3rd-parties

* Get zlib 1.2.8 source code and build it using `zlib-project/zlib.pro` file
* Copy results (z.lib, z.dll) to the `libs/` directory
* Update submodule with mman-win32 project and build `mman-win32/mman-win32.pro` file
* Copy results (mman.lib, mman.dll) to the `libs/` directory

###ssdll

* Open `ssdll/ssdll.pro` project and build it
* [optional] Copy results (ssdll.lib, ssdll.dll) to the `libs/` directory

Last step is optional and should be executed only in a case you want to quickly try the result using `ssdll-test` project. This project builds to an executable which accepts path to .ifo file as 1st argument and then loads it and translates user input.