#include <iostream>
#include <cstdlib>
#include "pugixml/src/pugixml.hpp"// библиотека для работы с xml-файлами
#include "ProccesorsInfo.hpp" // в этом файле описана информация о структуре для информации о процессорах
#include "WorkerInfo.hpp" // в этом файле описана информация о структуре для информации о процессах
#include "ConnectionInfo.hpp"// в этом файле описана информация о структуре для информации о соединениях
#include "Readingxml.hpp" // заголовочный файл для данного файла

using namespace pugi;
using namespace std;

err::err(int type, int num, int l) // структура, в которой хранится информация о ошибках
{
    numproc = num;
    load = l;
    typeerr = type;
}

ProccesorsInfo* ReadProccesorsInfo(const char* nameOfInputFile) // функция для чтения из входного файла nameOfInputFile информации о процессорах
{
    xml_document inputFile;
    inputFile.load_file(nameOfInputFile); //открываем входной файл для работы с ним
    if(!(inputFile.child("input_data"))) //проверяем структуру файла
    {
        throw err(-1, 0, 0);// отправляем сообщение об ошибки
    }
    xml_node processorInfo = inputFile.child("input_data"); //переходим в секцию файла под названием "input_data", хранящую все информацию
    if(!(processorInfo.child("PROCESSORS"))) //проверяем структуру файла
    {
        throw err(-1, 0, 0);// отправляем сообщение об ошибки
    }
    if(!(processorInfo.child("PROCESSORS").child("TO_WORK_PROCENT")))
    {
        throw err(-1, 0, 0);// отправляем сообщение об ошибки
    }
    int procCount = stoi(processorInfo.begin()->attributes_begin()->value());
    ProccesorsInfo* curentProccesorsInfo = new ProccesorsInfo(stoi(processorInfo.begin()->attributes_begin()->value())); // создаем объект класса, для храниния считанной информации
    processorInfo = inputFile.child("input_data").child("PROCESSORS").child("TO_WORK_PROCENT"); // переходим в секцию, хранящую информацию о максимально нагрузки процессоров

    int count = 0; // кол-во считанных строк
    for(xml_node_iterator values = processorInfo.begin(); values != processorInfo.end(); ++values) // проходим полность по секции 
    {
        count++; // увеличиваем кол-во прочитанных строк
        xml_node_iterator val_check = values; // запоминаем сущность, хранящую информацию об одном процессоре

        ++val_check; // передвигаемся на поле, хранящее информацию о максимально нагрузке процессора 
        if(stoi((++(values->attributes_begin()))->value()) > 100 || stoi((++(values->attributes_begin()))->value()) < 0 || (stoi((++(values->attributes_begin()))->value()) != 50 && stoi((++(values->attributes_begin()))->value()) != 70 && stoi((++(values->attributes_begin()))->value()) != 90 && stoi((++(values->attributes_begin()))->value()) != 100)) // проверяем, подходит ли максимальная нагрузка процессора под условия задачи
        {
            delete curentProccesorsInfo; // освобождаем выделенную память под структуру, хранящую информацию о процессорах
            throw err(0, stoi(values->attributes_begin()->value()), stoi((++(values->attributes_begin()))->value())); // отправляем сообщение об ошибки
        }

        if((count != curentProccesorsInfo->getCountOfProcessors() && val_check == processorInfo.end()) || (count == curentProccesorsInfo->getCountOfProcessors() && (val_check != (processorInfo.end())))) // проверяем, верно ли была указана информация о кол-ве процессорах
        {
            count = curentProccesorsInfo->getCountOfProcessors(); // запоминаем указанное кол-во процессоров для того, что бы его записать в структуру для ошибок
            delete curentProccesorsInfo; // освобождаем выделенную память под структуру, хранящую информацию о процессорах
            throw err(1, count, stoi(processorInfo.begin()->attributes_begin()->value()));// отправляем сообщение об ошибки
        }

        if(count != stoi(values->attributes_begin()->value())) // проверяем, начинаются ли номера процессоров с 1 и идут ли они последовательно
        {
            delete curentProccesorsInfo; // освобождаем выделенную память под структуру, хранящую информацию о процессорах
            throw err(2, stoi(values->attributes_begin()->value()), 0); // отправляем сообщение об ошибки
        }

        curentProccesorsInfo->AddLoad(stoi(values->attributes_begin()->value()), stoi((++(values->attributes_begin()))->value())); // добавляем в структуру информацию о процессоре
    }
    if(count != procCount) //ошибка в кол-ве процессоров
    {
        delete curentProccesorsInfo; // освобождаем выделенную память под структуру, хранящую информацию о процессорах
        throw err(1, procCount, count); // отправляем сообщение об ошибки
    }
    return curentProccesorsInfo; // возвращаем указатель на структуру
}

WorkerInfo* ReadWorkersInfo(const char* nameOfInputFile) // функция для чтения из входного файла nameOfInputFile информации о процессах
{
    xml_document inputFile;
    inputFile.load_file(nameOfInputFile); //открываем входной файл для работы с ним
    xml_node workersInfo = inputFile.child("input_data"); //переходим в секцию файла под названием "input_data", хранящую все информацию
    if(!(workersInfo.child("WORKERS"))) //проверяем структуру файла
    {
        throw err(-1, 0, 0);// отправляем сообщение об ошибки
    }
    if(!(workersInfo.child("WORKERS").child("WORKER_LOAD")))
    {
        throw err(-1, 0, 0);// отправляем сообщение об ошибки
    }
    int workerCount = stoi((++(workersInfo.begin()))->attributes_begin()->value());
    WorkerInfo* curentWorkerInfo = new WorkerInfo(stoi((++(workersInfo.begin()))->attributes_begin()->value())); // создаем объект класса, для храниния считанной информации
    workersInfo = inputFile.child("input_data").child("WORKERS").child("WORKER_LOAD"); // переходим в секцию, хранящую информацию о нагрузки процесса
    int count = 0;// кол-во считанных строк
    for(xml_node_iterator values = workersInfo.begin(); values != workersInfo.end(); ++values) // проходим полность по секции 
    {
        count++; // увеличиваем кол-во прочитанных строк
        xml_node_iterator val_check = values; // запоминаем сущность, хранящую информацию об одном процессе
        ++val_check;// передвигаемся на поле, хранящее информацию о максимально нагрузке процессора 
        if(stoi((++(values->attributes_begin()))->value()) > 100 || stoi((++(values->attributes_begin()))->value()) < 0 || (stoi((++(values->attributes_begin()))->value()) != 5 && stoi((++(values->attributes_begin()))->value()) != 10 && stoi((++(values->attributes_begin()))->value()) != 15 && stoi((++(values->attributes_begin()))->value()) != 20)) // проверяем, подходит ли нагрузка процесса под условия задачи
        {
            delete curentWorkerInfo;// освобождаем выделенную память под структуру, хранящую информацию о процессах
            throw err(0, stoi(values->attributes_begin()->value()), stoi((++(values->attributes_begin()))->value()));// отправляем сообщение об ошибки
        }

        if((count != curentWorkerInfo->getCountOfWorkers() && val_check == workersInfo.end()) || (count == curentWorkerInfo->getCountOfWorkers() && val_check != workersInfo.end())) // проверяем, верно ли была указана информация о кол-ве процессов
        {
            count = curentWorkerInfo->getCountOfWorkers(); // запоминаем указанное кол-во процессов для того, что бы его записать в структуру для ошибок
            delete curentWorkerInfo; // освобождаем выделенную память под структуру, хранящую информацию о процессах
            throw err(1, count, 0); // отправляем сообщение об ошибки
        }

        if(count != stoi(values->attributes_begin()->value())) // проверяем, начинаются ли номера процессов с 1 и идут ли они последовательно
        {
            delete curentWorkerInfo; // освобождаем выделенную память под структуру, хранящую информацию о процессах
            throw err(2, stoi(values->attributes_begin()->value()), 0); // отправляем сообщение об ошибки
        }

        curentWorkerInfo->AddLoad(stoi(values->attributes_begin()->value()), stoi((++(values->attributes_begin()))->value())); // добавляем в структуру информацию о процессе
    }
    if(count != workerCount) // ошибка в кол-ве процессов
    {
        delete curentWorkerInfo; // освобождаем выделенную память под структуру, хранящую информацию о процессах
        throw err(1, workerCount, count); // отправляем сообщение об ошибки
    }
    return curentWorkerInfo; // возвращаем указатель на структуру
}

ConnectionInfo* ReadConnectionInfo(const char* nameOfInputFile) // функция для чтения из входного файла nameOfInputFile информации о соединениях
{
    xml_document inputFile;
    inputFile.load_file(nameOfInputFile); //открываем входной файл для работы с ним
    if(!(inputFile.child("input_data").child("CONNECTION_PARS"))) //проверяем структуру файла
    {
        throw err(-1, 0, 0);// отправляем сообщение об ошибки
    }
    int count_workers = stoi((++(inputFile.child("input_data").begin()))->attributes_begin()->value()); // запоминаем количество процессов 
    xml_node ConnectionsInfo = inputFile.child("input_data").child("CONNECTION_PARS"); // переходим в секцию, хранящую информацию о соединениях

    int count = 0; // кол-во соединений
    for(xml_node_iterator values = ConnectionsInfo.begin(); values != ConnectionsInfo.end(); ++values) // подсчитываем кол-во соединений
    {
        count++;
    }
    ConnectionInfo* curentConnectionsInfo = new ConnectionInfo(count); // создаем структуру, для хранения информации о соединениях

    int numberOfConnections = 0; // переменная, для определения, какое соединение по счету мы смотрим
    for(xml_node_iterator values = ConnectionsInfo.begin(); values != ConnectionsInfo.end(); ++values) //проходим полность по секции 
    {
        if(stoi(values->attributes_begin()->value()) < 0 || stoi(values->attributes_begin()->value()) > count_workers) // проверяем, находится описан ли первый процесс в соединение
        {
            delete curentConnectionsInfo; // освобождаем выделенную память под структуру, хранящую информацию о соединениях
            throw err(0, numberOfConnections + 1, stoi(values->attributes_begin()->value()));// отправляем сообщение об ошибки
        }

        if(stoi((++(values->attributes_begin()))->value()) < 0 || stoi((++(values->attributes_begin()))->value()) > count_workers) // проверяем, находится описан ли второй процесс в соединение
        {
            delete curentConnectionsInfo; // освобождаем выделенную память под структуру, хранящую информацию о соединениях
            throw err(1, numberOfConnections + 1, stoi((++(values->attributes_begin()))->value())); // отправляем сообщение об ошибки
        }

        if(stoi((++(values->attributes_begin()))->value()) == stoi(values->attributes_begin()->value())) // проверяем, не соединяется ли процесс с самим собой
        {
            delete curentConnectionsInfo; // освобождаем выделенную память под структуру, хранящую информацию о соединениях
            throw err(2, numberOfConnections + 1, stoi((++(values->attributes_begin()))->value())); // отправляем сообщение об ошибки
        }

        if(stoi((++++(values->attributes_begin()))->value()) != 0 && stoi((++++(values->attributes_begin()))->value()) != 10 && stoi((++++(values->attributes_begin()))->value()) != 50 && stoi((++++(values->attributes_begin()))->value()) != 70 && stoi((++++(values->attributes_begin()))->value()) != 100) // проверяем, подходит ли нагрузка соединения под условие задачи
        {
            delete curentConnectionsInfo; // освобождаем выделенную память под структуру, хранящую информацию о соединениях
            throw err(3, numberOfConnections + 1, stoi((++++(values->attributes_begin()))->value())); // отправляем сообщение об ошибки
        }

        curentConnectionsInfo->AddLoad(numberOfConnections, stoi(values->attributes_begin()->value()), stoi((++(values->attributes_begin()))->value()), stoi((++++(values->attributes_begin()))->value())); // добавляем в структуру информацию о соединении
        numberOfConnections++; // увеличиваем счетчик прочитанных процессов
    }
    if(numberOfConnections != count)//ошибка в кол-ве соединений
    {
        delete curentConnectionsInfo; // освобождаем выделенную память под структуру, хранящую информацию о соединениях
        throw err(4, count, numberOfConnections); // отправляем сообщение об ошибки
    }
    return curentConnectionsInfo; // возвращаем указатель на структуру
}