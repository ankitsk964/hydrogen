#pragma once
#include "parser.h"

class Generator
{
public:
    inline explicit  Generator(NodeExit root):
        m_root(move(root))
    {

    }
    [[nodiscard]] string generate() const
    {
        stringstream output;
        output << "global _start\n_start:\n";
        output << "    mov rax, 60\n";

        // Extract the integer literal from the variant
        const auto &expr = m_root.expr.var;

        if (holds_alternative<NodeExprIntLit>(expr)) {
            const auto &int_node = get<NodeExprIntLit>(expr);
            output << "    mov rdi, " << int_node.int_lit.value.value() << "\n";
        } else {
            cerr << "Error: exit() requires an integer literal.\n";
            exit(EXIT_FAILURE);
        }

        output << "    syscall\n";
        return output.str();
    }
private:
    const NodeExit m_root;
};
