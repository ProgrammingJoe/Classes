#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import division
import numpy as np
from sklearn.feature_extraction.text import CountVectorizer
from nltk.stem import *
import sys
reload(sys)
sys.setdefaultencoding('utf-8')


class MyBayesClassifier():
    def __init__(self, smooth=1):
        self._smooth = smooth # This is for additive smoothing
        self._feat_prob = [] # do not change the name of these vars
        self._class_prob = []
        self._Ncls = []
        self._Nfeat = []
        self._chanceofclass = []

    def train(self, X, y):
        alpha_smooth = self._smooth
        cls = np.unique(y)
        Ncls, Nfeat = len(cls), X.shape[1]
        self._Ncls, self._Nfeat = Ncls, Nfeat
        self._feat_prob = np.zeros((Ncls, Nfeat))
        self._class_prob = np.zeros(Ncls)
        splicer, self._chanceofclass = self.section(y)
        class_arrays = np.split(X, splicer)
        print("smooth is " + str(alpha_smooth))
        print("The probabilities of each class are: " + str(self._chanceofclass))
        # Calculate probabilities for every feature for each class
        for title in range(0, Ncls):
            self._feat_prob[title] = (class_arrays[title].sum(0) + alpha_smooth)/(len(class_arrays[title]) + alpha_smooth*Nfeat) # + total number of words used in positive scenarios

    def predict(self, X):
        pred = np.zeros(len(X))
        counter = 0
        for sentence in X:
            problist = []
            for title in range(0, self._Ncls):
                location = 0
                prob = 1
                for word in sentence:
                    if word == 1:
                        prob *= self._feat_prob[title][location]
                    location += 1
                problist.append(prob*self._chanceofclass[title])
            with np.errstate(divide='ignore', invalid='ignore'):
                pred[counter] = list(reversed(problist)).index(max(problist))
            counter += 1
        return pred

    def section(self, y):
        cls, titlecounts = np.unique(y, return_counts=True)
        rows = 0
        splicer = []
        chanceofclass = []
        for title in range(0, len(cls)):
            splicer.append(titlecounts[title] + rows)
            rows += titlecounts[title]
            chanceofclass.append(titlecounts[title])
        chanceofclass = chanceofclass/rows
        return splicer[:-1], chanceofclass

    @property
    def probs(self):
        # please leave this intact, we will use it for marking
        return self._class_prob, self._feat_prob


# Used to stem lines of a file
def steminput(filelines):
    stemmed_list = []
    stemmer = PorterStemmer()
    for review in filelines:
        tempsent = []
        for word in review.split():
            tempsent.append(stemmer.stem(word))
        stemmed_list.append(' '.join(tempsent))
    return stemmed_list

"""
Here is the calling code
"""

with open('sentiment_data/rt-polarity_utf8.neg', 'r') as f:
    lines_neg = f.read().splitlines()

with open('sentiment_data/rt-polarity_utf8.pos', 'r') as f:
    lines_pos = f.read().splitlines()

# Comment out these two lines to remove stemming
lines_neg_stem = steminput(lines_neg)
lines_pos_stem = steminput(lines_pos)

data_train = lines_neg_stem[0:5000] + lines_pos_stem[0:5000] # sentences
data_test = lines_neg_stem[5000:] + lines_pos_stem[5000:] # sentences

y_train = np.append(np.ones((1,5000)), (np.zeros((1,5000)))) # 5000 1's then 5000 0's
y_test = np.append(np.ones((1,331)), np.zeros((1,331))) # 331 1's then 331 0's

# You will be changing the parameters to the CountVectorizer below
vectorizer = CountVectorizer(lowercase=True, stop_words='english',  max_df=1.0, min_df=1, max_features=None,  binary=True)
X_train = vectorizer.fit_transform(data_train).toarray()
X_test = vectorizer.transform(data_test).toarray()
feature_names = vectorizer.get_feature_names()

clf = MyBayesClassifier(1);
clf.train(X_train,y_train);
y_pred = clf.predict(X_test)
print np.mean((y_test-y_pred)==0)
