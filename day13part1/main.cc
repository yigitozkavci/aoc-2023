#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <numeric>
#include <stack>

using namespace std;

std::vector<std::string> split(std::string s, std::string delimiter)
{
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
  {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}

bool test_symmetry(const string &line, int col)
{
  for (int i = col; i < line.size(); i++)
  {
    int reflection_index = 2 * col - i - 1;
    if (reflection_index < 0)
    {
      break;
    }
    if (line.at(i) != line.at(reflection_index))
    {
      return false;
    }
  }
  return true;
}

bool test_h_sym(const vector<string> &input, int col, int row)
{
  // Go through input
  for (int i = row; i < input.size(); i++)
  {
    int reflection_index = 2 * row - i - 1;
    if (reflection_index < 0)
    {
      break;
    }
    if (input.at(i).at(col) != input.at(reflection_index).at(col))
    {
      return false;
    }
  }
  return true;
}
int horizontal_mirror(const vector<string> &input)
{
  unordered_set<int> candidate_rows;
  for (int row = 1; row < input.size(); row++)
  {
    candidate_rows.insert(row);
  }

  for (int col = 0; col < input.at(0).size(); col++)
  {
    unordered_set<int> rows_to_discard;
    for (const int row : candidate_rows)
    {
      if (!test_h_sym(input, col, row))
      {
        rows_to_discard.insert(row);
      }
    }

    for (const int row : rows_to_discard)
    {
      candidate_rows.erase(row);
    }
  }
  if (candidate_rows.size() == 1)
  {
    return *candidate_rows.begin();
  }
  else
  {
    return 0;
  }
}

int vertical_mirror(const vector<string> &input)
{
  unordered_set<int> candidate_cols;
  for (int col = 1; col < input.at(0).size(); col++)
  {
    candidate_cols.insert(col);
  }
  for (const string &input_line : input)
  {
    unordered_set<int> cols_to_discard;
    for (const int col : candidate_cols)
    {
      if (!test_symmetry(input_line, col))
      {
        cols_to_discard.insert(col);
      }
    }
    for (const int col : cols_to_discard)
    {
      candidate_cols.erase(col);
    }
  }
  if (candidate_cols.size() == 1)
  {
    return *candidate_cols.begin();
  }
  else
  {
    return 0;
  }
}

int solve(const vector<string> &input)
{
  return vertical_mirror(input) + horizontal_mirror(input) * 100;
}

int main()
{
  cout << endl;
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;

  unsigned int result = 0;

  vector<string> lines;
  while (getline(input_file, input_line))
  {
    if (input_line.empty())
    {
      result += solve(lines);
      lines.clear();
      continue;
    }
    lines.push_back(input_line);
  }
  result += solve(lines);
  cout << "Result: " << result << endl;
  return 0;
}