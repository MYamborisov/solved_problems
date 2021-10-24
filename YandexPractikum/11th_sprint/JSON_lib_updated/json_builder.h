#pragma once

#include "json.h"

namespace json {

    class Builder {
    public:
        Builder();

        Builder& StartDict();
        Builder& StartArray();

        Builder& EndDict();
        Builder& EndArray();

        Builder& Key(std::string key);
        Builder& Value(Node value);

        Node Build();
    private:
        Node root_;
        std::vector<Node*> nodes_stack_;
    };

}
