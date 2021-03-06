#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

class House {
public:
    explicit House(int length, int width, int height) : length_(length), width_(width), height_(height) {}
    int GetLength() const {
        return length_;
    }
    int GetWidth() const {
        return width_;
    }
    int GetHeight() const {
        return height_;
    }
private:
    int length_;
    int width_;
    int height_;
};

class Resources {
public:
    explicit Resources(int brick_count) : brick_count_(brick_count) {}
    void TakeBricks(int count) {
        if (count < 0 || brick_count_ - count < 0) {
            throw out_of_range("Num of bricks < 0, or > brick resources");
        }
        brick_count_ -= count;
    }
    int GetBrickCount() const {
        return brick_count_;
    }
private:
    int brick_count_ = 0;
};

struct HouseSpecification {
    int length = 0;
    int width = 0;
    int height = 0;
};

class Builder {
public:
    explicit Builder(Resources& resources) : resources_(resources) {}
    House BuildHouse(HouseSpecification spec) {
        int brick_needs = (spec.length + spec.width) * 2 * spec.height * 32;
        if (resources_.GetBrickCount() < brick_needs) {
            throw runtime_error("Brick needs higher than resources");
        }
        resources_.TakeBricks(brick_needs);
        return House{spec.length, spec.width, spec.height};
    }
private:
    Resources& resources_;
};

int main() {
    Resources resources{10000};
    Builder builder1{resources};
    Builder builder2{resources};

    House house1 = builder1.BuildHouse(HouseSpecification{12, 9, 3});
    assert(house1.GetLength() == 12);
    assert(house1.GetWidth() == 9);
    assert(house1.GetHeight() == 3);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    House house2 = builder2.BuildHouse(HouseSpecification{8, 6, 3});
    assert(house2.GetLength() == 8);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    House banya = builder1.BuildHouse(HouseSpecification{4, 3, 2});
    assert(banya.GetHeight() == 2);
    cout << resources.GetBrickCount() << " bricks left"s << endl;
}