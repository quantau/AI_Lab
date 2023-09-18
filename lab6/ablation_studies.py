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

include_args = {
    'include_unigrams': True,
    'include_bigrams': True,
    'include_trigrams': True,
    'include_sentence_length': True,
    'include_pos_features': True
}

for key in include_args.keys():
    include_args[key] = False

    # Create an instance of the DecisionTreeClassifierWithTextFeatures class
    tree_classifier = DecisionTreeClassifierWithTextFeatures(**include_args)

    # Fit the decision tree classifier on training data
    tree_classifier.fit(X_train, y_train)

    # Make predictions on test data
    predictions = tree_classifier.predict(X_test)

    print(f"Classification report with {key} = False: ")
    print(classification_report(y_test, predictions,
        target_names=int_to_class))

    include_args[key] = True

