#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <cstring>
#include <sstream>
using namespace std;
using namespace tinyxml2;
namespace svg
{

    void applyTransformation(SVGElement* child, const char* transform_attr) {

        // Verifique se a string de transformação está vazia
        if (transform_attr == nullptr || strlen(transform_attr) == 0)
            return;

        stringstream ss(transform_attr);
        string transform;
        while (ss >> transform) {
            if (transform.substr(0, 9) == "translate") {
                float tx, ty;
                char ch;
                ss >> ch >> tx >> ty >> ch;
                Point translation = {static_cast<int>(tx), static_cast<int>(ty)};
                child->translate(translation);
            }
            else if (transform.substr(0, 6) == "rotate")
            {
                float angle;
                char ch;
                ss >> ch >> angle >> ch;
                child->rotate({0, 0}, static_cast<int>(angle));
            }
            else if (transform.substr(0, 5) == "scale")
            {
                float sx, sy;
                char ch;
                ss >> ch >> sx >> sy >> ch;
                child->scale({0, 0}, static_cast<int>(sx));
            }
        }

    }

    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();
        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");
        
        // Read each child element of the root
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

                const char* transform_attr = child->Attribute("transform");
                if (svg_elements.back() != nullptr && transform_attr != nullptr)
                {
                    applyTransformation(svg_elements.back(), transform_attr);
                }
/*
                // Apply transformation
                if (transform_attr != nullptr)
                {
                    applyTransformation(circle, transform_attr);
                }
                svg_elements.push_back(circle);
                    */
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
                // Parse points string
                vector<Point> points;
                stringstream ss(points_str);
                string point;
                while (getline(ss, point, ' '))
                {
                    // Split each point into x and y coordinates
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
                // Parse points string
                vector<Point> points;
                stringstream ss(points_str);
                string point;
                while (getline(ss, point, ' '))
                {
                    // Split each point into x and y coordinates
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
                int x = child->FloatAttribute("x");
                int y = child->FloatAttribute("y");
                int width = child->FloatAttribute("width");
                int height = child->FloatAttribute("height");
                const char *fill_color = child->Attribute("fill");

                svg_elements.push_back(new Rect(parse_color(fill_color), {x,y}, width, height));
            }

            /*
            // child = child->NextSiblingElement();
            // Check if the element is not null before applying transformation
            const char* transform_attr = child->Attribute("transform");
            if (svg_elements.back() != nullptr && transform_attr != nullptr)
            {
                applyTransformation(svg_elements.back(), transform_attr);
            }
            */

            // Move to next child element
            child = child->NextSiblingElement();
        }
    }
}