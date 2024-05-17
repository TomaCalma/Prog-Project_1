#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <cstring>
#include <sstream>

using namespace std;
using namespace tinyxml2;

namespace svg
{
    /**
     * @brief Função para ler um arquivo SVG.
     *
     * Esta função lê um arquivo SVG e cria os elementos SVG correspondentes.
     *
     * @param svg_file O caminho do arquivo SVG a ser lido.
     * @param dimensions As dimensões do arquivo SVG.
     * @param svg_elements Um vetor de ponteiros para elementos SVG.
     */
    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Não foi possível carregar " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");

        XMLElement *child = xml_elem->FirstChildElement();
        while (child != nullptr)
        {
            if (strcmp(child->Name(), "circle") == 0)
            {
                float cx = child->FloatAttribute("cx");
                float cy = child->FloatAttribute("cy");
                float r = child->FloatAttribute("r");
                const char *fill_color = child->Attribute("fill");
                svg_elements.push_back(new Circle(parse_color(fill_color), {static_cast<int>(cx), static_cast<int>(cy)}, static_cast<int>(r)));
            }
            else if (strcmp(child->Name(), "ellipse") == 0)
            {
                float cx = child->FloatAttribute("cx");
                float cy = child->FloatAttribute("cy");
                float rx = child->FloatAttribute("rx");
                float ry = child->FloatAttribute("ry");
                const char *fill_color = child->Attribute("fill");
                svg_elements.push_back(new Ellipse(parse_color(fill_color), {static_cast<int>(cx), static_cast<int>(cy)}, {static_cast<int>(rx), static_cast<int>(ry)}));
            }

            else if (strcmp(child->Name(), "polyline") == 0)
            {
                const char *points_str = child->Attribute("points");
                const char *stroke_color = child->Attribute("stroke");
                vector<Point> points;
                stringstream ss(points_str);
                string point;
                while (getline(ss, point, ' '))
                {
                    stringstream ss_point(point);
                    string x_str, y_str;
                    getline(ss_point, x_str, ',');
                    getline(ss_point, y_str, ',');
                    int x = stoi(x_str);
                    int y = stoi(y_str);
                    points.push_back({x, y});
                }
                svg_elements.push_back(new Polyline(parse_color(stroke_color), points));
            }

            else if (strcmp(child->Name(), "line") == 0)
            {
                float x1 = child->FloatAttribute("x1");
                float y1 = child->FloatAttribute("y1");
                float x2 = child->FloatAttribute("x2");
                float y2 = child->FloatAttribute("y2");
                const char *stroke_color = child->Attribute("stroke");
                svg_elements.push_back(new Line(parse_color(stroke_color), {static_cast<int>(x1), static_cast<int>(y1)}, {static_cast<int>(x2), static_cast<int>(y2)}));
            }

            else if (strcmp(child->Name(), "polygon") == 0)
            {
                const char *points_str = child->Attribute("points");
                const char *fill_color = child->Attribute("fill");
                vector<Point> points;
                stringstream ss(points_str);
                string point;
                while (getline(ss, point, ' '))
                {
                    stringstream ss_point(point);
                    string x_str, y_str;
                    getline(ss_point, x_str, ',');
                    getline(ss_point, y_str, ',');
                    int x = stoi(x_str);
                    int y = stoi(y_str);
                    points.push_back({x, y});
                }
                svg_elements.push_back(new Polygon(parse_color(fill_color), points));
            }

            else if (strcmp(child->Name(), "rect") == 0)
            {
                float x = child->FloatAttribute("x");
                float y = child->FloatAttribute("y");
                float width = child->FloatAttribute("width");
                float height = child->FloatAttribute("height");
                const char *fill_color = child->Attribute("fill");
                
                svg_elements.push_back(new Rect(parse_color(fill_color), {static_cast<int>(x), static_cast<int>(y)}, static_cast<int>(width), static_cast<int>(height)));
            }

            
            child = child->NextSiblingElement();
        }
    }
}

