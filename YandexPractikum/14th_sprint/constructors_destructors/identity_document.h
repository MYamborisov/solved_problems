#pragma once

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class IdentityDocument;
struct VTable {
    void (IdentityDocument::*print_id)() const;
};

class IdentityDocument {
public:
    IdentityDocument()
            : unique_id_(++unique_id_count_)
    {
        vtable.print_id = &IdentityDocument::PrintID_;
        std::cout << "IdentityDocument::Ctor() : "sv << unique_id_ << std::endl;
    }

    ~IdentityDocument() {
        --unique_id_count_;
        std::cout << "IdentityDocument::Dtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument(const IdentityDocument& other)
            : unique_id_(++unique_id_count_)
    {
        std::cout << "IdentityDocument::CCtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument& operator=(const IdentityDocument&) = delete;

    void PrintID() const {
        vtable.print_id;
    }

    void PrintID_() const {
        std::cout << "IdentityDocument::PrintID() : "sv << unique_id_ << std::endl;
    }

    static void PrintUniqueIDCount() {
        std::cout << "unique_id_count_ : "sv << unique_id_count_ << std::endl;
    }

    VTable vtable;

    int GetID() const {
        return unique_id_;
    }

private:
    static int unique_id_count_;
    int unique_id_;
};

int IdentityDocument::unique_id_count_ = 0;