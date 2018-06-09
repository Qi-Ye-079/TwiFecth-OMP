import re
from textblob import TextBlob

# import C++ STL container declarations
from libcpp.string cimport string
from libcpp.list cimport list
from libcpp.pair cimport pair

cdef public string clean_tweet(string tweet):
    '''
    Utility function to clean tweet text by removing links, special characters
    using simple regex statements.
    '''
    return ' '.join(re.sub("(@[A-Za-z0-9]+)|([^0-9A-Za-z \t])|(\w+:\/\/\S+)", " ", tweet).split())


cdef public int get_tweet_sentiment(string tweet):
    '''
    Utility function to classify sentiment of passed tweet
    using textblob's sentiment method
    '''
    # create TextBlob object of passed tweet text
    analysis = TextBlob(tweet)
    # set sentiment
    if analysis.sentiment.polarity > 0:
        return 1
    elif analysis.sentiment.polarity == 0:
        return 0
    else:
        return -1


cdef public pair[int, int] sentiment_count(list[string] resp_list):
    """
    This function counts the number of positive and negative string in a list
    :param list: The list of strings
    :return: The tuple (no.positive strings, no.negative strings)
    """
    cdef int num_positive = 0
    cdef int num_negative = 0

    cdef string s          # Each string from the resp_list
    cdef int sentiment     # The sentiment for each string
    for s in resp_list:
        s = clean_tweet(s)
        sentiment = get_tweet_sentiment(s)
        if sentiment > 0:
            num_positive += 1
        elif sentiment < 0:
            num_negative += 1

    return num_positive, num_negative
