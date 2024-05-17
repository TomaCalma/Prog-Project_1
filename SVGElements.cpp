#include "SVGElements.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI acos(-1.0)
#endif

namespace svg {

    void SVGElement::applyTransform(PNGImage &img) const {
        // Implementação da função aqui
    }

    // Definição da função rotate
    void SVGElement::rotate(int angle) {
        // Implementação da função aqui
    }


    /**
     * @file SVGElements.hpp
     * @brief Define as classes para elementos SVG.
     */

    // Definição da classe SVGElement
    /**
     * @brief Classe base para elementos SVG.
     *
     * Esta classe serve como base para os elementos SVG.
     */
    SVGElement::SVGElement() {} // Construtor padrão

    /**
     * @brief Destrutor virtual para a classe SVGElement.
     */
    SVGElement::~SVGElement() {}

    

    // Definição da classe Ellipse
    /**
     * @class Ellipse
     * @brief Representa uma elipse SVG.
     *
     * Esta classe representa uma elipse SVG e é derivada da classe SVGElement.
     */
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
            : fill(fill), center(center), radius(radius) {}

    /**
     * @brief Desenha a elipse.
     *
     * @param img A imagem PNG onde a elipse será desenhada.
     */
    void Ellipse::draw(PNGImage &img) const {
        img.draw_ellipse(center, radius, fill);
    }

    // Definição da classe Circle
    /**
     * @class Circle
     * @brief Representa um círculo SVG.
     *
     * Esta classe representa um círculo SVG e é derivada da classe Ellipse.
     */
    Circle::Circle(const Color &fill, const Point &center, int radius)
            : Ellipse(fill, center, {radius, radius}) {}

    /**
     * @brief Desenha o círculo.
     *
     * @param img A imagem PNG onde o círculo será desenhado.
     */
    void Circle::draw(PNGImage &img) const {
        img.draw_ellipse(center, radius, fill);
    }

    // Definição da classe Polyline
    /**
     * @class Polyline
     * @brief Representa uma polilinha SVG.
     *
     * Esta classe representa uma polilinha SVG e é derivada da classe SVGElement.
     */
    Polyline::Polyline(const Color &stroke, const std::vector<Point> &points)
            : stroke(stroke), points(points) {}

    /**
     * @brief Desenha a polilinha.
     *
     * @param img A imagem PNG onde a polilinha será desenhada.
     */
    void Polyline::draw(PNGImage &img) const {
        // Desenha a polilinha ponto a ponto
        for (size_t i = 0; i < points.size() - 1; ++i) {
            img.draw_line(points[i], points[i + 1], stroke);
        }
    }

    // Definição da classe Line
    /**
     * @class Line
     * @brief Representa uma linha SVG.
     *
     * Esta classe representa uma linha SVG e é derivada da classe SVGElement.
     */
    Line::Line(const Color &stroke, const Point &start, const Point &end)
            : stroke(stroke), start(start), end(end) {}

    /**
     * @brief Desenha a linha.
     *
     * @param img A imagem PNG onde a linha será desenhada.
     */
    void Line::draw(PNGImage &img) const {
        img.draw_line(start, end, stroke);
    }

    // Definição da classe Polygon
    /**
     * @class Polygon
     * @brief Representa um polígono SVG.
     *
     * Esta classe representa um polígono SVG e é derivada da classe SVGElement.
     */
    Polygon::Polygon(const Color &fill, const std::vector<Point> &points)
            : fill(fill), points(points) {}

    /**
     * @brief Desenha o polígono.
     *
     * @param img A imagem PNG onde o polígono será desenhado.
     */
    void Polygon::draw(PNGImage &img) const {
        img.draw_polygon(points, fill);
    }

    // Definição da classe Rect
    /**
     * @class Rect
     * @brief Representa um retângulo SVG.
     *
     * Esta classe representa um retângulo SVG e é derivada da classe Polygon.
     */
    Rect::Rect(const Color &fill, const Point &upper_left, int width, int height)
            : Polygon(fill, {upper_left, {upper_left.x + width, upper_left.y}, {upper_left.x + width, upper_left.y + height}, {upper_left.x, upper_left.y + height}}) {}

} 
