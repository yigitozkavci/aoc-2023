#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

using namespace std;
int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;
  int result = 0;
  while (getline(input_file, input_line))
  {
    Game game = parse_game(input_line);
    bool game_valid = true;
    for (const auto &game_set : game.game_sets)
    {
      if (game_set.red > 12 || game_set.green > 13 || game_set.blue > 14)
      {
        game_valid = false;
      }
    }
    if (game_valid)
    {
      result += game.id;
    }
  }
  cout << "Result: " << result << endl;
}
