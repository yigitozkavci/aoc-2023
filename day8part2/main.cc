#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <numeric>

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

long long lcm(long long a, long long b)
{
  return (a / gcd(a, b)) * (b / gcd(a, b));
}

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

    string val = sm1[1];
    Node *node = new Node(val, val.at(val.size() - 1) == 'Z');
    nodes[val] = {node, sm1[2], sm1[3]};
  }

  vector<Node *> ghost_nodes;
  for (auto [k, v] : nodes)
  {
    Node *node = get<0>(v);
    string left = get<1>(v);
    string right = get<2>(v);
    node->left = get<0>(nodes.at(left));
    node->right = get<0>(nodes.at(right));
    if (node->val.at(node->val.size() - 1) == 'A')
    {
      ghost_nodes.push_back(node);
    }
  }

  int steps = 0;
  bool done = false;
  unordered_map<Node *, long> ghost_steps;
  cout << endl;
  while (!done)
  {
    for (const char &instruction : instructions)
    {
      if (ghost_steps.size() == ghost_nodes.size())
      {
        done = true;
        break;
      }
      for (Node *&ghost_node : ghost_nodes)
      {
        if (ghost_steps.find(ghost_node) == ghost_steps.end() && ghost_node->final)
        {
          ghost_steps[ghost_node] = steps;
        }
      }
      if (instruction == 'L')
      {
        for (Node *&ghost_node : ghost_nodes)
        {
          ghost_node = ghost_node->left;
        }
      }
      else
      {
        for (Node *&ghost_node : ghost_nodes)
        {
          ghost_node = ghost_node->right;
        }
      }
      steps++;
    }
  }
  long long result = 1;
  for (auto [k, v] : ghost_steps)
  {
    result = lcm(v, result);
  }
  cout << "Result: " << result << endl;
  return 0;
}