#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

struct Card
{
  unordered_set<int> winning_numbers;
  vector<int> possessed_numbers;
};

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

std::vector<std::string> split(std::string s, std::string delimiter)
{
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
  {
    if (delimiter == " ")
    {
      while (pos_end != string::npos && s.at(pos_end + 1) == ' ')
      {
        pos_end++;
      }
    }
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}

static inline void ltrim(std::string &s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                  { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                       { return !std::isspace(ch); })
              .base(),
          s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
  rtrim(s);
  ltrim(s);
}

Card parse_card(const string &input)
{
  vector<string> splitted_input = split(input, ": ");
  string &card_set = splitted_input.at(1);
  trim(card_set);
  vector<string> both_cards = split(card_set, " | ");
  string winning_cards_input = both_cards.at(0);
  trim(winning_cards_input);
  string possessed_cards_input = both_cards.at(1);
  trim(possessed_cards_input);
  unordered_set<int> winning_cards;
  vector<int> possessed_cards;
  for (const string &val : split(winning_cards_input, " "))
  {
    winning_cards.insert(stoi(val));
  }
  for (const string &val : split(possessed_cards_input, " "))
  {
    possessed_cards.push_back(stoi(val));
  }
  Card card = {
      winning_cards,
      possessed_cards};
  return card;
}

int calc_score(const Card &card)
{
  int result = 0;
  for (const int &val : card.possessed_numbers)
  {
    if (card.winning_numbers.find(val) != card.winning_numbers.end())
    {
      result = result == 0 ? 1 : result * 2;
    }
  }
  return result;
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
    Card card = parse_card(input_line);
    result += calc_score(card);
  }
  cout << "Result: " << result << endl;
}
