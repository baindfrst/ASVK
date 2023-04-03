#include <iostream>
#include <vector>
#include "MultiShm.hpp" // заголовочный файл для данного файла

MultiShm::MultiShm(int load, std::vector<int>* newVector) // конструктор
{
    minLoad = load;
    answerVector = newVector;
    successEnd = 0;
    etr = 0;
}