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

vector<pair<long, long>> parse_seeds(const string &input)
{
  vector<pair<long, long>> result;
  vector<string> nums = split(split(input, ": ")[1], " ");
  for (int i = 0; i < nums.size(); i += 2)
  {
    long from = stol(nums.at(i));
    long to = from + stol(nums.at(i + 1)) - 1;
    result.push_back({from, to});
  }
  return result;
}

vector<pair<long, long>> transformm(long left, long right, const vector<pair<long, long>> &transform_ranges, const unordered_map<long, long> &transform_amounts)
{
  vector<pair<long, long>> result;
  for (auto [transform_left, transform_right] : transform_ranges)
  {
    long offset = transform_amounts.at(transform_left);
    if (right < transform_left)
    {
      // Left disjoint
    }
    else if (left > transform_right)
    {
      // Right disjoint
    }
    else if (left >= transform_left && right <= transform_right)
    {
      // In
      result.push_back({left + offset, right + offset});
    }
    else if (left < transform_left && right > transform_right)
    {
      // Out
      for (auto p : transformm(left, transform_left - 1, transform_ranges, transform_amounts))
      {
        result.push_back(p);
      }
      result.push_back({transform_left + offset, transform_right + offset});
      for (auto p : transformm(transform_right, right, transform_ranges, transform_amounts))
      {
        result.push_back(p);
      }
    }
    else if (left >= transform_left && right > transform_right)
    {
      // Intersect from right
      result.push_back({left + offset, transform_right + offset});
      for (auto p : transformm(transform_right + 1, right, transform_ranges, transform_amounts))
      {
        result.push_back(p);
      }
    }
    else if (left < transform_left && right <= transform_right)
    {
      // Intersect from left
      for (auto p : transformm(left, transform_left - 1, transform_ranges, transform_amounts))
      {
        result.push_back(p);
      }
      result.push_back({transform_left + offset, right + offset});
    }
    else
    {
      cout << "ERROR" << endl;
    }
  }
  if (result.empty())
  {
    result.push_back({left, right});
  }
  return result;
}

using namespace std;
int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  long result = LONG_MAX;
  string input_line;
  getline(input_file, input_line);
  vector<pair<long, long>> current_seed_ranges = parse_seeds(input_line);
  vector<pair<long, long>> next_seeds;
  getline(input_file, input_line);
  vector<pair<long, long>> transform_ranges;
  unordered_map<long, long> transform_amounts;
  int i = 0;
  while (getline(input_file, input_line))
  {
    i++;
    transform_ranges.clear();
    while (getline(input_file, input_line))
    {
      if (input_line.empty())
      {
        break;
      }
      vector<string> numbers = split(input_line, " ");
      long destination_number = stol(numbers[0]);
      long source_from = stol(numbers[1]);
      long source_to = source_from + stol(numbers[2]) - 1;
      transform_ranges.push_back({source_from, source_to});
      transform_amounts[source_from] = destination_number - source_from;
    }
    vector<pair<long, long>> re;
    for (auto s : current_seed_ranges)
    {
      for (auto rp : transformm(s.first, s.second, transform_ranges, transform_amounts))
      {
        re.push_back(rp);
      }
    }
    current_seed_ranges = re;
  }
  for (auto v : current_seed_ranges)
  {
    result = min(result, v.first);
  }
  cout << "Result " << result << endl;
  return 0;
}
