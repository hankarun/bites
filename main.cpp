#include <iostream>

#include "aliiskenderturan.hpp"

int main(int argc, char const *argv[])
{
    aliiskenderturan::NumberSearch numberSearch;
    numberSearch.SetTargetNumber(302);
    numberSearch.SetInputNumbers("2 3 7 10 50 25");
    std::cout << numberSearch.GetSolution() << std::endl;
    return 0;
}