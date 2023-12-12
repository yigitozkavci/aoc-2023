#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <numeric>
#include <stack>

using namespace std;

unsigned int solve(string history, const string &input, stack<unsigned int> records, bool conseq)
{
  if (input.empty())
  {
    if (records.empty() || (records.top() == 0 && records.size() == 1))
    {
      // cout << history << endl;
      return 1;
    }
    else
    {
      return 0;
    }
  }
  if (input.at(0) == '.')
  {
    if (!records.empty() && records.top() == 0)
    {
      records.pop();
    }
    else if (!records.empty() && records.top() > 0 && conseq)
    {
      return 0;
    }
    return solve(history + '.', input.substr(1), records, false);
  }
  else if (input.at(0) == '#')
  {
    if (records.empty() || records.top() == 0)
    {
      return 0;
    }
    unsigned int t = records.top();
    records.pop();
    records.push(t - 1);
    return solve(history + '#', input.substr(1), records, true);
  }
  else if (input.at(0) == '?')
  {
    // This is ?
    if (records.empty())
    {
      // Only possibility is a '.'
      return solve(history + '.', input.substr(1), records, false);
    }
    if (records.top() == 0)
    {
      // Only possibility is a '.'
      records.pop();
      return solve(history + '.', input.substr(1), records, false);
    }
    stack<unsigned int> right_stack = records;

    unsigned int result = 0;

    if (!conseq)
    {
      result += solve(history + '.', input.substr(1), records, false);
    }
    unsigned int prev_top = right_stack.top();
    right_stack.pop();
    right_stack.push(prev_top - 1);

    result += solve(history + '#', input.substr(1), right_stack, true);
    return result;
  }
  else
  {
    cout << "ERROR" << endl;
    return 0;
  }
}

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

int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;

  unsigned int result = 0;
  while (getline(input_file, input_line))
  {
    vector<string> _split_input = split(input_line, " ");
    string springs = _split_input.at(0);
    stack<unsigned int> records;
    vector<string> splitted_records = split(_split_input.at(1), ",");
    reverse(splitted_records.begin(), splitted_records.end());
    for (const string &record : splitted_records)
    {
      records.push(stoi(record));
    }
    result += solve("", springs, records, false);
  }
  cout << "Result: " << result << endl;
  return 0;
}