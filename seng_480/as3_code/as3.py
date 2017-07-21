from __future__ import division
import re
import numpy as np
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.naive_bayes import MultinomialNB
from nltk.stem import *
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

def trimcontext(paragraph):
    start = paragraph.find('<context>') + 9
    end = paragraph.find('</context>', start)
    return paragraph[start:end].replace('<head>', '').replace('</head>', '').replace('  ', ' ').replace(',', '').replace('.', '')

def gettestid(paragraph):
    start = paragraph.find('appear.v.bnc.')
    return paragraph[start:start+8]

def main():
    with open('EnglishLS.train', 'r') as f:
        instances = f.read().replace('\n', ' ').split('</instance>')
    with open('EnglishLS.test', 'r') as f:
        testinstances = f.read().replace('\n', ' ').split('</instance>')
    with open('EnglishLS.test.key', 'r') as f:
        testkey = f.readlines()

    testingidssense1 = []
    testingidssense2 = []
    testingidssense3 = []
    for line in testkey:
        if re.match('.*190901.*', line):
            for paragraph in testinstances:
                if gettestid(line) in paragraph:
                    testingidssense1.append(trimcontext(paragraph))
        elif re.match('.*190902.*', line):
            for paragraph in testinstances:
                if gettestid(line) in paragraph:
                    testingidssense2.append(trimcontext(paragraph))
        elif re.match('.*190903.*', line):
            for paragraph in testinstances:
                if gettestid(line) in paragraph:
                    testingidssense3.append(trimcontext(paragraph))

    sense1 = []
    sense2 = []
    sense3 = []
    for instance in instances:
        if re.match('.*senseid="190901".*', instance):
            sense1.append(trimcontext(instance))
        elif re.match('.*senseid="190902".*', instance):
            sense2.append(trimcontext(instance))
        elif re.match('.*senseid="190903".*', instance):
            sense3.append(trimcontext(instance))

    data_train = sense1 + sense2 + sense3
    data_test = testingidssense1 + testingidssense2 + testingidssense3
    y_test = [190901]*len(testingidssense1) + [190902]*len(testingidssense2) + [190903]*len(testingidssense3)
    y_train = [190901]*len(sense1) + [190902]*len(sense2) + [190903]*len(sense3)
    print("Testing " + str(len(testingidssense1)) + " examples of sense 1")
    print("Testing " + str(len(testingidssense2)) + " examples of sense 2")
    print("Testing " + str(len(testingidssense3)) + " examples of sense 3")


    vectorizer = CountVectorizer(lowercase=True, stop_words='english', max_features=None)
    X_train = vectorizer.fit_transform(data_train)

    clf = MultinomialNB()
    clf.fit(X_train, y_train)

    X_test = vectorizer.transform(data_test)
    y_pred = clf.predict(X_test)

    right = 0
    total = 0
    sense1right = 0
    sense2right = 0
    sense3right = 0

    for word in range(0, len(y_pred)):
        if (y_pred[word] == y_test[word]):
            right += 1
            if y_pred[word] == 190901:
                sense1right += 1
            if y_pred[word] == 190902:
                sense2right += 1
            if y_pred[word] == 190903:
                sense3right += 1
        total += 1
    print(str((right/total)*100) + ' percent were calculated correct overall')
    print('Sense 1 was calculated correctly ' + str((sense1right/right)*100) + ' percent of the time')
    print('Sense 2 was calculated correctly ' + str((sense2right/right)*100) + ' percent of the time')
    print('Sense 3 was calculated correctly ' + str((sense3right/right)*100) + ' percent of the time')



if __name__ == '__main__':
   main()
