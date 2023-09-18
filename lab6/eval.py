from main2 import DecisionTreeClassifierWithTextFeatures
from sklearn.metrics import (
    confusion_matrix,
    accuracy_score,
    precision_score,
    recall_score,
    f1_score,
    classification_report
)
import numpy as np

class_to_int = {
    'ABBR': 0,
    'DESC': 1,
    'ENTY': 2,
    'HUM': 3,
    'LOC': 4,
    'NUM': 5
}

int_to_class = sorted(class_to_int.keys())

# Function to preprocess and map words to integers
def preprocess_data(data):
    sentences = []
    labels = []

    for line in data:
        label, _, sentence = line.partition(' ')
        labels.append(class_to_int[label])
        sentences.append(sentence)

    return sentences, np.array(labels)

# Load training and test data
train_data = open('parsed_train.txt', 'r').readlines()
test_data = open('parsed_test.txt', 'r').readlines()

# Preprocess data and map words to integers
X_train, y_train = preprocess_data(train_data)
X_test, y_test = preprocess_data(test_data)

# Create an instance of the DecisionTreeClassifierWithTextFeatures class
tree_classifier = DecisionTreeClassifierWithTextFeatures(
    include_unigrams=True,
    include_bigrams=True,
    include_trigrams=True,
    include_sentence_length=True,
    include_pos_features=True,
    max_depth=None
)

# Fit the decision tree classifier on training data
tree_classifier.fit(X_train, y_train)

import pickle
with open('save_classifier.model_saved', 'wb') as f:
    pickle.dump(tree_classifier, f)

# Make predictions on test data
predictions = tree_classifier.predict(X_test)
print(predictions)

# Map predicted labels back to text
predicted_labels = [int_to_class[i] for i in predictions]
predicted_labels = np.array(predicted_labels)
print(predictions)

print(classification_report(y_test, predictions, target_names=int_to_class))

