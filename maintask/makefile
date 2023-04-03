startMain: main
	./main $(filter-out $@,$(MAKECMDGOALS))

main: Source1.o ProccesorsInfo.o WorkerInfo.o ConnectionInfo.o Readingxml.o VectorsWork.o
	g++ Source1.o ProccesorsInfo.o WorkerInfo.o ConnectionInfo.o Readingxml.o VectorsWork.o -o main

SourceMulti.o: SourceMulti.cpp
	g++ -c SourceMulti.cpp

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