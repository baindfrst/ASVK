
startMulti: mainMulti
	./mainMulti $(filter-out $@,$(MAKECMDGOALS))

mainMulti: SourceMulti.o ProccesorsInfo.o WorkerInfo.o ConnectionInfo.o Readingxml.o VectorsWork.o MultiShm.o MultiFunc.o
	g++ SourceMulti.o ProccesorsInfo.o WorkerInfo.o ConnectionInfo.o Readingxml.o VectorsWork.o MultiShm.o MultiFunc.o -fopenmp -o mainMulti

startMultiScnd: mainMultiScnd
	./mainMultiScnd $(filter-out $@,$(MAKECMDGOALS))

mainMultiScnd: SourceMultiScnd.o ProccesorsInfo.o WorkerInfo.o ConnectionInfo.o Readingxml.o VectorsWork.o MultiShm.o MultiFunc.o
	g++ SourceMultiScnd.o ProccesorsInfo.o WorkerInfo.o ConnectionInfo.o Readingxml.o VectorsWork.o MultiShm.o MultiFunc.o -fopenmp -o mainMultiScnd


SourceMulti.o: SourceMulti.cpp
	g++ -c -fopenmp SourceMulti.cpp

SourceMultiScnd.o: SourceMultiScnd.cpp
	g++ -c -fopenmp SourceMultiScnd.cpp

MultiShm.o: MultiShm.cpp
	g++ -c MultiShm.cpp

MultiFunc.o: MultiFunc.cpp
	g++ -c MultiFunc.cpp

VectorsWork.o: VectorsWork.cpp
	g++ -c VectorsWork.cpp

Readingxml.o: Readingxml.cpp 
	g++ -c Readingxml.cpp

ConnectionInfo.o: ConnectionInfo.cpp
	g++ -c ConnectionInfo.cpp

WorkerInfo.o: WorkerInfo.cpp
	g++ -c WorkerInfo.cpp

ProccesorsInfo.o: ProccesorsInfo.cpp
	g++ -c ProccesorsInfo.cpp

Source1.o: Source1.cpp
	g++ -c Source1.cpp
	
clean:
	rm *.o main