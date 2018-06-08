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
    string *s = (string*)userp;
    s->append(contents);

    return size * nmemb;
}


// ================= Public functions of TweetFetcher ==================
TweetFetcher::TweetFetcher(): TweetFetcher("", "", "", "")
{
}

TweetFetcher::TweetFetcher(const string& cKey,
                           const string& cSecret,
                           const string& aToken,
                           const string& aTokenSecret):
    consumerKey(cKey), consumerSecret(cSecret), accessToken(aToken), accessTokenSecret(aTokenSecret),
    nonce(""), timeStamp("")
{
    // Allocate the responses list on the heap
    responses = new StringList();

    // Initialize winsock
    curl_global_init(CURL_GLOBAL_ALL);
}

TweetFetcher::TweetFetcher(const TweetFetcher &other):
    consumerKey(other.consumerKey),
    consumerSecret(other.consumerSecret),
    accessToken(other.accessToken),
    accessTokenSecret(other.accessTokenSecret),
    nonce(other.nonce),
    timeStamp(other.timeStamp)
{
    // Copy the contents of the response list from other tweet fetcher
    responses = new StringList(*other.responses);

    // Initialize the curl globally
    curl_global_init(CURL_GLOBAL_ALL);
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

    // First deallocate the memory of this response list
    responses->clear();
    delete responses;
    // Then copy the list from other
    responses = new StringList(*other.responses);

    // Initialize the curl globally
    curl_global_init(CURL_GLOBAL_ALL);
}

TweetFetcher::~TweetFetcher()
{
    // Clean the CURL
  	curl_global_cleanup();
    // free the response's memory
    responses->clear();
    delete responses;
}

bool TweetFetcher::searchWithOmp(const string& query, int count, int numThreads)
{
    // Build the search url with query
    string url = "https://api.twitter.com/1.1/search/tweets.json?count=100&lang=en&include_entities=false&q=";

    // Start the https request
    return request(url, query, count, numThreads); //&& (response.size != 0);
}

const StringList* TweetFetcher::getResponseListPtr() const
{
    return responses;
}

ostream& operator<<(ostream &os, const TweetFetcher &tf)
{
    os << "====== Printing info for Tweet Fethcer =======" << endl;
    os << "Consumer Key: " << tf.consumerKey << endl;
    os << "Consumer Secret: " << tf.consumerSecret << endl;
    os << "Access Token: " << tf.accessToken << endl;
    os << "Access Token Secret: " << tf.accessTokenSecret << endl;
    os << "============================================================" << endl;
    os << "There are now " << tf.responses->size() << " responses stored in this Tweet Fetcher." << endl;
    return os;
}


// ===================== Private functions of Tweet Function ==================
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

string TweetFetcher::generateHeader(const string& url)
{
	// First split URL as base url and data
    string baseUrl(""), dataUrl(""), httpHeader("");
	size_t pos = url.find_first_of("?");
    if (pos != string::npos) {
        // Split base and data parts of URL
		baseUrl = url.substr(0, pos);
		dataUrl = url.substr(pos + 1);

		// Then collect all the parameters needed for Authentication
        string rawParams("");
		collectParameters(baseUrl, dataUrl, rawParams);

		// Get complete header
        httpHeader = "Authorization: OAuth ";
		httpHeader.append(rawParams);
	}

    return httpHeader;

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


void TweetFetcher::extractTextIntoList(StringList& resps, string& response)
{
    if (!response.empty())
    {
        // Parse the response string to DOM-styled JSON
        Document d;
        d.Parse(response.c_str());

        // Reset response memory
        // response.clear();

        assert(d.IsObject());

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


bool TweetFetcher::request(const string& URL, const string& query, int count, int numThreads)
{
	// Get and set complete OAuth header for request
    string OAuthHeader = generateHeader(URL+query);
    if (!OAuthHeader.length())
        return false;

    bool success = false;

    // Start fetching tweets in parallel with OpenMP
    #pragma omp parallel num_threads(numThreads)
    {
        // The list to store the parsed response strings
        StringList tRespList;
        bool tSuccess = false;

        // Create curl handler of this thread
        CURL *threadCurl = curl_easy_init();

        // Loop to fetch the tweets
        for (int i = 0; i < count / 100 /numThreads; ++i)
        {
            // The raw response string from a single request in json format
            string resp("");

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
                tSuccess = tSuccess || true;
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
            responses->splice(responses->end(), tRespList);
        }
    }

    return success;
}
