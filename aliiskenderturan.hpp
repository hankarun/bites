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

    //Convert postfix equation to infix string.
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

    //Calculate the result of the equation.
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

    //Calculate the result with the operant, check if the operation is valid (no divition by zero, no float divitions)
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

        // Create individual string buffer for integers
        std::istringstream iss(numbers);
        std::string n;
        std::vector<std::string> inputNumbers;
        while (iss >> n) {
            inputNumbers.push_back(n);
        }

        // Create a pointer buffer to integers
        std::vector<const char*> numbers;
        for (auto& n : inputNumbers)
            numbers.push_back(n.c_str());

        // Bruteforce postfix equations and try to find the solution
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
        // Start with empthy equation
        eqStack.push({ _numbers, 0, {} });
        while (!eqStack.empty()) {

            // Pop one at the top for testing
            auto data = eqStack.top();
            eqStack.pop();

            //If the equation gives correct result return from the function.
            int result = 0;
            if (data.eq.calculateResult(result)) {
                if (targetNumber == result) {
                    solution = data.eq;
                    return true;
                }
            }

            // If there is enough value add for possible operants
            if (data.currentStack >= 2) {
                for (const char& op : operators) {
                    auto temp = data.eq;
                    temp.addItem(&op);
                    eqStack.push({ data.numbers, data.currentStack - 1, temp });
                }
            }

            // Create a new eqation for each of the numbers in the vector
            // Disable current integer and create a new variant of the equation.
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