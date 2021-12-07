#include <iostream>

#include "aliiskenderturan.hpp"

int main(int argc, char const *argv[])
{
    aliiskenderturan::NumberSearch numberSearch;
    numberSearch.SetTargetNumber(302);
    numberSearch.SetInputNumbers("2 3 7 10 25 50");
    std::cout << numberSearch.GetSolution() << std::endl;
    return 0;
}