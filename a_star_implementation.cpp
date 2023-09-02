#include <bits/stdc++.h>
#define vvi vector<vector<int>>
#define pii pair<int, int>

using namespace std;

int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};

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

bool a_star(vvi &matrix, vvi &final_state, int (*h)(vvi &start, vvi &tar),
            set<vvi> &close_list, int &total_states_visited, int &states_on_optimal_path,
            map<vvi, vvi> &child_to_parent_mapping, float &time_taken) {
  auto t_start = std::chrono::high_resolution_clock::now();

  priority_queue<pair<int, vvi>, std::vector<pair<int, vvi> >,
                 std::greater<pair<int, vvi> > > open_list; // acts as open list

  std::map<vvi, int> cost_so_far;

  open_list.push({h(matrix, final_state), matrix});
  cost_so_far[matrix] = 0;

  while(!open_list.empty()) {
    auto cur = open_list.top().second;
    close_list.insert(cur);
    total_states_visited++;
    open_list.pop();

    if(cur == final_state) {
      auto t_end = std::chrono::high_resolution_clock::now();
      time_taken = std::chrono::duration<float, std::milli>(t_end-t_start).count();
      return true;
    }

    auto nextStatesList = getNextStateList(cur);
    for(auto next : nextStatesList) {
      int new_cost = cost_so_far[cur] + 1;
      if(cost_so_far.find(next) == cost_so_far.end()
         || new_cost < cost_so_far[next]) {
        cost_so_far[next] = new_cost;
        int priority = new_cost + h(next, final_state);
        open_list.push({priority, next});
        child_to_parent_mapping[next] = cur;
      }
    }
  }

  auto t_end = std::chrono::high_resolution_clock::now();

  time_taken = std::chrono::duration<float, std::milli>(t_end-t_start).count();

  return false;
}
            
/*
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
*/

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
            map<vvi, vvi> &child_to_parent_mapping) {
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

int h1(vvi &mat, vvi &tar) {
  return 0;
}

int h2(vvi &mat, vvi &tar) {
  return displacedTileHeuristic(mat, tar, 0);
}

int h3(vvi &mat, vvi &tar) {
  return manhattanDistanceHeuristic(mat, tar, 0);
}

int h4(vvi &mat, vvi &tar) {
  srand(234234);
  srand(displacedTileHeuristic(mat, tar, 0));
  return rand()%27;
}

int main()
{
    vvi final_state;
    vvi matrix(3, vector<int>(3, 0));
    initialization(matrix, final_state); // taking user input

    set<vvi> close_list;

    int total_states_visited = 0;
    int states_on_optimal_path = 0;
    float time_taken;

    map<vvi, vvi> child_to_parent_mapping;
    bool success = a_star(matrix, final_state, &h1, close_list, total_states_visited, states_on_optimal_path, child_to_parent_mapping, time_taken);


    vector<vvi> optimalPath;
    if (success)
    {
        optimalPath = getOptimalPath(matrix, final_state, child_to_parent_mapping);
        states_on_optimal_path = optimalPath.size();
    }

    std::cout << "For h1: \n";
    output_message(success, total_states_visited, states_on_optimal_path, optimalPath, time_taken);

    std::cout << "For h2: \n";

    total_states_visited = 0;
    close_list.clear();
    child_to_parent_mapping.clear();
    optimalPath.clear();
    success = a_star(matrix, final_state, &h2, close_list, total_states_visited, states_on_optimal_path, child_to_parent_mapping, time_taken);
    if(success) {
        optimalPath = getOptimalPath(matrix, final_state, child_to_parent_mapping);
        states_on_optimal_path = optimalPath.size();
    }
    output_message(success, total_states_visited, states_on_optimal_path, optimalPath, time_taken);

    std::cout << "For h3: \n";

    total_states_visited = 0;
    close_list.clear();
    child_to_parent_mapping.clear();
    optimalPath.clear();
    success = a_star(matrix, final_state, &h3, close_list, total_states_visited, states_on_optimal_path, child_to_parent_mapping, time_taken);
    if(success) {
        optimalPath = getOptimalPath(matrix, final_state, child_to_parent_mapping);
        states_on_optimal_path = optimalPath.size();
    }
    output_message(success, total_states_visited, states_on_optimal_path, optimalPath, time_taken);

    std::cout << "For h4: \n";

    total_states_visited = 0;
    close_list.clear();
    child_to_parent_mapping.clear();
    optimalPath.clear();
    success = a_star(matrix, final_state, &h4, close_list, total_states_visited, states_on_optimal_path, child_to_parent_mapping, time_taken);
    if(success) {
        optimalPath = getOptimalPath(matrix, final_state, child_to_parent_mapping);
        states_on_optimal_path = optimalPath.size();
    }
    output_message(success, total_states_visited, states_on_optimal_path, optimalPath, time_taken);

    return 0;
}
