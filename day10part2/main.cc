#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <numeric>
#include <queue>

using namespace std;

struct hashFunction
{
  size_t operator()(const pair<int,
                               int> &x) const
  {
    return x.first ^ x.second;
  }
};

void insert(queue<tuple<int, unsigned int, pair<int, int>, pair<int, int>>> &q, int dir, unordered_map<int, unordered_set<pair<int, int>, hashFunction>> &loops, int steps, pair<int, int> curr, pair<int, int> from, pair<int, int> to)
{
  loops[dir].insert({curr.first, curr.second});
  if (from.first != to.first || from.second != to.second)
  {
    q.push({dir, steps + 1, curr, to});
  }
}

void print_matrix(const vector<string> &matrix, const unordered_set<pair<int, int>, hashFunction> &loop)
{
  for (int y = 0; y < matrix.size(); y++)
  {
    const string &s = matrix.at(y);
    for (int x = 0; x < s.size(); x++)
    {
      if (loop.find({x, y}) == loop.end())
      {
        cout << s.at(x);
      }
      else
      {
        switch (s.at(x))
        {
        case 'L':
          cout << "╚";
          break;
        case 'J':
          cout << "╝";
          break;
        case '7':
          cout << "╗";
          break;
        case 'F':
          cout << "╔";
          break;
        case '|':
          cout << "║";
          break;
        case '-':
          cout << "═";
          break;
        default:
          cout << s.at(x);
          break;
        }
      }
    }
    cout << endl;
  }
}

void solve_for(const unordered_set<pair<int, int>, hashFunction> &loop, vector<string> &matrix)
{
  print_matrix(matrix, loop);
  int result = 0;
  for (int y = 0; y < matrix.size(); y++)
  {
    bool increasing = false;
    int loop_dir = -1;
    for (int x = 0; x < matrix.at(0).size(); x++)
    {
      if (loop.find({x, y}) == loop.end())
      {
        // Not in the loop
        if (increasing)
        {
          result++;
        }
      }
      else
      {
        // In the looop
        char ty = matrix.at(y).at(x);
        if (ty == '|')
        {
          increasing = !increasing;
        }
        else if (ty == 'L')
        {               // North-east
          loop_dir = 0; // Pipe connects north
        }
        else if (ty == 'F')
        {               // South-east
          loop_dir = 1; // Pipe connects south
        }
        else if (ty == 'J')
        { // North-west
          if (loop_dir == 1)
          {
            increasing = !increasing;
          }
          loop_dir = -1;
        }
        else if (ty == '7')
        { // South-west
          if (loop_dir == 0)
          {
            increasing = !increasing;
          }
          loop_dir = -1;
        }
      }
    }
  }
  cout << "Result" << result << endl;
}

int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;
  unordered_map<int, unordered_set<pair<int, int>, hashFunction>> loops;
  int result = 0;
  int y = 0;
  pair<int, int> start_pos = {-1, -1};
  vector<string> matrix;
  while (getline(input_file, input_line))
  {
    for (int i = 0; i < input_line.size(); i++)
    {
      if (input_line.at(i) == 'S')
      {
        start_pos = {i, matrix.size()};
      }
    }
    matrix.push_back(input_line);
  };
  cout << endl;
  queue<tuple<int, unsigned int, pair<int, int>, pair<int, int>>> q;
  q.push({-1, 0, start_pos, start_pos});
  unordered_map<int, unsigned int> max_steps;

  while (!q.empty())
  {
    auto q_result = q.front();
    int dir = get<0>(q_result);
    unsigned int steps = get<1>(q_result);
    auto from = get<2>(q_result);
    auto [x, y] = get<3>(q_result);
    q.pop();
    if (x >= matrix.at(0).size() || x < 0 || y >= matrix.size() || y < 0)
    {
      // Out of bounds
      continue;
    }
    max_steps[dir] = max(max_steps[dir], steps);
    char symbol = matrix.at(y).at(x);
    unordered_map<char, vector<pair<int, int>>> offsets = {
        {'|', {{0, -1}, {0, 1}}},
        {'-', {{1, 0}, {-1, 0}}},
        {'L', {{0, -1}, {1, 0}}},
        {'J', {{0, -1}, {-1, 0}}},
        {'7', {{0, 1}, {-1, 0}}},
        {'F', {{0, 1}, {1, 0}}}};
    switch (symbol)
    {
    case '.':
      break;
    case 'S':
      if (from.first == x && from.second == y)
      {
        q.push({0, steps + 1, {x, y}, {x + 1, y}});
        q.push({1, steps + 1, {x, y}, {x - 1, y}});
        q.push({2, steps + 1, {x, y}, {x, y + 1}});
        q.push({3, steps + 1, {x, y}, {x, y - 1}});
      }
      for (int i = 0; i < 4; i++)
      {
        loops[i].insert({x, y});
      }
      break;
    default:
      for (auto [offset_x, offset_y] : offsets[symbol])
      {
        insert(q, dir, loops, steps, {x, y}, from, {x + offset_x, y + offset_y});
      }
      break;
    }
  }

  unordered_set<int> result_steps;
  for (auto [k, v] : max_steps)
  {
    if (result_steps.find(v) != result_steps.end() && v != 1)
    {
      // Tied the knot
      solve_for(loops.at(k), matrix);
      break;
    }
    else
    {
      result_steps.insert(v);
    }
  }
  return 0;
}