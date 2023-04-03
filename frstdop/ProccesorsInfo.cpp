#include "ProccesorsInfo.hpp" // заголовочный файл для данного файла
#include <iostream>

ProccesorsInfo::ProccesorsInfo(int input): countOfProcessors(input) // конструктор
{
    maxProcessorsLoad = new int[input];
}

ProccesorsInfo::ProccesorsInfo() // коструктор по умолчанию
{}

ProccesorsInfo::ProccesorsInfo(ProccesorsInfo &f) // коструктор копирования
{
    delete[] maxProcessorsLoad;
    countOfProcessors = f.countOfProcessors;
    maxProcessorsLoad = new int[countOfProcessors];
    for(int i = 0; i != countOfProcessors; i++)
    {
        maxProcessorsLoad[i] = f.maxProcessorsLoad[i];
    }
}

ProccesorsInfo::~ProccesorsInfo() // деструктор
{
    delete[] maxProcessorsLoad;
}

void ProccesorsInfo::AddLoad(int number, int percent) // метод, добавляющий в структуру информацию о процессоре 
{
    maxProcessorsLoad[number-1] = percent;
}

int ProccesorsInfo::getCountOfProcessors() // get-функция для кол-ва 
{
    return countOfProcessors;
}

int ProccesorsInfo::getMaxProcessorLoad(int number) // get-функция для максимальной нагрузки процессора под индексом "number"
{
    return maxProcessorsLoad[number];
}

std::ostream& operator << (std::ostream& out, const ProccesorsInfo &inp) //определение оператора << для вывода информации (было написанно для проверки работы программы)
{
    out << "Number of proccesors: " << inp.countOfProcessors << std::endl;
    out << "Max load of proccesors: " << std::endl;
    for(int i = 0; i != inp.countOfProcessors; i++)
    out << i + 1 << " - " << inp.maxProcessorsLoad[i] << std::endl;
    return out;
}