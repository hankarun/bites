#include "INumberSearch.h"
#include <sstream>
#include <stack>
#include <string>
#include <vector>

namespace aliiskenderturan {
std::string operators = "+-/*";
int target = 0;
bool found = false;
std::string resultString;

int caluculate(int a, int b, char op) {
  if (op == '+')
    return a + b;
  if (op == '-')
    return a - b;
  if (op == '*')
    return a * b;
  if (op == '/') {
    if (b == 0)
      return 0;
    float s = a / (float)b;
    if (std::fmod(s, 1.0) == 0)
      return (int)s;
    return 0;
  }

  return 0;
}

int calculate(std::string postfix) {
  std::stack<int> expr;
  std::stringstream sc(postfix);
  std::string t;
  while (sc >> t) {
    if (operators.find(t) == std::string::npos) {
      expr.push(std::atoi(t.c_str()));
    } else {
      auto f1 = expr.top();
      expr.pop();
      auto f2 = expr.top();
      expr.pop();
      expr.push(caluculate(f1, f2, t[0]));
    }
  }
  return expr.top();
}

std::string convertPostFix(std::string postfix) {
  std::stack<std::string> expr;
  std::stringstream sc(postfix);
  std::string t;
  while (sc >> t) {
    if (operators.find(t) == std::string::npos) {
      expr.push(t);
    } else {
      auto f1 = expr.top();
      expr.pop();
      auto f2 = expr.top();
      expr.pop();
      expr.push("(" + f1 + t + f2 + ")");
    }
  }
  return expr.top();
}

void brute(std::vector<int> numbers, int stackHeight, std::string eq) {
  if (found)
    return;

  if (stackHeight >= 2) {
    for (char op : operators) {
      brute(numbers, stackHeight - 1, eq + " " + op);
    }
  }

  bool allUsedUp = true;
  for (int i = 0; i < numbers.size(); i++) {
    if (numbers[i] != -1) {
      allUsedUp = false;
      int n = numbers[i];
      numbers[i] = -1;
      brute(numbers, stackHeight + 1, eq + " " + std::to_string(n));
      numbers[i] = n;
    }
  }

  if (allUsedUp && stackHeight == 1) {
    int result = calculate(eq);
    if (target == result) {
      found = true;
      resultString = convertPostFix(eq);
      //std::cout << eq << " = " << target << " - " << convertPostFix(eq) << std::endl;
    }
  }
}

class NumberSearch : public INumberSearch {
public:
  void SetTargetNumber(const int value) override {
    targetNumber = value;
    target = value;
  }

  void SetInputNumbers(const std::string &values) override {
    std::istringstream iss(values);
    int n;
    while (iss >> n) {
      inputNumbers.push_back(n);
    }
  }

  const std::string &GetSolution() override {
    brute(inputNumbers, 0, "");
    return resultString;
  }

private:
  int targetNumber;
  std::vector<int> inputNumbers;
};
} // namespace aliiskenderturan