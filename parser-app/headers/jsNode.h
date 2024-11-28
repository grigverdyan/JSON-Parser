#ifndef JSNODE_H
#define JSNODE_H

#include <string>
#include <variant>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <iostream>

namespace js
{

// JSON Node interface
struct Node
{
    virtual ~Node() = default;
    // Keep rule of 5
    // Node(const Node&) = delete;
    // Node& operator=(const Node&) = delete;
    // Node(Node&&) = delete;
    // Node& operator=(Node&&) = delete;

    virtual std::string stringify() const = 0;
    virtual void print() const
    {
        std::cout << stringify() << std::endl;
    }
};

class ValueNode : public Node
{
public:
    using Value = std::variant<bool, int, std::string, std::nullptr_t>;
private:
    Value value_;
public:
    ValueNode() = default;
    virtual ~ValueNode() = default;

    // Construct JSON Value by its value.
    ValueNode(Value value) 
        : value_(value) {}

    // Get JSON Value and return it.
    const Value& getValue() const
    {
        return value_;
    }

    // Serialize JSON Value to JSON string and return it.
    virtual std::string stringify() const override
    {
        return std::visit(
            [](auto&& arg) -> std::string
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, bool>)
                {
                    return arg ? "true" : "false";
                }
                else if constexpr (std::is_same_v<T, int>)
                {
                    return std::to_string(arg);
                }
                else if constexpr (std::is_same_v<T, std::string>)
                {
                    return "\"" + arg + "\"";
                }
                else return "null";
            }, value_
        );
    }

};

class ObjectNode : public Node
{ 
public:
    using Object = std::unordered_map<std::string, std::shared_ptr<Node>>;
private:
    Object children_;
public:
    ObjectNode() = default;

    // Add Object Value by its attribute name and value itself.
    void addChild(const std::string& key, std::shared_ptr<Node> child)
    {
        children_[key] = child;
    }

    // Get JSON Object Value by its attribute name.
    std::shared_ptr<Node> getChild(const std::string& key) const
    {
        auto it = children_.find(key);
        return it != children_.end() ? it->second : nullptr;
    }

    // Serialize JSON Object Node to JSON string and return it.
    virtual std::string stringify() const override
    {
        std::string resultJson = "{";
        bool first = true;
        for (const auto& [key, child] : children_)
        {
            if (!first)
                resultJson += ",";
            resultJson += "\"" + key + "\":" + child->stringify();
            first = false;
        }
        resultJson += "}";
        return resultJson;
    }
};

class ArrayNode : public Node
{
public:
    using Array = std::vector<std::shared_ptr<Node>>;
private:
    Array children_;
public:
    ArrayNode() = default;

    // Add JSON Array Value.
    void addChild(std::shared_ptr<Node> child)
    {
        children_.push_back(child);
    }

    // Get JSON Array Value by its index and return it.
    std::shared_ptr<Node> getChild(size_t index) const
    {
        return index < children_.size() ? children_[index] : nullptr;
    }

    // Serialize JSON Array to JSON string and return it.
    std::string stringify() const override
    {
        std::string resultJson = "[";
        bool first = true;
        std::for_each(
            children_.begin(), children_.end(),
            [&](const auto& child)
            {
                if (!first)
                    resultJson += ",";
                resultJson += child->stringify();
                first = false;
            }
        );
        // resultJson.pop_back(); // remove last "," // redundant ? need to test
        resultJson += "]";
        return resultJson;
    }
};


} // namespace js

#endif /* JSNODE_H */