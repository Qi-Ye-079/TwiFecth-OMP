# TwiFetch-OMP

This repo is hosting a simple tweet-fetching (in C++ with OpenMP) and sentiment analysis (in Python) program, 
which can fetch a large number of tweets in parallel using multiple threads spawned by OpenMP, and achieve a
huge performance improvement compared to fetching such a large number of tweets in serial.

# Usage 
Check out the [example.cpp](https://bitbucket.org/QiYe079/twifetch-omp/src/master/src/example.cpp) for simple use case.

# Prerequisite..
* 	Python-dev (2.7) if you would like analyze the sentiments of fetched tweets.
*	OpenSSL.
*	CURL & libcurl - make sure to install with SSL enabled and have HTTPS protocol available after installation. Check out the official [installation guide](https://curl.haxx.se/docs/install.html).
*	OpenMP - already available: should be already with GCC on Linux.
*	rapidjson - already available: included in the project. Check out the original Github repo for [rapidjson](https://github.com/Tencent/rapidjson) for more information.
*	SHA1 - already available: included in the project. Provides some HMAC-SHA1 encode/decode algorithms.
