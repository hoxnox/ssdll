# Simple StarDict Lookup Library

This is a **cross-platform** (Windows, OS X, Linux) library which is capable of loading dictionaries in StarDict format and searching for a full-match translations. A lot of code is derived/compiled from the original _StarDict_ project, _sdcv_ and _QDict_ projects although this library contains a lot of improvements such as:

* **no dependency on `glib`**
* **correct support of 64-bit offsets** in dictionaries for all supported platforms
* **correct support of Unicode filepaths in Windows**
* substitution of tons of direct pointer allocations/memory access with safer alternatives such as `std::vector` and `std::string`
* the readability has been enchanced dramatically in comparison to aforementioned projects (_this is a subjective opinion of course_) except of the original `dictziplib.cpp` which you better don't want to touch

Build instructions can be found in [BUILD.md](https://github.com/Ribtoks/ssdll/blob/master/BUILD.md) file.

## Usage

    ```c++
    #include <iostream>
    #include <lookupdictionary.h>

    int
    main(int argc, char* argv[])
    {
        if (argc < 3)
        {
            std::cout << "Usage: " << argv[0] << " <ifo-file> <word>" << std::endl;
            return EXIT_FAILURE;
        }

        LookupDictionary dic;
        dic.setLogger([](const std::string& msg) { std::cerr << "E: " << msg << std::endl; });

        if (!dic.setIfoFilePath(argv[1]))
            return EXIT_FAILURE;

        if (!dic.loadDictionary())
            return EXIT_FAILURE;

        std::string translation;
        if (!dic.translate(argv[2], translation))
            return EXIT_FAILURE;

        std::cout << translation << std::endl;
        return EXIT_SUCCESS;
    }
    ```

## How to contribute:

- [Fork](http://help.github.com/forking/) repository on GitHub
- Clone your fork locally
- Configure the upstream repo (`git remote add upstream git@github.com:Ribtoks/ssdll.git`)
- Create local branch (`git checkout -b your_feature`)
- Work on your feature
- Push the branch to GitHub (`git push origin your_feature`)
- Send a [pull request](https://help.github.com/articles/using-pull-requests) on GitHub

Contibutions are highly welcome!
