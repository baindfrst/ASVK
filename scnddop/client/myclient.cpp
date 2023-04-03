#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include "pugixml/src/pugixml.hpp"
#include <csignal>

int sock = 0;

void sigReact(int signum) // обработчик сигнала при прирывании программы 
{
    std::cout << "uncon" << std::endl;
    char cstr[] = "unconnect";
    unsigned long long size = strlen(cstr);
    send(sock, &size, 8, 0);
    send(sock, cstr, size, 0);
    exit(-1);
}

#define PORT 123
int main()
{
    sock = 0;
    int valread;
    struct sockaddr_in serv_addr;
    srand(time(NULL));

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) // создание сокета с IPv4 и двусторонним соединением 
    {

        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET; // задаем протокол соединения для сокета
    serv_addr.sin_port = htons(PORT); // преобразовываем номер порта в сетевой проядок расположения байтов
    std::cout << "введите ip для соединения :";
    std::string ip_con;
    std::getline(std::cin, ip_con, '\n');
    if (inet_pton(AF_INET, ip_con.data(), &serv_addr.sin_addr) <= 0) // преобразуем адресс в структуру сетевого адреса сетевого семейства AF_INET
    {
        std::cout << "Address is invalid ... " << std::endl;
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // пытаемся подключиться к серверу
    {
        std::cout << "Connection Failed. Try again! ..." << std::endl;
        return -1;
    }
    while(true)
    {
        signal(SIGINT, sigReact); // ставим оброботчики сигналов прерывания
        signal(SIGABRT, sigReact);
        std::string names;
        std::vector<std::string> nameString;
        std::cout<<"введите название файлов " << std::endl;
        std::getline(std::cin, names, '\n');
        const char separator = ' ';
        std::string val;
        std::string s = "";
        for(int i = 0; i != names.length(); i++) // считываем названия входных файлов
        {
            if(names[i] != ' ')
            {
                s+=names[i];
            }
            else
            {
                nameString.push_back(s);
                s = "";
            }
        }
        nameString.push_back(s); 
        int countTestAll[nameString.size()]; //смотрим, сколько поступило файлов 
        for(int i = 0; i != nameString.size(); i++)
        {
            pugi::xml_document inputFile;

            std::string format = "";
            for(int j = nameString[i].size() - 1; j != 0 && nameString[i][j] != '.'; j--) //получаем расширение файла
            {
                format += nameString[i][j];
            }
            std::reverse(format.begin(), format.end());
            if(format != "xml") //проверяем, является ли i-ты файл xml-файлом
            {
                std::cout << "file: "  << nameString[i] << " have wrong format"<< std::endl;
                char cstr[] = "unconnect";
                unsigned long long size = strlen(cstr);
                send(sock, &size, 8, 0);
                send(sock, cstr, size, 0);//отсылаем серверу информацию, что клиент отключается
                exit(-1);
                return -1;
            }

            if (!inputFile.load_file(nameString[i].data())) //проверяем, существует ли такой файл
            {
                std::cout << "file: "  << nameString[i] << " not found"<< std::endl; 
                char cstr[] = "unconnect";
                unsigned long long size = strlen(cstr);
                send(sock, &size, 8, 0);
                send(sock, cstr, size, 0);//отсылаем серверу информацию, что клиент отключается
                exit(-1);
                return -1;
            }
            
            pugi::xml_node comand = inputFile.child("root").child("methodCall");
            std::string cmd;
            cmd = comand.text().as_string(); //получаем команду, которую надо выполнить на сервере (работает только команда main и unconnect)
            char *cstr = cmd.data(); 
            unsigned long long size;
            size = strlen(cstr);
            send(sock, &size, 8, 0);
            send(sock, cstr, size, 0); //отсылаем команду
            if(cmd == "unconnect") //если поступила команда на отключение клиента
            {
                return 0;
            }
            pugi::xml_node dataAllNode = inputFile.child("root").child("dataZone"); //переходим в чать документа, где хранятся сами тесты
            std::string countTest = dataAllNode.attribute("count").value();
            cstr = countTest.data();
            countTestAll[i] =  stoi(countTest);
            size = strlen(cstr);
            send(sock, &size, 8, 0);
            send(sock, cstr, size, 0); // отправляем информацию о кол-ве тестов

            for(pugi::xml_node_iterator tests = dataAllNode.begin(); tests != dataAllNode.end(); ++tests) //отправляем тесты друг за другом
            {
                std::string countProc = tests->child("input_data").begin()->attributes_begin()->value();
                char *cstr = countProc.data();
                size = strlen(cstr);
                send(sock, &size, 8, 0);
                send(sock, cstr, size, 0); //отправка информации о кол-ве процессоров

                pugi::xml_node procInf = tests->child("input_data").child("PROCESSORS").child("TO_WORK_PROCENT");
                for(pugi::xml_node_iterator values = procInf.begin(); values != procInf.end(); ++values) 
                {
                    std::string numProc = values->attributes_begin()->value();
                    cstr = numProc.data();
                    size = strlen(cstr);
                    send(sock, &size, 8, 0); //отправка информации о номере процессора
                    send(sock, cstr, size, 0);

                    std::string percentProc = (++(values->attributes_begin()))->value();
                    cstr = percentProc.data();
                    size = strlen(cstr);
                    send(sock, &size, 8, 0); //отправка информации о его максимальной нагрузке
                    send(sock, cstr, size, 0);
                }
                
                std::string countWorkers = tests->child("input_data").child("WORKERS").attribute("Count").value();
                cstr = countWorkers.data();
                size = strlen(cstr);
                send(sock, &size, 8, 0);
                send(sock, cstr, size, 0);//отправка информации о кол-ве команд

                pugi::xml_node workInf = tests->child("input_data").child("WORKERS").child("WORKER_LOAD");
                for(pugi::xml_node_iterator values = workInf.begin(); values != workInf.end(); ++values)
                {
                    std::string numProc = values->attributes_begin()->value();
                    char *cstr = numProc.data();
                    size = strlen(cstr);
                    send(sock, &size, 8, 0);
                    send(sock, cstr, size, 0); // отправка информации о номере команды

                    std::string percentProc = (++(values->attributes_begin()))->value();
                    cstr = percentProc.data();
                    size = strlen(cstr);
                    send(sock, &size, 8, 0); // отправка информации о нагрузке команды на процессор
                    send(sock, cstr, size, 0);
                }

                pugi::xml_node conInf = tests->child("input_data").child("CONNECTION_PARS");
                int countCon = 0;
                for(pugi::xml_node_iterator values = conInf.begin(); values != conInf.end(); ++values)
                {
                    countCon += 1; // подсчет кол-ва соединений
                }

                std::string countConStr = std::to_string(countCon);
                cstr = countConStr.data();
                size = strlen(cstr);
                send(sock, &size, 8, 0);
                send(sock, cstr, size, 0); // отправка информации о кол-ве соединений

                for(pugi::xml_node_iterator values = conInf.begin(); values != conInf.end(); ++values)
                {
                    std::string numProc = values->attributes_begin()->value();
                    char *cstr = numProc.data();
                    size = strlen(cstr);
                    send(sock, &size, 8, 0);
                    send(sock, cstr, size, 0); // отправка информации о номере первой программы в соединении

                    std::string percentProc = (++(values->attributes_begin()))->value();
                    cstr = percentProc.data();
                    size = strlen(cstr);
                    send(sock, &size, 8, 0);
                    send(sock, cstr, size, 0); // отправка информации о номере второй программы в соединении

                    std::string loadProc = (++++(values->attributes_begin()))->value();
                    cstr = loadProc.data();
                    size = strlen(cstr);
                    send(sock, &size, 8, 0);
                    send(sock, cstr, size, 0); // отправка информации о нагрузки соединения на сеть
                }
            }
        }
        for(int i = 0; i != nameString.size(); i++)
        {
            unsigned long long size;
            read(sock, &size, 8);
            char Comand[size];
            read(sock, Comand, size); // считываем команду, которая поступила от сервера
            std::string strCom(Comand, size);
            read(sock, &size, 8);
            char countAnsw[size];
            read(sock, countAnsw, size); // считываем кол-во ответов
        
            if(strCom == "print")
            {
                std::cout << "Answer to: " << nameString[i] << std::endl;
                for(int j = 0; j != countTestAll[i]; j++) //печатаем ответы для каждой задачи из входных данных
                {
                    std::cout << "Answer on test num: " << j + 1 << std::endl;
                    read(sock, &size, 8);
                    char Sucs[size];
                    read(sock, Sucs, size);
                    std::string strSucs(Sucs, size);
                    read(sock, &size, 8);
                    char Etr[size];
                    read(sock, Etr, size);
                    std::string strEtr(Etr, size);
                    read(sock, &size, 8);
                    char ArrAnsw[size];
                    read(sock, ArrAnsw, size);
                    std::string strArrAnsw(ArrAnsw, size);
                    std::cout << strSucs << std::endl;
                    std::cout << strEtr << std::endl;
                    std::cout << strArrAnsw << std::endl;
                }
            }
            else
            {
                std::cout<<"error from server" << std::endl; // в случае, если от сервера пришла ошибка
            }
        }
    }
    return 0;

}