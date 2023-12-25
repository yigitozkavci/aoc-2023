#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <numeric>
#include <stack>

using namespace std;

struct hashFunction
{
  size_t operator()(const pair<int,
                               int> &x) const
  {
    return x.first ^ x.second;
  }
};

class Barrier
{
private:
  int left, right, top, bottom;

public:
  int row, col, leaning_boulders;
  Barrier(int row_, int col_) : row(row_), col(col_), leaning_boulders(0) {}
  void clear()
  {
    this->leaning_boulders = 0;
  }
};

template <typename T>
int sgn(T val)
{
  return (T(0) < val) - (val < T(0));
}

class Boulder
{
public:
  int row, col;
  Boulder(int row_, int col_) : row(row_), col(col_) {}

  int move(const vector<vector<Barrier *>> &walls, int wall_sign, int row_offset, int col_offset)
  {
    Barrier *wall = walls.at(row).at(col);
    if (row_offset != 0)
      this->row = wall->row + wall_sign * wall->leaning_boulders + row_offset;
    if (col_offset != 0)
      this->col = wall->col + wall_sign * wall->leaning_boulders + col_offset;
    wall->leaning_boulders++;
    return this->row;
  }
};

tuple<vector<vector<Barrier *>>,
      vector<vector<Barrier *>>,
      vector<vector<Barrier *>>,
      vector<vector<Barrier *>>>
mk_dir_matrix(const vector<string> &input, const unordered_map<pair<int, int>, Barrier *, hashFunction> &barriers)
{
  vector<vector<Barrier *>> north(input.size(), vector<Barrier *>(input.at(0).size()));
  vector<vector<Barrier *>> east(input.size(), vector<Barrier *>(input.at(0).size()));
  vector<vector<Barrier *>> south(input.size(), vector<Barrier *>(input.at(0).size()));
  vector<vector<Barrier *>> west(input.size(), vector<Barrier *>(input.at(0).size()));

  for (int row = input.size() - 1; row >= 0; row--)
  {
    for (int col = input.at(0).size() - 1; col >= 0; col--)
    {
      char c = input.at(row).at(col);
      if (c == '#')
      {
        east[row][col] = barriers.at({row, col});
        south[row][col] = barriers.at({row, col});
      }
      else
      {
        east[row][col] = col == input.at(0).size() - 1 ? barriers.at({row, col + 1}) : east[row][col + 1];
        south[row][col] = row == input.size() - 1 ? barriers.at({row + 1, col}) : south[row + 1][col];
      }
    }
  }
  for (int row = 0; row < input.size(); row++)
  {
    for (int col = 0; col < input.at(0).size(); col++)
    {
      char c = input.at(row).at(col);
      if (c == '#')
      {
        west[row][col] = barriers.at({row, col});
        north[row][col] = barriers.at({row, col});
      }
      else
      {
        west[row][col] = col == 0 ? barriers.at({row, col - 1}) : west[row][col - 1];
        north[row][col] = row == 0 ? barriers.at({row - 1, col}) : north[row - 1][col];
      }
    }
  }
  return {north, east, south, west};
}

void fill_barriers_and_boulders(const vector<string> &input, unordered_map<pair<int, int>, Barrier *, hashFunction> &barriers, unordered_map<pair<int, int>, Boulder *, hashFunction> &boulders)
{
  int row_size = input.size();
  int col_size = input.at(0).size();
  for (int i = 0; i < row_size; i++)
  {
    barriers[{i, -1}] = new Barrier({i, -1});
    barriers[{i, col_size}] = new Barrier({i, col_size});
  }
  for (int j = 0; j < col_size; j++)
  {
    barriers[{-1, j}] = new Barrier({-1, j});
    barriers[{row_size, j}] = new Barrier({row_size, j});
  }
  barriers[{-1, -1}] = new Barrier({-1, -1});
  barriers[{-1, col_size}] = new Barrier({-1, col_size});
  barriers[{row_size, -1}] = new Barrier({row_size, -1});
  barriers[{row_size, col_size}] = new Barrier({row_size, col_size});
  for (int i = 0; i < row_size; i++)
  {
    for (int j = 0; j < col_size; j++)
    {
      if (input.at(i).at(j) == '#')
      {
        barriers[{i, j}] = new Barrier(i, j);
      }
      else if (input.at(i).at(j) == 'O')
      {
        boulders[{i, j}] = (new Boulder(i, j));
      }
    }
  }
}

void clear_barriers(unordered_map<pair<int, int>, Barrier *, hashFunction> &barriers)
{
  for (auto [_, barrier] : barriers)
  {
    barrier->clear();
  }
}

struct Data
{
  unordered_map<pair<int, int>, Barrier *, hashFunction> barriers;
  unordered_map<pair<int, int>, Boulder *, hashFunction> boulders;
  vector<vector<Barrier *>> north, east, south, west;
};

int run_cycle(Data data)
{
  for (auto [_, boulder] : data.boulders)
  {
    boulder->move(data.north, 1, 1, 0);
  }
  clear_barriers(data.barriers);

  for (auto [_, boulder] : data.boulders)
  {
    boulder->move(data.west, 1, 0, 1);
  }
  clear_barriers(data.barriers);

  for (auto [_, boulder] : data.boulders)
  {
    boulder->move(data.south, -1, -1, 0);
  }
  clear_barriers(data.barriers);

  int result = 0;
  for (auto [_, boulder] : data.boulders)
  {
    result += boulder->move(data.east, -1, 0, -1);
  }
  clear_barriers(data.barriers);
  return data.boulders.size() * data.east.size() - result;
}

void search(const vector<int> &num_seq, int i)
{
  int val = num_seq.at(i);
  for (int j = i + 1; j < num_seq.size(); j++)
  {
    if (num_seq.at(j) == val)
    {
      bool valid = true;
      for (int offset = 0; offset < j - i && j + offset < num_seq.size(); offset++)
      {
        if (num_seq.at(j + offset) != num_seq.at(i + offset))
        {
          valid = false;
          break;
        }
      }
      if (valid)
      {
        if (j - i == 65)
        {
          cout << endl;
          for (int k = 0; k < 65; k++)
          {
            cout << num_seq.at(i + k) << " ";
          }
          cout << endl;
        }
        cout << "Sequence found between " << i << " and " << j << " size: " << j - i << endl;
      }
    }
  }
}

int solve(const vector<string> &input)
{
  unordered_map<pair<int, int>, Barrier *, hashFunction> barriers;
  unordered_map<pair<int, int>, Boulder *, hashFunction> boulders;
  fill_barriers_and_boulders(input, barriers, boulders);

  vector<vector<Barrier *>>
      west, east, north, south;
  auto dir_matrix = mk_dir_matrix(input, barriers);
  north = get<0>(dir_matrix);
  east = get<1>(dir_matrix);
  south = get<2>(dir_matrix);
  west = get<3>(dir_matrix);
  Data data{
      barriers, boulders, north, east, south, west};

  vector<int> num_seq;
  for (int i = 0; i < 250; i++)
  {
    num_seq.push_back(run_cycle(data));
  }

  for (int i = 0; i < num_seq.size(); i++)
  {
    search(num_seq, i);
  }

  return 0;
}

int main()
{
  cout << endl;
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;

  vector<string> lines;

  vector<string> input;
  while (getline(input_file, input_line))
  {
    input.push_back(input_line);
  }
  cout << "Result: " << solve(input) << endl;
  return 0;
}