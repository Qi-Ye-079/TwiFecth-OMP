#ifndef TWEETAPI_H
#define TWEETAPI_H

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
    Response response;

    // Url request/response handler
    CURL *curl;
    CURLcode resCode;

    // ======== Helper functions to generate OAuth header ===========
    void generateNonceAndTimeStamp();

    bool request(const string&, const string&);

    string generateSignature(const string&, OAuthParamPairs&);

    void collectRequestPairs(string, OAuthParamPairs&);

    void collectOAuthParams(const string&, OAuthParamPairs&);

    void collectParameters(const string&, const string&, string&);

    void generateHeader(const string&, string&);

    void buildRawParamString(const OAuthParamPairs&, const string&, string&);

    void resetResponseMem();

public:
    TweetFetcher(const string& cKey,
                 const string& cSecret,
                 const string& aToken,
                 const string& aTokenSecret);

    ~TweetFetcher();

    bool search(const string& query, int count);

    void getResponse(StringList& resps);

};

#endif // TWEETAPI_H
