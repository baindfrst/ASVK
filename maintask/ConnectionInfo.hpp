// в этом файле описана информация о структуре для информации о соединениях

#include <ostream>
class ConnectionInfo;
class ConnectionPars // класс, хранящая информацию об одном соединении
{
    public:
    void setPar(int frst, int scnd, int load); //коснтруктор
    friend std::ostream& operator<< (std::ostream& out, const ConnectionInfo& inp); //определение оператора << для вывода информации (было написанно для проверки работы программы)
    friend ConnectionInfo; // обЪявляем другом класс, содержащий информацию о всех соединениях
    int getFrstConnection(); // get-функция для первого процесса соединения
    int getScndConnection(); // get-функция для второго процесса соединения
    int getLoadOfConnection(); // get-функция для нагрузки соединения
    private:
    int frstConnection = 0; //поле для первого процесса соединения
    int scndConnection = 0; //поле для второго процесса соединения
    int loadOfConnection = 0;//поле для нагрузки соединения
};

class ConnectionInfo //класс, содержащий информацию о всех соединениях
{
    public:
    ConnectionInfo(int input); // конструктор
    ~ConnectionInfo(); // деструктор
    int AddLoad(int number, int frst, int scnd, int load); // метод, для добавления нового соединения
    int GetCountOfConnections(); // get-функция для количества соединений
    int CalculationMaxLoad(); // метод, для того, что бы расчитать максимальную нагрузку соединений
    ConnectionPars getConnection(int number); // get-функция для соединение под индексом "number"
    friend std::ostream& operator<< (std::ostream& out, const ConnectionInfo& inp); // определение оператора << для вывода информации (было написанно для проверки работы программы)
    private:
    int countOfConnections; // поле для количества соединений
    ConnectionPars *connections; // поле для хранения массива структур соединений 
};