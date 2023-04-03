// в этом файле описсана структура для передачи между процессами информации

struct MultiShm
{
    int minLoad; // поле минимально найденной нагрузки на сеть
    std::vector<int>* answerVector; // вектор с ответом
    bool successEnd = 0; // флаг, отвечающий за то, что мы нашли решение
    long long etr; // количество итераций при поиске
    MultiShm(int load, std::vector<int>* newVector); // конструктор
};
