#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

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

long solve(long time, long distance)
{
  long l = 1, r = time / 2;
  long min_press_time = time / 2;
  long min_distance_exceeding_record = (time / 2) * (time - time / 2);
  while (l <= r)
  {
    long press_time = (r + l) / 2;
    long local_distance = press_time * (time - press_time);
    if (local_distance > distance)
    {
      if (local_distance < min_distance_exceeding_record)
      {
        min_press_time = press_time;
        min_distance_exceeding_record = local_distance;
      }

      // Shrink
      r = press_time - 1;
    }
    else
    {
      l = press_time + 1;
    }
  }
  long max_press_time = time - min_press_time;
  return max_press_time - min_press_time + 1;
}

int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;
  getline(input_file, input_line);
  vector<string> time_line = split_whitespace(input_line);
  vector<long> times;
  for (int i = 1; i < time_line.size(); i++)
  {
    times.push_back(stol(time_line.at(i)));
  }

  getline(input_file, input_line);
  vector<string> distance_line = split_whitespace(input_line);
  vector<long> distances;
  for (int i = 1; i < distance_line.size(); i++)
  {
    distances.push_back(stol(distance_line.at(i)));
  }

  long result = 1;
  for (int i = 0; i < times.size(); i++)
  {
    result *= solve(times.at(i), distances.at(i));
  }
  cout << "Result: " << result << endl;
}