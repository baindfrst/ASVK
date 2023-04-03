// в этом файле описаны функции для чтения информации из входного файла

struct err // структура, в которой хранится информация о ошибках
{
    int numproc;
    int load;
    int typeerr; // тип ошибки
    err(int type, int num, int l);
};


ProccesorsInfo* ReadProccesorsInfo(const char* nameOfInputFile); // функция для чтения из входного файла nameOfInputFile информации о процессорах
WorkerInfo* ReadWorkersInfo(const char* nameOfInputFile); // функция для чтения из входного файла nameOfInputFile информации о процессах
ConnectionInfo* ReadConnectionInfo(const char* nameOfInputFile); // функция для чтения из входного файла nameOfInputFile информации о соединениях