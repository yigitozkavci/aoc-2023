#include <iostream>
#include <fstream>
#include <string>

using namespace std;
int main()
{
  ifstream input_file;
  input_file.open("input.txt");

  string input_line;
  int result = 0;
  while (input_file >> input_line)
  {
    int left = -1, right = -1;
    for (const char &c : input_line)
    {
      if (c >= '0' && c <= '9')
      {
        int digit = c - '0';
        if (left == -1)
        {
          left = digit;
        }
        right = digit;
      }
    }
    result += left * 10 + right;
  }
  cout << "Result: " << result << endl;
}