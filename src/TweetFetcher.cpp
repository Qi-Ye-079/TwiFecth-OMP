#include "TweetFetcher.h"
#include "rapidjson/document.h"
#include "SHA1/base64.h"
#include "SHA1/HMAC_SHA1.h"

#include <omp.h>
#include <iostream>
#include <algorithm>

// Namespace used
using namespace rapidjson;
using namespace misc;

static size_t write_callback(char *contents, size_t size, size_t nmemb, void *userp)
{
    size_t dataSize = size * nmemb; // Get the size of received data
    Response *mem = (Response*)userp; // Pointer to the chunk that saves the data

    // Grow the size of the chunk by dataSize + 1
    mem->memory = (char*)realloc(mem->memory, mem->size + dataSize + 1);
    if (mem->memory == NULL) {
        printf("Out of memory!!");
        return 0;
    }

    /* Copy the received data to the grown area of chunk */
    memcpy(&(mem->memory[mem->size]), contents, dataSize);
    mem->size += dataSize;
    mem->memory[mem->size] = 0;

    return dataSize;
}

TweetFetcher::TweetFetcher(const string& cKey,
                           const string& cSecret,
                           const string& aToken,
                           const string& aTokenSecret):
    consumerKey(cKey), consumerSecret(cSecret), accessToken(aToken), accessTokenSecret(aTokenSecret),
    nonce(""), timeStamp("")
{
    // Initialize winsock
    curl_global_init(CURL_GLOBAL_ALL);

    // Get a curl handler
    curl = curl_easy_init();

    // Initialize the memory chunk to store response
    response.memory = (char*)malloc(1);
    response.size = 0;
}

TweetFetcher::~TweetFetcher()
{
    // Clean the CURL and free the response's memory
  	curl_easy_cleanup(curl);
  	free(response.memory);
  	curl_global_cleanup();
}

void TweetFetcher::resetResponseMem()
{
  	free(response.memory);
  	response.memory = (char*)malloc(1);
  	response.size = 0;
}

string TweetFetcher::generateSignature(const string& baseUrl, OAuthParamPairs& parameters)
{
    string rawParams("");
    // 1. Build parameter strings
    buildRawParamString(parameters, "&", rawParams);
    // 2: Get the base signature string
    string baseSig = "GET&" + percentEncode(baseUrl) + "&" + percentEncode(rawParams);

	  // 3: Get the signing key
    string signingKey = percentEncode(consumerSecret) + "&" + percentEncode(accessTokenSecret);

	  // 4: Calculate the signature using HMAC_SHA1 algorithm
	  BYTE digest[20];
	  memset(digest, 0, 20);

	  CHMAC_SHA1 hmacsha1;
	  hmacsha1.HMAC_SHA1((BYTE*)baseSig.c_str(), baseSig.length(), (BYTE*)signingKey.c_str(), signingKey.length(), digest);

    // 5: Base64 encode
    string b64en = base64_encode(digest, 20);

    // 6: Percent encode the base64 string
    return percentEncode(b64en);
}

void TweetFetcher::collectRequestPairs(string dataUrl, OAuthParamPairs& requestPairs)
{
  	size_t pos = dataUrl.find_first_of("&");
  	// For n-1 request pairs
    while (pos != string::npos) {
        size_t splitPos = dataUrl.find_first_of("=");

        // Add pair to map
        requestPairs[dataUrl.substr(0, splitPos)] = percentEncode(dataUrl.substr(splitPos+1, pos-splitPos-1));

        // Shrink data url
        dataUrl = dataUrl.substr(pos + 1);
        pos = dataUrl.find_first_of("&");
    }

  	// For the last request pair
  	size_t splitPos = dataUrl.find_first_of("=");
  	requestPairs[dataUrl.substr(0, splitPos)] = percentEncode(dataUrl.substr(splitPos + 1));
}

void TweetFetcher::collectOAuthParams(const string& signature, OAuthParamPairs& authParams)
{
  	if (!signature.length()) generateNonceAndTimeStamp();

  	authParams["oauth_consumer_key"] = consumerKey;
  	authParams["oauth_nonce"] = nonce;
    if (signature.length())
        authParams["oauth_signature"] = signature;
  	authParams["oauth_signature_method"] = "HMAC-SHA1";
  	authParams["oauth_timestamp"] = timeStamp;
  	authParams["oauth_token"] = accessToken;
  	authParams["oauth_version"] = "1.0";
}

void TweetFetcher::collectParameters(const string& baseUrl,
                                     const string& dataUrl,
                                     string& rawParams)
{
  	OAuthParamPairs parameters;

  	// Get request pairs
  	collectRequestPairs(dataUrl, parameters);

  	// Then get auth parameters without signature
  	collectOAuthParams("", parameters);

  	// Create signature using collected parameters
    string sig = generateSignature(baseUrl, parameters);

  	// Get complete parameters using signature
  	parameters.clear();
  	collectOAuthParams(sig, parameters);

  	// Build the raw parameter string
  	buildRawParamString(parameters, ", ", rawParams);
}

void TweetFetcher::generateHeader(const string& url, string& httpHeader)
{
	// First split URL as base url and data
    string baseUrl(""), dataUrl("");
	size_t pos = url.find_first_of("?");
    if (pos != string::npos) {
		baseUrl = url.substr(0, pos);
		dataUrl = url.substr(pos + 1);

		// Then collect all the parameters needed for Authentication
        string rawParams("");
		collectParameters(baseUrl, dataUrl, rawParams);

		// Get complete header
		httpHeader = "Authorization: OAuth ";
		httpHeader.append(rawParams);
	}

}

void TweetFetcher::buildRawParamString(const OAuthParamPairs& parameters,
                                       const string& seperator,
                                       string& paramString)
{
	// Append each raw parameter pair and store them in a list
    StringList paramList;
    for (auto i = parameters.cbegin(); i != parameters.cend(); i++) {
        string str = i->first;
		str.append("=");
		if (seperator == ", ") str.append("\"");
		str.append(i->second);
		if (seperator == ", ") str.append("\"");
		paramList.push_back(str);
	}

	// Sort the parameter list in alphabetical order by key
	paramList.sort();

	// Get the raw parameters string
	paramString = "";
    for (auto i = paramList.cbegin(); i != paramList.cend(); i++) {
        if (paramString.length())
            paramString.append(seperator);
		paramString.append(*i);
	}
}

void TweetFetcher::generateNonceAndTimeStamp()
{
	char szTime[1024];
	char szRand[1024];
	memset(szTime, 0, 1024);
	memset(szRand, 0, 1024);
	srand((unsigned int)time(NULL));
	sprintf(szRand, "%x", rand() % 1000);
	sprintf(szTime, "%ld", time(NULL));

	nonce = szTime;
	nonce.append(szRand);
	timeStamp = szTime;
}

bool TweetFetcher::request(const string& URL, const string& query)
{
	struct curl_slist *headers = nullptr;

	// Get and set complete OAuth header for request
    string OAuthHeader("");
	generateHeader(URL+query, OAuthHeader);
    if (!OAuthHeader.length())
        return false;

	headers = curl_slist_append(headers, OAuthHeader.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	// Send request
	curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(curl, CURLOPT_URL, (URL+percentEncode(query)).c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	resCode = curl_easy_perform(curl);
	if (resCode == CURLE_OK) {
        if (headers)
            curl_slist_free_all(headers);
		return true;
	}
	else {
		return false;
	}
}

bool TweetFetcher::search(const string& query, int count)
{
	// Build the search url with query
    string countStr = std::to_string(count);
    string url = "https://api.twitter.com/1.1/search/tweets.json?count=" +
				   countStr + "&lang=en&include_entities=false&q=";

	// Start the https request
    return request(url, query) && (response.size != 0);
}

void TweetFetcher::getResponse(StringList& resps)
{
    if (response.size) {

		// Parse the response string to DOM-styled JSON
		Document d;
		d.Parse(response.memory);

		// Reset response memory
		resetResponseMem();

        if (d.HasMember("statuses"))
        {
			// Extract tweets from parsed JSON
			const Value& rootArr = d["statuses"];
            for (auto &e : rootArr.GetArray())
            {
                string text = e["text"].GetString();
				resps.push_back(text);
			}
		}
        else
        {
			printf("Rate limit execeeded!! Please try again after 15 minutes.\n\n");
			exit(1);
		}
	}

}

/*
void TweetApi::searchWithOMP(const string& cKey, const string& cSecret, const string& aToken, const string& aTokenSecret,
                             const string& query, const int& count, const int& numThreads)
{
	// Responses and number of tweets
	StringList resps;
	int numPos = 0, numNeg = 0;

	auto start = Clock::now();
	#pragma omp parallel num_threads(numThreads)
	{
		TweetApi api(cKey, cSecret, aToken, aTokenSecret);
		StringList partialResp;

		#pragma omp for ordered
		for (int i = 0; i < count / 100; ++i) {
			if (api.search(query, 100)) {
				api.getResponse(partialResp);
			}
		}
		#pragma omp critical
		{
			resps.splice(resps.end(), partialResp);
		}
	}

	auto fetch = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count();

	// Initialize Python program for sentiment analysis
	pyAnalyzeSentiment(resps, numPos, numNeg);

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count();

    for (const string& s : resps) printf("%s", s.c_str());
	int numTweets = resps.size();
	printf("\n\n=======================\nTotal number of tweets: %d", numTweets);
	printf("\nNumber of positive tweets: %d, percentage: %2.2f%%", numPos, numPos*100.f / numTweets);
	printf("\nNumber of negative tweets: %d, percentage: %2.2f%%", numNeg, numNeg*100.f / numTweets);
	printf("\n\nTime for fetching %d tweets: %2.3f seconds", numTweets, fetch / 1000.f);
	printf("\nTime for sentiment analysis: %2.3f seconds, ", (duration - fetch) / 1000.f);
	printf("\nThe total execution time: %2.3f sec.\n\n", duration / 1000.f);
}*/
