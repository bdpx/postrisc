#include "BinaryRandomGenerator.hpp"

int main(int argc, char**argv)
{
    try {
        if (argc < 4) {
            std::cerr << "Usage: " << argv[0] << " codesize rodatasize datasize" << std::endl;
            return EXIT_FAILURE;
        }
        postrisc::util::setup();
        // postrisc::util::Logger::OpenLogFile("generator.log");

        unsigned codesize = std::atol(argv[1]);
        unsigned rodatasize = std::atol(argv[2]);
        unsigned datasize = std::atol(argv[3]);

        postrisc::Decoder decoder;
        postrisc::BinaryRandomGenerator generator(decoder);

        return generator.AssembleGenerate(std::cout,
            U64C(1024) * codesize,
            U64C(1024) * rodatasize,
            U64C(1024) * datasize);

    } catch (std::exception& e) {
        std::cerr << "Exception occur: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception occur" << std::endl;
        return EXIT_FAILURE;
    }
}
