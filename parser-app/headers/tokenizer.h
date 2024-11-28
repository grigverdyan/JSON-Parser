#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <iostream>

class Tokenizer
{
public:
    enum class TokenType
    {
        ObjectStart,    // {
        ObjectEnd,      // }
        ArrayStart,     // [
        ArrayEnd,       // ]
        String,         // " "
        Number,         // integers, floats, etc
        Boolean,        // true || false
        Comma,          // ,
        Colon,          // :
        Null            // null
    };

    struct Token
    {
        TokenType type;
        std::string value;
    };

private:
    std::string json_;
    size_t index_;
public:
    explicit Tokenizer(const std::string& json)
                : json_(json), index_(0) {}

    std::vector<Token> tokenize()
    {
        std::vector<Token> tokens;
        size_t jsonSize = json_.size();
        while (index_ < jsonSize)
        {
            skipWhiteSpaces();
            if (index_ >= jsonSize)
                break;
            
            char currentChar = json_[index_];
            if (currentChar == '{')
            {
                tokens.push_back({TokenType::ObjectStart, "{"});
                ++index_;
            }
            else if (currentChar == '}')
            {
                tokens.push_back({TokenType::ObjectEnd, "}"});
                ++index_;
            }
            else if (currentChar == '[')
            {
                tokens.push_back({TokenType::ArrayStart, "["});
                ++index_;
            }
            else if (currentChar ==  ']')
            {
                tokens.push_back({TokenType::ArrayEnd, "]"});
                ++index_;
            }
            else if (currentChar ==  ':')
            {
                tokens.push_back({TokenType::Colon, ":"});
                ++index_;
            }
            else if (currentChar == ',')
            {
                tokens.push_back({TokenType::Comma, ","});
                ++index_;
            }
            else if (currentChar == '"')
            {
                tokens.push_back({TokenType::String, std::move(parseString())});
            }
            else if (std::strcmp(json_.substr(index_, std::strlen("true")).c_str(), "true") == 0)
            {
                tokens.push_back({TokenType::Boolean, "true"});
                index_ += std::strlen("true");
            }
            else if (std::strcmp(json_.substr(index_, std::strlen("false")).c_str(), "false") == 0)
            {
                tokens.push_back({TokenType::Boolean, "false"});
                index_ += std::strlen("false");
            }
            else if (std::strcmp(json_.substr(index_, std::strlen("null")).c_str(), "null") == 0)
            {
                tokens.push_back({TokenType::Null, "null"});
                index_ += std::strlen("null");
            }
            else if (std::isdigit(currentChar) || currentChar == '-')
            {
                tokens.push_back({TokenType::Number, std::move(parseNumber())});
            }
            else 
            {
                throw std::runtime_error("[Error]: Invalid JSON. Unexpected character found -> " + std::string(1, currentChar));
            }
        }
    }

private:
    void    skipWhiteSpaces()
    {
        while (index_ < json_.size() && std::isspace(json_[index_]))
        {
            ++index_;
        }
    }

    std::string& parseString()
    {
        std::string result;
        ++index_;
        auto jsonSize = json_.size();
        while (index_ < jsonSize && json_[index_] != '"')
        {
            result += json_[index_];
            ++index_;
        }
        
        if (index_ >= jsonSize || json_[index_] != '"')
            throw std::runtime_error("[Error]: Invalid JSON. Missing closing double quote(\") for string.");
        ++index_;
        return result;
    }

    std::string parseNumber()
    {
        std::string result;
        auto jsonSize = json_.size();
        while (index_ < jsonSize && (std::isdigit(json_[index_]) 
                                    || json_[index_] == '-' 
                                    || json_[index_] == '.'
                                    || json_[index_] == 'e'
                                    || json_[index_] == 'E'))
        {
            result += json_[index_];
            ++index_;
        }
        return result;
    }
};

#endif /* TOKENIZER_H */