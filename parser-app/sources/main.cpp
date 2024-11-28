#include "parser.h"
#include "tokenizer.h"

#include <iostream>
#include <fstream>

std::string readFile(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::in);
    if (!file.is_open())
    {
        throw std::runtime_error("[Error]: Unable to open file -> " + filePath);
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <json-file>" << std::endl;
        return 1;
    }

    try
    {
        std::string json = readFile(argv[1]);
        js::Tokenizer tokenizer(json);
        auto tokens = tokenizer.tokenize();

        js::Parser parser(std::move(tokens));
        auto rootNode = parser.parse();

        rootNode->print();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}