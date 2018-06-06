import re
from textblob import TextBlob
import time


def clean_tweet(tweet):
    '''
    Utility function to clean tweet text by removing links, special characters
    using simple regex statements.
    '''
    return ' '.join(re.sub("(@[A-Za-z0-9]+)|([^0-9A-Za-z \t])|(\w+:\/\/\S+)", " ", tweet).split())


def get_tweet_sentiment(tweet):
    '''
    Utility function to classify sentiment of passed tweet
    using textblob's sentiment method
    '''
    # create TextBlob object of passed tweet text
    analysis = TextBlob(tweet)
    # set sentiment
    if analysis.sentiment.polarity > 0:
        return 'pos'
    elif analysis.sentiment.polarity == 0:
        return 'neu'
    else:
        return 'neg'


def sentiment_count(list):
    """
    This function counts the number of positive and negative string in a list
    :param list: The list of strings
    :return: The tuple (no.positive strings, no.negative strings)
    """
    num_positive, num_negative = 0, 0
    for s in list:
        s = clean_tweet(s)
        sentiment = get_tweet_sentiment(s)
        if sentiment == 'pos':
            num_positive += 1
        elif sentiment == 'neg':
            num_negative += 1

    return num_positive, num_negative
