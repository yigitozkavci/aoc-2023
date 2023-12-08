#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>

using namespace std;

class Node
{
public:
  string val;
  Node *left;
  Node *right;
  bool final;

  Node(string val_, bool final_) : val(val_), final(final_){};
};

int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  // Instructions
  string instructions;
  getline(input_file, instructions);

  string input_line;
  getline(input_file, input_line); // Empty line

  // Nodes
  unordered_map<string, tuple<Node *, string, string>> nodes;
  while (getline(input_file, input_line))
  {
    smatch sm1;
    regex_search(input_line, sm1, regex("(.+) = \\((.+), (.+)\\)"));
    Node *node = new Node(sm1[1], sm1[1] == "ZZZ");
    nodes[sm1[1]] = {node, sm1[2], sm1[3]};
  }
  for (auto [k, v] : nodes)
  {
    Node *node = get<0>(v);
    string left = get<1>(v);
    string right = get<2>(v);
    node->left = get<0>(nodes.at(left));
    node->right = get<0>(nodes.at(right));
  }
  Node *node = get<0>(nodes.at("AAA"));

  int steps = 0;
  bool done = false;
  while (!done)
  {
    for (const char &instruction : instructions)
    {
      if (node->final)
      {
        done = true;
        break;
      }
      if (instruction == 'L')
      {
        node = node->left;
      }
      else
      {
        node = node->right;
      }
      steps++;
    }
  }
  return 0;
}