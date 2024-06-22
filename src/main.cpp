#include "../include/revenge.h"

int main(int argc, char* argv[]) {
    if (argc == 1)
        std::cout << "Usage: ./revenge [path/to/shadowfile] [path/to/dictionaryFile] [options]\n" << std::endl;

    parseInputForOptions(argc, argv);
    crackShadowFile(argv[1], argv[2]);

    return 0;
}