#include <iostream>
#include <vector>
#include "ConnectionInfo.hpp" // в этом файле описана информация о структуре для информации о соединениях
#include "ProccesorsInfo.hpp" // в этом файле описана информация о структуре для информации о процессорах
#include "WorkerInfo.hpp" // в этом файле описана информация о структуре для информации о процессах
#include "VectorsWork.hpp" // заголовочный файл для данного файла
#include <random>

std::vector<int>* genVectorOfProcces(int countOfConnections, int countOfProccesors) // функция для генерации случайного ответа
{
    std::vector<int>* randomVector = new std::vector<int>(countOfConnections); // выделение памяти, под вектор
    std::random_device rd; // описываем структуры для получения случайного числа
    std::mt19937 gen(rd());  
    std::uniform_int_distribution<> dist(1, countOfProccesors); 
    for(int i = 0; i != countOfConnections; i++)
    {
        (*randomVector)[i] = dist(gen); // заполняем вектор случайными числами
    }
    return randomVector; // возвращаем указатель на вектор
}

std::vector<int>* calculationLoadVect(std::vector<int> *vectorAnswer, int countOfProccesor, WorkerInfo* workers) // функция, возвращающая ветор, хранящий нагрузку на каждый процесс в полученном ответе
{
    std::vector<int>* calcVector = new std::vector<int>(countOfProccesor, 0); // выделение памяти, под вектор
    for(int i=0; i != vectorAnswer->size(); i++)
    {
        (*calcVector)[(*vectorAnswer)[i] - 1] += workers->getLoadOfWorker(i); // подсчитываем нагрузку на каждый процессор
    }
    return calcVector; // возвращаем указатель на вектор
}

bool checkVectLoad(std::vector<int> *loadVect, ProccesorsInfo* procInfo) // функция, возвращающая значение "true", если в заданном ответе не превышены максимальные допистимые нагрузки процессов на процессоры, в противном случае возвращается значение "false"
{
    for(int i=0; i != loadVect->size(); i++)
    {
        if((*loadVect)[i] > procInfo->getMaxProcessorLoad(i)) // проверка, превышают ли нагрузки на каждый процессор допустимые значения 
        {
            return false;
        }
    }
    return true;
}

int calculLoadConnection(std::vector<int> *vectorAnswer, ConnectionInfo *conInfo) // функция, подсчитывающая нагрузку на сеть в переданном векторе-ответе
{
    int calcLoad = 0; //переменная, отвечающая за нагрузку
    for(int i = 0; i != conInfo->GetCountOfConnections(); i++)
    {
        ConnectionPars couple = conInfo->getConnection(i); // получаем из структуры, хранящей информацию о всех соединениях, структуру, хранящую информацию о i-том соединении
        if((*vectorAnswer)[couple.getFrstConnection()] != (*vectorAnswer)[couple.getScndConnection()]) // проверяем, не находятся ли процессы из i-ого соединения на 1 процессоре
        {
            calcLoad += couple.getLoadOfConnection(); // добавляем нагрузку i-ого соединения
        }
    }
    return calcLoad; //возвращаем полученнуб нагрузку
}