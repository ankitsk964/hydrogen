#pragma once

#include <variant>

#include "tokenization.h"
using namespace std;

struct NodeExprIntLit
{
    Token int_lit;
};
struct NodeExprIdent
{
    Token ident;
};

struct NodeExpr
{
    variant<NodeExprIntLit, NodeExprIdent> var;
};

struct NodeStmtExit
{
    NodeExpr expr;
};
struct NodeStmtLet
{
    Token ident;
    NodeExpr expr;
};
struct NodeStmt
{
    variant<NodeStmtExit, NodeStmtLet> var;
};
struct NodeProg
{
    vector<NodeStmt> stmts;
};
struct NodeExit
{
    NodeExpr expr;
};

class Parser
{
    public:
    inline explicit Parser(vector<Token> tokens):
        m_tokens(move(tokens))
    {

    }

        optional<NodeExpr> parse_expr()
        {
            if (peak().has_value() && peak().value().type == TokenType::int_lit)
            {

                return NodeExpr{.var = NodeExprIntLit{.int_lit = consume()}};
            }
            else if (peak().has_value() && peak().value().type == TokenType::ident)
            {
                return NodeExpr{.var = NodeExprIdent {.ident = consume()}};
            }
            else
            {
                return {};
            }
        }

    // optional<NodeStmt> parse_stmt()
    // {
    //     if (peak().value().type == TokenType::exit && peak(1).has_value() && peak(1).value().type == TokenType::open_paren)
    //     {
    //         consume();
    //         consume();
    //         NodeStmtExit stmt_exit;
    //         if (auto node_expr = parse_expr())
    //         {
    //             stmt_exit = {.expr = node_expr.value()};
    //         }
    //         if (auto node_expr = parse_expr())
    //         {
    //             exit_node = NodeExit{.expr = node_expr.value()};
    //         }
    //         else
    //         {
    //             cerr << "Error parsing expression" << endl;
    //             exit(EXIT_FAILURE);
    //         }
    //         if (peak().has_value() && peak().value().type == TokenType::close_paren)
    //         {
    //             consume();
    //
    //         }
    //         else
    //         {
    //             cerr << "Expected ')'" << endl;
    //             exit(EXIT_FAILURE);
    //         }
    //         if (peak().has_value() && peak().value().type == TokenType::semi)
    //         {
    //             consume();
    //         }
    //         else
    //         {
    //             cerr << "Expected Semicolon" << endl;
    //             exit(EXIT_FAILURE);
    //         }
    //         return NodeStmt {.var = stmt_exit};
    //     }
    //
    // }

    optional<NodeExit> parse()
    {
        optional<NodeExit> exit_node;
        while (peak().has_value())
        {
            if (peak().value().type == TokenType::exit && peak(1).has_value() && peak(1).value().type == TokenType::open_paren)
            {
                consume();
                consume();
                if (auto node_expr = parse_expr())
                {
                    exit_node = NodeExit{.expr = node_expr.value()};
                }
                else
                {
                    cerr << "Error parsing expression" << endl;
                    exit(EXIT_FAILURE);
                }
                if (peak().has_value() && peak().value().type == TokenType::close_paren)
                {
                    consume();

                }
                else
                {
                    cerr << "Expected ')'" << endl;
                    exit(EXIT_FAILURE);
                }
                if (peak().has_value() && peak().value().type == TokenType::semi)
                {
                    consume();
                }
                else
                {
                    cerr << "Expected Semicolon" << endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
        m_index =0;
        return exit_node;
    }

    private:
    [[nodiscard]] inline optional<Token> peak(int offset = 0) const
    {
        if (m_index + offset >= m_tokens.size())
        {
            return {};
        }
        else
        {
            return m_tokens.at(m_index + offset);
        }

    }
    inline Token consume()
    {
        return m_tokens.at(m_index++);
    }
    size_t m_index = 0;
    const vector<Token> m_tokens;

};


