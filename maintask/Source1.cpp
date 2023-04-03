#include <iostream>
#include <vector>
#include "pugixml/src/pugixml.hpp" // библиотека для работы с xml-файлами
#include "ConnectionInfo.hpp" // в этом файле описана информация о структуре для информации о соединениях
#include "ProccesorsInfo.hpp" // в этом файле описана информация о структуре для информации о процессорах
#include "WorkerInfo.hpp" // в этом файле описана информация о структуре для информации о процессах
#include "Readingxml.hpp" // в этом файле описаны функции для чтения информации из входного файла
#include "VectorsWork.hpp" // в этом файле описаны функции для работы с векторами из алгоритма поиска решения
#include <chrono>
#include <string>

using namespace std;
using namespace pugi;

int main(int argc, char** argv) //главная функция программы, получающая в командной строке название файла с входными данными
{
    //auto start = std::chrono::steady_clock::now(); //нужно для проверки скорости работы программы

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
    
    int minLoadOfConnections = curentConnectionInfo->CalculationMaxLoad(); // высчитываем максимальную нагрузку на сеть и считаем ее изначально худшим вариантом
    vector<int> *answerVector = new vector<int>(curentWorkersInfo->getCountOfWorkers(), -1); // создаем вектор, в котором будет храниться ответ на задачу, изначально он заполнен -1
    bool successEnd = 0; // флаг, отвечающий за то, что мы нашли решение
    long long etr = 0; // количество итераций при поиске
    int i = 0; // количество итераций, которое прошло до нахождения решения
    for(i = 0; i != 5000; i++) // начинаем цикл для поиска решения
    {
        vector<int>* randomAnswer = genVectorOfProcces(answerVector->size(), curentProccesorsInfo->getCountOfProcessors()); // генерируем вектор со случайным распредилением процессов
        vector<int>* calculOfLoad = calculationLoadVect(randomAnswer, curentProccesorsInfo->getCountOfProcessors(), curentWorkersInfo); //расчитываем нагрузку на каждый процессор при сгенерированном решении, записываем полученную информацию в вектор
        if(checkVectLoad(calculOfLoad, curentProccesorsInfo)) // проверяем, не превышена ли максимальная нагрузка на каждый процессор при заданном праспределении нагрузки
        {
            int LoadOfRandVec = calculLoadConnection(randomAnswer, curentConnectionInfo); // расчитываем нагрузку на сеть при полученном решении
            if(LoadOfRandVec < minLoadOfConnections ) // проверяем, лучше ли полученное решение того, что мы до этого считаль лучшим 
            {
                successEnd = true; // устанавливаем флаг того, что мы нашли решение на "true"
                delete answerVector; // освобождаем память, выделенную на "старый" ответ
                answerVector = randomAnswer; // запоминаем указатель на вектор с полученным ответом
                minLoadOfConnections = LoadOfRandVec; // обновляем минимальную нагрузку на сеть, что мы смогли получить
                if(LoadOfRandVec == 0) // проверяем, является ли полученное решение "идеальным", с нулевой нагрузкой на сеть
                {
                    delete calculOfLoad; // освобождаем память, выделенную под вектор с нагрузкой на процессоры
                    break; // завершаем поиск решений
                }
                etr += i;// прибавляем кол-во интераций, которые были сделанны между получением прошлого и нового ответа
                i=0; // начинаем заново считать количество итераций
            }
            else
            {
                delete randomAnswer; // если мы не нашли ответ, то освобождаем память, выделенную под новый ответ
            }
        }
        else
        {
            delete randomAnswer; //если в полученном ответе была превышена нагрузка на хотя бы 1 процессор, то освобождаем память, выделенную под новый ответ
        } 
        delete calculOfLoad; //освобождаем память, выделенную под вектор с нагрузкой на процессоры
    }

    etr += i; //прибавляем к общему числу итераций то, что мы проделали до выхода их цикла алгоритма

    if(successEnd) // проверяем, нашелся ли ответ
    {//вывод при успешном варианте
        cout << "succese"<< endl;
        cout << etr << endl;
        for(int j = 0; j != answerVector->size(); j++)
        {
            cout << (*answerVector)[j] << " ";
        }
        cout << "LOAD: " << minLoadOfConnections <<endl;;
    }
    else
    {//вывод при неуспешном варианте
        cout << "failure"<< endl;
        cout << etr << endl;
    }
    delete answerVector; // освобождаем память, выделенную под вектро с ответом
    delete curentProccesorsInfo; // освобождаем память, выделенную под информацию о процессорах
    delete curentWorkersInfo; // освобождаем память, выделенную под информацию о процессах
    delete curentConnectionInfo; // освобождаем память, выделенную под информацию о соединениях
    // auto end = std::chrono::steady_clock::now();//нужно для проверки скорости работы программы 
    // auto diff = end - start;
    // std::cout << std::chrono::duration <double, std::milli> (diff).count() << std::endl; 
    return 0;
}