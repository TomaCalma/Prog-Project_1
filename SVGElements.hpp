//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"

using namespace std;

namespace svg
{
        
    /**
     * @class SVGElement
     * @brief Classe base para elementos SVG.
     *
     * Esta classe serve como base para os elementos SVG.
     */
    class SVGElement
    {

    public:
        /**
         * @brief Construtor padrão da classe SVGElement.
         */
        SVGElement();

        /**
         * @brief Destrutor virtual para a classe SVGElement.
         */
        virtual ~SVGElement();

        virtual void translate(const Point &t) {}
        virtual void rotate(const Point &origin, int degrees) {}
        virtual void scale(const Point &origin, int v) {}

        /**
         * @brief Função virtual pura para desenhar o elemento SVG.
         *
         * @param img A imagem PNG onde o elemento será desenhado.
         */
        virtual void draw(PNGImage &img) const = 0;

    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    
    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);

    void convert(const std::string &svg_file,
                 const std::string &png_file);
    
    /**
     * @class Ellipse
     * @brief Representa uma elipse SVG.
     *
     * Esta classe representa uma elipse SVG e é derivada da classe SVGElement.
     */
    class Ellipse : public SVGElement
    {
    public:
        /**
         * @brief Construtor da classe Ellipse.
         *
         * @param fill A cor de preenchimento da elipse.
         * @param center O centro da elipse.
         * @param radius O raio da elipse.
         */
        Ellipse(const Color &fill, const Point &center, const Point &radius);

        /**
         * @brief Desenha a elipse.
         *
         * @param img A imagem PNG onde a elipse será desenhada.
         */
        void draw(PNGImage &img) const override;

    protected:
        Color fill;
        Point center; 
        Point radius; 
    };

     /**
     * @class Circle
     * @brief Representa um círculo SVG.
     *
     * Esta classe representa um círculo SVG e é derivada da classe Ellipse.
     */
    class Circle : public Ellipse
    {
    public:
        /**
         * @brief Construtor da classe Circle.
         *
         * @param fill A cor de preenchimento do círculo.
         * @param center O centro do círculo.
         * @param radius O raio do círculo.
         */
        Circle(const Color &fill, const Point &center, int radius);

        /**
         * @brief Desenha o círculo.
         *
         * @param img A imagem PNG onde o círculo será desenhado.
         */
        void draw(PNGImage &img) const override;

        void translate(const Point &t) override {
            center = center.translate(t);
        }

        void rotate(const Point &origin, int degrees) override {
            center = center.rotate(origin, degrees);
        }

        void scale(const Point &origin, int v) override {
            center = center.scale(origin, v);
            radius = { radius.x * v, radius.y * v }; // Multiplica os componentes do raio
        }


    };

    /**
     * @class Polyline
     * @brief Representa uma polilinha SVG.
     *
     * Esta classe representa uma polilinha SVG e é derivada da classe SVGElement.
     */
    class Polyline : public SVGElement
    {
    public:
                /**
         * @brief Construtor da classe Polyline.
         *
         * @param stroke A cor da linha da polilinha.
         * @param points Os pontos da polilinha.
         */
        Polyline(const Color &stroke, const std::vector<Point> &points);

        /**
         * @brief Desenha a polilinha.
         *
         * @param img A imagem PNG onde a polilinha será desenhada.
         */
        void draw(PNGImage &img) const override;

    protected:
        Color stroke; 
        std::vector<Point> points; 
    };

    /**
     * @class Line
     * @brief Representa uma linha SVG.
     *
     * Esta classe representa uma linha SVG e é derivada da classe SVGElement.
     */
    class Line : public SVGElement
    {
    public:
        /**
         * @brief Construtor da classe Line.
         *
         * @param stroke A cor da linha.
         * @param start O ponto de início da linha.
         * @param end O ponto de término da linha.
         */
        Line(const Color &stroke, const Point &start, const Point &end);

        /**
         * @brief Desenha a linha.
         *
         * @param img A imagem PNG onde a linha será desenhada.
         */
        void draw(PNGImage &img) const override;

    protected:
        Color stroke; 
        Point start; 
        Point end; 
    };

    /**
     * @class Polygon
     * @brief Representa um polígono SVG.
     *
     * Esta classe representa um polígono SVG e é derivada da classe SVGElement.
     */
    class Polygon : public SVGElement
    {   
    public:
        /**
         * @brief Construtor da classe Polygon.
         *
         * @param fill A cor de preenchimento do polígono.
         * @param points Os pontos que formam o polígono.
         */
        Polygon(const Color &fill, const vector<Point> &points);

        /**
         * @brief Desenha o polígono.
         *
         * @param img A imagem PNG onde o polígono será desenhado.
         */
        void draw(PNGImage &img) const override;

    private:
        Color fill; 
        vector<Point> points; 
    };

    /**
     * @class Rect
     * @brief Representa um retângulo SVG.
     *
     * Esta classe representa um retângulo SVG e é derivada da classe Polygon.
     */
    class Rect : public Polygon
    {
    public:
        /**
         * @brief Construtor da classe Rect.
         *
         * @param fill A cor de preenchimento do retângulo.
         * @param upper_left O ponto de canto superior esquerdo do retângulo.
         * @param width A largura do retângulo.
         * @param height A altura do retângulo.
         */
        Rect(const Color &fill, const Point &upper_left, int width, int height);
    };

} 

#endif // __svg_SVGElements_hpp__

         
