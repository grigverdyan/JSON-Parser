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

    const Token& nextToken()
    {
        if (index_ >= tokens_.size())
        {
            throw std::runtime_error("[Error]: Invalid JSON. Unexpected end of JSON.");
        }
        return tokens_[index_++];
    }

    const Token& peekToken() const
    {
        if (index_ >= tokens_.size())
        {
            throw std::runtime_error("[Error]: Invalid JSON. Unexpected end of JSON.");
        }
        return tokens_[index_];
    }

    std::shared_ptr<Node>   parseValue()
    {
        const Token& token = peekToken();
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

    std::shared_ptr<ValueNode> parseString()
    {
        const Token& token = nextToken();
        return std::make_shared<ValueNode>(token.value);
    }

    std::shared_ptr<ValueNode> parseNumber()
    {
        const Token& token = nextToken();
        return std::make_shared<ValueNode>(token.value);
    }

    std::shared_ptr<ValueNode> parseLiteral()
    {
        const Token& token = nextToken();
        if (token.type == TokenType::Boolean)
        {
            if (token.value == "true")
                return std::make_shared<ValueNode>(true);
            else
                return std::make_shared<ValueNode>(false);
        }
        else
            return std::make_shared<ValueNode>(nullptr);
    }

    std::shared_ptr<ObjectNode> parseObject()
    {
        nextToken(); // release '{'
        auto object = std::make_shared<ObjectNode>();
        
        while (peekToken().type != TokenType::ObjectEnd)
        {
            if (peekToken().type != TokenType::String)
            {
                throw std::runtime_error("[Error]: Invalid JSON. Expected key in object.");
            }

            auto& keyToken = nextToken();
            if (nextToken().type != TokenType::Colon)
            {
                throw std::runtime_error("[Error]: Invalid JSON. Expected ':' after key in object.");
            }

            object->addChild(keyToken.value, parseValue());
            if (peekToken().type == TokenType::Comma)
            {
                nextToken(); // release ','
            }
            else if (peekToken().type != TokenType::ObjectEnd)
            {
                throw std::runtime_error("[Error]: Invalid JSON. Expected ',' or '}' after value in object.");
            }
        }
        nextToken(); // release '}'
        return object;
    }

    std::shared_ptr<ArrayNode> parseArray()
    {
        nextToken(); // release '['
        auto array = std::make_shared<ArrayNode>();
        
        while (peekToken().type != TokenType::ArrayEnd)
        {
            array->addChild(parseValue());
            if (peekToken().type == TokenType::Comma)
            {
                nextToken(); // release ','
            }
            else if (peekToken().type != TokenType::ArrayEnd)
            {
                throw std::runtime_error("[Error]: Invalid JSON. Expected ',' or ']' after value in array.");
            }
        }
        nextToken(); // release ']'
        return array;
    }
};

} // namespace js

#endif /* PARSER_H */