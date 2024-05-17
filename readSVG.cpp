#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <cstring>
#include <sstream>
#include <vector>
using namespace std;
using namespace tinyxml2;

namespace svg {

    void applyTransformation(SVGElement* element, const char* transform_str, const char* transform_origin_attr) {
        if (transform_str == nullptr || strlen(transform_str) == 0) return;
        Point origin = {0, 0};

        if (transform_origin_attr != nullptr) {
            istringstream origin_stream(transform_origin_attr);
            int x, y;
            origin_stream >> x >> y;
            origin = {x, y};
        }
        stringstream ss(transform_str);
        string transform;

        while (ss >> transform) {
            if (transform.find("translate") == 0) {
                int tx, ty = 0;

                //Replace spaces with commas - not working
                size_t open_bracket_pos = transform.find("(");
                size_t comma_pos = transform.find(" ");
                if (open_bracket_pos != string::npos && comma_pos != string::npos) {
                    transform[open_bracket_pos + 1] = ',';
                }
                sscanf(transform.c_str(), "translate(%i %i)", &tx, &ty);
                Point translation = {tx, ty};
                element->translate(translation);
            }
            else if (transform.find("rotate") == 0) {
                float angle;
                sscanf(transform.c_str(), "rotate(%f)", &angle);
                element->rotate(origin, (angle));
            }
            else if (transform.find("scale") == 0) {
                float sx, sy = 1;
                sscanf(transform.c_str(), "scale(%f,%f)", &sx, &sy);
                if (sx < 0) sx = 0;
                if (sy < 0) sy = 0;
                element->scale(origin, (sx));
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

        //Check the type of SVG element & create an instance
        while (child != nullptr) {
            SVGElement* element = nullptr;
            const char* transform_attr = nullptr;

            if (strcmp(child->Name(), "circle") == 0) {
                //Read attributes for circle element
                int cx = child->FloatAttribute("cx");
                int cy = child->FloatAttribute("cy");
                int r = child->FloatAttribute("r");
                const char* fill_color = child->Attribute("fill");

                //Create a circle element instance, similar to other SVG Element types
                element = new Circle(parse_color(fill_color), {(cx), cy}, r);
            }
            else if (strcmp(child->Name(), "ellipse") == 0) {
                int cx = child->FloatAttribute("cx");
                int cy = child->FloatAttribute("cy");
                int rx = child->FloatAttribute("rx");
                int ry = child->FloatAttribute("ry");
                const char* fill_color = child->Attribute("fill");

                element = new Ellipse(parse_color(fill_color), {cx, cy}, {rx, ry});
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
                int x1 = child->FloatAttribute("x1");
                int y1 = child->FloatAttribute("y1");
                int x2 = child->FloatAttribute("x2");
                int y2 = child->FloatAttribute("y2");
                const char* stroke_color = child->Attribute("stroke");

                element = new Line(parse_color(stroke_color), {x1, y1}, {x2, y2});
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
                //Extract transformation attributes
                transform_attr = child->Attribute("transform");

                //Apply transformations if available
                const char* transform_origin_attr = child->Attribute("transform-origin");
                if (transform_attr != nullptr) {
                    applyTransformation(element, transform_attr, transform_origin_attr);
                }
                //Add the element to the vector of SVG elements
                svg_elements.push_back(element);
            }
            //Move to the next child element
            child = child->NextSiblingElement();
        }
    }
}