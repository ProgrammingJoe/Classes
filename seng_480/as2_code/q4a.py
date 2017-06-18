from __future__ import division
from sklearn.preprocessing import normalize
import numpy as np
from scipy import spatial

def main():
    with open('questions-words.txt', 'r') as f:
        words = f.read().replace('\n', ' ').split()
    with open('questions-words.txt', 'r') as f:
        comparisons = f.read().splitlines()
    with open('glove.6B.50d.txt', 'r') as f:
        vectors = f.read().splitlines()
    words = set(words)

    # Get the vectors that are included in the words
    usedvectors = []
    for vector in vectors:
        if vector.split()[0] in words:
            usedvectors.append(vector)

    # Create dictionary of all vectors used
    vectordict = {}
    for vector in usedvectors:
        vectordict[vector.split()[0]] = [float(i) for i in vector.split()[1:]]

    # Determine X_d
    fiveclosest = 0
    closest = 0
    comparisoncount = 0
    for comparison in comparisons:
        analogies = comparison.split()
        if(len(analogies) == 4 and vectordict.has_key(analogies[0]) and vectordict.has_key(analogies[1]) and vectordict.has_key(analogies[2])):
            test = np.array(vectordict[analogies[1]]) - np.array(vectordict[analogies[0]]) + np.array(vectordict[analogies[2]])
            fivebest = {}
            comparisoncount = comparisoncount + 1
            for key, value in vectordict.items():
                result = 1 - spatial.distance.cosine(value, test)
                if(len(fivebest) < 5):
                    fivebest[key] = result
                else:
                    smallest = min(fivebest, key=fivebest.get)
                    if(fivebest[smallest] < result):
                        del fivebest[smallest]
                        fivebest[key] = result
            greatest = max(fivebest, key=fivebest.get)
            if(analogies[3] == greatest):
                closest = closest + 1
            if(analogies[3] in fivebest.keys()):
                fiveclosest = fiveclosest + 1
    print("Five Closest Percentage: " + str(fiveclosest/comparisoncount))
    print("Closest Percentage: " + str(closest/comparisoncount))


if __name__ == '__main__':
   main()
