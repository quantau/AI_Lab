import pandas as pd
import numpy as np

max_val = 30
def hypothesis(theta, X):
    dp = np.dot(theta, X.T)
    dp = np.clip(dp, a_min=-max_val, a_max=max_val)
    return 1 / (1 + np.exp(-dp)) - 0.0000001

def cost(X, y, theta):
    y1 = hypothesis(X, theta)
    return -(1/len(X)) * np.sum(y*np.log(y1) + (1-y)*np.log(1-y1))

def gradient_descent(X, y, theta, alpha, epochs):
    m = len(X)
    for i in range(0, epochs):
        for j in range(0, 10):
            theta = pd.DataFrame(theta)
            h = hypothesis(theta.iloc[:,j], X)
            for k in range(0, theta.shape[0]):
                theta.iloc[k, j] -= (alpha/m) * np.sum((h-y.iloc[:, j])*X.iloc[:, k])
            theta = pd.DataFrame(theta)

    print("\n")
    return theta

def test(theta):
    test_file = "mnist_test.csv"
    df = pd.read_csv(test_file)
    y = df['label']
    X = df.drop(columns=['label'])
    X.insert(0, '1x0', 1)
    y1 = np.zeros([X.shape[0], len(y.unique())])
    y1 = pd.DataFrame(y1)
    for i in range(0, len(y1)):
        y1.iloc[i, y.iloc[i]] = 1
    output = []
    for i in range(0, 10):
        theta1 = pd.DataFrame(theta)
        h = hypothesis(theta1.iloc[:,i], X)
        output.append(h)
    output=pd.DataFrame(output)

    output = np.array(output)
    output = output.T

    accuracy = 0
    cf_matrix = np.zeros((10, 10), dtype = int)
    for row in range(len(y1)):
        pred = 0
        real_value = 0
        for col in range(10):
            if y1.iloc[row, col] == 1:
                real_value = col
            if output[row, col] > output[pred, col]:
                pred=col
        cf_matrix[real_value, pred] += 1
        if real_value == pred:
            accuracy+=1
    print("Confusion matrix:")
    print(cf_matrix)
    print("\n")
    accuracy = accuracy/len(X)
    print("Accuracy: ",accuracy*100,"%\n")

def main():
    train_file = "mnist_train.csv"
    df = pd.read_csv(train_file)
    y = df['label']
    X = df.drop(columns=['label'])
    X.insert(0, '1x0', 1)
    y1 = np.zeros([X.shape[0], len(y.unique())])
    y1 = pd.DataFrame(y1)
    for i in range(0, len(y1)):
        y1.iloc[i, y.iloc[i]] = 1

    theta = np.zeros([X.shape[1], y1.shape[1]])
    theta = gradient_descent(X, y1, theta, 0.08, 1500)
    test(theta)
    print("Theta: ")
    print(theta)
    file_path = 'theta.csv'
    theta.to_csv(file_path, index=False)

if __name__=="__main__":
    main()

