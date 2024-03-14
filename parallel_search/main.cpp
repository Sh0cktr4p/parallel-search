
#include "test_client.h"


int main() {
    

    std::cout << "Run profiling client: [c/C]" << std::endl;
    std::cout << "Test tree search on custom inputs: [t/T]" << std::endl;

    std::string action;
    std::cin >> action;

    if (action == "c" || action == "C") {
        runTestClient();
    }
    else if (action == "t" || action == "T") {
        useTreeSearch();
    }
    return 0;
}
