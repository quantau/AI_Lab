#include <bits/stdc++.h>
#define vvi vector<vector<int>>
#define pii pair<int, int>

using namespace std;

int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};
map<vvi, vvi> child_to_parent_mapping;

void initialization(vvi &matrix, vvi &final_state)
{
    final_state = {
        {1, 2, 3},
        {4, 0, 5},
        {6, 7, 8}};

    cout << "Enter the input matrix\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int curr_val;
            cin >> curr_val;
            matrix[i][j] = curr_val;
        }
    }
}

pii getZeroIndex(vvi currentState)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (currentState[i][j] == 0)
            {
                return {i, j};
            }
        }
    }
}

vector<vvi> getNextStateList(vvi currentState)
{
    pii zero_index = getZeroIndex(currentState);
    int x = zero_index.first;
    int y = zero_index.second;

    vector<vvi> nextStates;

    for (int i = 0; i < 4; i++)
    {
        int new_x = x + dx[i];
        int new_y = y + dy[i];
        if (new_x >= 0 && new_x <= 2 && new_y >= 0 && new_y <= 2)
        {
            vvi nextState = currentState;
            swap(nextState[x][y], nextState[new_x][new_y]);
            nextStates.push_back(nextState);
        }
    }
    return nextStates;
}

void print_matrix(vvi input)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << input[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

int displacedTileHeurestic(vvi &curr, vvi &target, bool account_empty_tile)
{
    int heurestic_value = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (!account_empty_tile && curr[i][j] == 0)
                continue;
            heurestic_value += curr[i][j] != target[i][j];
        }
    }
    return heurestic_value;
}

int manhattanDistanceHeurestic(vvi &curr, vvi &target, bool account_empty_tile)
{
    int manhattan_distance[9][2];
    for (int i = 0; i < 9; i++)
    {
        manhattan_distance[i][0] = 0;
        manhattan_distance[i][1] = 0;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            manhattan_distance[curr[i][j]][0] += i;
            manhattan_distance[curr[i][j]][1] += j;

            manhattan_distance[target[i][j]][0] -= i;
            manhattan_distance[target[i][j]][1] -= j;
        }
    }
    int heurestic_value = 0;
    for (int i = 1 - account_empty_tile; i <= 8; i++)
    {
        heurestic_value += abs(manhattan_distance[i][0]) + abs(manhattan_distance[i][1]);
    }
    return heurestic_value;
}

bool bfs(vvi &matrix, vvi &final_state, set<vvi> &close_list, int &total_states_visited, int &states_on_optimal_path)
{
    priority_queue<pair<int, vvi>> my_queue;
    my_queue.push({0, matrix});
    while (!my_queue.empty())
    {
        vvi currentState = my_queue.top().second;
        int states_on_path = -my_queue.top().first;
        total_states_visited++;
        close_list.insert(currentState);
        my_queue.pop();
        if (currentState == final_state)
        {
            states_on_optimal_path = states_on_path;
            return true;
        }
        vector<vvi> nextStateList = getNextStateList(currentState);
        for (auto nextState : nextStateList)
        {
            if (close_list.find(nextState) != close_list.end())
                continue;
            child_to_parent_mapping[nextState] = currentState;
            my_queue.push({-(states_on_path + 1), nextState});
        }
    }
    return false;
}

void output_message(bool search_successfull, int total_states_visited, int states_on_optimal_path, vector<vvi> optimalPath, set<vvi> close_list)
{
    cout << search_successfull << "\n";
    if (!search_successfull)
    {
        cout << "FAIL\n";
    }
    else
    {
        cout << "PASS\n";
        for (auto state : optimalPath)
        {
            print_matrix(state);
        }
    }
    cout << "Following is the close list:\n";
    for (auto state : close_list)
    {
        print_matrix(state);
    }
    cout << "States on optimal path: " << states_on_optimal_path << "\n";
    cout << "The total number of states explored: " << total_states_visited << "\n";
}

vector<vvi> getOptimalPath(vvi start_state, vvi final_state)
{
    vector<vvi> optimalPath;
    vvi curr_state = final_state;
    while (curr_state != start_state)
    {
        optimalPath.push_back(curr_state);
        curr_state = child_to_parent_mapping[curr_state];
    }
    optimalPath.push_back(start_state);
    reverse(optimalPath.begin(), optimalPath.end());
    return optimalPath;
}

int main()
{
    vvi final_state;
    vvi matrix(3, vector<int>(3, 0));
    initialization(matrix, final_state); // taking user input

    set<vvi> close_list;
    set<vvi> open_list; // did not make much use because it is same as the nodes in pririrty queue? you can add the nodes as you deem apropriate

    int total_states_visited = 0;
    int states_on_optimal_path = 0;

    bool success = bfs(matrix, final_state, close_list, total_states_visited, states_on_optimal_path); // main shit

    vector<vvi> optimalPath;
    if (success)
    {
        optimalPath = getOptimalPath(matrix, final_state);
    }

    output_message(success, total_states_visited, states_on_optimal_path, optimalPath, close_list);

    return 0;
}
