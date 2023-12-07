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

vector<int> read_card_freqs(const string &hand)
{
  unordered_map<char, int> card_freqs;
  for (const char &card : hand)
  {
    card_freqs[card]++;
  }
  vector<int> freqs;
  for (auto [_, v] : card_freqs)
  {
    freqs.push_back(v);
  }
  sort(freqs.begin(), freqs.end());
  return freqs;
}

bool freq_equal(vector<int> freq_left, vector<int> freq_right)
{
  size_t size = freq_left.size();
  if (freq_right.size() != size)
  {
    return false;
  }
  return equal(freq_left.begin(), freq_left.begin() + size, freq_right.begin());
}

int read_jokers(const string &hand)
{
  int result = 0;
  for (const char &card : hand)
  {
    if (card == 'J')
    {
      result++;
    }
  }
  return result;
}

int hand_type(const string &hand)
{
  std::vector<std::function<bool(const string &)>> hand_functions;
  vector<int> card_freqs = read_card_freqs(hand);
  int jokers = read_jokers(hand);

  auto high_card = [&](const string &hand)
  {
    return freq_equal(card_freqs, {1, 1, 1, 1, 1});
  };
  hand_functions.push_back(high_card);
  auto one_pair = [&](const string &hand)
  {
    bool result = freq_equal(card_freqs, {1, 1, 1, 2});
    result = result || (freq_equal(card_freqs, {1, 1, 1, 1, 1}) && jokers == 1);
    return result;
  };
  hand_functions.push_back(one_pair);
  auto two_pair = [&](const string &hand)
  {
    return freq_equal(card_freqs, {1, 2, 2});
  };
  hand_functions.push_back(two_pair);
  auto three_of_a_kind = [&](const string &hand)
  {
    bool result = freq_equal(card_freqs, {1, 1, 3});
    result = result || (freq_equal(card_freqs, {1, 1, 1, 2}) && jokers >= 1);
    return result;
  };
  hand_functions.push_back(three_of_a_kind);
  auto full_house = [&](const string &hand)
  {
    bool result = freq_equal(card_freqs, {2, 3});
    result = result || (freq_equal(card_freqs, {1, 2, 2}) && jokers == 1);
    return result;
  };
  hand_functions.push_back(full_house);
  auto four_of_a_kind = [&](const string &hand)
  {
    bool result = freq_equal(card_freqs, {1, 4});
    result = result || (freq_equal(card_freqs, {1, 1, 3}) && jokers >= 1);
    result = result || (freq_equal(card_freqs, {1, 2, 2}) && jokers == 2);
    return result;
  };
  hand_functions.push_back(four_of_a_kind);
  auto five_of_a_kind = [&](const string &hand)
  {
    bool result = freq_equal(card_freqs, {5});
    result = result || (freq_equal(card_freqs, {1, 4}) && jokers >= 1);
    result = result || (freq_equal(card_freqs, {2, 3}) && jokers >= 2);
    return result;
  };
  hand_functions.push_back(five_of_a_kind);

  for (int i = hand_functions.size() - 1; i >= 0; i--)
  {
    if (hand_functions.at(i)(hand))
    {
      return i;
    }
  }
  return -1;
}

int card_strength(const char &c)
{
  vector<char> cards_by_strength{'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J'};
  for (int i = 0; i < cards_by_strength.size(); i++)
  {
    if (cards_by_strength.at(i) == c)
    {
      return i;
    }
  }
  return -1;
}

bool compare_cards(const string &c1, const string &c2)
{
  if (c1 == c2)
  {
    return false;
  }
  int type1 = hand_type(c1);
  int type2 = hand_type(c2);

  // Return true if c1 is weaker than c2.
  // Lower the type index is weaker the hand is.
  if (type1 < type2)
  {
    return true;
  }
  else if (type1 > type2)
  {
    return false;
  }
  else
  {
    for (int i = 0; i < 5; i++)
    {
      int c1_strength = card_strength(c1.at(i));
      int c2_strength = card_strength(c2.at(i));
      if (c1_strength > c2_strength)
      {
        return true;
      }
      else if (c1_strength < c2_strength)
      {
        return false;
      }
    }
  }
}

int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;
  unordered_map<string, long> bids;
  vector<string> hands;
  while (getline(input_file, input_line))
  {
    vector<string> s = split(input_line, " ");
    const string &hand = s.at(0);
    const long &bid = stol(s.at(1));
    bids[hand] = bid;
    hands.push_back(hand);
  }
  sort(hands.begin(), hands.end(), [](const string &c1, const string &c2)
       { return compare_cards(c1, c2); });
  int result = 0;
  for (int i = 0; i < hands.size(); i++)
  {
    int rank = i + 1;
    int bid = bids.at(hands.at(i));
    result += rank * bid;
  }
  cout << "Result: " << result << endl;
}