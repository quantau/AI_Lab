import numpy as np

class Node:
  """A node in a decision tree."""

  def __init__(self, feature, split_value, left_child, right_child, class_label=None):
    self.feature = feature
    self.split_value = split_value
    self.left_child = left_child
    self.right_child = right_child
    self.class_label = class_label

  def predict(self, data_point):
    """Predicts the class label for the given data point."""

    if self.class_label != -1:
        return self.class_label

    if data_point[self.feature] < self.split_value:
      return self.left_child.predict(data_point)
    else:
      return self.right_child.predict(data_point)
    
  def print_tree(self):

    if self.class_label != -1:
        print(self.class_label)
        return
    
    self.left_child.print_tree()
    self.right_child.print_tree()

    
class DecisionTree:
    """A decision tree classifier."""

    def __init__(self, max_depth=1000, min_samples_split=2, min_samples_leaf=1):
        self.root_node = None
        self.max_depth = max_depth
        self.min_samples_split = min_samples_split
        self.min_samples_leaf = min_samples_leaf

    def fit(self, X, y):
        """Trains the decision tree on the given data."""
        self.classes = np.unique(y)
        self.root_node = self._build_tree(X, y, 0)

    def _build_tree(self, X, y, depth):
        """Recursively builds the decision tree."""

        if depth >= self.max_depth or len(X) < self.min_samples_split:
            # Create a leaf node.
            class_label = np.argmax(np.bincount(y))
            return Node(feature=None, split_value=None, left_child=None, right_child=None, class_label=class_label)
        
        best_feature = None
        best_gini_index = 1.0
        split_value = 0
        for feature in range(X.shape[1]):
            feature_split_value, gini_index = self.find_split(X, y, feature)

            if gini_index < best_gini_index:
                best_feature = feature
                best_gini_index = gini_index
                split_value = feature_split_value

        if best_feature is None:
            class_label = np.argmax(np.bincount(y))
            return Node(feature=None, split_value=None, left_child=None, right_child=None, class_label=class_label)
        # Split the data on the best feature.
        left_child = self._build_tree(X[X[:, best_feature] < split_value], y[X[:, best_feature] < split_value], depth + 1)
        right_child = self._build_tree(X[X[:, best_feature] >= split_value], y[X[:, best_feature] >= split_value], depth + 1)

        return Node(feature=best_feature, split_value=split_value, left_child=left_child, right_child=right_child, class_label= -1)

    def find_split(self, X, y, feature):
        unique_values = np.unique(X[:, feature])
        values = X[:, feature]

        minimum_gini = 1.0
        split_value = -1
        for pos in range(len(unique_values) - 1):
            value = (unique_values[pos] + unique_values[pos+1]) / 2

            left_mask = []
            right_mask = []
            for i in range(len(values)):
                if value < values[i]:
                    left_mask.append(y[i])
                else:
                    right_mask.append(y[i])

            if len(left_mask) > 0 and len(right_mask) > 0:
                gini = self.gini_index(left_mask, right_mask)
                if minimum_gini > gini:
                    minimum_gini = gini
                    split_value = value

        return split_value, minimum_gini
                    
    def gini_index(self, left, right):
        total = len(left) + len(right)
        p_left = len(left) / total
        p_right = len(right) / total

        gini_left = 1.0
        for i in self.classes:
            gini_left = gini_left - (left.count(i) / len(left))**2

        gini_right = 1.0
        for i in self.classes:
            gini_right = gini_right - (right.count(i) / len(right))**2
            
        gini = p_left * gini_left + p_right * gini_right

        return gini
    
    def predict(self, X):
        """Predicts the class labels for the given data."""

        predictions = []
        for data_point in X:
            prediction = self.root_node.predict(data_point)
            predictions.append(prediction)
        return predictions
    def print_tree(self):
        self.root_node.print_tree()
    
# Example usage
if __name__ == '__main__':
    # Create a toy dataset
    X = np.array([[3, 2], [5, 5], [6, 9], [4, 4], [1, 8], [2, 1]])  # Data with features
    y = np.array([0, 1, 1, 0, 2, 2])  # Multi-class classification with 3 classes

    # Create and train the multi-class decision tree
    tree = DecisionTree()
    tree.fit(X, y)
    
    # Make predictions
    new_data = np.array([[3, 5], [7, 2], [0, 1]])
    predictions = tree.predict(new_data)
    print(predictions)
