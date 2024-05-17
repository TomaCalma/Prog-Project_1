#include "SVGElements.hpp"
namespace svg
{
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    Ellipse::Ellipse(const Color &fill, const Point &center, const Point &radius)
            : fill(fill), center(center), radius(radius)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    //Ellipse Transformations
    void Ellipse::translate(const Point &t)
    {
        center = center.translate(t);
    }
    void Ellipse::rotate(const Point &origin, int degrees)
    {
        center = center.rotate(origin, degrees);
    }
    void Ellipse::scale(const Point &origin, int v)
    {
        center = center.scale(origin, v);
        radius = {radius.x * v, radius.y * v};
    }

    //Circle
    Circle::Circle(const Color &fill, const Point &center, int radius)
            : Ellipse(fill, center, {radius, radius})
    {
    }
    void Circle::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }

    //Polyline
    Polyline::Polyline(const Color &stroke, const std::vector<Point> &points)
            : stroke(stroke), points(points)
    {
    }
    void Polyline::draw(PNGImage &img) const
    {
        for (size_t i = 0; i < points.size() - 1; ++i)
        {
            img.draw_line(points[i], points[i + 1], stroke);
        }
    }
    //Polyline Transformations
    void Polyline::translate(const Point &t)
    {
        for (auto &point : points)
        {
            point = point.translate(t);
        }
    }
    void Polyline::rotate(const Point &origin, int degrees)
    {
        for (auto &point : points)
        {
            point = point.rotate(origin, degrees);
        }
    }
    void Polyline::scale(const Point &origin, int v)
    {
        for (auto &point : points)
        {
            point = point.scale(origin, v);
        }
    }

    //Line
    Line::Line(const Color &stroke, const Point &start, const Point &end)
            : stroke(stroke), start(start), end(end)
    {
    }
    void Line::draw(PNGImage &img) const
    {
        img.draw_line(start, end, stroke);
    }
    //line transformations
    void Line::translate(const Point &t)
    {
        start = start.translate(t);
        end = end.translate(t);
    }
    void Line::rotate(const Point &origin, int degrees)
    {
        start = start.rotate(origin, degrees);
        end = end.rotate(origin, degrees);
    }
    void Line::scale(const Point &origin, int v)
    {
        start = start.scale(origin, v);
        end = end.scale(origin, v);
    }

    //Polygon
    Polygon::Polygon(const Color &fill, const vector<Point> &points)
            : fill(fill), points(points)
    {
    }
    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }
    //Polygon Transformations
    void Polygon::translate(const Point &t)
    {
        for (auto &point : points)
        {
            point = point.translate(t);
        }
    }
    void Polygon::rotate(const Point &origin, int degrees)
    {
        for (auto &point : points)
        {
            point = point.rotate(origin, degrees);
        }
    }
    void Polygon::scale(const Point &origin, int v)
    {
        for (auto &point : points)
        {
            point = point.scale(origin, v);
        }
    }

    //Rectangle
    Rect::Rect(const Color &fill, const Point &upper_left, int width, int height)
            : Polygon(fill, {upper_left, {upper_left.x + width - 1, upper_left.y},
            {upper_left.x + width - 1, upper_left.y + height - 1},
            {upper_left.x, upper_left.y + height - 1}})
    {
    }


}