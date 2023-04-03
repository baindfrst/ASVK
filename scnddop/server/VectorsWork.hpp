// в этом файле описаны функции для работы с векторами из алгоритма поиска решения

std::vector<int>* genVectorOfProcces(int countOfConnections, int countOfProccesors); // функция для генерации случайного ответа
std::vector<int>* calculationLoadVect(std::vector<int> *vectorAnswer, int countOfProccesor, WorkerInfo* workers); // функция, возвращающая ветор, хранящий нагрузку на каждый процесс в полученном ответе
bool checkVectLoad(std::vector<int> *loadVect, ProccesorsInfo* procInfo); // функция, возвращающая значение "true", если в заданном ответе не превышены максимальные допистимые нагрузки процессов на процессоры, в противном случае возвращается значение "false"
int calculLoadConnection(std::vector<int> *vectorAnswer, ConnectionInfo *conInfo); // функция, подсчитывающая нагрузку на сеть в переданном векторе-ответе