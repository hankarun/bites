#include "INumberSearch.h"
#include <array>
#include <sstream>
#include <stack>
#include <string>
#include <vector>


namespace aliiskenderturan {
std::array<char, 4> operators = {'+', '-', '/', '*'};
int target = 0;
bool found = false;
std::string resultString;

int caluculate(int a, int b, char op, bool &valid) {
  valid = true;
  if (op == '+')
    return a + b;
  if (op == '-')
    return a - b;
  if (op == '*')
    return a * b;
  if (op == '/') {
    if (b == 0) {
      valid = false;
      return 0;
    }
    float s = a / (float)b;
    if (std::fmod(s, 1.0) == 0)
      return (int)s;
    valid = false;
    return -1;
  }
  return 0;
}

int calculateFromPostfix(const std::vector<const char *> &postfix) {
  std::stack<int> expr;
  for (auto &t : postfix) {
    if (std::find(std::begin(operators), std::end(operators), *t) ==
        std::end(operators)) {
      expr.push(std::atoi(t));
    } else {
      auto f1 = expr.top();
      expr.pop();
      auto f2 = expr.top();
      expr.pop();
      bool operationValid = false;
      int result = caluculate(f1, f2, t[0], operationValid);
      if (!operationValid)
        return 0;
      expr.push(result);
    }
  }
  return expr.top();
}

std::string converToInFix(const std::vector<const char *> &postfix) {
  std::stack<std::string> expr;
  for (auto &t : postfix) {
    if (std::find(std::begin(operators), std::end(operators), *t) ==
        std::end(operators)) {
      expr.push(t);
    } else {
      auto f1 = expr.top();
      expr.pop();
      auto f2 = expr.top();
      expr.pop();
      expr.push("(" + f1 + t[0] + f2 + ")");
    }
  }
  return expr.top();
}

void generatePostFix(std::vector<const char *> numbers, int stackHeight,
                     std::vector<const char *> eq) {
  if (found)
    return;

  if (stackHeight >= 2) {
    for (const char &op : operators) {
      auto temp = eq;
      temp.push_back(&op);
      generatePostFix(numbers, stackHeight - 1, temp);
    }
  }

  bool allUsedUp = true;
  for (int i = 0; i < numbers.size(); i++) {
    if (numbers[i] != nullptr) {
      allUsedUp = false;
      const char *n = numbers[i];
      numbers[i] = nullptr;
      auto temp = eq;
      temp.push_back(n);
      generatePostFix(numbers, stackHeight + 1, temp);
      numbers[i] = n;
    }
  }

  if (allUsedUp && stackHeight == 1) {
    int result = calculateFromPostfix(eq);
    if (target == result) {
      found = true;
      resultString = converToInFix(eq);
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
    std::string n;
    while (iss >> n) {
      inputNumbers.push_back(n);
    }
  }

  const std::string &GetSolution() override {
    std::vector<const char *> numbers;
    for (auto &n : inputNumbers)
      numbers.push_back(n.c_str());
    generatePostFix(numbers, 0, {});
    return resultString;
  }

private:
  int targetNumber;
  std::vector<std::string> inputNumbers;
};
} // namespace aliiskenderturan