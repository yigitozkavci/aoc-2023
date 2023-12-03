#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

struct GameSet
{
  int red;
  int blue;
  int green;
};

struct Game
{
  int id;
  vector<GameSet> game_sets;
};

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

int parse_game_id(const string &input)
{
  int game_id_pos_start = input.find(" ", 0) + 1;
  int game_id_pos_end = input.find(" ", game_id_pos_start) - 1;
  const string &game_id_str = input.substr(game_id_pos_start, game_id_pos_end - game_id_pos_start);
  return stoi(game_id_str);
}

GameSet parse_game_set(const string &input)
{
  const vector<string> &cube_amounts = split(input, ", ");
  int red = 0, blue = 0, green = 0;
  for (const string &cube_amount : cube_amounts)
  {
    vector<string> amount_and_color = split(cube_amount, " ");
    int amount = stoi(amount_and_color[0]);
    string color = amount_and_color[1];
    if (color == "red")
    {
      red = amount;
    }
    else if (color == "blue")
    {
      blue = amount;
    }
    else if (color == "green")
    {
      green = amount;
    }
  }
  GameSet game_set = {
      red,
      blue,
      green};
  return game_set;
}

Game parse_game(const string &input)
{
  int game_id = parse_game_id(input);
  int set_start_pos = input.find(" ", input.find(" ", 0) + 1) + 1;
  const vector<string> &sets = split(input.substr(set_start_pos), "; ");

  vector<GameSet> game_sets;
  for (const string &set : sets)
  {
    game_sets.push_back(parse_game_set(set));
  }

  Game game = {
      game_id,
      game_sets,
  };
  return game;
}

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

vector<int> const parse_nums(const string &input_line, int y, const unordered_set<pair<int, int>, hashFunction> whitelist)
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
    for (int x = i; x < j; x++)
    {
      if (whitelist.find({x, y}) != whitelist.end())
      {
        result.push_back(num);
        break;
      }
    }
    search_start_pos = j;
  }
  return result;
}

using namespace std;
int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;
  int line_number = 0;
  unordered_set<pair<int, int>, hashFunction> whitelist;
  while (getline(input_file, input_line))
  {
    int col_ix = 0;
    for (const char &c : input_line)
    {
      if (is_symbol(c))
      {
        whitelist_pos(col_ix, line_number, whitelist);
      }
      col_ix++;
    }
    line_number++;
  }

  input_file.clear();
  input_file.seekg(0, ios::beg);

  line_number = 0;
  unsigned int result = 0;
  while (getline(input_file, input_line))
  {
    for (const int &i : parse_nums(input_line, line_number, whitelist))
    {
      result += i;
    }
    line_number++;
  }
  cout << "Result: " << result << endl;
}
