#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <cstring>
#include <sstream>


using namespace std;
using namespace tinyxml2;

namespace svg
{
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

        // Loop through each child element of the root
        XMLElement *child = xml_elem->FirstChildElement();
        while (child != nullptr)
        {
            // Check if the element is a circle
            if (strcmp(child->Name(), "circle") == 0)
            {
                // Read circle attributes
                float cx = child->FloatAttribute("cx");
                float cy = child->FloatAttribute("cy");
                float r = child->FloatAttribute("r");
                const char *fill_color = child->Attribute("fill");
                // Create Circle object and add to vector
                svg_elements.push_back(new Circle(parse_color(fill_color), {static_cast<int>(cx), static_cast<int>(cy)}, static_cast<int>(r)));
            }
            // Check if the element is an ellipse
            else if (strcmp(child->Name(), "ellipse") == 0)
            {
                // Read ellipse attributes
                float cx = child->FloatAttribute("cx");
                float cy = child->FloatAttribute("cy");
                float rx = child->FloatAttribute("rx");
                float ry = child->FloatAttribute("ry");
                const char *fill_color = child->Attribute("fill");
                // Create Ellipse object and add to vector
                svg_elements.push_back(new Ellipse(parse_color(fill_color), {static_cast<int>(cx), static_cast<int>(cy)}, {static_cast<int>(rx), static_cast<int>(ry)}));
            }

                // Check if the element is a polyline
            else if (strcmp(child->Name(), "polyline") == 0)
            {
                // Read polyline attributes
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
                // Create Polyline object and add to vector
                svg_elements.push_back(new Polyline(parse_color(stroke_color), points));
            }

                // Check if the element is a line
            else if (strcmp(child->Name(), "line") == 0)
            {
                // Read line attributes
                float x1 = child->FloatAttribute("x1");
                float y1 = child->FloatAttribute("y1");
                float x2 = child->FloatAttribute("x2");
                float y2 = child->FloatAttribute("y2");
                const char *stroke_color = child->Attribute("stroke");
                // Create Line object and add to vector
                svg_elements.push_back(new Line(parse_color(stroke_color), {static_cast<int>(x1), static_cast<int>(y1)}, {static_cast<int>(x2), static_cast<int>(y2)}));
            }

            //TODO: polygon

            //TODO: rect



            // Move to next child element
            child = child->NextSiblingElement();
        }
    }
}

