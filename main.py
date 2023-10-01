import matplotlib.pyplot as plt
import pandas as pd

def Linear_Regression(x, y) :
    # Calculate the mean of x and y
    mean_x = sum(x) / len(x)
    mean_y = sum(y) / len(y)

    # Calculate the slope (m) and intercept (b) of the linear regression line
    numerator = sum([(x[i] - mean_x) * (y[i] - mean_y) for i in range(len(x))])
    denominator = sum([(x[i] - mean_x) ** 2 for i in range(len(x))])
    slope = numerator / denominator
    intercept = mean_y - slope * mean_x
    return slope, intercept

def Plot(x, y, expected_y):

    # Plot the data and regression line
    plt.scatter(x, y, label='Data')
    plt.plot(x, expected_y, color='red', label='Regression Line')
    plt.xlabel('Daily')
    plt.ylabel('Sunday')
    plt.legend()
    plt.title('Linear Regression')
    plt.grid(True)

    plt.show()


def main():

    # Define the path to your TSV file
    tsv_file = 'newspaper_data.tsv'

    # Read the TSV file into a Pandas DataFrame
    df = pd.read_csv(tsv_file, sep='\t')

    x = df['Daily'].tolist()
    y = df['Sunday'].tolist()

    slope, intercept = Linear_Regression(x, y)

    test_data_file = 'newspaper_data_test.tsv'
    test_data = pd.read_csv(test_data_file, sep='\t')

    x_testdata = df['Daily'].tolist()
    x_newspapers = df['Newspaper'].tolist()
    expected_y = [slope * xi + intercept for xi in x_testdata]

    size = len(x_testdata)

    print("Slope", slope)
    print("Intercept", intercept)

    not_sell_list = []
    for i in range(size):
        if expected_y[i] <= 1.3*x_testdata[i]:
            not_sell_list.append(x_newspapers[i])
            
    print("Companies not to sell list: ", not_sell_list)
    Plot(x,y , expected_y)


if __name__ == "__main__":
    main()
