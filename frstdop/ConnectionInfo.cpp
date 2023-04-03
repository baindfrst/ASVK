#include "ConnectionInfo.hpp" // заголовочный файл для данного файла
#include <iostream>

void ConnectionPars::setPar(int frst, int scnd, int load) //коснтруктор класса, хранящего информацию об одном соединении
{
    frstConnection = frst;
    scndConnection = scnd;
    loadOfConnection = load;
}

ConnectionInfo::ConnectionInfo(int input): countOfConnections(input) // конструктор класса, содержащего информацию о всех соединениях
{
    connections = new ConnectionPars[countOfConnections+1]; // выделяем память под массив структур соединений
}
ConnectionInfo::~ConnectionInfo() // деструктор класса, содержащего информацию о всех соединениях
{
    delete[] connections;
}

int ConnectionInfo::AddLoad(int number, int frst, int scnd, int load) // метод, для добавления нового соединения
{
    connections[number].setPar(frst, scnd, load);
    return (1);
}
std::ostream& operator<< (std::ostream& out, const ConnectionInfo& inp)// определение оператора << для вывода информации (было написанно для проверки работы программы)
{
    out << "Number of connections: " << inp.countOfConnections << std::endl;
    for (int i = 0; i != inp.countOfConnections; i++)
    {
        out << "Connection: "<< inp.connections[i].frstConnection << " to " << inp.connections[i].scndConnection << " - " << inp.connections[i].loadOfConnection << std::endl;
    }
    return out;
}

int ConnectionInfo::CalculationMaxLoad()// метод класса, содержащего информацию о всех соединениях, для того, что бы расчитать максимальную нагрузку соединений
{
    int resul = 0;
    for(int i = 0; i != countOfConnections; i++)
    {
        resul += connections[i].loadOfConnection;
    }
    return resul;
}

ConnectionPars ConnectionInfo::getConnection(int number) // get-функция для соединение под индексом "number"
{
    return connections[number];
}

int ConnectionInfo::GetCountOfConnections() // get-функция для количества соединений
{
    return countOfConnections;
}
int ConnectionPars::getFrstConnection() // get-функция для первого процесса соединения
{
    return frstConnection - 1;
}
int ConnectionPars::getScndConnection() // get-функция для второго процесса соединения
{
    return scndConnection - 1;
}
int ConnectionPars::getLoadOfConnection() // get-функция для нагрузки соединения
{
    return loadOfConnection;
}