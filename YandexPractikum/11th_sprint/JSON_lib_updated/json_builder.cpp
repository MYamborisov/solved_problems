#include "json_builder.h"

namespace json {
    Builder::Builder() {
        nodes_stack_.push_back(&root_);
    }

    Builder &Builder::StartDict() {
        if (nodes_stack_.empty() || (!nodes_stack_.back()->IsNull() && !nodes_stack_.back()->IsArray())) {
            throw std::logic_error("Try to build before object is ready");
        }
        if (nodes_stack_.back()->IsArray()) {
            const_cast<Array &>(nodes_stack_.back()->AsArray()).push_back(Dict());
            Node* node = &const_cast<Array &>(nodes_stack_.back()->AsArray()).back();
            nodes_stack_.push_back(node);
        } else {
            *nodes_stack_.back() = Dict();
        }
        return *this;
    }

    Builder &Builder::StartArray() {
        if (nodes_stack_.empty() || (!nodes_stack_.back()->IsNull() && !nodes_stack_.back()->IsArray())) {
            throw std::logic_error("Try to build before object is ready");
        }
        if (nodes_stack_.back()->IsArray()) {
            const_cast<Array &>(nodes_stack_.back()->AsArray()).push_back(Array());
            Node* node = &const_cast<Array &>(nodes_stack_.back()->AsArray()).back();
            nodes_stack_.push_back(node);
        } else {
            *nodes_stack_.back() = Array();
        }
        return *this;
    }

    Builder &Builder::EndDict() {
        if (nodes_stack_.empty() || !nodes_stack_.back()->IsDict()) {
            throw std::logic_error("Try to build before object is ready");
        }
        nodes_stack_.erase(nodes_stack_.end() - 1);
        return *this;
    }

    Builder &Builder::EndArray() {
        if (nodes_stack_.empty() || !nodes_stack_.back()->IsArray()) {
            throw std::logic_error("Try to build before object is ready");
        }
        nodes_stack_.erase(nodes_stack_.end() - 1);
        return *this;
    }

    Builder &Builder::Key(std::string key) {
        if (nodes_stack_.empty() || !nodes_stack_.back()->IsDict()) {
            throw std::logic_error("Try to build before object is ready");
        }
        nodes_stack_.emplace_back(&const_cast<Dict&>(nodes_stack_.back()->AsDict())[key]);
        return *this;
    }

    Builder &Builder::Value(Node value) {
        if (nodes_stack_.empty() || (!nodes_stack_.back()->IsNull() && !nodes_stack_.back()->IsArray())) {
            throw std::logic_error("Try to build before object is ready");
        }
        if (nodes_stack_.back()->IsArray()) {
            const_cast<Array &>(nodes_stack_.back()->AsArray()).push_back(value);
        } else {
            *nodes_stack_.back() = value;
            nodes_stack_.erase(nodes_stack_.end() - 1);
        }
        return *this;
    }

    Node Builder::Build() {
        if (!nodes_stack_.empty()) {
            throw std::logic_error("Try to build before object is ready");
        }
        return root_;
    }
}
