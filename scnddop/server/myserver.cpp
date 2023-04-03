#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
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
    int server_fd = 0;
    int new_socket = 0;
    int valread = 0;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    std::string nameString;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) // создание сокета с IPv4 и двусторонним соединением
    {
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) //устанавливаем параметры сокета
    {
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET; //задаем 
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) //привязываем к сокету локальный адресс
    {
        std::cout << "bind failed" << std::endl; 
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) // создаем очередь соединений
    {
        std::cout << "listen" << std::endl;
        exit(EXIT_FAILURE);
    }
    while(true)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) // получаем новое соединение
        {
            std::cout << "accept" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << "connected: " << new_socket << std::endl;
        int count_prog = 0;
        unsigned long long *sizeOfBuffer = new unsigned long long;
        while(true)
        {
            bool errorFlag = false; // флаг ошибки во время выполнения программы
            valread = read(new_socket, sizeOfBuffer, 8);
            char Comand[*sizeOfBuffer];
            valread = read(new_socket, Comand, *sizeOfBuffer); // получаем команду от клиента
            std::string strComand(Comand, *sizeOfBuffer);
            std::cout << strComand << std::endl;
            if(strComand == "unconnect") // если команда была unconnect, то ждем новое подключение
            {
                std::cout << "unconnecting..." << std::endl;
                break;
            }

            valread = read(new_socket, sizeOfBuffer, 8);
            char CountTests[*sizeOfBuffer];
            valread = read(new_socket, CountTests, *sizeOfBuffer); //получаем кол-во тестов в файле, который был отправлен
            std::string rename(CountTests, *sizeOfBuffer);
            std::ofstream c("answ.xml"); //создаем фалй для хранения ответов
            c.close();
            pugi::xml_document docAnsw;
            docAnsw.load_file("answ.xml");
            pugi::xml_node declaration_node = docAnsw.append_child(pugi::node_declaration);
            declaration_node.append_attribute("version") = "1.0";
            declaration_node.append_attribute("encoding") = "UTF-8";
            pugi::xml_node nodeAnsw = docAnsw.append_child("out_data");
            pugi::xml_node ComNode = nodeAnsw.append_child("methodCall");
            pugi::xml_node mainNode = nodeAnsw.append_child("AnswAll");
            int to_do = stoi(rename);
            for(int i = 0; i != to_do; i++)
            {
                std::ofstream create_file("TestInp.xml"); //создаем файл с тестом, для отправления в программу для решения задачи
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
                valread = read(new_socket, CountProc, *sizeOfBuffer); // получаем кол-во процессоров
                rename = std::string(CountProc, *sizeOfBuffer);
                proc.append_attribute("Count").set_value(stoi(rename));
                pugi::xml_node procWork = proc.append_child("TO_WORK_PROCENT");
                int loop = stoi(rename);
                for(int j = 0; j != loop; j++)
                {
                    pugi::xml_node procVal = procWork.append_child("values");
                    valread = read(new_socket, sizeOfBuffer, 8);
                    char NumProc[*sizeOfBuffer];
                    valread = read(new_socket, NumProc, *sizeOfBuffer); // получаем номер процессора
                    rename = std::string(NumProc, *sizeOfBuffer);
                    procVal.append_attribute("Number").set_value(stoi(rename));
                    valread = read(new_socket, sizeOfBuffer, 8);
                    char LoadProc[*sizeOfBuffer];
                    valread = read(new_socket, LoadProc, *sizeOfBuffer); // получаем его максимальную нагрузку
                    rename = std::string(LoadProc, *sizeOfBuffer);
                    procVal.append_attribute("Percents").set_value(rename.data());
                }
                pugi::xml_node work = root.append_child("WORKERS");
                valread = read(new_socket, sizeOfBuffer, 8);
                char WorkProc[*sizeOfBuffer];
                valread = read(new_socket, WorkProc, *sizeOfBuffer); //получаем кол-во программ
                rename = std::string(WorkProc, *sizeOfBuffer);
                work.append_attribute("Count").set_value(stoi(rename));
                pugi::xml_node wWork = work.append_child("WORKER_LOAD");
                loop = stoi(rename);
                for(int j = 0; j != loop; j++)
                {
                    pugi::xml_node procVal = wWork.append_child("values");
                    valread = read(new_socket, sizeOfBuffer, 8);
                    char WorkProc[*sizeOfBuffer];
                    valread = read(new_socket, WorkProc, *sizeOfBuffer); //получаем номер команды
                    rename = std::string(WorkProc, *sizeOfBuffer);
                    procVal.append_attribute("Number").set_value(stoi(rename));
                    valread = read(new_socket, sizeOfBuffer, 8);
                    char WorkLoad[*sizeOfBuffer];
                    valread = read(new_socket, WorkLoad, *sizeOfBuffer); //получаем ее нагрузку
                    rename = std::string(WorkLoad, *sizeOfBuffer);
                    procVal.append_attribute("Percents").set_value(rename.data());
                }

                pugi::xml_node con = root.append_child("CONNECTION_PARS");
                valread = read(new_socket, sizeOfBuffer, 8);
                char CountCon[*sizeOfBuffer];
                valread = read(new_socket, CountCon, *sizeOfBuffer); //получаем кол-во соединений
                rename = std::string(CountCon, *sizeOfBuffer);
                loop = stoi(rename);
                for(int j = 0; j != loop; j++)
                {
                    pugi::xml_node procVal = con.append_child("values");
                    valread = read(new_socket, sizeOfBuffer, 8);
                    char ConFrst[*sizeOfBuffer];
                    valread = read(new_socket, ConFrst, *sizeOfBuffer); //получаем номер первой программы в соединении
                    rename = std::string(ConFrst, *sizeOfBuffer);
                    procVal.append_attribute("Frst").set_value(stoi(rename));
                    valread = read(new_socket, sizeOfBuffer, 8);
                    char ConScnd[*sizeOfBuffer];
                    valread = read(new_socket, ConScnd, *sizeOfBuffer); //получаем номер второй программы в соединении
                    rename = std::string(ConScnd, *sizeOfBuffer);
                    procVal.append_attribute("Frst").set_value(stoi(rename));
                    valread = read(new_socket, sizeOfBuffer, 8);
                    char ConLoad[*sizeOfBuffer];
                    valread = read(new_socket, ConLoad, *sizeOfBuffer); //получаем нагрузку соединения на сеть
                    rename = std::string(ConLoad, *sizeOfBuffer);
                    procVal.append_attribute("Load").set_value(stoi(rename));
                }
                inpDoc.save_file("TestInp.xml");
                std::ifstream rofl("TestInp.xml");
                std::string roflstr;
                std::string com = "./" + std::string(Comand, 4) + " TestInp.xml >outprog.txt";
                system(com.data()); //запускаем основную программу на решение задачи
                std::ifstream fileRez("outprog.txt");//файл, в котором хранятся выходные данные программы
                std::string succes;
                std::string countEtr;
                std::string answArr;

                std::getline(fileRez, succes);//считываем выходные данные программы 
                std::getline(fileRez, countEtr);
                std::getline(fileRez, answArr);
                if(succes != "succese" && succes != "failure") //если во время выполнения программы была ошибка - устанавливаем флаг
                {
                    errorFlag = true;
                }
                pugi::xml_node ans =  mainNode.append_child("sucs");
                pugi::xml_node sucs = ans.append_child("sucs");
                sucs.text().set(succes.data());
                pugi::xml_node countEtrNode = ans.append_child("countEtr");
                countEtrNode.text().set(countEtr.data());
                pugi::xml_node AnswArr = ans.append_child("AnswArr");
                AnswArr.text().set(answArr.data());
            }
            if(errorFlag) //задаем команду для ответа
            {
                ComNode.text().set("error in work");
            }
            else
            {
                ComNode.text().set("print");
            }
            docAnsw.save_file("answ.xml");


            docAnsw.load_file("answ.xml");
            pugi::xml_node rootAnsw = docAnsw.child("out_data");
            
            std::cout << "answ sending..." << std::endl;
            char *cstr;
            pugi::xml_node ComandN = rootAnsw.child("methodCall");
            std::string comOut = ComandN.text().as_string();
            cstr = comOut.data();
            *sizeOfBuffer = comOut.size();
            send(new_socket, sizeOfBuffer, 8, 0);
            send(new_socket, cstr, *sizeOfBuffer, 0); //отправляем команду клиенту
        
            cstr = CountTests;
            *sizeOfBuffer = strlen(cstr);
            send(new_socket, sizeOfBuffer, 8, 0);
            send(new_socket, cstr, *sizeOfBuffer, 0); //отправляем кол-во тестов

            pugi::xml_node AnswAll = rootAnsw.child("AnswAll");

            for(pugi::xml_node_iterator AnswData = AnswAll.begin(); AnswData != AnswAll.end(); ++AnswData)
            {
                std::string succ = AnswData->child("sucs").text().as_string();
                std::string Etr = AnswData->child("countEtr").text().as_string();
                std::string AnsArr = AnswData->child("AnswArr").text().as_string();
                char *cstr = succ.data();
                *sizeOfBuffer = strlen(cstr);
                send(new_socket, sizeOfBuffer, 8, 0);
                send(new_socket, cstr, *sizeOfBuffer, 0); //отправляем результаты решения тестов.
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
        delete sizeOfBuffer;
    }
}