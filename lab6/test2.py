import pickle
from sklearn.metrics import accuracy_score
import numpy as np
from main2 import DecisionTreeClassifierWithTextFeatures

from sklearn.metrics import ConfusionMatrixDisplay, confusion_matrix

import matplotlib.pyplot as plt

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


with open('save_classifier.model_saved', 'rb') as f:
    tree_classifier = pickle.load(f)

# Make predictions on test data
predictions = tree_classifier.predict(X_test)

print(predictions)

# Map predicted labels back to text
# predicted_labels = [int_to_class[i] for i in predictions]
# predicted_labels = np.array(predicted_labels)
# print(predicted_labels)

# Calculate accuracy
accuracy = accuracy_score(y_test, predictions)
print(f"Accuracy: {accuracy * 100:.2f}%")

ConfusionMatrixDisplay(confusion_matrix(y_test, predictions)).plot()
plt.show()
