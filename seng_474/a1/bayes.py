#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import division
import numpy as np
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.datasets import fetch_20newsgroups
import logging
import sys
from time import time


class MyBayesClassifier():
    # For graduate and undergraduate students to implement Bernoulli Bayes
    def __init__(self, smooth=1):
        self._smooth = smooth # This is for add one smoothing, don't forget!
        self._feat_prob = []
        self._class_prob = []
        self._Ncls = []
        self._Nfeat = []

    def train(self, X, y):
        alpha_smooth = self._smooth
        label = np.unique(y)
        Ncls, Nfeat = len(label), X.shape[1]
        self._Ncls, self._Nfeat = Ncls, Nfeat
        self._feat_prob = np.zeros((Ncls, Nfeat))
        splicer, self._class_prob = self.section(y)
        class_arrays = np.split(X, splicer)
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
                problist.append(prob*self._class_prob[title])
            with np.errstate(divide='ignore', invalid='ignore'):
                # pred[counter] = list(reversed(problist)).index(max(problist))
                pred[counter] = list(problist).index(max(problist))
            counter += 1
        return pred

    def section(self, y):
        label, titlecounts = np.unique(y, return_counts=True)
        rows = 0
        splicer = []
        chanceofclass = []
        for title in range(0, len(label)):
            splicer.append(titlecounts[title] + rows)
            rows += titlecounts[title]
            chanceofclass.append(titlecounts[title])
        chanceofclass = chanceofclass/rows
        return splicer[:-1], chanceofclass

    def sortTrainingData(self, X, y):
        y_2darray = np.reshape(y, (-1, 1))
        merged = np.hstack((X, y_2darray))
        merged = merged[np.argsort(merged[:, -1])]
        X = merged[:,0:-1]
        y = merged[:,-1]
        return X, y

class MyMultinomialBayesClassifier():
    # For graduate students only
    def __init__(self, smooth=1):
        self._smooth = smooth # This is for add one smoothing, don't forget!
        self._feat_prob = []
        self._class_prob = []
        self._Ncls = []
        self._Nfeat = []

    # Train the classifier using features in X and class labels in Y
    def train(self, X, y):
        # Your code goes here.
        return

    # should return an array of predictions, one for each row in X
    def predict(self, X):
        # This is just a place holder so that the code still runs.
        # Your code goes here.
        return np.zeros([X.shape[0],1])



"""
Here is the calling code

"""

categories = [
        'alt.atheism',
        'talk.religion.misc',
        'comp.graphics',
        'sci.space',
    ]
remove = ('headers', 'footers', 'quotes')

data_train = fetch_20newsgroups(subset='train', categories=categories,
                                shuffle=True, random_state=42,
                                remove=remove)

data_test = fetch_20newsgroups(subset='test', categories=categories,
                               shuffle=True, random_state=42,
                               remove=remove)
print('data loaded')

y_train, y_test = data_train.target, data_test.target

print("Extracting features from the training data using a count vectorizer")
t0 = time()

vectorizer = CountVectorizer(stop_words='english', binary=True)#, analyzer='char', ngram_range=(1,3))
X_train = vectorizer.fit_transform(data_train.data).toarray()
X_test = vectorizer.transform(data_test.data).toarray()
feature_names = vectorizer.get_feature_names()

alpha = 1
clf = MyBayesClassifier(alpha)
X_train, y_train = clf.sortTrainingData(X_train, y_train)
clf.train(X_train,y_train)
y_pred = clf.predict(X_test)
print 'alpha=%i accuracy = %f' %(alpha, np.mean((y_test-y_pred)==0))
