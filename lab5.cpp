#include <bits/stdc++.h>
#define vvi vector<vector<int>>
#define pii pair<int, int>

using namespace std;

int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};

void initialization(vvi &start_state, vvi &final_state)
{

    cout << "Enter the start state\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int curr_val;
            cin >> curr_val;
            start_state[i][j] = curr_val;
        }
    }

    cout << "Enter the final state\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int curr_val;
            cin >> curr_val;
            final_state[i][j] = curr_val;
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

void print_state(vvi input)
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

bool simulated_annealing(vvi &start_state, vvi &final_state, int (*h)(vvi &start, vvi &tar),
                         int &total_states_visited, int &states_on_optimal_path,
                         map<vvi, vvi> &child_to_parent_mapping, float &time_taken)
{
    auto t_start = std::chrono::high_resolution_clock::now();

    double temperature = 1;
    vvi curr_state = start_state;
    int prev_cost = h(final_state, curr_state);
    total_states_visited++;
    int loop = 100;
    while (loop-- && curr_state != final_state)
    {
        // cout << loop << "\n";
        // print_state(curr_state);
        vector<vvi> nextStateList = getNextStateList(curr_state);

        int potentialNextStateIndex = rand() % (nextStateList.size());
        vvi potentialNextState = nextStateList[potentialNextStateIndex];
        // print_state(potentialNextState);
        int curr_cost = h(final_state, potentialNextState);
        if (curr_cost <= prev_cost)
        {
            child_to_parent_mapping[potentialNextState] = curr_state;
            curr_state = potentialNextState;
            prev_cost = curr_cost;
            total_states_visited++;
        }
        else
        {
            double exploration_probability = exp((prev_cost - curr_cost) / temperature);
            exploration_probability *= 100;
            // cout << "exploration probability: " << exploration_probability << "\n";
            if (rand() % 100 <= (int)exploration_probability)
            {
                child_to_parent_mapping[potentialNextState] = curr_state;
                curr_state = potentialNextState;
                prev_cost = curr_cost;
                total_states_visited++;
            }
            temperature *= 0.99;
        }
    }

    auto t_end = std::chrono::high_resolution_clock::now();

    time_taken = std::chrono::duration<float, std::milli>(t_end - t_start).count();

    return curr_state == final_state;
}

void output_message(bool search_successfull, int total_states_visited, int states_on_optimal_path, vector<vvi> optimalPath, float time_taken)
{
    if (!search_successfull)
    {
        cout << "FAIL\n";
    }
    else
    {
        cout << "PASS\n";
        for (auto state : optimalPath)
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    cout << state[i][j] << " ";
                }
                cout << "\n";
            }
            cout << "\n";
        }
        cout << "States on optimal path: " << states_on_optimal_path << "\n";
        cout << "Time taken (ms): " << time_taken << "\n";
    }

    cout << "The total number of states explored: " << total_states_visited << "\n";
}

vector<vvi> getOptimalPath(vvi start_state, vvi final_state,
                           map<vvi, vvi> &child_to_parent_mapping)
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

int displacedTileHeuristic(vvi &curr, vvi &target, bool account_empty_tile)
{
    int heuristic_value = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (!account_empty_tile && curr[i][j] == 0)
                continue;
            heuristic_value += curr[i][j] != target[i][j];
        }
    }
    return heuristic_value;
}

int manhattanDistanceHeuristic(vvi &curr, vvi &target, bool account_empty_tile)
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
    int heuristic_value = 0;
    for (int i = 1 - account_empty_tile; i <= 8; i++)
    {
        heuristic_value += abs(manhattan_distance[i][0]) + abs(manhattan_distance[i][1]);
    }
    return heuristic_value;
}

int h1(vvi &mat, vvi &tar)
{
    return displacedTileHeuristic(mat, tar, 0);
}

int h2(vvi &mat, vvi &tar)
{
    return manhattanDistanceHeuristic(mat, tar, 0);
}

int main()
{
    srand(time(0));
    vvi final_state(3, vector<int>(3, 0));
    vvi start_state(3, vector<int>(3, 0));
    initialization(start_state, final_state); // taking user input

    int total_states_visited = 0;
    int states_on_optimal_path = 0;
    float time_taken;

    map<vvi, vvi> child_to_parent_mapping;
    vector<vvi> optimalPath;
    bool success = false;

    std::cout << "For h1: \n";

    total_states_visited = 0;
    child_to_parent_mapping.clear();
    optimalPath.clear();
    success = simulated_annealing(start_state, final_state, &h1, total_states_visited, states_on_optimal_path, child_to_parent_mapping, time_taken);
    if (success)
    {
        optimalPath = getOptimalPath(start_state, final_state, child_to_parent_mapping);
        states_on_optimal_path = optimalPath.size();
    }
    output_message(success, total_states_visited, states_on_optimal_path, optimalPath, time_taken);

    std::cout << "For h2: \n";

    total_states_visited = 0;
    child_to_parent_mapping.clear();
    optimalPath.clear();
    success = simulated_annealing(start_state, final_state, &h2, total_states_visited, states_on_optimal_path, child_to_parent_mapping, time_taken);
    if (success)
    {
        optimalPath = getOptimalPath(start_state, final_state, child_to_parent_mapping);
        states_on_optimal_path = optimalPath.size();
    }
    output_message(success, total_states_visited, states_on_optimal_path, optimalPath, time_taken);

    return 0;
}
