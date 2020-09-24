#include "iostream"
#include <ctime> 
 
using namespace std;
unsigned t01, t11, t02, t12;

int main()
{
    int MAX;
    cin >> MAX;
    cout <<MAX<<endl;
    double A[MAX][MAX], x[MAX], y[MAX];


    // First pair of loops 
    t01 = clock();
    for(int i = 0; i < MAX; i++)//Ciclo externo
    {    
        for(int j = 0; j < MAX; j++)//Ciclo interno
        {
            y[i] += A[i][j]*x[j];
        }
    }
    t11 = clock();
    double timeBUCLE1 = (double(t11-t01)/CLOCKS_PER_SEC);
    cout << "Execution TimeBUCLE1: " << timeBUCLE1 << endl;

    return 0;
}