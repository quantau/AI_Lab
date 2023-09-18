from classification import DecisionTree
from grams import TextVectorizer
from pos_tagger import POSFeatureExtractor 
import numpy as np

class DecisionTreeClassifierWithTextFeatures:
    def __init__(self, include_unigrams=True, include_bigrams=True,
            include_trigrams=True, include_sentence_length=True,
            include_pos_features=True, max_depth=None):
        self.include_unigrams = include_unigrams
        self.include_bigrams = include_bigrams
        self.include_trigrams = include_trigrams
        self.include_sentence_length = include_sentence_length
        self.include_pos_features = include_pos_features
        self.vectorizer = TextVectorizer()
        self.vectorizer.load_text('vocab.txt')
        self.pos_extractor = POSFeatureExtractor()
        self.max_depth = max_depth
        self.tree = DecisionTree()  # Initialize your custom DecisionTree here

    def _extract_text_features(self, X):
        # Extract text features using TextVectorizer
        feature_vectors = []
        for sentence in X:
            feature_vector = self.vectorizer.vectorize_sentence(
                sentence,
                include_unigrams=self.include_unigrams,
                include_bigrams=self.include_bigrams,
                include_trigrams=self.include_trigrams,
                include_sentence_length=self.include_sentence_length
            )
            
            if self.include_pos_features:
                pos_vector = self.pos_extractor.pos_feature_vector(sentence)
                feature_vector = np.concatenate((feature_vector, pos_vector))
            
            feature_vectors.append(feature_vector)
        return np.array(feature_vectors)

    def fit(self, X, y):
        # Initialize the TextVectorizer
        vectorizer = TextVectorizer()

        # Extract text features from training data
        X_text_features = self._extract_text_features(X)

        # Fit your custom DecisionTree on text features and class labels
        self.tree.fit(X_text_features, y)

    def predict(self, X):
        # Extract text features from input data
        X_text_features = self._extract_text_features(X)

        # Make predictions using your custom DecisionTree
        return self.tree.predict(X_text_features)

# Example usage:
if __name__ == "__main__":
    # Create an instance of the DecisionTreeClassifierWithTextFeatures class
    tree_classifier = DecisionTreeClassifierWithTextFeatures(
        include_unigrams=True,
        include_bigrams=True,
        include_trigrams=True,
        include_sentence_length=True,
        max_depth=None
    )

    # Training data in the form (sentence, class)
    training_data = [
        ("This is a positive sentence.", 1),
        ("This is a negative sentence.", 0),
        ("Another positive example.", 1),
        ("Another negative example.", 0)
    ]

    # Separate training data into sentences and labels
    sentences = [data[0] for data in training_data]
    labels = [data[1] for data in training_data]

    # Fit the decision tree classifier on training data
    tree_classifier.fit(sentences, labels)

    # Test sentences for prediction
    test_sentences = ["This is a test sentence.", "Another test sentence."]
    
    # Make predictions on test sentences
    predictions = tree_classifier.predict(test_sentences)
    
    # Display the predictions
    for sentence, prediction in zip(test_sentences, predictions):
        print(f"Sentence: {sentence}, Predicted Class: {prediction}")
