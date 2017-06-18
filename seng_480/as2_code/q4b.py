from scipy import spatial

def main():
    with open('SimLex-999.txt', 'r') as f:
        wordpairlines = f.read().splitlines()
    with open('glove.6B.50d.txt', 'r') as f:
        vectors = f.read().splitlines()

    words = []
    for line in wordpairlines:
        words.append(line.split()[0])
        words.append(line.split()[1])
    words = set(words)

    usedvectors = []
    for vector in vectors:
        if vector.split()[0] in words:
            usedvectors.append(vector)

    vectordict = {}
    for vector in usedvectors:
        vectordict[vector.split()[0]] = [float(i) for i in vector.split()[1:]]

    for line in wordpairlines:
        word1 = line.split()[0]
        word2 = line.split()[1]
        spearman = line.split()[3]
        if(vectordict.has_key(word1) and vectordict.has_key(word2)):
            result = 1 - spatial.distance.cosine(vectordict[word1], vectordict[word2])
            print(str(result))
            # print(str(spearman))

if __name__ == '__main__':
   main()
