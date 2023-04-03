// в этом файле описана информация о структуре для информации о процессорах

#include <ostream>

class ProccesorsInfo // класс, хранящий информацию о процессорах
{
    public:
    ProccesorsInfo(int input); // конструктор
    ProccesorsInfo(); // коструктор по умолчанию
    ProccesorsInfo(ProccesorsInfo &f); // коструктор копирования
    ~ProccesorsInfo(); // деструктор
    void AddLoad(int number, int percent); // метод, добавляющий в структуру информацию о процессоре 
    friend std::ostream& operator << (std::ostream& out, const ProccesorsInfo &inp); //определение оператора << для вывода информации (было написанно для проверки работы программы)
    int getCountOfProcessors(); // get-функция для кол-ва 
    int getMaxProcessorLoad(int number); // get-функция для максимальной нагрузки процессора под индексом "number"
    
    private:
    int countOfProcessors; // поле кол-в процессоров
    int *maxProcessorsLoad; // поле для массива максимальных нагрузок процессоров
};