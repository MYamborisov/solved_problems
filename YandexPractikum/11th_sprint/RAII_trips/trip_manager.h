#pragma once
#include "flight_provider.h"
#include "hotel_provider.h"

#include <string>
#include <vector>

class Trip {
public:
    std::vector<HotelProvider::BookingId> hotels;
    std::vector<FlightProvider::BookingId> flights;

    Trip(HotelProvider& hp, FlightProvider& fp) : hotel_provider_(hp), flight_provider_(fp) {}
    Trip(const Trip& trip) : hotels(trip.hotels), flights(trip.flights), hotel_provider_(trip.hotel_provider_), flight_provider_(trip.flight_provider_) {}
    Trip(Trip&& trip) : hotels(std::move(trip.hotels)), flights(std::move(trip.flights)), hotel_provider_(trip.hotel_provider_), flight_provider_(trip.flight_provider_) {}
    Trip& operator=(const Trip& trip) {
        hotel_provider_ = trip.hotel_provider_;
        flight_provider_ = trip.flight_provider_;
        hotels = trip.hotels;
        flights = trip.flights;
        return *this;
    }
    Trip& operator=(Trip&& trip) {
        hotel_provider_ = trip.hotel_provider_;
        flight_provider_ = trip.flight_provider_;
        hotels = move(trip.hotels);
        flights = move(trip.flights);
        return *this;
    }
    void Cancel() {
        // отменяем бронирования у провайдеров
        for (auto& id : hotels) {
            hotel_provider_.Cancel(id);
        }
        // чистим вектора в структуре trip
        hotels.clear();
        for (auto& id : flights) {
            flight_provider_.Cancel(id);
        }
        flights.clear();
    }
    ~Trip() {
        Cancel();
    }

private:
    HotelProvider& hotel_provider_;
    FlightProvider& flight_provider_;
};

class TripManager {
public:
    using BookingId = std::string;
    struct BookingData {
        std::string city_from;
        std::string city_to;
        std::string date_from;
        std::string date_to;
    };

    Trip Book(const BookingData& data) {
        Trip trip(hotel_provider_, flight_provider_);
        {
            FlightProvider::BookingData flight_booking_data;
            flight_booking_data.city_from = data.city_from;
            flight_booking_data.city_to = data.city_to;
            flight_booking_data.date = data.date_from;
            trip.flights.push_back(flight_provider_.Book(flight_booking_data));
        }
        {
            HotelProvider::BookingData hotel_booking_data;
            hotel_booking_data.date_from = data.date_from;
            hotel_booking_data.city = data.city_to;
            hotel_booking_data.date_to = data.date_to;
            trip.hotels.push_back(hotel_provider_.Book(hotel_booking_data));
        }
        {
            FlightProvider::BookingData flight_booking_data;
            flight_booking_data.city_from = data.city_to;
            flight_booking_data.city_to = data.city_from;
            flight_booking_data.date = data.date_to;
            trip.flights.push_back(flight_provider_.Book(flight_booking_data));
        }
        return trip;
    }

    void Cancel(Trip& trip) {
        trip.Cancel();
    }

private:
    HotelProvider hotel_provider_;
    FlightProvider flight_provider_;
};