#include <iostream>
#include <vector>
#include "ProccesorsInfo.hpp" // в этом файле описана информация о структуре для информации о процессорах
#include "WorkerInfo.hpp" // в этом файле описана информация о структуре для информации о процессах
#include "ConnectionInfo.hpp" // в этом файле описана информация о структуре для информации о соединениях
#include "VectorsWork.hpp" // в этом файле описаны функции для работы с векторами из алгоритма поиска решения
#include "MultiShm.hpp" // в этом файле описана структура класс для передачи между процессами информации
#include "MultiFunc.hpp" // заголовочный файл для данного файла

void MultiFunc(MultiShm *shm, ProccesorsInfo* curentProccesorsInfo, WorkerInfo* curentWorkersInfo, ConnectionInfo *curentConnectionInfo) // функция, которую выполняют создаваемые процессоры, которая выполняет алгоритм для поиска решений
{
    int i = 0;// количество итераций, которое прошло до нахождения решения
    for(i = 0; i != 5000 && shm->minLoad != 0; i++)
    {
        std::vector<int>* randomAnswer = genVectorOfProcces(shm->answerVector->size(), curentProccesorsInfo->getCountOfProcessors()); // высчитываем максимальную нагрузку на сеть и считаем ее изначально худшим вариантом
        std::vector<int>* calculOfLoad = calculationLoadVect(randomAnswer, curentProccesorsInfo->getCountOfProcessors(), curentWorkersInfo); //расчитываем нагрузку на каждый процессор при сгенерированном решении, записываем полученную информацию в вектор
        if(checkVectLoad(calculOfLoad, curentProccesorsInfo)) // проверяем, не превышена ли максимальная нагрузка на каждый процессор при заданном праспределении нагрузки
        {
            int LoadOfRandVec = calculLoadConnection(randomAnswer, curentConnectionInfo); // расчитываем нагрузку на сеть при полученном решении
            if(LoadOfRandVec < shm->minLoad)// проверяем, лучше ли полученное решение того, что мы до этого считаль лучшим 
            {
                
                shm->successEnd = true;// устанавливаем флаг того, что мы нашли решение на "true"
                delete shm->answerVector;// освобождаем память, выделенную на "старый" ответ
                shm->answerVector = randomAnswer;// запоминаем указатель на вектор с полученным ответом
                shm->minLoad = LoadOfRandVec;// обновляем минимальную нагрузку на сеть, что мы смогли получить
                if(LoadOfRandVec == 0) // проверяем, является ли полученное решение "идеальным", с нулевой нагрузкой на сеть
                {
                    delete calculOfLoad; // освобождаем память, выделенную под вектор с нагрузкой на процессоры
                    break; // завершаем поиск решений
                }
                shm->etr += i; // прибавляем кол-во интераций, которые были сделанны между получением прошлого и нового ответа
                i=0;// начинаем заново считать количество итераций
            }
            else
            {
                delete randomAnswer;// если мы не нашли ответ, то освобождаем память, выделенную под новый ответ
            }
        }
        else
        {
            delete randomAnswer; //если в полученном ответе была превышена нагрузка на хотя бы 1 процессор, то освобождаем память, выделенную под новый ответ
        } 
        delete calculOfLoad; //лсвобождаем память, выделенную под вектор с нагрузкой на процессоры
    }
    shm->etr += i;//прибавляем к общему числу итераций то, что мы проделали до выхода их цикла алгоритма
}