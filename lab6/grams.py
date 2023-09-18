import re
import collections
import numpy as np

class TextVectorizer:
    def __init__(self, vocab_size_unigrams=500, vocab_size_bigrams=300, vocab_size_trigrams=200):
        self.vocab_size_unigrams = vocab_size_unigrams
        self.vocab_size_bigrams = vocab_size_bigrams
        self.vocab_size_trigrams = vocab_size_trigrams
        self.unigram_vocab = None
        self.bigram_vocab = None
        self.trigram_vocab = None
    
    def load_text(self, file_path):
        # Read the text file and tokenize it into sentences
        with open(file_path, 'r') as file:
            text = file.read().lower()  # Read and convert to lowercase for case insensitivity
        sentences = re.split(r'[.!?]', text)
        
        # Tokenize and count unigrams, bigrams, and trigrams
        unigrams = []
        bigrams = []
        trigrams = []
        for sentence in sentences:
            tokens = re.findall(r'\b\w+\b', sentence)  # Tokenize by words
            unigrams.extend(tokens)
            bigrams.extend(zip(tokens, tokens[1:]))
            trigrams.extend(zip(tokens, tokens[1:], tokens[2:]))
        
        # Calculate the most common unigrams, bigrams, and trigrams
        unigram_counts = collections.Counter(unigrams)
        bigram_counts = collections.Counter(bigrams)
        trigram_counts = collections.Counter(trigrams)
        
        self.unigram_vocab = [word for word, _ in unigram_counts.most_common(self.vocab_size_unigrams)]
        self.bigram_vocab = [bigram for bigram, _ in bigram_counts.most_common(self.vocab_size_bigrams)]
        self.trigram_vocab = [trigram for trigram, _ in trigram_counts.most_common(self.vocab_size_trigrams)]
    
    def vectorize_sentence(self, sentence, include_unigrams=True, include_bigrams=True, include_trigrams=True, include_sentence_length=False):
        # Initialize one-hot feature vectors for unigrams, bigrams, and trigrams
        feature_vectors = []
        
        if include_unigrams:
            unigram_vector = np.zeros(self.vocab_size_unigrams)
            feature_vectors.append(unigram_vector)
        
        if include_bigrams:
            bigram_vector = np.zeros(self.vocab_size_bigrams)
            feature_vectors.append(bigram_vector)
        
        if include_trigrams:
            trigram_vector = np.zeros(self.vocab_size_trigrams)
            feature_vectors.append(trigram_vector)
        
        # Tokenize the sentence
        tokens = re.findall(r'\b\w+\b', sentence.lower())
        
        # Create one-hot vectors for unigrams
        if include_unigrams:
            for token in tokens:
                if token in self.unigram_vocab:
                    unigram_vector[self.unigram_vocab.index(token)] = 1
        
        # Create one-hot vectors for bigrams
        if include_bigrams:
            for bigram in zip(tokens, tokens[1:]):
                if bigram in self.bigram_vocab:
                    bigram_vector[self.bigram_vocab.index(bigram)] = 1
        
        # Create one-hot vectors for trigrams
        if include_trigrams:
            for trigram in zip(tokens, tokens[1:], tokens[2:]):
                if trigram in self.trigram_vocab:
                    trigram_vector[self.trigram_vocab.index(trigram)] = 1
        
        # Add sentence length feature if requested
        if include_sentence_length:
            sentence_length = len(tokens)
            feature_vectors.append(np.array([sentence_length]))
        
        return np.concatenate(feature_vectors)

# Example usage:
if __name__ == "__main__":
    # Create an instance of TextVectorizer
    vectorizer = TextVectorizer()
    
    # Load text data to build the vocabulary
    vectorizer.load_text('sample_text.txt')
    
    # Test sentence to be vectorized
    input_sentence = "This is a sample sentence for testing."
    
    # Generate the one-hot feature vector for the sentence
    feature_vector = vectorizer.vectorize_sentence(input_sentence, include_unigrams=True, include_bigrams=True, include_trigrams=True, include_sentence_length=True)
    print(feature_vector)
