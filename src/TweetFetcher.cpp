#include "TweetFetcher.h"
#include "rapidjson/document.h"
#include "SHA1/base64.h"
#include "SHA1/HMAC_SHA1.h"

#include <iostream>
#include <algorithm>

// Namespace used
using namespace rapidjson;
using namespace misc;

// ================== Local helper function ======================
static size_t write_callback(char *contents, size_t size, size_t nmemb, void *userp)
{
    std::string *s = (std::string*)userp;
    s->append(contents);

    return size * nmemb;
}


// ================= Public functions of TweetFetcher ==================
TweetFetcher::TweetFetcher(const std::string& cKey,
                           const std::string& cSecret,
                           const std::string& aToken,
                           const std::string& aTokenSecret):
    consumerKey(cKey), consumerSecret(cSecret), accessToken(aToken), accessTokenSecret(aTokenSecret),
    nonce(""), timeStamp("")
{
    // Initialize winsock
    curl_global_init(CURL_GLOBAL_ALL);

    // Initialize Python environment and sentiment analysis module
    Py_Initialize();
    initsentiment_analysis();
}

TweetFetcher::TweetFetcher(const TweetFetcher &other):
    consumerKey(other.consumerKey),
    consumerSecret(other.consumerSecret),
    accessToken(other.accessToken),
    accessTokenSecret(other.accessTokenSecret),
    nonce(other.nonce),
    timeStamp(other.timeStamp)
{
    // Initialize the curl globally
    curl_global_init(CURL_GLOBAL_ALL);

    // Initialize Python environment and sentiment_analysis module
    Py_Initialize();
    initsentiment_analysis();
}

TweetFetcher& TweetFetcher::operator=(const TweetFetcher &other)
{
    // Check if assigning itself
    if (&other == this)
        return *this;

    // Assign all values from other to this
    consumerKey = other.consumerKey;
    consumerSecret = other.consumerSecret;
    accessToken = other.accessToken;
    accessTokenSecret = other.accessTokenSecret;
    nonce = other.nonce;
    timeStamp = other.timeStamp;
}

TweetFetcher::~TweetFetcher()
{
    // Clean the CURL
    curl_global_cleanup();

    // End Python session
    Py_Finalize();
}

// Getters
std::string TweetFetcher::getConsumetKey() const  {   return consumerKey;   }
std::string TweetFetcher::getConsumetSecret() const  {   return consumerSecret;  }
std::string TweetFetcher::getAccessToken() const  {  return accessToken;  }
std::string TweetFetcher::getAccessTokenSecret() const  {   return accessTokenSecret;  }

// Setters
void TweetFetcher::setConsumerKey(std::string cKey) { consumerKey = cKey; }
void TweetFetcher::setConsumerSecret(std::string cSecret) { consumerSecret = cSecret; }
void TweetFetcher::setAccessToken(std::string aToken) { accessToken = aToken; }
void TweetFetcher::setAccessTokenSecret(std::string aTokenSecret) { accessTokenSecret = aTokenSecret; }


bool TweetFetcher::searchTweets(ResponseList& OutList, const std::string& query, int count, int numThreads)
{
    // Build the search url with query
    int requestCount = std::min((count/numThreads), 100);
    std::string url = "https://api.twitter.com/1.1/search/tweets.json?count="
                      + std::to_string(requestCount) + "&lang=en&include_entities=false&q=";

    // Start the https request
    return request(OutList, url, query, count, numThreads);
}

std::ostream& operator<<(std::ostream &os, const TweetFetcher &tf)
{
    os << "====== Printing info for Tweet Fethcer =======" << std::endl;
    os << "Consumer Key: " << tf.consumerKey << std::endl;
    os << "Consumer Secret: " << tf.consumerSecret << std::endl;
    os << "Access Token: " << tf.accessToken << std::endl;
    os << "Access Token Secret: " << tf.accessTokenSecret << std::endl;
    return os;
}


// ===================== Private functions of Tweet Function ==================
std::string TweetFetcher::generateSignature(const std::string& baseUrl, OAuthParamPairs& parameters)
{
    std::string rawParams("");
    // 1. Build parameter strings
    buildRawParamString(parameters, "&", rawParams);
    // 2: Get the base signature string
    std::string baseSig = "GET&" + percentEncode(baseUrl) + "&" + percentEncode(rawParams);

	  // 3: Get the signing key
    std::string signingKey = percentEncode(consumerSecret) + "&" + percentEncode(accessTokenSecret);

	  // 4: Calculate the signature using HMAC_SHA1 algorithm
    BYTE digest[20];
    memset(digest, 0, 20);

    CHMAC_SHA1 hmacsha1;
    hmacsha1.HMAC_SHA1((BYTE*)baseSig.c_str(), baseSig.length(), (BYTE*)signingKey.c_str(), signingKey.length(), digest);

    // 5: Base64 encode
    std::string b64en = base64_encode(digest, 20);

    // 6: Percent encode the base64 string
    return percentEncode(b64en);
}

void TweetFetcher::collectRequestPairs(std::string dataUrl, OAuthParamPairs& requestPairs)
{
  	size_t pos = dataUrl.find_first_of("&");
  	// For n-1 request pairs
    while (pos != std::string::npos) {
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

void TweetFetcher::collectOAuthParams(const std::string& signature, OAuthParamPairs& authParams)
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

void TweetFetcher::collectParameters(const std::string& baseUrl,
                                     const std::string& dataUrl,
                                     std::string& rawParams)
{
  	OAuthParamPairs parameters;

  	// Get request pairs
  	collectRequestPairs(dataUrl, parameters);

  	// Then get auth parameters without signature
  	collectOAuthParams("", parameters);

  	// Create signature using collected parameters
    std::string sig = generateSignature(baseUrl, parameters);

  	// Get complete parameters using signature
  	parameters.clear();
  	collectOAuthParams(sig, parameters);

  	// Build the raw parameter string
  	buildRawParamString(parameters, ", ", rawParams);
}

std::string TweetFetcher::generateHeader(const std::string& url)
{
	// First split URL as base url and data
    std::string baseUrl(""), dataUrl(""), httpHeader("");
	size_t pos = url.find_first_of("?");
    if (pos != std::string::npos) {
        // Split base and data parts of URL
		baseUrl = url.substr(0, pos);
		dataUrl = url.substr(pos + 1);

		// Then collect all the parameters needed for Authentication
        std::string rawParams("");
		collectParameters(baseUrl, dataUrl, rawParams);

		// Get complete header
        httpHeader = "Authorization: OAuth ";
		httpHeader.append(rawParams);
	}

    return httpHeader;

}

void TweetFetcher::buildRawParamString(const OAuthParamPairs& parameters,
                                       const std::string& seperator,
                                       std::string& paramString)
{
	// Append each raw parameter pair and store them in a list
    std::list<std::string> paramList;
    for (auto i = parameters.cbegin(); i != parameters.cend(); i++) {
        std::string str = i->first;
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


void TweetFetcher::extractTextIntoList(ResponseList& resps, const std::string& response)
{
    if (!response.empty())
    {
        // Parse the response string to DOM-styled JSON
        Document d;
        d.Parse(response.c_str());

        assert(d.IsObject());

        if (d.HasMember("statuses"))
        {
            // Extract tweets from parsed JSON
            const Value& rootArr = d["statuses"];
            for (auto &e : rootArr.GetArray())
            {
                std::string text = e["text"].GetString();
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


bool TweetFetcher::request(ResponseList& OutList, const std::string& URL, const std::string& query, int count, int numThreads)
{
	// Get and set complete OAuth header for request
    std::string OAuthHeader = generateHeader(URL+query);
    if (!OAuthHeader.length())
        return false;

    bool success = false;

    // Start fetching tweets in parallel with OpenMP
    #pragma omp parallel num_threads(numThreads)
    {
        // The list to store the parsed response strings
        ResponseList tRespList;
        bool tSuccess = false;

        // Create curl handler of this thread
        CURL *threadCurl = curl_easy_init();

        // Loop to fetch the tweets
        int numLoops = std::max(1, (count/100/numThreads));
        for (uint8_t i = 0; i < numLoops; ++i)
        {
            // The raw response string from a single request in json format
            std::string resp("");

            // Process CURL request handling
            curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, OAuthHeader.c_str());
            curl_easy_setopt(threadCurl, CURLOPT_HTTPHEADER, headers);

            // Send request
            curl_easy_setopt(threadCurl, CURLOPT_HTTPGET, 1);
            curl_easy_setopt(threadCurl, CURLOPT_URL, (URL+percentEncode(query)).c_str());
            curl_easy_setopt(threadCurl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(threadCurl, CURLOPT_WRITEDATA, &resp);
            if (curl_easy_perform(threadCurl) == CURLE_OK)
            {
                // Extract all strings into the thread string list
                extractTextIntoList(tRespList, resp);

                // Set success to true
                tSuccess = true;
            }

            // Free all curl headers
            if (headers)
                curl_slist_free_all(headers);
        }

        // Clean up Curl handler for this thread
        curl_easy_cleanup(threadCurl);

        #pragma omp critical
        {
            success = success || tSuccess;
            OutList.splice(OutList.end(), tRespList);
        }
    }

    return success;
}
