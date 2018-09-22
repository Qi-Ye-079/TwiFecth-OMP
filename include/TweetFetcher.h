#ifndef TWEETAPI_H
#define TWEETAPI_H

#include <omp.h>
#include <memory>
#include "curl/curl.h"
#include "misc.h"
#include "Sentiment.h"

// Tweet fetcher class
class TweetFetcher {
    // Type aliases
    using OAuthParamPairs = std::map<std::string, std::string>;
    using ResponseList = std::list<std::string>;

private:
    std::string consumerKey;
    std::string consumerSecret;
    std::string accessToken;
    std::string accessTokenSecret;
    std::string nonce;
    std::string timeStamp;

    // Fetched response from the Twitter's API
    ResponseList* responses;

    // ======== Helper functions to generate OAuth header ===========
    void generateNonceAndTimeStamp();

    bool request(const std::string&, const std::string&, int count, int numThreads);

    std::string generateSignature(const std::string&, OAuthParamPairs&);

    void collectRequestPairs(std::string, OAuthParamPairs&);

    void collectOAuthParams(const std::string&, OAuthParamPairs&);

    void collectParameters(const std::string&, const std::string&, std::string&);

    std::string generateHeader(const std::string&);

    void buildRawParamString(const OAuthParamPairs&, const std::string&, std::string&);

    void extractTextIntoList(std::list<std::string>& resps, std::string& responseStr);

public:
    // Constructors
    TweetFetcher();
    TweetFetcher(const std::string &cKey,
                 const std::string &cSecret,
                 const std::string &aToken,
                 const std::string &aTokenSecret);
    TweetFetcher(const TweetFetcher &other);
    TweetFetcher& operator=(const TweetFetcher &other);

    // Destrcutor
    ~TweetFetcher();

    // Search for tweets with OpenMP
    bool searchWithOmp(const std::string &query, int count, int numThreads = 1);

    // Getters
    std::string getConsumetKey() const;
    std::string getConsumetSecret() const;
    std::string getAccessToken() const;
    std::string getAccessTokenSecret() const;
    std::list<std::string>* getResponseListPtr() const;

    // Setters
    void setConsumerKey(std::string cKey);
    void setConsumerSecret(std::string cSecret);
    void setAccessToken(std::string aToken);
    void setAccessTokenSecret(std::string aTokenSecret);

    friend std::ostream& operator<<(std::ostream &os, const TweetFetcher &tf);
};

#endif // TWEETAPI_H
