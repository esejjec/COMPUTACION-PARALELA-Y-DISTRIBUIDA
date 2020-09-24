#include<iostream> 
#include <ctime> 

unsigned t0, t1;
using namespace std; 

void multiplicacion(double A[500][500],int FA,int CA,double B[500][500],int FB,int CB,double C[500][500] ){
    
    if(CA==FB){//para que se puedan multiplicar dos matrices
        for(int i=0;i<FA;i++){
            for(int j=0;j<CB;j++){
                C[i][j]=0;//inicializar
                for(int k=0;k<CA;k++){
                    C[i][j]=C[i][j]+A[i][k]*B[k][j];
                }
            }
        }
    }
} 

void Generar_matriz(double M[500][500],int f,int c){
    for(int i=0;i<f;i++){
        for(int j=0;j<c;j++){
            M[i][j] = rand()%51;
        }
    }
} 

int main (int argc, char *argv[]) {
    
    double A[500][500];
    double B[500][500];
    double C[500][500];

    int FA,CA,FB,CB;
    cout<<"MATRIZ A:"<<endl;
    cout<<"Ingrese FILAS de la Matriz A: "; cin>>FA;
    cout<<"Ingrese COLUMNAS de la Matriz A: "; cin>>CA;
    Generar_matriz(A,FA,CA);
    cout<<"MATRIZ B:"<<endl;
    FB=CA; //Mismo numero de filas B y Columnas A
    cout<<"ingrese COLUMNAS de la Matriz B: "; cin>>CB;
    Generar_matriz(B,FB,CB);
    
    t0 = clock();
    multipliCAcion(A,FA,CA,B,FB,CB,C);
    t1 = clock();
    double Time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << Time << endl;
    
    return 0;
}