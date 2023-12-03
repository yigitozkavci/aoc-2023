#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

bool is_int(const char &c)
{
  return c >= '0' && c <= '9';
}

bool is_symbol(const char &c)
{
  return c != '.' && !is_int(c);
}

struct hashFunction
{
  size_t operator()(const pair<int,
                               int> &x) const
  {
    return x.first ^ x.second;
  }
};

void whitelist_pos(int x, int y, unordered_set<pair<int, int>, hashFunction> &eligible_positions)
{
  for (int i = -1; i <= 1; i++)
  {
    for (int j = -1; j <= 1; j++)
    {
      if (i == 0 && j == 0)
      {
        continue;
      }
      eligible_positions.insert({x + i, y + j});
    }
  }
}

vector<int> const parse_nums(const string &input_line, int y, vector<int> &numbers, unordered_map<pair<int, int>, int, hashFunction> &number_positions)
{
  const string &digits = "0123456789";
  size_t search_start_pos = 0;
  vector<int> result;
  while (true)
  {
    const size_t i = input_line.find_first_of(digits, search_start_pos);
    if (i == string::npos)
    {
      break;
    }
    const size_t j = input_line.find_first_not_of(digits, i);
    int num = stoi(input_line.substr(i, j != std::string::npos ? j - i : j));
    numbers.push_back(num);
    for (int x = i; x < (j == string::npos ? input_line.size() : j); x++)
    {
      number_positions[{x, y}] = numbers.size() - 1;
    }
    search_start_pos = j;
  }
  return result;
}

vector<int> find_surrounding_nums(const string &input, int x, int y, const vector<int> &numbers, const unordered_map<pair<int, int>, int, hashFunction> &number_positions)
{
  unordered_set<int> num_indexes;
  vector<int> result;
  for (int i = -1; i <= 1; i++)
  {
    for (int j = -1; j <= 1; j++)
    {
      if (i == 0 && j == 0)
      {
        continue;
      }
      if (number_positions.find({x + i, y + j}) != number_positions.end())
      {
        num_indexes.insert(number_positions.at({x + i, y + j}));
      }
    }
  }
  for (const int &i : num_indexes)
  {
    result.push_back(numbers[i]);
  }
  return result;
}

using namespace std;
int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;
  int line_number = 0, result = 0;

  vector<int> numbers;
  unordered_map<pair<int, int>, int, hashFunction> number_positions;
  while (getline(input_file, input_line))
  {
    parse_nums(input_line, line_number, numbers, number_positions);
    line_number++;
  }

  input_file.clear();
  input_file.seekg(0, ios::beg);
  line_number = 0;

  while (getline(input_file, input_line))
  {
    int col_ix = 0;
    for (const char &c : input_line)
    {
      if (c == '*')
      {
        vector<int> surrounding_nums = find_surrounding_nums(input_line, col_ix, line_number, numbers, number_positions);
        if (surrounding_nums.size() == 2)
        {
          result += surrounding_nums[0] * surrounding_nums[1];
        }
      }
      col_ix++;
    }
    line_number++;
  }
  cout << "Result: " << result << endl;
}
