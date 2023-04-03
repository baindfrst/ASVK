#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include "pugixml/src/pugixml.hpp"




int main()
{
    std::string nameFile;
    std::cout << "введите название для создаваемого файла: ";
    std::cin >> nameFile;
    std::ofstream c(nameFile);
    c.close();
    pugi::xml_document doc;
    doc.load_file(nameFile.data());
    pugi::xml_node declaration_node = doc.append_child(pugi::node_declaration);
    declaration_node.append_attribute("version") = "1.0";
    declaration_node.append_attribute("encoding") = "UTF-8";
    pugi::xml_node root = doc.append_child("root");
    pugi::xml_node comand = root.append_child("methodCall");
    comand.text().set("main");
    pugi::xml_node dataZone = root.append_child("dataZone");
    int count;
    std::cout << "введите кол-во файлов с данными: ";
    std::cin >> count;
    dataZone.append_attribute("count").set_value(count);
    for(int i = 0; i != count; i++)
    {
        std::string nameDataFile;
        std::cout << "введите название файлов с тестами (название выглядит как: \"dataset/<название файла>\"): ";
        std::cin >> nameDataFile;
        pugi::xml_document docData;
        if(!(docData.load_file(nameDataFile.data())))
        {
            std::cout << "такого файла нет(" <<std::endl;
            i--;
        }
        else
        {
            pugi::xml_node dataNow = dataZone.append_child("test");
            pugi::xml_node toClone = docData.child("input_data");
            dataNow.append_copy(toClone);
        }
    }
    doc.save_file(nameFile.data());
}
        