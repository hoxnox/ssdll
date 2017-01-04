#pragma once

#include <ctime>
#include <string>
#include <zlib.h>

#include "mapfile.hpp"

struct DictCache {
    int chunk;
    char *inBuffer;
    int stamp;
    int count;
};

class DictData {
public:
    static const size_t DICT_CACHE_SIZE = 5;

	DictData() {}
	~DictData() { close(); }

public:
#ifdef _WIN32
    bool open(const std::wstring& filename, int computeCRC);
#else
    bool open(const std::string& filename, int computeCRC);
#endif
	void close();
	void read(char *buffer, unsigned long start, unsigned long size);

private:
#ifdef _WIN32
    int read_header(const std::wstring &filename, int computeCRC);
#else
    int read_header(const std::string &filename, int computeCRC);
#endif

private:
	const char    *start;	/* start of mmap'd area */
	const char    *end;		/* end of mmap'd area */
	unsigned long size;		/* size of mmap */
	
	int           type;
	z_stream      zStream;
	int           initialized;
  
	int           headerLength;
	int           method;
	int           flags;
	time_t        mtime;
	int           extraFlags;
	int           os;
	int           version;
	int           chunkLength;
	int           chunkCount;
	int           *chunks;
	unsigned long *offsets;	/* Sum-scan of chunks. */
	std::string    origFilename;
	std::string    comment;
	unsigned long crc;
	unsigned long length;
	unsigned long compressedLength;
	DictCache     cache[DICT_CACHE_SIZE];
    MapFile       mapfile;
};

