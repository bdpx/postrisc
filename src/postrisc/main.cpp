#include <exception>

#include "util/common.hpp"
#include "config/configuration.hpp"

namespace postrisc {

int run(int argc, char **argv)
{
    util::setup();
    util::LoggerFileFinalizer loggerFileFinalizer;

    Configuration configuration;
    return configuration.run(argc, argv);
}

} // namespace postrisc

int main(int argc, char **argv)
{
    try {
        return postrisc::run(argc, argv);
    } catch (std::exception& e) {
        std::cerr << "Exception occur: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception occur" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
