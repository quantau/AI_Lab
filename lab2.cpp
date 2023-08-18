#include <bits/stdc++.h>

using namespace std;

int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};

class Grid
{
public:
    Grid(vector<vector<int>> inputGrid)
    {
        currentState = inputGrid;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (currentState[i][j] == 0)
                {
                    zero_index = {i, j};
                }
            }
        }
    }

    pair<int, int> getZeroIndex()
    {
        return zero_index;
    }

    vector<vector<int>> getCurrentState()
    {
        return currentState;
    }

    vector<Grid> getNextStateList()
    {
        int x = zero_index.first;
        int y = zero_index.second;

        vector<Grid> nextStates;

        for (int i = 0; i < 4; i++)
        {
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if (new_x >= 0 && new_x <= 2 && new_y >= 0 && new_y <= 2)
            {
                vector<vector<int>> nextState = currentState;
                swap(nextState[x][y], nextState[new_x][new_y]);
                nextStates.push_back(nextState);
            }
        }
        return nextStates;
    }

private:
    vector<vector<int>> currentState;
    pair<int, int> zero_index;
};

int main()
{

    vector<vector<int>> matrix(3, vector<int>(3, 0));

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int curr_val;
            cin >> curr_val;
            matrix[i][j] = curr_val;
        }
    }

    Grid myGrid(matrix);
    vector<Grid> nextStates = myGrid.getNextStateList();
    for (auto presentState : nextStates)
    {
        vector<vector<int>> presentStateGrid = presentState.getCurrentState();
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                cout << presentStateGrid[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
    return 0;
}