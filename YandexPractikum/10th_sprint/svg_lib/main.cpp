#define _USE_MATH_DEFINES
#include "svg.h"

#include <fstream>
#include <cmath>

using namespace std::literals;
using namespace svg;

namespace shapes {

    class Star : public Drawable {
    public:
        Star(svg::Point center, double outer_radius, double inner_radius, int num_rays) : center_(center), outer_radius_(outer_radius), inner_radius_(inner_radius), num_rays_(num_rays) {}

        void Draw(ObjectContainer& container) const override {
            Polyline polyline;
            for (int i = 0; i <= num_rays_; ++i) {
                double angle = 2 * M_PI * (i % num_rays_) / num_rays_;
                polyline.AddPoint({center_.x + outer_radius_ * sin(angle), center_.y - outer_radius_ * cos(angle)});
                if (i == num_rays_) {
                    break;
                }
                angle += M_PI / num_rays_;
                polyline.AddPoint({center_.x + inner_radius_ * sin(angle), center_.y - inner_radius_ * cos(angle)});
            }
            container.Add(polyline.SetFillColor("red"s)
                                      .SetStrokeColor("black"s));
        }
    private:
        svg::Point center_;
        double outer_radius_;
        double inner_radius_;
        int num_rays_;
    };

    class Snowman : public Drawable {
    public:
        Snowman(svg::Point head_center, double head_radius) : head_center_(head_center), head_radius(head_radius) {}

        void Draw(ObjectContainer& container) const override {
            container.Add(Circle().SetCenter({head_center_.x, head_center_.y + head_radius * 5})
                                      .SetRadius(2 * head_radius)
                                      .SetFillColor("rgb(240,240,240)"s)
                                      .SetStrokeColor("black"s));
            container.Add(Circle().SetCenter({head_center_.x, head_center_.y + head_radius * 2})
                                      .SetRadius(1.5 * head_radius)
                                      .SetFillColor("rgb(240,240,240)"s)
                                      .SetStrokeColor("black"s));
            container.Add(Circle().SetCenter(head_center_)
                                      .SetRadius(head_radius)
                                      .SetFillColor("rgb(240,240,240)"s)
                                      .SetStrokeColor("black"s));
        }
    private:
        svg::Point head_center_;
        double head_radius;
    };

    class Triangle : public svg::Drawable {
    public:
        Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
                : p1_(p1)
                , p2_(p2)
                , p3_(p3) {
        }

        // ?????????????????? ?????????? Draw ???????????????????? svg::Drawable
        void Draw(svg::ObjectContainer& container) const override {
            container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
        }

    private:
        svg::Point p1_, p2_, p3_;
    };

} //namespace shapes

template <typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template <typename Container>
void DrawPicture(const Container& container, svg::ObjectContainer& target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}

// ?????????????????? ???????????????? ???????????????????????? ???????????????? ???? from ???? to ?? ?????????????????????? ???? ?????????????????? t
uint8_t Lerp(uint8_t from, uint8_t to, double t) {
    return static_cast<uint8_t>(std::round((to - from) * t + from));
}

// ?????????????????? ???????????????? ???????????????????????? Rgb ?????????? ???? from ???? to ?? ?????????????????????? ???? ?????????????????? t
svg::Rgb Lerp(svg::Rgb from, svg::Rgb to, double t) {
    return {Lerp(from.red, to.red, t), Lerp(from.green, to.green, t), Lerp(from.blue, to.blue, t)};
}

int main() {
    using namespace svg;
    using namespace std;

    Rgb start_color{0, 255, 30};
    Rgb end_color{20, 20, 150};

    const int num_circles = 10;
    Document doc;
    for (int i = 0; i < num_circles; ++i) {
        const double t = double(i) / (num_circles - 1);

        const Rgb fill_color = Lerp(start_color, end_color, t);

        doc.Add(Circle()
                        .SetFillColor(fill_color)
                        .SetStrokeColor("black"s)
                        .SetCenter({i * 20.0 + 40, 40.0})
                        .SetRadius(15));
    }
    doc.Render(cout);
}