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

bool test_v_sym(const string &line, int col)
{
  int errors = 0;
  for (int i = col; i < line.size(); i++)
  {
    int reflection_index = 2 * col - i - 1;
    if (reflection_index < 0)
    {
      break;
    }
    if (line.at(i) != line.at(reflection_index))
    {
      errors++;
    }
  }
  return errors;
}

int test_h_sym(const vector<string> &input, int col, int row)
{
  // Go through input
  int errors = 0;
  for (int i = row; i < input.size(); i++)
  {
    int reflection_index = 2 * row - i - 1;
    if (reflection_index < 0)
    {
      break;
    }
    if (input.at(i).at(col) != input.at(reflection_index).at(col))
    {
      errors++;
    }
  }
  return errors;
}

int horizontal_mirror(const vector<string> &input)
{
  unordered_set<int> candidate_rows, smudge_rows;
  for (int row = 1; row < input.size(); row++)
  {
    candidate_rows.insert(row);
  }

  for (int col = 0; col < input.at(0).size(); col++)
  {
    unordered_set<int> rows_to_discard;
    for (const int row : candidate_rows)
    {
      int errors = test_h_sym(input, col, row);
      if (errors > 1)
      {
        rows_to_discard.insert(row);
      }
      else if (errors == 1)
      {
        if (smudge_rows.find(row) == smudge_rows.end())
        {
          smudge_rows.insert(row);
        }
        else
        {
          rows_to_discard.insert(row);
        }
      }
    }

    for (const int row : rows_to_discard)
    {
      candidate_rows.erase(row);
    }
  }

  for (int candidate_row : candidate_rows)
  {
    if (smudge_rows.find(candidate_row) != smudge_rows.end())
    {
      return candidate_row;
    }
  }
  return 0;
}

int vertical_mirror(const vector<string> &input)
{
  unordered_set<int> candidate_cols, smudge_cols;
  for (int col = 1; col < input.at(0).size(); col++)
  {
    candidate_cols.insert(col);
  }
  for (const string &input_line : input)
  {
    unordered_set<int> cols_to_discard;
    for (const int col : candidate_cols)
    {
      int errors = test_v_sym(input_line, col);
      if (errors > 1)
      {
        cols_to_discard.insert(col);
      }
      else if (errors == 1)
      {
        if (smudge_cols.find(col) == smudge_cols.end())
        {
          smudge_cols.insert(col);
        }
        else
        {
          cols_to_discard.insert(col);
        }
      }
    }
    for (const int col : cols_to_discard)
    {
      candidate_cols.erase(col);
    }
  }

  for (int candidate_col : candidate_cols)
  {
    if (smudge_cols.find(candidate_col) != smudge_cols.end())
    {
      return candidate_col;
    }
  }
  return 0;
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