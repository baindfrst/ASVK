#include <iostream>
#include "pugixml/src/pugixml.hpp"

int main()
{
    pugi::xml_document docAnsw;
    docAnsw.load_file("answ.xml");
    pugi::xml_node rootAnsw = docAnsw.child("out_data");
    pugi::xml_node AnswAll = rootAnsw;
    for(pugi::xml_node_iterator AnswData = AnswAll.begin(); AnswData != AnswAll.end(); ++AnswData)
    {
        std::string succ = AnswData->child("sucs").text().as_string();
        std::string Etr = AnswData->child("countEtr").text().as_string();
        std::string AnsArr = AnswData->child("AnswArr").text().as_string();
        std::cout << succ << std::endl;
         std::cout << Etr << std::endl;
          std::cout << AnsArr << std::endl;
    }
}