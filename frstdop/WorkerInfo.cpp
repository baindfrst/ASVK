#include "WorkerInfo.hpp" // заголовочный файл для данного файла
#include <iostream>

WorkerInfo::WorkerInfo(int input): countOfWorkers(input) // конструктор
{
    WorkersLoad = new int[countOfWorkers];
}

WorkerInfo::WorkerInfo()  // коструктор по умолчанию
{}

WorkerInfo::WorkerInfo(WorkerInfo &f) // коструктор копирования
{
    countOfWorkers = f.countOfWorkers;
    delete[] WorkersLoad;
    WorkersLoad = new int[f.countOfWorkers];
    for(int i = 0; i != f.countOfWorkers; i++)
    {
        WorkersLoad[i] = f.WorkersLoad[i];
    }
}

WorkerInfo::~WorkerInfo() // деструктор
{
    delete[] WorkersLoad;
}

void WorkerInfo::AddLoad(int number, int percent) // метод, добавляющий в структуру информацию о процессе
{
    WorkersLoad[number-1] = percent;
}

std::ostream& operator << (std::ostream& out, const WorkerInfo &inp) //определение оператора << для вывода информации (было написанно для проверки работы программы)
{
    out << "Number of workers: " << inp.countOfWorkers << std::endl;
    out << "Max load of workers: " << std::endl;
    for(int i = 0; i != inp.countOfWorkers; i++)
    out << i + 1 << " - " << inp.WorkersLoad[i] << std::endl;
    return out;
}

int WorkerInfo::getCountOfWorkers() // get-функция для кол-ва 
{
    return countOfWorkers;
}

int WorkerInfo::getLoadOfWorker(int number)  // get-функция для нагрузки процесса под индексом "number"
{
    if(number < 0 || number >= countOfWorkers)
    {
        return -1;
    }
    else
    {
        return WorkersLoad[number];
    }
}
