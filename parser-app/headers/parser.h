#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "tokenizer.h"
#include "jsNode.h"

namespace js
{

class Parser
{
private:
    std::vector<Token> tokens_;
    size_t index_;
public:
    explicit Parser(const std::vector<Token>& tokens)
            : tokens_(tokens), index_(0) {}
    
    std::shared_ptr<Node> parse()
    {
        if (tokens_.empty())
        {
            throw std::runtime_error("[Error]: Invalid JSON. Empty JSON.");
        }
        return parseValue();
    }

private:
    std::shared_ptr<Node>   parseValue()
    {
        if (index_ >= tokens_.size())
        {
            throw std::runtime_error("[Error]: Invalid JSON. Unexpected end of JSON.");
        }

        const Token& token = tokens_[index_];
        switch (token.type)
        {
            case TokenType::ObjectStart:
                return parseObject();
            case TokenType::ArrayStart:
                return parseArray();
            case TokenType::String:
                return parseString();
            case TokenType::Number:
                return parseNumber();
            case TokenType::Boolean:
            case TokenType::Null:
                return parseLiteral();
            default:
                throw std::runtime_error("[Error]: Invalid JSON. Unexpected token found -> " + token.value);
        }
    }

    std::shared_ptr<ValueNode> parseString();

    std::shared_ptr<ValueNode> parseNumber();

    std::shared_ptr<ValueNode> parseLiteral();

    std::shared_ptr<ObjectNode> parseObject();

    std::shared_ptr<ArrayNode> parseArray();
};

} // namespace js

#endif /* PARSER_H */