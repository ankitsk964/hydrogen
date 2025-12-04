#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>
#include "./generation.h"
#include  "./parser.h"
#include "./tokenization.h"


using namespace std;


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Incorrect Usage. Correct Usage is ... " << endl;
        cerr << "hydro <input.cpp>" << endl;
        return EXIT_FAILURE;
    }
    string contents;
    {
        stringstream content_stream;
        fstream input(argv[1], ios::in);
        if (!input)
        {
            cerr << "Failed to open file: " << argv[1] << endl;
            return EXIT_FAILURE;
        }
        content_stream << input.rdbuf();
        contents = content_stream.str();
    }
    Tokenizer tokenizer(move(contents));
    vector<Token> tokens = tokenizer.tokenize();

    Parser parser(move(tokens));
    optional<NodeExit>tree = parser.parse();
    if (!tree.has_value())
    {
        cerr << "Failed to parse tree" << endl;
        exit(EXIT_FAILURE);
    }
    Generator generator(tree.value());
    {
        fstream file("out.asm", ios::out);
        file << generator.generate();
    }


    system("nasm -felf64 ../out.asm");
    system("ld -o out ../out.o");
    return EXIT_SUCCESS;
}