#include <iostream>
#include <vector>
#include "pugixml/src/pugixml.hpp"  // библиотека для работы с xml-файлами
#include "ConnectionInfo.hpp" // в этом файле описана информация о структуре для информации о соединениях
#include "ProccesorsInfo.hpp" // в этом файле описана информация о структуре для информации о процессорах
#include "WorkerInfo.hpp" // в этом файле описана информация о структуре для информации о процессах
#include "Readingxml.hpp" // в этом файле описаны функции для чтения информации из входного файла
#include "VectorsWork.hpp" // в этом файле описаны функции для работы с векторами из алгоритма поиска решения
#include "MultiShm.hpp" // в этом файле описана структура класс для передачи между процессами информации
#include "MultiFunc.hpp"
#include <chrono>

using namespace std;
using namespace pugi;

int main(int argc, char** argv) //главная функция программы, получающая в командной строке название файла с входными данными
{
    //auto start = std::chrono::steady_clock::now(); //для проверки скорости выполнения программы

    ProccesorsInfo* curentProccesorsInfo; // объявление указателя на структуру, хранящую информацию о процессорах
    if(argc != 2) //неправильное количество входных файлов
    {
        cerr << "arg err"<<endl;
        return -1;
    }
    char *inputFileName = argv[1]; // получаем название файла из командной строки
    xml_document check;
    if(!check.load_file(inputFileName))
    { //входной файл не найден
        cerr << "cant find file"<<endl;
        return -1;
    }
    try
    {
        curentProccesorsInfo = ReadProccesorsInfo(inputFileName); // попытка считать информацию о процессорах из входного файла 

    }
    catch(err& e) // обработка ошибок, если они были вызваны
    {
        switch (e.typeerr)
        {
        case -1:
            cerr << "Error struct file"<<endl;
            break;
        case 0: // ошибка в максимальной нагрузки процессора
            cerr << "Error input data, processor num: "<< e.numproc << " have wrong value: " << e.load <<endl;
            break;
        case 1: // ошибка в количестве процессоров
            cerr << "Error input data, wrong caunt of processors: "<< e.numproc <<endl;
            break;
        case 2: // ошибка в номерах процессоров (номера начинаются с 1 и идут друг за другом)
            cerr << "Error input data, please write correct number of processor(the processors numbers start with 1 and go in order one by one): " << e.numproc << endl;
            break;
        default:
            break;
        }
        return -1;
    }

    WorkerInfo* curentWorkersInfo;// попытка считать информацию о процессах из входного файла 
    try
    {
        curentWorkersInfo = ReadWorkersInfo(inputFileName);
    }
    catch(err& e) // обработка ошибок, если они были вызваны
    {
        delete curentProccesorsInfo; // удаляем информацию о процессорах для высвобождения памяти
        switch (e.typeerr)
        {
        case -1:
            cerr << "Error struct file"<<endl;
            break;
        case 0: // ошибка в максимальной нагрузки процесса
            cerr << "Error input data, worker num: "<< e.numproc << " have wrong value: " << e.load <<endl;
            break;
        case 1: // ошибка в количестве процессов
            cerr << "Error input data, wrong caunt of workers: "<< e.numproc <<endl;
            break;
        case 2: // ошибка в номерах процессов (номера начинаются с 1 и идут друг за другом)
            cerr << "Error input data, please write correct number of worker(the workers numbers start with 1 and go in order one by one): " << e.numproc << endl;
            break;
        default:
            break;
        }
        return -2;
    }

    ConnectionInfo* curentConnectionInfo;
    try
    {
        curentConnectionInfo = ReadConnectionInfo(inputFileName); // попытка считать информацию о соединениях из входного файла 
    }
    catch(err& e)  // обработка ошибок, если они были вызваны
    {
        delete curentProccesorsInfo;// удаляем информацию о процессорах для высвобождения памяти
        delete curentWorkersInfo;// удаляем информациипроцессов для высвобождения памяти
        switch (e.typeerr)
        {
        case -1:
            cerr << "Error struct file"<<endl;
            break;
        case 0:  // ошибка в номере первого процесса в соединении
            cerr << "Error input data, connection num"<< e.numproc << " have wrong value of frst worker: " << e.load <<endl;
            break;
        case 1: // ошибка в номере второго процесса в соединении
            cerr << "Error input data, connection num "<< e.numproc << " have wrong value of scnd worker: " << e.load <<endl;
            break;
        case 2: // ошибка при соединении процесса к самому себе
            cerr << "Error input data, connection num "<< e.numproc << " val is equal: " << e.load <<endl;
            break;
        case 3: // ошибка в значении нагрузки соединения
            cerr << "Error input data, connection num "<< e.numproc << " load is incorrect: " << e.load <<endl;
            break;
        case 4: // ошибка в кол-ве соединений
            cerr << "Error input data, connection count: mast have: "<< e.numproc << ", but in file: " << e.load <<endl;
            break;
        default:
            break;
        }
        return -2;
    }

    MultiShm *shmem = new MultiShm(curentConnectionInfo->CalculationMaxLoad(), new vector<int>(curentWorkersInfo->getCountOfWorkers(), -1)); // выделяем память и создаем структуру для передачи между процессами информации
    #pragma omp parallel num_threads(2) //запускаем код ниже параллельно
    {
        MultiFunc(shmem, curentProccesorsInfo, curentWorkersInfo, curentConnectionInfo); // вызываем функцию с алгортмом
    }
    
    if(shmem->successEnd) // проверяем, нашелся ли ответ
    {//вывод при успешном варианте
        cout << "succese"<< endl;
        cout << shmem->etr << endl;
        for(int j = 0; j != shmem->answerVector->size(); j++)
        {
            cout << (*(shmem->answerVector))[j] << " ";
        }
        cout << "LOAD: " << shmem->minLoad << endl;
    }
    else
    {//вывод при неуспешном варианте
        cout << "failure"<< endl;
        cout << shmem->etr << endl;
    }
    delete shmem->answerVector; //освобождаем память от структуры для обмена между потоками
    delete shmem;
    delete curentProccesorsInfo; // освобождаем память, выделенную под информацию о процессорах
    delete curentWorkersInfo; // освобождаем память, выделенную под информацию о процессах
    delete curentConnectionInfo; // освобождаем память, выделенную под информацию о соединениях
    // auto end = std::chrono::steady_clock::now();
    // auto diff = end - start;
    // std::cout << std::chrono::duration <double, std::milli> (diff).count() << std::endl; //вывод времени для 1 доп задания
    return 0;
}