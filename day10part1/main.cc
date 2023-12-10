#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <numeric>
#include <queue>

using namespace std;

void insert(queue<tuple<int, unsigned int, pair<int, int>, pair<int, int>>> &q, int dir, int steps, pair<int, int> curr, pair<int, int> from, pair<int, int> to)
{
  if (from.first != to.first || from.second != to.second)
  {
    q.push({dir, steps + 1, curr, to});
  }
}

int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;

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
    switch (symbol)
    {
    case '|':
      insert(q, dir, steps, {x, y}, from, {x, y - 1});
      insert(q, dir, steps, {x, y}, from, {x, y + 1});
      break;
    case '-':
      insert(q, dir, steps, {x, y}, from, {x + 1, y});
      insert(q, dir, steps, {x, y}, from, {x - 1, y});
      break;
    case 'L':
      insert(q, dir, steps, {x, y}, from, {x, y - 1});
      insert(q, dir, steps, {x, y}, from, {x + 1, y});
      break;
    case 'J':
      insert(q, dir, steps, {x, y}, from, {x, y - 1});
      insert(q, dir, steps, {x, y}, from, {x - 1, y});
      break;
    case '7':
      insert(q, dir, steps, {x, y}, from, {x, y + 1});
      insert(q, dir, steps, {x, y}, from, {x - 1, y});
      break;
    case 'F':
      insert(q, dir, steps, {x, y}, from, {x, y + 1});
      insert(q, dir, steps, {x, y}, from, {x + 1, y});
      break;
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
      break;
    }
  }

  unordered_set<int> result_steps;
  for (auto [k, v] : max_steps)
  {
    if (result_steps.find(v) != result_steps.end())
    {
      // Tied the knot
      cout << "Result: " << v / 2 << endl;
    }
    else
    {
      result_steps.insert(v);
    }
  }
  return 0;
}