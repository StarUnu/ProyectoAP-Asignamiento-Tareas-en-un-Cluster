#include <iostream>
#include "algoritmo_hungariano.cpp"
#include <fstream>
//#include <stdio.h>
#include "stdio.h"
//#include "conio.h"
#include <stdlib.h>
#include <iomanip>
#include <cstring>

#include "mpi.h"
using namespace std;

#define MAX 8
#define COTA 5

class Mapeo{
	float **CM;//Costo matrix
	int N;
	int arity; //nro e arrays almacenados por diferentes niveles
	float **old_result;
	float **result;
    int num_of_level;
    float **new_matrix_;
    float **aggregated_mat;
    float **mat_adj;
    float **new_result;
    float **oldresult; 
    HungarianMethod objetoHungaro;
	public:

	Mapeo(int arity_);

	void fill(float **&a, int tam, int value);
	void ejecutar();
	float ** aggregate_Cost_Matrix(float **old_matrix_, float **old_result_);
    void re_assignment_of_process_Ranks(int **adjacency_matrix);
    void enter_graph_values (int num_prcs, int *destinations,int *sources, int *degrees, int **adjacency_matrix);
    float **  generarMatrizAdyacencia(float **matrix);   
    void  GenerarDot(float **matrix_, char nombre); 
    float ** Leer_matriz(float ** mat);
    void imprimir(float **mat);
	

};

Mapeo::Mapeo(int arity_){
		N = MAX;
		arity = arity_;
		num_of_level = 3;
		/* vector */
		CM = new float*[MAX];
      	old_result = new float*[MAX];
      	result = new float*[MAX];
      	new_matrix_ = new float*[MAX];
      	aggregated_mat = new float*[MAX];
        mat_adj = new float*[MAX];
        new_result = new float*[MAX];
        oldresult = new float*[MAX];

      	for(int i=0; i<MAX; i++){
        	CM[i]=new float[MAX];
        	old_result[i] = new float[MAX];
        	result[i] = new float[MAX];
        	new_matrix_[i] = new float[MAX];
        	aggregated_mat[i] = new float[MAX];
            mat_adj[i] = new float[MAX];
            new_result[i] = new float[MAX];
            oldresult[i] = new float[MAX];


      	}

        objetoHungaro.set_tamano(N);
}

void Mapeo::fill(float **&a,int tam,int value=1){

  
    srand (time(NULL));
    for(int i=0;i<tam;i++){
      for(int j=0;j<tam;j++){
        a[i][j]=rand() % COTA *value;
      }
    }
};

float **  Mapeo::Leer_matriz(float ** mat)
{
  
  char cadena[128];
  
  ifstream archivo("matriz_2.txt");
  int i=0, j=0;
  float tmp;
  while (!archivo.eof()) {
    archivo >> cadena;//mat[i][j];
    tmp = atof(cadena);
    
    mat[i][j] = tmp;
    
    
    if(j==(N-1)){
        i = i+1;
        j = 0;
    }
    else {
        j++; //avanza en la fila
    }
    
  }

  archivo.close();
  return mat;
}

void Mapeo::imprimir(float **mat){
    cout<<"Imprimir"<<endl;
    for(int i=0;i<N;i++){
      for(int j=0;j<N;j++){
        cout<<mat[i][j]<<"-";
      }
      cout<<endl;
    }
}
void Mapeo::ejecutar(){
	cout<<"Mapeo"<<num_of_level <<endl;
    
    CM = Leer_matriz(CM);
    cout<<"Costo Matriz"<<endl;
    imprimir(CM);
	//fill(CM,N,2);
	
    for(int i = num_of_level; i >= 0; i--){
        //cout<<"Primer i"<<i<<endl;
        for(int j=1; j<= i; j = j*2){
            cout<<"i ="<<i<<" j= "<<j<<endl;
            
            objetoHungaro.makeAllocation(CM,result);
            
            cout<<"Imprimir REsult"<<endl;
            imprimir(result);
            cout<<"------------------"<<endl;
            //if (!old_result){
                //new_result  = aggregate_Cost_Matrix(old_result, new_result);
            //}
            
            //oldresult = new_result;
            //CM = aggregated_mat;
        }
    }
    
    //imprimir(CM);
    //cout<<"------------------"<<endl;
    //cout<<"Ingreso a ejecutar"<<endl;
    //GenerarDot(aggregated_mat,'hola');
    
}
float ** Mapeo::aggregate_Cost_Matrix(float **old_matrix_, float **old_result_){

	for(int i = 0;i <= N/arity + 1;i++){
		//cout<<"i: "<<i<<endl;
        for(int j = 0;j <= N/arity + 1;j++){
        	//cout<<"j:"<<j<<endl;
            new_matrix_[i][j] = 0;

            if(i!= j){
            	//cout<<"Dentro del for "<<endl;
            	
                for(int k = 0;k <= arity + 1;k++){
                	//cout<<"k: "<<k<<endl;
                	
                    for(int p = 0;p <= arity + 1;p++){
                        //cout<<"p: "<<p<<endl;
                        cout<<"i="<<i<<"k="<<k<<"j: "<<j<<"p: "<<p<<endl;

                        cout<<"old_resul l: "<<old_result_[i][k]<<endl;
                        cout<<"old_resul m: "<<old_result_[i][k]<<endl;

                        int l = (int)old_result_[i][k];
                        int m = (int)old_result_[j][p];

                        //cout<<"aristas="<<arity<<endl;
                        //cout<<"i="<<i<<"j="<<j<<"k="<<k<<"p="<<p<<endl;
                        cout<<"Old_matrix: "<<old_matrix_[l][m]<<endl;
                        new_matrix_[i][j] += old_matrix_[l][m];
                        //cout<<"new matriz"<<old_matrix[i][j]<<endl;
                        
                    }
                }
            }
            //cout<<new_matrix_[i][j]<<endl;
            
        }
    }

    for(int i = 0;i <= N/arity + 1;i++){
        for(int j = 0;j <= N/arity + 1;j++){
            cout<<new_matrix_[i][j]<<" ";
            }
            cout<<endl;
    }

    return new_matrix_;
}

void Mapeo::enter_graph_values ( int num_prcs, int *destinations,int *sources, int *degrees, int **adjacency_matrix){

}
void Mapeo::re_assignment_of_process_Ranks(int **adjacency_matrix){
   
    /*MPI_Init(NULL, NULL);
    //int **adjacency_matrix;

    float **cost_matrix;
    
    int n =1;
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int num_prcs;
    int sources = my_rank;
    int degrees[1] = {0};
    int destinations[1] = {0};
    int MPI_UNWEIGHTED[1] = {0};

    MPI_Comm comm_to_use;

    MPI_Info info; int reorder; 
    MPI_Comm *comm_topology;

    if(my_rank == 0){
        //enter_graph_values ( num_prcs, destinations, sources, degrees, adjacency_matrix);
        MPI_Dist_graph_create(MPI_COMM_WORLD, 1, &source, &degree, dest, weight, MPI_INFO_NULL, 1, &graph);

        MPI_Dist_graph_create(MPI_COMM_WORLD, n, &sources, degrees, destinations, MPI_UNWEIGHTED, MPI_INFO_NULL, reorder, &comm_topology);
    }
    else{
        MPI_Dist_graph_create(MPI_COMM_WORLD, 0, NULL, NULL, NULL, MPI_UNWEIGHTED, MPI_INFO_NULL, reorder, &comm_topology);
        
        if(comm_topology != MPI_COMM_NULL){
            comm_to_use = comm_topology;
        }
        else{
            comm_to_use = MPI_COMM_WORLD;
        }
    }
    MPI_Comm_rank ( comm_to_use, &my_rank);
    MPI_Finalize();
    */


}

float **  Mapeo::generarMatrizAdyacencia(float **matrix){
    for(int i = 0;i <= N + 1;i++){
        cout<<"i: "<<i<<endl;
        for(int j = 0;j <= N + 1;j++){
            if(matrix[i][j] >=0){
                mat_adj[i][j] = 1;
            }
            else {
                mat_adj[i][j] = 0;    
            }
            

        }
    }
    return mat_adj;
}

void Mapeo::GenerarDot(float **matrix_, char nombre){
    cout<<"Hello ingreso generar Dot"<<endl;
     /* Dibujar Grafo Principal*/
    ofstream fout("dibujo.dot");
    fout << "digraph G {" << endl;
    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {
            fout << "\t" << i << " -> ";
            fout << "\t" << j<<"[label="<<matrix_[i][j]<<",weight="<<matrix_[i][j]<<"]";
        }
        fout  << ";" << endl;
    }
    fout << "}" << endl;
    fout.close();
}

int main(void)
{
	Mapeo map(3);
	map.ejecutar();

}