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

    // Second pair of loops 
    t02 = clock();
    for(int j = 0; j < MAX; j++)//Ciclo externo
    {    
        for(int i = 0; i < MAX; i++)//Ciclo interno
        {
            y[i] += A[i][j]*x[j];
        }
    }
    t12 = clock();
    double timeBUCLE2 = (double(t12-t02)/CLOCKS_PER_SEC);
    cout << "Execution TimeBUCLE2: " << timeBUCLE2 << endl;

    return 0;
}