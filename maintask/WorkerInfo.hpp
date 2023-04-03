// в этом файле описана информация о структуре для информации о процессах

#include <ostream>

class WorkerInfo
{
    public:
    WorkerInfo(int input);// конструктор
    WorkerInfo(); // коструктор по умолчанию
    WorkerInfo(WorkerInfo &f);  // коструктор копирования
    ~WorkerInfo(); // деструктор
    int getCountOfWorkers(); // get-функция для кол-ва 
    int getLoadOfWorker(int number); // get-функция для нагрузки процесса под индексом "number"
    friend std::ostream& operator << (std::ostream& out, const WorkerInfo &inp); //определение оператора << для вывода информации (было написанно для проверки работы программы)
    void AddLoad(int number, int percent); // метод, добавляющий в структуру информацию о процессе
    private:
    int countOfWorkers; // поле кол-в процессов
    int *WorkersLoad;  // поле для массива нагрузок процессов
};