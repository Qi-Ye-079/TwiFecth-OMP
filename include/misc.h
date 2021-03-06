#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <map>
#include <list>
#include <chrono>

namespace misc
{
    std::string char2hex(char dec);

    std::string percentEncode(const std::string& s);
}

#endif // UTILITY_H
