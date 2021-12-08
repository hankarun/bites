#include <array>
#include <sstream>
#include <stack>
#include <vector>

#include "INumberSearch.h"

namespace aliiskenderturan {
const std::array<char, 4> operators = { '+', '-', '/', '*' };

class PostFixEquation {
    public:
    void addItem(const char* value) { data.push_back(value); }

    std::string toInFixString()
    {
        std::stack<std::string> expr;
        for (auto& t : data) {
            if (std::find(std::begin(operators), std::end(operators), *t) == std::end(operators)) {
                expr.push(t);
            } else {
                auto f1 = expr.top();
                expr.pop();
                auto f2 = expr.top();
                expr.pop();
                expr.push("(" + f1 + t[0] + f2 + ")");
            }
        }
        if (!expr.empty())
            return expr.top();
        return "";
    }

    bool calculateResult(int& result)
    {
        std::stack<int> expr;
        for (auto& t : data) {
            if (std::find(std::begin(operators), std::end(operators), *t) == std::end(operators)) {
                expr.push(std::atoi(t));
            } else {
                auto f1 = expr.top();
                expr.pop();
                auto f2 = expr.top();
                expr.pop();
                bool operationValid = false;
                int fnResult = calculate(f1, f2, t[0], operationValid);
                if (!operationValid)
                    return false;
                expr.push(fnResult);
            }
        }
        if (!expr.empty()) {
            result = expr.top();
            return true;
        }
        return false;
    }

    private:
    int calculate(int a, int b, char op, bool& valid)
    {
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

    private:
    std::vector<const char*> data;
};

class NumberSearch : public INumberSearch {
    public:
    NumberSearch()
        : targetNumber(0)
    {
    }

    void SetTargetNumber(const int value) override { targetNumber = value; }

    void SetInputNumbers(const std::string& values) override { numbers = values; }

    const std::string& GetSolution() override
    {
        resultString.clear();

        std::istringstream iss(numbers);
        std::string n;
        std::vector<std::string> inputNumbers;
        while (iss >> n) {
            inputNumbers.push_back(n);
        }

        std::vector<const char*> numbers;
        for (auto& n : inputNumbers)
            numbers.push_back(n.c_str());

        PostFixEquation solution;
        if (testPostFixPosibilities(numbers, solution)) {
            resultString = solution.toInFixString();
        }

        return resultString;
    }

    private:
    bool testPostFixPosibilities(std::vector<const char*> _numbers, PostFixEquation& solution)
    {
        struct EqData {
            std::vector<const char*> numbers;
            int currentStack;
            PostFixEquation eq;
        };
        std::stack<EqData> eqStack;
        eqStack.push({ _numbers, 0, {} });
        while (!eqStack.empty()) {
            auto data = eqStack.top();
            eqStack.pop();
            int result = 0;
            if (data.eq.calculateResult(result)) {
                if (targetNumber == result) {
                    solution = data.eq;
                    return true;
                }
            }

            if (data.currentStack >= 2) {
                for (const char& op : operators) {
                    auto temp = data.eq;
                    temp.addItem(&op);
                    eqStack.push({ data.numbers, data.currentStack - 1, temp });
                }
            }

            for (int i = 0; i < data.numbers.size(); i++) {
                if (data.numbers[i] != nullptr) {
                    const char* n = data.numbers[i];
                    data.numbers[i] = nullptr;
                    auto temp = data.eq;
                    temp.addItem(n);
                    eqStack.push({ data.numbers, data.currentStack + 1, temp });
                    data.numbers[i] = n;
                }
            }
        }

        return false;
    }

    private:
    int targetNumber;
    std::string numbers;
    std::string resultString;
};
} // namespace aliiskenderturan