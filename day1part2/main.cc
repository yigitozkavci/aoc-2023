#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <algorithm>

using namespace std;

unordered_map<string, int> keywords{
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9},
    {"1", 1},
    {"2", 2},
    {"3", 3},
    {"4", 4},
    {"5", 5},
    {"6", 6},
    {"7", 7},
    {"8", 8},
    {"9", 9},
};

class Trie
{
private:
  unordered_map<char, Trie *> map;
  optional<string> content;

public:
  Trie()
  {
    this->content = nullopt;
  }

  void add(const string &s)
  {
    this->addR("", s);
  }
  void addR(const string &prev_content, const string &s)
  {
    if (s.empty())
    {
      this->content = prev_content;
      return;
    }
    Trie *branch = map[s.at(0)];
    if (branch == nullptr)
    {
      branch = new Trie();
      branch->addR(prev_content + s.substr(0, 1), s.substr(1));
      map[s.at(0)] = branch;
    }
    else
    {
      branch->addR(prev_content + s.substr(0, 1), s.substr(1));
    }
  }

  optional<string> findPrefix(const string &s)
  {
    if (this->content.has_value())
    {
      return this->content.value();
    }
    if (s.empty())
    {
      return nullopt;
    }
    Trie *branch = this->map[s.at(0)];
    if (branch == nullptr)
    {
      return nullopt;
    }
    else
    {
      return branch->findPrefix(s.substr(1));
    }
  }

  optional<string> find(const string &s)
  {
    if (s.empty())
    {
      return nullopt;
    }

    optional<string> prefix = this->findPrefix(s);
    if (prefix.has_value())
    {
      return prefix.value();
    }
    else
    {
      return find(s.substr(1));
    }
  }
};

int main()
{
  Trie *trie = new Trie();
  for (const auto &kv : keywords)
  {
    trie->add(kv.first);
  }

  unordered_map<string, int> reverse_keywords;
  for (const auto &kv : keywords)
  {
    string key = kv.first;
    reverse(key.begin(), key.end());
    reverse_keywords[key] = kv.second;
  }

  Trie *reverse_trie = new Trie();
  for (const auto &kv : reverse_keywords)
  {
    reverse_trie->add(kv.first);
  }

  // assert(trie->find("eon") == nullopt);
  // assert(trie->find("eo1n") == "1");
  // assert(trie->find("eo1onen") == "1");
  // assert(trie->find("one") == "one");
  // assert(trie->find("wwwone") == "one");
  // assert(trie->find("wwwonetwo") == "one");

  ifstream input_file;
  input_file.open("./input.txt");

  string input_line;
  int value = 0;
  while (input_file >> input_line)
  {
    optional<string> result = trie->find(input_line);
    value += keywords[result.value()] * 10;

    string reverse_input = input_line;
    reverse(reverse_input.begin(), reverse_input.end());
    optional<string> reverse_result = reverse_trie->find(reverse_input);
    value += reverse_keywords[reverse_result.value()];
  }
  cout << "Result: " << value << endl;
}
