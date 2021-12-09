#include <iostream>

#include "aliiskenderturan.hpp"

int main(int argc, char const* argv[]) {
    aliiskenderturan::NumberSearch numberSearch;
    numberSearch.SetTargetNumber(1234);
    numberSearch.SetInputNumbers("2 3 7 10 50 25 84");
    std::cout << numberSearch.GetSolution() << std::endl;
    return 0;
}