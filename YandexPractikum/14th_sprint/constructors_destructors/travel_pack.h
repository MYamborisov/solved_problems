#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

extern const vtable vtable_TravelPack;

class TravelPack : public IdentityDocument {
public:
    TravelPack()
            : IdentityDocument(&vtable_TravelPack)
            , identity_doc1_(new Passport())
            , identity_doc2_(new DrivingLicence())
    {
        std::cout << "TravelPack::Ctor()"sv << std::endl;
    }

    TravelPack(const TravelPack& other)
            : IdentityDocument(other)
            , identity_doc1_(new Passport(*static_cast<Passport*>(other.identity_doc1_)))
            , identity_doc2_(new DrivingLicence(*static_cast<DrivingLicence*>(other.identity_doc2_)))
            , additional_pass_(other.additional_pass_)
            , additional_dr_licence_(other.additional_dr_licence_)
    {
        std::cout << "TravelPack::CCtor()"sv << std::endl;
    }

    ~TravelPack() {
        delete identity_doc1_;
        delete identity_doc2_;
        std::cout << "TravelPack::Dtor()"sv << std::endl;
    }

    void PrintID() const {
        identity_doc1_->PrintID();
        identity_doc2_->PrintID();
        additional_pass_.PrintID();
        additional_dr_licence_.PrintID();
    }

    void Delete() {
        delete this;
    }

private:
    IdentityDocument* identity_doc1_;
    IdentityDocument* identity_doc2_;
    Passport additional_pass_;
    DrivingLicence additional_dr_licence_;
};

const vtable vtable_TravelPack(
        static_cast<void (IdentityDocument::*)() const>(&TravelPack::PrintID), static_cast<void (IdentityDocument::*)()>(&TravelPack::Delete));
