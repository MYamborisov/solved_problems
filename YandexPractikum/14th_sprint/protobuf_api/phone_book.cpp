#include "phone_book.h"

using namespace std;

PhoneBook::PhoneBook(std::vector<Contact> contacts) {
    std::sort(contacts.begin(), contacts.end(), [](const Contact& lhs, const Contact& rhs){
        return lhs.name < rhs.name;
    });
    contacts_ = move(contacts);
}

PhoneBook::ContactRange PhoneBook::FindByNamePrefix(std::string_view name_prefix) const {
    Iterator first = &*lower_bound(contacts_.begin(), contacts_.end(), name_prefix,  [](const Contact& l, std::string_view r){
        return l.name < r;
    });
    Iterator last = &*upper_bound(contacts_.begin(), contacts_.end(), name_prefix, [](std::string_view l, const Contact& r){
        if (r.name.size() > l.size()) {
            return l < r.name.substr(0, r.name.size());
        }
        return l < r.name;
    });
    return {first, last};
}

void PhoneBook::SaveTo(std::ostream &output) const {
    phone_book_serialize::ContactList v;
    for (auto contact : contacts_) {
        phone_book_serialize::Contact temp_cnt;
        temp_cnt.set_name(contact.name);
        for (int i = 0 ; i < contact.phones.size(); ++i) {
            temp_cnt.add_phone_number(contact.phones[i]);
        }
        if (contact.birthday.has_value()) {
            phone_book_serialize::Date temp_date;
            temp_date.set_year(contact.birthday->year);
            temp_date.set_month(contact.birthday->month);
            temp_date.set_day(contact.birthday->day);
            *temp_cnt.mutable_birthday() = temp_date;
        }
        *v.add_contact() = std::move(temp_cnt);
    }
    v.SerializeToOstream(&output);
}

PhoneBook DeserializePhoneBook(std::istream& input) {
    phone_book_serialize::ContactList v;
    v.ParseFromIstream(&input);
    int size = v.contact_size();
    std::vector<Contact> result(size);
    for (int i = 0; i < size; ++i) {
        result[i].name = v.contact(i).name();
        if (v.contact(i).has_birthday()) {
            Date date;
            date.year = v.contact(i).birthday().year();
            date.month = v.contact(i).birthday().month();
            date.day = v.contact(i).birthday().day();
            result[i].birthday = date;
        }
        int phones_size = v.contact(i).phone_number_size();
        for (int j = 0 ; j < phones_size; ++j) {
            result[i].phones.push_back(v.contact(i).phone_number(j));
        }
    }
    return PhoneBook(result);
}