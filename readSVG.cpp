#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <cstring>
#include <sstream>
#include <vector>
using namespace std;
using namespace tinyxml2;

namespace svg {

    void applyTransformation(SVGElement* child, const char* transform_str) {




        if (transform_str == nullptr || strlen(transform_str) == 0) return;

        stringstream ss(transform_str);
        string transform;
        while (ss >> transform) {
            if (transform.find("translate") == 0) {
                float tx, ty = 0;
                sscanf(transform.c_str(), "translate(%f,%f)", &tx, &ty);
                Point translation = {static_cast<int>(tx), static_cast<int>(ty)};
                child->translate(translation);

                cout << "Aplly translate"<< endl;
            }
            else if (transform.find("rotate") == 0) {
                float angle;
                sscanf(transform.c_str(), "rotate(%f)", &angle);
                child->rotate({0, 0}, static_cast<int>(angle));
            }
            else if (transform.find("scale") == 0) {
                float sx, sy = 1;
                sscanf(transform.c_str(), "scale(%f,%f)", &sx, &sy);
                child->scale({0, 0}, static_cast<int>(sx));
            }
        }
    }

    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement*>& svg_elements) {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS) {
            throw runtime_error("Unable to load " + svg_file);
        }

        XMLElement* xml_elem = doc.RootElement();
        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");

        XMLElement* child = xml_elem->FirstChildElement();
        while (child != nullptr) {
            SVGElement* element = nullptr;
            const char* transform_attr = nullptr;

            if (strcmp(child->Name(), "circle") == 0) {
                float cx = child->FloatAttribute("cx");
                float cy = child->FloatAttribute("cy");
                float r = child->FloatAttribute("r");
                const char* fill_color = child->Attribute("fill");
                element = new Circle(parse_color(fill_color), {static_cast<int>(cx), static_cast<int>(cy)}, static_cast<int>(r));
            }
            else if (strcmp(child->Name(), "ellipse") == 0) {
                float cx = child->FloatAttribute("cx");
                float cy = child->FloatAttribute("cy");
                float rx = child->FloatAttribute("rx");
                float ry = child->FloatAttribute("ry");
                const char* fill_color = child->Attribute("fill");
                element = new Ellipse(parse_color(fill_color), {static_cast<int>(cx), static_cast<int>(cy)}, {static_cast<int>(rx), static_cast<int>(ry)});
            }
            else if (strcmp(child->Name(), "polyline") == 0) {
                const char* points_str = child->Attribute("points");
                const char* stroke_color = child->Attribute("stroke");
                vector<Point> points;
                stringstream ss(points_str);
                string point;
                while (getline(ss, point, ' ')) {
                    stringstream ss_point(point);
                    string x_str, y_str;
                    getline(ss_point, x_str, ',');
                    getline(ss_point, y_str, ',');
                    int x = stoi(x_str);
                    int y = stoi(y_str);
                    points.push_back({x, y});
                }
                element = new Polyline(parse_color(stroke_color), points);
            }
            else if (strcmp(child->Name(), "line") == 0) {
                float x1 = child->FloatAttribute("x1");
                float y1 = child->FloatAttribute("y1");
                float x2 = child->FloatAttribute("x2");
                float y2 = child->FloatAttribute("y2");
                const char* stroke_color = child->Attribute("stroke");
                element = new Line(parse_color(stroke_color), {static_cast<int>(x1), static_cast<int>(y1)}, {static_cast<int>(x2), static_cast<int>(y2)});
            }
            else if (strcmp(child->Name(), "polygon") == 0) {
                const char* points_str = child->Attribute("points");
                const char* fill_color = child->Attribute("fill");
                vector<Point> points;
                stringstream ss(points_str);
                string point;
                while (getline(ss, point, ' ')) {
                    stringstream ss_point(point);
                    string x_str, y_str;
                    getline(ss_point, x_str, ',');
                    getline(ss_point, y_str, ',');
                    int x = stoi(x_str);
                    int y = stoi(y_str);
                    points.push_back({x, y});
                }
                element = new Polygon(parse_color(fill_color), points);
            }
            else if (strcmp(child->Name(), "rect") == 0) {
                int x = child->FloatAttribute("x");
                int y = child->FloatAttribute("y");
                int width = child->FloatAttribute("width");
                int height = child->FloatAttribute("height");
                const char* fill_color = child->Attribute("fill");
                element = new Rect(parse_color(fill_color), {x, y}, width, height);
            }

            if (element != nullptr) {
                transform_attr = child->Attribute("transform");
                if (transform_attr != nullptr) {
                    applyTransformation(element, transform_attr);
                }
                svg_elements.push_back(element);
            }

            child = child->NextSiblingElement();
        }
    }
}