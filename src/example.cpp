#include <iostream>
#include <stdio.h>
#include "TweetFetcher.h"
#include "Sentiment.h"

#define OUTPUT // Define a parameter is output (by non-const reference)

int main(int argc, char *argv[])
{
    std::string query = "Trump";        // Search query
    int count = 3000;                    // Number of tweets to fetch
    int numThreads = 6;                 // Number of threads to use
    bool bAnalyzeSentiment = true;      // Whether or not analyze sentiment. Set it to false to disable sentiment analysis.

    // keys and tokens from the Twitter App.
    // Replace them with your own keys and secrets below.
    const std::string consumerKey = "7uGuUXw5gV1m33UxQUDrAgBGH";
    const std::string consumerSecret = "WoggjUArpZ6VlZHTlMpAvvdXF7LivtB5jWGhYCnw9KssZHjlHn";
    const std::string accessToken = "1738543285-5gCabhy3H9MfgugIpoSXx5Ui4WZIX5aZODHep5n";
    const std::string accessTokenSecret = "rw6QxaX8pCZtzJe0Y49QwX0JsnmoueYyipIFMr5ZQGTX5";

    // Create a tweet fetcher
    TweetFetcher tf(consumerKey, consumerSecret, accessToken, accessTokenSecret);

    // Search tweets in parallel with OpenMP
    std::list<std::string> resps;
    if (!tf.searchTweets(OUTPUT resps, query, count, numThreads))
    {
        std::cout << "Something went wrong when fetching tweets." << std::endl;
        return -1;
    }

    if (resps.empty())
    {
        std::cout << "Oops!! No responses??" << std::endl;
        return 1;
    }

    // Print current status and all fetched tweets
    std::cout << tf << std::endl;
    std::cout << "There are " << resps.size() << " tweets fetched." << std::endl;
    std::cout << "============================================================" << std::endl;
    for (const auto& s: resps)
    {
        std::cout << s << std::endl;
    }
    std::cout << "==========================================================" << std::endl;

    if (bAnalyzeSentiment)
    {
        // Analyze the sentiments of fetched tweets and return the results pair (tuple from Python)
        auto results = sentiment_count(&resps);
        std::cout << "Number of positive: " << results.first << ", number of negative: " << results.second << ".\n" << std::endl;
    }

	return 0;
}
