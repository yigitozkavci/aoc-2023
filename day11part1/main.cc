#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <numeric>
#include <queue>

using namespace std;

int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;

  unordered_set<int> expanded_rows, expanded_cols;
  vector<pair<int, int>> galaxies;

  int y = 0;
  int col_size = 0;
  while (getline(input_file, input_line))
  {
    col_size = input_line.size();
    for (int x = 0; x < input_line.size(); x++)
    {
      if (input_line.at(x) == '#')
      {
        galaxies.push_back({x, y});
      }
    }
    y++;
  };
  for (int i = 0; i < y; i++)
  {
    expanded_rows.insert(i);
  }
  for (int i = 0; i < col_size; i++)
  {
    expanded_cols.insert(i);
  }
  for (auto [x, y] : galaxies)
  {
    expanded_rows.erase({y});
    expanded_cols.erase({x});
  }

  long result = 0;
  int scale = 1;
  for (auto [x1, y1] : galaxies)
  {
    for (auto [x2, y2] : galaxies)
    {
      if (x1 == x2 && y1 == y2)
      {
        continue;
      }
      long distance = abs(x2 - x1) + abs(y2 - y1);
      int minx = min(x1, x2);
      int maxx = max(x1, x2);
      for (int i = minx + 1; i < maxx; i++)
      {
        if (expanded_cols.find(i) != expanded_cols.end())
        {
          distance += scale;
        }
      }
      int miny = min(y1, y2);
      int maxy = max(y1, y2);
      for (int i = miny + 1; i < maxy; i++)
      {
        if (expanded_rows.find(i) != expanded_rows.end())
        {
          distance += scale;
        }
      }
      result += distance;
    }
  }
  cout << result / 2 << endl;
  return 0;
}