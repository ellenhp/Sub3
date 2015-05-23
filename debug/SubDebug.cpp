#include "SubDebug.hpp"

#include <iostream>

//This lets all subDebug messages do nothing if we use it for our ostream.
class NullBuffer : public std::streambuf
{
public:
    int overflow(int c) { return c; }
};

//Only forward subDebug to std::cout if we want debug messages.
#ifdef DEBUG_MESSAGES
std::ostream& subDebug = std::cout;
#else
static NullBuffer null_buffer;
std::ostream null_stream(&null_buffer);
std::ostream& subDebug = null_stream;
#endif
