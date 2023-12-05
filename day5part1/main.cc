#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

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

vector<long> parse_seeds(const string &input)
{
  vector<long> result;
  for (const string &s : split(split(input, ": ")[1], " "))
  {
    result.push_back(stol(s));
  }
  return result;
}

using namespace std;
int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;
  getline(input_file, input_line);
  vector<long> current_values = parse_seeds(input_line);
  getline(input_file, input_line);
  unordered_set<long> transformed;
  while (getline(input_file, input_line))
  {
    transformed.clear();
    while (getline(input_file, input_line))
    {
      if (input_line.empty())
      {
        break;
      }
      vector<string> numbers = split(input_line, " ");
      long destination_number = stol(numbers[0]);
      long source_number = stol(numbers[1]);
      long range = stol(numbers[2]);
      for (int i = 0; i < current_values.size(); i++)
      {
        long value = current_values[i];
        if (transformed.find(i) != transformed.end())
        {
          continue;
        }
        if (value >= source_number && value < source_number + range)
        {
          current_values[i] += destination_number - source_number;
          transformed.insert(i);
        }
      }
    }
  }
  long result = LONG_MAX;
  for (const long &v : current_values)
  {
    result = min(result, v);
  }
  cout << result << endl;
  return 0;
}