#ifndef TWEETAPI_H
#define TWEETAPI_H

#include <omp.h>
#include <memory>
#include "curl/curl.h"
#include "misc.h"
#include "Sentiment.h"

/**
 * The Tweet fetcher class to fetch tweets with multiple threads and analyze the sentiments of
 * fetched tweets.
 */
class TweetFetcher {

    // Type aliases
    using OAuthParamPairs = std::map<std::string, std::string>;
    using ResponseList = std::list<std::string>;

private:
    /*
     * The consumer key
     */
    std::string consumerKey;

    /*
     * The consumer secret
     */
    std::string consumerSecret;

    /*
     * The access token
     */
    std::string accessToken;

    /*
     * The access token secret
     */
    std::string accessTokenSecret;

    /*
     * The nouce
     */
    std::string nonce;

    /*
     * The timestamp
     */
    std::string timeStamp;

public:
    /*
     * Default onstructor. All attributes are default initialized.
     */
    TweetFetcher() = default;

    /*
     * Constructor to set all keys and secrets needed for request authentication.
     */
    TweetFetcher(const std::string& cKey,
                 const std::string& cSecret,
                 const std::string& aToken,
                 const std::string& aTokenSecret);

    /*
     * Copy constructor
     */
    TweetFetcher(const TweetFetcher& other);

    /*
     * Copy assignment operator
     */
    TweetFetcher& operator=(const TweetFetcher& other);

    /*
     * Destructor responsible for cURL cleanup and ending python session.
     */
    ~TweetFetcher();

    /*
     * Get consumer key
     */
    std::string getConsumetKey() const;

    /*
     * Get consumer secret
     */
    std::string getConsumetSecret() const;

    /*
     * Get access token
     */
    std::string getAccessToken() const;

    /*
     * Get access token secret
     */
    std::string getAccessTokenSecret() const;

    /*
     * Set the consumer key
     */
    void setConsumerKey(std::string cKey);

    /*
     * Set the consumer secret
     */
    void setConsumerSecret(std::string cSecret);

    /*
     * Set the access token
     */
    void setAccessToken(std::string aToken);

    /*
     * Set the access token secret
     */
    void setAccessTokenSecret(std::string aTokenSecret);

    /*
     * Interface to search tweets.
     *
     * @param OutList    - (output) the list of strings to store the fetched tweets
     * @param query      - the search query
     * @param count      - number of tweets to fetch
     * @param numThreads - number of threads to use to search. Default is 1
     * @return whether the request has been made and response has been received successfully.
     */
    bool searchTweets(ResponseList& OutList, const std::string &query, int count, int numThreads = 1);

    /*
     * Print the current status of this Tweet Fetcher.
     *
     * @param os - the reference to the output stream object
     * @param tf - the Tweet Fetcher whose status is to be printed.
     * @return reference to an output stream object
     */
    friend std::ostream& operator<<(std::ostream &os, const TweetFetcher &tf);


private:
    /*
     * Generate the nonce and time stamp.
     */
    void generateNonceAndTimeStamp();

    /*
     * Generate the request signature with HMAC_SHA1 algorithm and percent encode it
     *
     * @param baseURL    - the base url without signature
     * @param parameters - the query parameters
     * @return the percent-encoded request signature
     */
    std::string generateSignature(const std::string& baseURL, OAuthParamPairs& parameters);

    void collectRequestPairs(std::string, OAuthParamPairs&);

    void collectOAuthParams(const std::string&, OAuthParamPairs&);

    void collectParameters(const std::string&, const std::string&, std::string&);

    std::string generateHeader(const std::string&);

    void buildRawParamString(const OAuthParamPairs&, const std::string&, std::string&);

    /*
     * Parse the JSON format response, extract the response text and put them into the respons list.
     *
     * @param resps       - (output) the response list to store the extraced strings
     * @param responseStr - the raw response text in JSON format
     * @return none
     */
    void extractTextIntoList(ResponseList& resps, const std::string& responseStr);

    /*
     * The actual method to send requests, set up multi threading and collect reponses.
     *
     * @param OutList    - (output) the list of strings to store the fetched tweets
     * @param url        - the request URL to be sent
     * @param query      - the search query
     * @param count      - number of tweets to fetch
     * @param numThreads - number of threads to use to search. Default is 1
     * @return whether the request has been made and response has been received successfully.
     */
    bool request(ResponseList& OutList, const std::string& url, const std::string& query, int count, int numThreads);


};

#endif // TWEETAPI_H
