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

#define PORT 123

int main()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    std::string nameString;
    char *buffer = new char;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cout << "bind failed" << std::endl; 
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        std::cout << "listen" << std::endl;
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        std::cout << "accept" << std::endl;
        exit(EXIT_FAILURE);
    }
    int count_prog = 0;
    unsigned long long *sizeOfBuffer = new unsigned long long;
    while(true)
    {
        std::cout << "start" << std::endl;

        valread = read(new_socket, sizeOfBuffer, 8);
        char Comand[*sizeOfBuffer];
        valread = read(new_socket, Comand, *sizeOfBuffer);

        valread = read(new_socket, sizeOfBuffer, 8);
        char CountTests[*sizeOfBuffer];
        valread = read(new_socket, CountTests, *sizeOfBuffer);
        std::string rename(CountTests, *sizeOfBuffer);
        std::ofstream c("answ.xml");
        c.close();
        pugi::xml_document docAnsw;
        docAnsw.load_file("answ.xml");
        pugi::xml_node declaration_node = docAnsw.append_child(pugi::node_declaration);
        std::cout <<"decl"<<std::endl;
        declaration_node.append_attribute("version") = "1.0";
        declaration_node.append_attribute("encoding") = "UTF-8";
        pugi::xml_node nodeAnsw = docAnsw.append_child("out_data");
        pugi::xml_node mainNode = nodeAnsw.append_child("AnswAll");
        pugi::xml_node ComNode = nodeAnsw.append_child("methodCall");

        for(int i = 0; i != stoi(rename); i++);
        {
            std::cout << atoi(CountTests) << std::endl;
            std::ofstream create_file("TestInp.xml");
            create_file.close();

            pugi::xml_document inpDoc;
            inpDoc.load_file("TestInp.xml");
            pugi::xml_node declaration_node = inpDoc.append_child(pugi::node_declaration);
            declaration_node.append_attribute("version") = "1.0";
            declaration_node.append_attribute("encoding") = "UTF-8";
            pugi::xml_node root = inpDoc.append_child("input_data");

            pugi::xml_node proc = root.append_child("PROCESSORS");
            valread = read(new_socket, sizeOfBuffer, 8);
            char CountProc[*sizeOfBuffer];
            valread = read(new_socket, CountProc, *sizeOfBuffer);
            rename = std::string(CountProc, *sizeOfBuffer);
            proc.append_attribute("Count").set_value(atoi(CountProc));
            pugi::xml_node procWork = proc.append_child("TO_WORK_PROCENT");
            for(int i = 0; i != stoi(rename); i++)
            {
                pugi::xml_node procVal = procWork.append_child("values");
                valread = read(new_socket, sizeOfBuffer, 8);
                char NumProc[*sizeOfBuffer];
                valread = read(new_socket, NumProc, *sizeOfBuffer);
                rename = std::string(NumProc, *sizeOfBuffer);
                procVal.append_attribute("Number").set_value(atoi(rename));
                valread = read(new_socket, sizeOfBuffer, 8);
                char LoadProc[*sizeOfBuffer];
                valread = read(new_socket, LoadProc, *sizeOfBuffer);
                rename = std::string(LoadProc, *sizeOfBuffer);
                procVal.append_attribute("Percents").set_value(rename);
            }

            pugi::xml_node work = root.append_child("WORKERS");
            valread = read(new_socket, sizeOfBuffer, 8);
            char WorkProc[*sizeOfBuffer];
            valread = read(new_socket, WorkProc, *sizeOfBuffer);
            work.append_attribute("Count").set_value(atoi(WorkProc));
            pugi::xml_node wWork = work.append_child("WORKER_LOAD");
            for(int i = 0; i != atoi(WorkProc); i++)
            {
                pugi::xml_node procVal = wWork.append_child("values");
                valread = read(new_socket, sizeOfBuffer, 8);
                char WorkProc[*sizeOfBuffer];
                valread = read(new_socket, WorkProc, *sizeOfBuffer);
                rename = std::string(WorkProc, *sizeOfBuffer);
                procVal.append_attribute("Number").set_value(atoi(rename));
                valread = read(new_socket, sizeOfBuffer, 8);
                char WorkLoad[*sizeOfBuffer];
                valread = read(new_socket, WorkLoad, *sizeOfBuffer);
                rename = std::string(WorkLoad, *sizeOfBuffer);
                procVal.append_attribute("Percents").set_value(rename);
            }

            pugi::xml_node con = root.append_child("CONNECTION_PARS");
            valread = read(new_socket, sizeOfBuffer, 8);
            char CountCon[*sizeOfBuffer];
            valread = read(new_socket, CountCon, *sizeOfBuffer);
            for(int i = 0; i != atoi(CountCon); i++)
            {
                pugi::xml_node procVal = con.append_child("values");
                valread = read(new_socket, sizeOfBuffer, 8);
                char ConFrst[*sizeOfBuffer];
                valread = read(new_socket, ConFrst, *sizeOfBuffer);
                rename = std::string(ConFrst, *sizeOfBuffer);
                procVal.append_attribute("Frst").set_value(atoi(rename));
                valread = read(new_socket, sizeOfBuffer, 8);
                char ConScnd[*sizeOfBuffer];
                valread = read(new_socket, ConScnd, *sizeOfBuffer);
                rename = std::string(ConScnd, *sizeOfBuffer);
                procVal.append_attribute("Frst").set_value(atoi(rename));
                valread = read(new_socket, sizeOfBuffer, 8);
                char ConLoad[*sizeOfBuffer];
                valread = read(new_socket, ConLoad, *sizeOfBuffer);
                rename = std::string(ConLoad, *sizeOfBuffer);
                procVal.append_attribute("Load").set_value(atoi(rename));
            }
            inpDoc.save_file("TestInp.xml");
            std::ifstream rofl("TestInp.xml");
            std::string roflstr;
            while(std::getline(rofl, roflstr))
              {
                std::cout << roflstr << std::endl;
              }
            std::cout <<"file created"<<std::endl;
            std::string com = "./" + std::string(Comand) + " TestInp.xml >outprog.txt";
            std::cout << com << std::endl;
            system(com.data());
            std::cout <<"com called"<<std::endl;
            std::ifstream fileRez("outprog.txt");
            std::string succes;
            std::string countEtr;
            std::string answArr;

            std::getline(fileRez, succes);
            std::getline(fileRez, countEtr);
            std::getline(fileRez, answArr);
            std::cout <<"read data"<<std::endl;
            std::cout << succes << countEtr << answArr << std::endl;
            pugi::xml_node ans =  mainNode.append_child("sucs");
            pugi::xml_node sucs = ans.append_child("sucs");
            sucs.text().set(succes.data());
            pugi::xml_node countEtrNode = ans.append_child("countEtr");
            countEtrNode.text().set(countEtr.data());
            pugi::xml_node AnswArr = ans.append_child("AnswArr");
            AnswArr.text().set(answArr.data());
        }
        ComNode.text().set("print");
        docAnsw.save_file("answ.xml");


        docAnsw.load_file("answ.xml");
        pugi::xml_node rootAnsw = docAnsw.child("out_data");

        pugi::xml_node ComandN = rootAnsw.child("methodCall");
        std::string comOut = ComandN.text().as_string();
        char *cstr = comOut.data();
        *sizeOfBuffer = comOut.size();
        std::cout << "sending " << *sizeOfBuffer << std::endl;
        std::cout << cstr << std::endl;
        send(new_socket, sizeOfBuffer, 8, 0);
        send(new_socket, cstr, *sizeOfBuffer, 0);
        std::cout << "sended" << std::endl;
        
        cstr = CountTests;
        *sizeOfBuffer = strlen(cstr);
        std::cout << "sending" << std::endl;
        std::cout << cstr << std::endl;
        send(new_socket, sizeOfBuffer, 8, 0);
        send(new_socket, cstr, *sizeOfBuffer, 0);

        pugi::xml_node AnswAll = rootAnsw.child("AnswAll");

        for(pugi::xml_node_iterator AnswData = AnswAll.begin(); AnswData != AnswAll.end(); ++AnswData)
        {
            std::string succ = AnswData->child("sucs").text().as_string();
            std::string Etr = AnswData->child("countEtr").text().as_string();
            std::string AnsArr = AnswData->child("AnswArr").text().as_string();
            std::cout << succ << std::endl;
            char *cstr = succ.data();
            *sizeOfBuffer = strlen(cstr);
            std::cout << "Asdasdasdasd " << *sizeOfBuffer <<std::endl;
            send(new_socket, sizeOfBuffer, 8, 0);
            send(new_socket, cstr, *sizeOfBuffer, 0);
            cstr = Etr.data();
            *sizeOfBuffer = strlen(cstr);
            send(new_socket, sizeOfBuffer, 8, 0);
            send(new_socket, cstr, *sizeOfBuffer, 0);
            cstr = AnsArr.data();
            *sizeOfBuffer = strlen(cstr);
            send(new_socket, sizeOfBuffer, 8, 0);
            send(new_socket, cstr, *sizeOfBuffer, 0);
        }
    }
}