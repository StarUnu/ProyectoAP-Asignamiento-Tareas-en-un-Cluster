#include <stdio.h>
#include <mpi.h>
#include <iostream>
using namespace std;
#define TAG_MESSAGE 1
int main(int argc, char** argv){
	int size;
	int rank;
	double start, finish;
	double time;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int * send_data = new int[size];
	int* recv_data = new int[size];
	MPI_Request* envio_cero;
	MPI_Request* send_request = new  MPI_Request[size];
	MPI_Request* recv_request =new MPI_Request[size];
        start = MPI_Wtime();

	for(int r = 0; r < size; ++r){
		if (r != rank){
			send_data[r] = rank;//rank*1000 + r;
			start = MPI_Wtime();
			MPI_Isend(&send_data[r], 1, MPI_INT, r, TAG_MESSAGE,MPI_COMM_WORLD, &send_request[r]);
	        finish = MPI_Wtime();
	        //cout<<"Actual proceso: "<<rank<<"hacia proceso: "<<r<<" total: "<<finish-start<<endl;
	        double tiempo=finish-start;
	        printf("Actual proceso %d hacia el proceso : %d latencia de envio :) : %lf \n",rank,r,tiempo );
			MPI_Irecv(&recv_data[r], 1, MPI_INT, r, TAG_MESSAGE,MPI_COMM_WORLD, &recv_request[r]);
		}
		else{
			send_request[r] = MPI_REQUEST_NULL;
			recv_request[r] = MPI_REQUEST_NULL;
		}
	}

	int received = 0;
	do{
		int r;
		MPI_Waitany(size, recv_request, &r, MPI_STATUS_IGNORE);
		printf("%i recibio mensaje '%i' desde %i\n", rank, recv_data[r], r);
		++received;
	} while (received < size - 1);
	time=0;

	finish = MPI_Wtime();
	double resta=finish-start;
	double diferencia=resta;
	printf("tiempo total = %f \n",diferencia);
	MPI_Reduce(&diferencia,&time, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);
       if (rank == 0){
         printf("la suma total es = %f \n",time);
        }
	/*
	if(rank==0){
		int * matriz_costo = new int[size*size];
		for(int i=0;i<size-1;i++){
			MPI_Irecv(matriz_costo[i], 1, MPI_DOUBLE, r, TAG_MESSAGE,MPI_COMM_WORLD, &recv_request[r]);
		}
	}
	else{
		MPI_Isend(tiempo, 1, MPI_DOUBLE, 0, TAG_MESSAGE,MPI_COMM_WORLD, &envio_cero);
	}*/
	delete [] recv_request;
	delete [] send_request;
	delete [] recv_data;
	delete [] send_data;
	MPI_Finalize();
	return 0;
	}
