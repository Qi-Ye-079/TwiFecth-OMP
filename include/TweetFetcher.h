#ifndef TWEETAPI_H
#define TWEETAPI_H

#include <omp.h>
#include "curl/curl.h"
#include "misc.h"

// Tweet fetcher class
class TweetFetcher {
private:
    string consumerKey;
    string consumerSecret;
    string accessToken;
    string accessTokenSecret;
    string nonce;
    string timeStamp;

    // Fetched response from the Twitter's API
    StringList *responses;

    // ======== Helper functions to generate OAuth header ===========
    void generateNonceAndTimeStamp();

    bool request(const string&, const string&, int count, int numThreads);

    string generateSignature(const string&, OAuthParamPairs&);

    void collectRequestPairs(string, OAuthParamPairs&);

    void collectOAuthParams(const string&, OAuthParamPairs&);

    void collectParameters(const string&, const string&, string&);

    string generateHeader(const string&);

    void buildRawParamString(const OAuthParamPairs&, const string&, string&);

    void extractTextIntoList(StringList& resps, string& responseStr);

public:
    TweetFetcher();

    TweetFetcher(const string &cKey,
                 const string &cSecret,
                 const string &aToken,
                 const string &aTokenSecret);

    TweetFetcher(const TweetFetcher &other);

    TweetFetcher& operator=(const TweetFetcher &other);

    ~TweetFetcher();

    bool searchWithOmp(const string &query, int count, int numThreads = 1);

    const StringList* getResponseListPtr() const;

    friend ostream& operator<<(ostream &os, const TweetFetcher &tf);
};

#endif // TWEETAPI_H
