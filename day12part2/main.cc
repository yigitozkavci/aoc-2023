#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <numeric>
#include <stack>

using namespace std;

template <typename T>
vector<T> to_vector(stack<T> input)
{
  vector<T> result;
  while (!input.empty())
  {
    result.push_back(input.top());
    input.pop();
  }
  return result;
}

string gen_key(bool conseq, const string &input, const vector<unsigned int> &records)
{
  string result = "";
  result += conseq ? "1" : "0";
  result += input;

  if (records.empty())
  {
    return result + "end";
  }
  for (int i = 0; i < records.size() - 1; i++)
  {
    result += to_string(records.at(i));
    result += ",";
  }
  result += to_string(records.at(records.size() - 1));
  return result + "end";
}

unsigned long solve(string history, bool use_cache, const string &input, stack<unsigned int> records, bool conseq, unordered_map<string, unsigned long> &known_solutions)
{
  string cache_key = gen_key(conseq, input, to_vector(records));
  if (use_cache && known_solutions.find(cache_key) != known_solutions.end())
  {
    return known_solutions[cache_key];
  }
  if (input.empty())
  {
    if (records.empty() || (records.top() == 0 && records.size() == 1))
    {
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
    unsigned long result = solve(history + '.', use_cache, input.substr(1), records, false, known_solutions);
    known_solutions[cache_key] = result;
    return result;
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
    unsigned long result = solve(history + '#', use_cache, input.substr(1), records, true, known_solutions);
    known_solutions[cache_key] = result;
    return result;
  }
  else if (input.at(0) == '?')
  {
    // This is ?
    if (records.empty())
    {
      // Only possibility is a '.'
      return solve(history + '.', use_cache, input.substr(1), records, false, known_solutions);
    }
    if (records.top() == 0)
    {
      // Only possibility is a '.'
      records.pop();
      unsigned long result = solve(history + '.', use_cache, input.substr(1), records, false, known_solutions);
      known_solutions[cache_key] = result;
      return result;
    }
    stack<unsigned int> right_stack = records;

    unsigned long result = 0;

    if (!conseq)
    {
      result += solve(history + '.', use_cache, input.substr(1), records, false, known_solutions);
    }
    unsigned int prev_top = right_stack.top();
    right_stack.pop();
    right_stack.push(prev_top - 1);

    result += solve(history + '#', use_cache, input.substr(1), right_stack, true, known_solutions);
    known_solutions[cache_key] = result;
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

string expand(string v)
{
  string result = v;
  for (int i = 0; i < 4; i++)
  {
    result += "?";
    result += v;
  }
  return result;
}

int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;

  unsigned long result = 0;
  while (getline(input_file, input_line))
  {
    vector<string> _split_input = split(input_line, " ");
    string springs = expand(_split_input.at(0));
    stack<unsigned int> records;
    vector<string> splitted_records = split(_split_input.at(1), ",");
    reverse(splitted_records.begin(), splitted_records.end());

    for (int i = 0; i < 5; i++)
    {
      for (const string &record : splitted_records)
      {
        records.push(stoi(record));
      }
    }
    unordered_map<string, unsigned long> known_solutions;
    result += solve("", true, springs, records, false, known_solutions);
  }
  cout << "Result: " << result << endl;
  return 0;
}