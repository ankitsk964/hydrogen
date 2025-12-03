#pragma once
#include <vector>
#include <string>
using namespace std;

//Tokens
enum class TokenType
{
    exit,
    int_lit,
    semi,
    add,
    sub,
    mul,
    div,
    mod,
    equal,
    pr_inc,
    pr_dec,
    po_inc,
    po_dec,

};

struct Token
{
    TokenType type;

    optional<string> value {};
};

class Tokenizer
{
    public:
        inline explicit  Tokenizer(const string& src)
            :m_src(move(src)), m_index(0)
        {

        }

        inline vector<Token> tokenize()
        {
            vector<Token> tokens;
            string buf;
            while (peek().has_value())
            {
                if (isalpha(peek().value()))
                {
                    buf.push_back(consume());
                    while (peek().has_value() && isalnum(peek().value()))
                    {
                        buf.push_back(consume());
                    }
                    if (buf == "exit")
                    {
                        tokens.push_back({.type = TokenType::exit});
                        buf.clear();
                        continue;
                    } else
                    {
                        cerr << "error: unexpected token type" << endl;
                        exit(EXIT_FAILURE);
                    }
                }
                else if (isdigit(peek().value()))
                {
                    buf.push_back(consume());
                    while (peek().has_value() && isdigit(peek().value()))
                    {
                        buf.push_back(consume());
                    }
                    tokens.push_back({.type = TokenType::int_lit, .value = buf});
                    buf.clear();
                    continue;
                }
                else if (iswspace(peek().value()))
                {
                    consume();
                    continue;
                }
                else if (peek().value() == ';')
                {
                    tokens.push_back({.type = TokenType::semi});
                    consume();
                    continue;
                }

                else
                {
                    cerr << "error: unexpected token type" << endl;
                    exit(EXIT_FAILURE);
                }
            }
            m_index = 0;
            return tokens;

        }
    private:
        [[nodiscard]]optional<char> peek(int ahead = 0) const
        {
            if (m_index + ahead >= m_src.length())
            {
                return {};
            }
            else
            {
                return m_src.at(m_index + ahead);
            }

        }
        char consume()
        {
            return m_src.at(m_index++);
        }
        const string m_src;
        int m_index;
};