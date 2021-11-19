#pragma once

#include "driving_licence.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

extern const vtable vtable_InternationalDrivingLicence;

class InternationalDrivingLicence : public DrivingLicence {
public:
    InternationalDrivingLicence() : DrivingLicence(&vtable_InternationalDrivingLicence){
        std::cout << "InternationalDrivingLicence::Ctor()"sv << std::endl;
    }

    InternationalDrivingLicence(const InternationalDrivingLicence& other)
            : DrivingLicence(other)
    {
        std::cout << "InternationalDrivingLicence::CCtor()"sv << std::endl;
    }

    ~InternationalDrivingLicence() {
        std::cout << "InternationalDrivingLicence::Dtor()"sv << std::endl;
    }

    void PrintID() const {
        std::cout << "InternationalDrivingLicence::PrintID() : "sv << GetID() << std::endl;
    }

    void Delete() {
        delete this;
    }
};

const vtable vtable_InternationalDrivingLicence(
        static_cast<void (IdentityDocument::*)() const>(&InternationalDrivingLicence::PrintID), static_cast<void (IdentityDocument::*)()>(&InternationalDrivingLicence::Delete));
