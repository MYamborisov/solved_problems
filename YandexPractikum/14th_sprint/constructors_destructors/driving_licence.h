#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

extern const vtable vtable_DrivingLicence;

class DrivingLicence : public IdentityDocument {
public:
    DrivingLicence() : IdentityDocument(&vtable_DrivingLicence) {
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }

    DrivingLicence(const DrivingLicence& other)
            : IdentityDocument(other)
    {
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }

    DrivingLicence(const vtable* vtable_ptr) : IdentityDocument(vtable_ptr) {
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }

    ~DrivingLicence() {
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
    }

    void PrintID() const {
        std::cout << "DrivingLicence::PrintID() : "sv << GetID() << std::endl;
    }

    void Delete() {
        delete this;
    }
};

const vtable vtable_DrivingLicence(
        static_cast<void (IdentityDocument::*)() const>(&DrivingLicence::PrintID), static_cast<void (IdentityDocument::*)()>(&DrivingLicence::Delete));
