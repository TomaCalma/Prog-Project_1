#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
            : fill(fill), center(center), radius(radius)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }

    // Circle
    Circle::Circle(const Color &fill, const Point &center, int radius)
            : Ellipse(fill, center, {radius, radius})
    {
    }
    void Circle::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }

    // Polyline
    Polyline::Polyline(const Color &stroke, const std::vector<Point> &points)
            : stroke(stroke), points(points)
    {
    }
    void Polyline::draw(PNGImage &img) const
    {
        // Desenhe a polyline ponto a ponto
        for (size_t i = 0; i < points.size() - 1; ++i)
        {
            img.draw_line(points[i], points[i + 1], stroke);
        }
    }


    //line
    Line::Line(const Color &stroke, const Point &start, const Point &end)
            : stroke(stroke), start(start), end(end)
    {
    }

    void Line::draw(PNGImage &img) const
    {
        img.draw_line(start, end, stroke);
    }

    // Polygon
    Polygon::Polygon(const Color &fill, const vector<Point> &points)
            : fill(fill), points(points)
    {
    }
    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }


}