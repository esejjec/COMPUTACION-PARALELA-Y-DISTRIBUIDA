#include<iostream> 
#include <ctime> 

unsigned t0, t1;
using namespace std; 

void multiplicacion(double A[500][500],double B[500][500],double C[500][500], int MAX ){
    
    //if(CA==FB){//para que se puedan multiplicar dos matrices
        C[MAX][MAX] = {0}; //Matriz de ceros 
        for(int i=0; i<MAX; ++i)
        {
            for(int j=0; j<MAX; ++j)
            {
                for(int z=0; z<MAX; ++z)
                {
                    //Acumulacion de bloques
                    C[i][j] += (A[i][z] * B[z][j]);
                }
            }
        }   
    //}
} 

void Generar_matriz(double M[500][500],int f,int c){
    for(int i=0;i<f;i++){
        for(int j=0;j<c;j++){
            M[i][j] = rand()%51;
        }
    }
} 

int main (int argc, char *argv[]) {
    
int MAX;

    double A[500][500];
    double B[500][500];
    double C[500][500];
    cin>>MAX;
    //int FA,CA,FB,CB;
    //cout<<"MATRIZ A:"<<endl;
    //cout<<"Ingrese FILAS de la Matriz A: "; cin>>FA;
    //cout<<"Ingrese COLUMNAS de la Matriz A: "; cin>>CA;
    Generar_matriz(A,MAX,MAX);
    //cout<<"MATRIZ B:"<<endl;
    //FB=CA; //Mismo numero de filas B y Columnas A
    //cout<<"ingrese COLUMNAS de la Matriz B: "; cin>>CB;
    Generar_matriz(B,MAX,MAX);
    
    t0 = clock();
    multiplicacion(A,B,C,MAX);
    t1 = clock();
    double Time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << Time << endl;
    
    return 0;
}