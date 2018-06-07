#include <iostream>
#include <stdio.h>
#include "TweetFetcher.h"

int main(int argc, char *argv[])
{
    const string query = "Trump";  // Search query
    const int count = 800;        // Number of tweets to fetch
    const int numThreads = 8;      // Number of threads to use

    // keys and tokens from the Twitter App.
    // Replace them with your own keys and secrets below.
    const string apiKey = "7uGuUXw5gV1m33UxQUDrAgBGH";
    const string apiSecret = "WoggjUArpZ6VlZHTlMpAvvdXF7LivtB5jWGhYCnw9KssZHjlHn";
    const string accessToken = "1738543285-5gCabhy3H9MfgugIpoSXx5Ui4WZIX5aZODHep5n";
    const string accessTokenSecret = "rw6QxaX8pCZtzJe0Y49QwX0JsnmoueYyipIFMr5ZQGTX5";

    // Create a tweet fetcher
    TweetFetcher tf(apiKey, apiSecret, accessToken, accessTokenSecret);

    // Search tweets in parallel with OpenMP
    StringList resps;
    if (tf.searchWithOmp(query, count))
        tf.getResponse(resps);

    // Print all tweets
    for (const string &s: resps)
        cout << s << endl;
    cout << "==========================================================" << endl;

    // Analyze the sentiments of fetched tweets
    int numPos = 0, numNeg = 0;
    misc::pyAnalyzeSentiment(resps, numPos, numNeg);
    cout << "Number of positive: " << numPos << ", number of negative: " << numNeg << "." << endl;

	return 0;
}
