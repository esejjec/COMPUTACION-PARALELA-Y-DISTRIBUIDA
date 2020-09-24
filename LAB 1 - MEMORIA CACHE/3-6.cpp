#include<iostream> 
#include <ctime> 

unsigned t0, t1;
using namespace std; 

void Multi_6_Bucles(double A[500][500],double B[500][500],double C[500][500], int MAX ){    
    //if(CA==FB){//para que se puedan multiplicar dos matrices
        int salto = MAX/10;
        for(int i1=0; i1<MAX; i1+=salto)
        {
            for(int i2=0; i2<MAX; i2+=salto)
            {
                for(int i3=0; i3<MAX; i3+=salto)
                {
                    int I1 =  min(i1 + salto, MAX);
                    for (int i = i1; i < I1; ++i)
                    {
                        int I2 = min(i2 + salto, MAX);
                        for (int j = i2; j < I2; ++j)
                        {
                            int I3 =min(i3 + salto, MAX);
                            for (int k = i3; k < I3; ++k)
                            {
                                C[i][j] += A[i][k] * B[k][j];
                            }
                        }
                    }
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
    Multi_6_Bucles(A,B,C,MAX);
    t1 = clock();
    double Time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << Time << endl;
    
    return 0;
}