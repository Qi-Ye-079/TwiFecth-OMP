#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <map>
#include <list>
#include <chrono>

using namespace std;

// Type definitions
using OAuthParamPairs = std::map<string, string>;
using StringList = std::list<string>;
using Clock = std::chrono::high_resolution_clock;

// Response structure
struct Response {
    char *memory;
    size_t size;
};

namespace misc
{
    void pyAnalyzeSentiment(const StringList& list, int& numPos, int& numNeg);

    string char2hex(char dec);

    string percentEncode(const string& s);
}

#endif // UTILITY_H
