#pragma once

//#include "new_flight_provider.h"
//#include "new_hotel_provider.h"

namespace raii {

    template<typename Provider>
    class Booking {
    private:
        using BookingId = typename Provider::BookingId;

        Provider *provider_;
        int counter_;

    public:
        Booking(Provider *p, int counter) : provider_(p), counter_(counter) {}

        Booking(const Booking & booking) : provider_(booking.provider_), counter_(booking.counter_) {}

        Booking(Booking &&other) = default;

        Booking &operator=(const Booking &) = default;

        Booking &operator=(Booking &&other) = default;

        ~Booking() {
            if (provider_) {
                provider_->CancelOrComplete(*this);
            }
        }

        // Эта функция не требуется в тестах, но в реальной программе она может быть нужна
        BookingId GetId() const;
    };
}