#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class DrivingLicence : public IdentityDocument {
public:
    DrivingLicence() {
        base_.vtable.print_id = reinterpret_cast<DrivingLicence::*()>(&DrivingLicence::PrintID_);
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }

    DrivingLicence(const DrivingLicence& other)
            : IdentityDocument(other)
    {
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }

    ~DrivingLicence() {
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
    }

    void PrintID() const {
        base_.vtable.print_id;
    }

    void PrintID_() const {
        std::cout << "DrivingLicence::PrintID() : "sv << GetID() << std::endl;
    }

    IdentityDocument base_;
};