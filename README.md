# TwiFetch-OMP

This repo is hosting a simple tweet-fetching (in C++ with OpenMP) and sentiment analysis (in Python) program, 
which can fetch a large number of tweets in parallel using multiple threads spawned by OpenMP, and achieve a
huge performance improvement compared to fetching such a large number of tweets in serial.

# UPDATE

Now Cython has been used to bridge C++ and Python codes.

# Usage 
Check out the [example.cpp](https://bitbucket.org/QiYe079/twifetch-omp/src/master/src/example.cpp) for simple use case.

# Prerequisite
* 	Python-dev 2.7 if you would like analyze the sentiments of fetched tweets.
*	UPDATED: Cython 0.28.3 (no ganrantee that newer version will work). Just use `pip install cython` to install it.
*	OpenSSL - should be already in the Ubuntu system. You also need to find the path of `ssl` directory to install CURL.
*	CURL & libcurl - make sure to install with SSL enabled and have HTTPS protocol available after installation. Check out the official [installation guide](https://curl.haxx.se/docs/install.html).
*	OpenMP - already available: should be already with GCC on Linux.
*	rapidjson - already available: included in the project. Check out the original [Github repo for rapidjson](https://github.com/Tencent/rapidjson) for more information.
*	SHA1 - already available: included in the project. Provides some HMAC-SHA1 encode/decode algorithms.

# Useful resources
*	[Compilation - Cython 0.28.3 documentation](https://cythoncython.readthedocs.io/en/stable/src/reference/compilation.html#configuring-the-c-build) - How to Cythonize python modules.
*	[Interfacing with external C code](http://cython.readthedocs.io/en/latest/src/userguide/external_C_code.html#public-declarations) - How to create C functions accessible from other C/C++ codes.
*	[Cython reference guide](https://media.readthedocs.org/pdf/cython/latest/cython.pdf)