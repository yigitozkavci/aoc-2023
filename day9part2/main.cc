#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <numeric>

using namespace std;

std::vector<std::string> split_whitespace(std::string s)
{
  size_t pos_start = 0, pos_end;
  std::string token;
  std::vector<std::string> res;

  while (pos_start != string::npos && s.at(pos_start) == ' ')
  {
    pos_start++;
  }
  while ((pos_end = s.find(' ', pos_start)) != std::string::npos)
  {
    while (pos_end != string::npos && s.at(pos_end + 1) == ' ')
    {
      pos_end++;
    }
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + 1;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}

int solve(vector<int> &nums, int size)
{
  bool all_zero = true;
  for (int i = 0; i < size - 1; i++)
  {
    int &num = nums[i];
    if (num != 0)
    {
      all_zero = false;
    }
    nums[i] = nums[i + 1] - nums[i];
  }
  if (all_zero)
  {
    return 0;
  }
  else
  {
    return nums.at(size - 1) + solve(nums, size - 1);
  }
}

int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;

  int result = 0;
  while (getline(input_file, input_line))
  {
    vector<string> nums_input = split_whitespace(input_line);
    vector<int> nums;
    for (const string &nums_single : nums_input)
    {
      nums.push_back(stoi(nums_single));
    }
    reverse(nums.begin(), nums.end());

    int r = solve(nums, nums.size());
    result += r;
  }
  cout << "Result: " << result << endl;
  return 0;
}