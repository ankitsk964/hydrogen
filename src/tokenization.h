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
    open_paren,
    close_paren,
    ident,

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
            while (peak().has_value())
            {
                if (isalpha(peak().value()))
                {
                    buf.push_back(consume());
                    while (peak().has_value() && isalnum(peak().value()))
                    {
                        buf.push_back(consume());
                    }
                    if (buf == "exit")
                    {
                        tokens.push_back({.type = TokenType::exit});
                        buf.clear();
                        continue;
                    }

                    else
                    {
                        tokens.push_back({.type = TokenType::ident, .value = buf});
                        buf.clear();
                        continue;
                    }
                }
                else if (isdigit(peak().value()))
                {
                    buf.push_back(consume());
                    while (peak().has_value() && isdigit(peak().value()))
                    {
                        buf.push_back(consume());
                    }
                    tokens.push_back({.type = TokenType::int_lit, .value = buf});
                    buf.clear();
                    continue;
                }
                else if (iswspace(peak().value()))
                {
                    consume();
                    continue;
                }
                else if (peak().value() == '(')
                {
                    consume();
                    tokens.push_back({.type = TokenType::open_paren, .value = buf});
                    continue;
                }
                else if (peak().value() == ')')
                {
                    consume();
                    tokens.push_back({.type = TokenType::close_paren, .value = buf});
                    continue;
                }
                else if (peak().value() == ';')
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
        [[nodiscard]] inline optional<char> peak(int offset = 0) const
        {
            if (m_index + offset >= m_src.length())
            {
                return {};
            }
            else
            {
                return m_src.at(m_index + offset);
            }

        }
        inline char consume()
        {
            return m_src.at(m_index++);
        }
        const string m_src;
        size_t m_index = 0;
};