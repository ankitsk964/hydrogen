#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>
#include "./tokenization.h"
using namespace std;




string token_to_asm(const vector<Token>& tokens)
{
    stringstream output;
    output << "global _start\n_start:\n";
    for (int i = 0; i < tokens.size(); i++)
    {
        const Token& token = tokens.at(i);
        if (token.type == TokenType::exit)

        {
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit)
            {
                if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi)
                {
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
                    output << "    syscall\n";
                    break;
                }
            }

        }
        
    }
    return output.str();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Incorrect Usage. Correct Usage is ... " << endl;
        cerr << "hydro <input.hy>" << endl;
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
    string asm_code = token_to_asm(tokens);
    cout << asm_code << endl;
    {
        fstream file("../out.asm", ios::out);
        file << asm_code;
    }

    system("nasm -felf64 ../out.asm");
    system("ld -o out ../out.o");
    return EXIT_SUCCESS;
}