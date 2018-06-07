#include <iostream>
#include <stdio.h>
#include "TweetFetcher.h"

int main(int argc, char *argv[])
{
    const string query = "Trump";  // Search query
    const int count = 500;        // Number of tweets to fetch
    const int numThreads = 5;      // Number of threads to use

    // keys and tokens from the Twitter App.
    // Replace them with your own keys and secrets below.
    const string apiKey = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    const string apiSecret = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    const string accessToken = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    const string accessTokenSecret = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

    // Create a tweet fetcher
    TweetFetcher tf(apiKey, apiSecret, accessToken, accessTokenSecret);

    // Search tweets in parallel with OpenMP
    const StringList *resps = nullptr;
    if (tf.searchWithOmp(query, count, numThreads))
    {
        resps = tf.getResponseListPtr();
        if (resps && !resps->empty())
        {
            cout << tf << endl;
            // Print all tweets
            for (const string &s: *resps)
                cout << s << endl;
            cout << "==========================================================" << endl;

            // Analyze the sentiments of fetched tweets
            int numPos = 0, numNeg = 0;
            misc::pyAnalyzeSentiment(resps, numPos, numNeg);
            cout << "Number of positive: " << numPos << ", number of negative: " << numNeg << "." << endl;
        }
        else
        {
            cout << "Oops!! No responses??" << endl;
        }
    }

	return 0;
}
