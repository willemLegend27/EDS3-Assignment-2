#include "tinyxml2.h"

#include <iostream>

int main()
{
    const char* xml = "<?xml version=\"1.0\"?>"
                      "<course name=\"EDS\">"
                      "<module>"
                      "  <number>1</number>"
                      "  <name>Introduction to C++</name>"
                      "</module>"
                      "<module>"
                      "  <number>2</number>"
                      "  <name>Communication</name>"
                      "</module>"
                      "<module>"
                      "  <number>3</number>"
                      "  <name>Design, part 1</name>"
                      "</module>"
                      "</course>";

    tinyxml2::XMLDocument doc;
    // parse the XML structure you got through your socket (in this example it is
    // hardcoded)
    tinyxml2::XMLError result = doc.Parse(xml);

    if (result == tinyxml2::XML_SUCCESS)// check if XML had a valid structure
    {
        // <course> is a child element, that contains new child elements (in our case
        // modules). I declare course const because I don't intend to change anything.
        const tinyxml2::XMLElement* course = doc.FirstChildElement("course");
        if (course != NULL)
        {
            // the name part within <course> is an attribute
            const tinyxml2::XMLAttribute* attr = course->FindAttribute("name");
            if (attr != NULL)
            {
                std::cout << "Course name: " << attr->Value() << std::endl;
            }

            // as there are multiple <module> elements, the first is a child, the others
            // are siblings
            for (const tinyxml2::XMLElement* e = course->FirstChildElement("module"); e != NULL;
                 e = e->NextSiblingElement("module"))
            {
                // GetText is a quick and easy way of getting the text from an element,
                // beware that this only works when the element *only* has text.
                // In <name><b>Communication</b></name>, element "name" doesn't have
                // text but a new element "b"!
                std::cout << "Module: " << e->FirstChildElement("number")->GetText()
                          << std::endl;
                std::cout << "Name:   " << e->FirstChildElement("name")->GetText()
                          << std::endl;
            }
        }
    }

    return 0;
}
