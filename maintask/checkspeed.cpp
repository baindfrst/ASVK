#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    float f = 0;
    for(int i = 0; i != 300; i++)
    {
        system("make startMain dataset/TestNum5_1281.xml >ofile.txt");
        string s;
        string sucs;
        string time;
        ifstream out("ofile.txt");
        getline(out, s);
        getline(out, sucs);
        getline(out, s);
        getline(out, s);
        getline(out, time);
        if(sucs == "succese")
        {
            f += stof(time);
        }
        else
        {
            i--;
        }
    }
    cout<< f/300 << endl;
}