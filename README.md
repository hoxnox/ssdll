Simple StarDict Lookup Library

This is a cross-platform (Windows, OS X, Linux) library which is capable of loading dictionaries in StarDict format and search for a full-match translations. A lot of code is derived/compiled from the original StarDict project, sdcv and QDict projects although this library contains a lot of improvements such as:

* no dependency on `glib`
* correct support of 64-bit offsets in dictionaries for all supported platforms
* substitution of tons of direct pointer allocations/memory access with safer alternatives such as std::vector and std::string
* except of the original `dictziplib.cpp` the readability was enchanced with refactoring and renaming (this is subjective opinion of course)

Build instructions can be found in BUILD.md file.

Contibutions are highly welcome!
