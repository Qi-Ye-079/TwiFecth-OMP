#include <iostream>
#include <stdio.h>
#include "TweetFetcher.h"
#include "Sentiment.h"

// Typs aliases
using StringList = std::list<std::string>;

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
    StringList *resps = nullptr;
    if (tf.searchWithOmp(query, count, numThreads))
    {
        resps = tf.getResponseListPtr();
        if (resps && !resps->empty())
        {
            // Print current status and all fetched tweets
            std::cout << tf << std::endl;
            for (const std::string &s: *resps)  std::cout << s << std::endl;
            std::cout << "==========================================================" << std::endl;

            if (bAnalyzeSentiment)
            {
                // Analyze the sentiments of fetched tweets and return the results pair (tuple from Python)
                std::pair<int, int> results = sentiment_count(resps);
                std::cout << "Number of positive: " << results.first << ", number of negative: " << results.second << "." << std::endl;
            }
        }
        else
        {
            std::cout << "Oops!! No responses??" << std::endl;
        }
    }

	return 0;
}
