#include <exception>

#include "util/common.hpp"
#include "config/configuration.hpp"

int main(int argc, char **argv)
{
    try {
        postrisc::util::setup();
        postrisc::Configuration configuration;
        return configuration.run(argc, argv);
    } catch (std::exception& e) {
        std::cerr << "Exception occur: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception occur" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
