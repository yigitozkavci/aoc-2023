#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <numeric>
#include <stack>

using namespace std;

int main()
{
  cout << endl;
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;

  vector<string> lines;

  /*
  From top to the bottom:
  - If input is a .
    - Increase the available slots in that column
  - If input is O
    - Decrease the available slots
    - Increase the score accordingly
  - If input is #
    - Reset the number of available slots
  */

  unordered_map<int, int> boulders, start_rows;
  int row_ix = 0;
  int width = -1;
  int total_score, num_of_boulders;
  while (getline(input_file, input_line))
  {
    width = input_line.size();
    if (start_rows.empty())
    {
      for (int i = 0; i < input_line.size(); i++)
      {
        start_rows[i] = -1;
      }
    }
    for (int i = 0; i < input_line.size(); i++)
    {
      switch (input_line.at(i))
      {
      case '.':
        break;
      case 'O':
        boulders[i]++;
        num_of_boulders++;
        break;
      case '#':
        for (int row = start_rows[i] + 1; row < row_ix && boulders[i] > 0; row++)
        {
          boulders[i]--;
          total_score -= row;
        }
        start_rows[i] = row_ix;
        break;
      }
    }
    row_ix++;
  }
  for (int i = 0; i < width; i++)
  {
    for (int row = start_rows[i] + 1; row < row_ix && boulders[i] > 0; row++)
    {
      boulders[i]--;
      total_score -= row;
    }
    start_rows[i] = row_ix;
  }
  cout << "S: " << total_score + (row_ix * num_of_boulders) << endl;
  return 0;
}