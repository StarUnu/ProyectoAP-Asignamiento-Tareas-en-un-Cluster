#include<iostream>
//#include<conio.h>
#include<stdio.h>
#include<iomanip>
#include <curses.h>

//#define tamano 50

 enum boolean{
  False,
  True,
} ;
/*
enum boolean{ false, true };
typedef enum boolean boolean;*/
//typedef enum boolean { FALSE, TRUE } boolean;

using namespace std;


void incializacion(float **&a,int tam){
      a=new float*[tam];
      for(int i=0;i<tam;i++){
        a[i]=new float[tam];
      }
};

class HungarianMethod{
private:
	float ** data;
	int CostoNormalizado;
	float ** allocation;
	int no_of_rows, no_of_columns, no_of_lines;
	int bal_stat;
	int tamano;
public:
	HungarianMethod(){
	}
	
	void set_tamano(int tamano1){
		this->tamano=tamano1;
	}

	HungarianMethod(int tamano1){
		tamano=tamano1;
		int i, j;
		incializacion(data,tamano);
		incializacion(allocation,tamano);
		for (i = 0; i<tamano; i++){
			for (j = 0; j<tamano; j++){
				data[i][j] = 0;
				allocation[i][j] = 0;
			}
		}
		
		no_of_rows = no_of_columns = bal_stat = no_of_lines = 0;
	}

	void getData();
	void makeAllocation(float ** m_data,float ** & temp_data);
	void display();
	void checkZero(float** & temp, int row, int col,float fil_0[],float col_0[]);
	void rowMinima(float ** & temp);
	void columnMinima(float** & temp);
	boolean checkValue(int, int,float ** temp);
};

void HungarianMethod::getData(){
	int i, j;
	cout<<"el tamano "<<endl;
	cout << "enter cost Metrix :\n";
	for (i = 0; i<tamano; i++){
		cout << "enter " << i + 1 << " row :";
		for (j = 0; j<tamano; j++)
			cin >> data[i][j];
	}
	
	
	
}
/*
int getMaxVal(int temp[][tamano], int row, int col){
	int tamano = temp[0][0];
	for (int j = 0; j < row; j++)
	for (int i = 0; i < col; i++)
		if (tamano>temp[i][j])
			tamano = temp[i][j];
	return tamano;
}
int getPosition(int temp[], int no, int value){
	for (int i = 0; i<no; i++)
		if (temp[i] == value)
			return i;
	return -1;
}
int countVal(int temp[], int no, int value){
	int i, sum = 0;
	for (i = 0; i<no; i++)
		if (temp[i] == value)
			sum++;
	return sum;
}*/

void HungarianMethod::rowMinima(float ** & temp){
	int i, j, min;
	for (i = 0; i<tamano; i++){
		min = 9999;
		for (j = 0; j<tamano; j++)
			if (min>temp[i][j])
				min = temp[i][j];
		for (j = 0; j<tamano; j++)
			temp[i][j] -= min;
	}
}

void HungarianMethod::columnMinima(float ** & temp){
	int i, j, min;
	for (i = 0; i<tamano; i++){
		min = 9999;
		for (j = 0; j<tamano; j++)
			if (min>temp[j][i])
				min = temp[j][i];
		for (j = 0; j<tamano; j++)
			temp[j][i] -= min;
	}
}

void HungarianMethod::checkZero(float** & temp, int row, int col,float fil_0[],float col_0[])
{
	rowMinima(temp);
	columnMinima(temp);

	for (int i = 0; i<tamano; i++)
	{

		for (int j = 0; j < tamano; j++){
			cout<<temp[i][j]<<" ";
		}
		cout<<endl;
	}

	int zrow, zcol;
	int check = TRUE;//boolean

	float temp_col[tamano]={0};
	float temp_fil[tamano]={0};
	//cout<<"cantidad de lineas inicio"<< no_of_lines <<endl;
	while (check)
	{
		int mrow = 0, mcol = 0;
		int trow = 0, tcol = 0;
		for (int i = 0; i < tamano; i++)
		{
			zrow = 0;
			for (int j = 0; j < tamano; j++)
				if (temp[i][j] == 0)
				{
					zrow++;
					if (mrow < zrow)
					{
						mrow = zrow;//cantidad de ceros que hay en una fila
						trow = i;
					}
				}
		}

		for (int i = 0; i < tamano ; i++)
		{
			zcol = 0;
			for (int j = 0; j < tamano; j++)
				if (temp[j][i] == 0)
				{
					zcol++;
					if (mcol < zcol)
					{
						mcol = zcol;//cantidad de ceros que hay en una columna
						tcol = i;
					}
				}
		}
		/*cout<<mrow<<"  "<<mcol<<endl;
		int a;
		cin>>a;*/

		if (mrow == 0 && mcol == 0)
		{check = FALSE;}

		else if (mrow >= mcol)
		{
			
			no_of_lines++;
			int entra=0;//0 falso
			int linea_vertical=0;
			int indice_g=0;
			for (int j = 0; j < tamano; j++)
			{	
				if (temp[trow][j] == 0){
					if(mrow==mcol and temp_col[j]==1 ){
						linea_vertical+=1;
						indice_g=j;
					}
					entra+=1;
					temp[trow][j] = 999999;//identificacion que hay una linea en la fila
					temp_col[j]=1;//presencia que hay una line vertical
				}
			}
			if(entra>=2 or linea_vertical!= entra)
				fil_0[trow]=1;

			if(entra==1){
				col_0[indice_g]=1;
			}
			
			//cout<<"fila que afecta deberas o que no "<<trow<<"  "<<fil_0[trow];
			//cout<<"primero entra por aca en las filas mayor a columas"<<no_of_lines<<endl;
			
		}
		else if (mrow < mcol)
		{
			no_of_lines++;
			//bool entra=false;
			for (int i = 0; i < tamano; i++)
				if (temp[i][tcol] == 0)
				{
					/*
					if(mrow==mcol and temp_fil[i]==1 ){
						entra=true;
						col_0[j]=1;
					}*/
					temp_col[tcol]=1;
					temp[i][tcol] = 999998;//identificacion que hay linea en la
					temp_fil[i]=1;
				}
			col_0[tcol]=1;
			
		}

	}	

	for (int i = 0; i<tamano; i++)
	{

		for (int j = 0; j < tamano; j++){
			if(temp[i][j]==999999 or temp[i][j]==999998){
						temp[i][j]=0;
			}
		}
	}
	cout << "number of lines: " << no_of_lines << endl;

	cout<<"cantidad de lineas final"<< no_of_lines <<endl;
}

void HungarianMethod::makeAllocation(float ** m_data,float ** & temp_data){
	data=m_data;
	//int tmp_data[tamano][tamano] = { 0 };
	int i, j;
	/*
	if (no_of_rows>no_of_columns){
		for (i = 0; i<no_of_rows; i++)
			data[i][no_of_columns] = getMaxVal(temp_data, no_of_rows, no_of_columns);
		no_of_columns++;
		bal_stat = 1;
	}
	else if (no_of_rows<no_of_columns){
		for (i = 0; i<no_of_columns; i++)
			data[no_of_rows][i] = 0;
		no_of_rows++;
		bal_stat = 2;
	}*/

	cout<<"Matriz de entrada"<<endl;
	for (int i = 0; i<tamano; i++){
		for (int j = 0; j<tamano; j++)
			cout<<data[i][j]<<" ";
		cout<<endl;
	}

	for (int i = 0; i<tamano; i++)
	{
		for (int j = 0; j<tamano; j++)
			temp_data[i][j]=data[i][j];
		cout << endl;
	} 
	
	int count=0;


	while (true){

	float fil_0[tamano] = { 0 };
	float col_0[tamano] = { 0 };
	no_of_lines=0;

	checkZero(temp_data, no_of_rows, no_of_columns,fil_0,col_0);//son laslinesas
	
	for (int cf=0;cf<tamano;cf++)
	{
		cout<<" "<<fil_0[cf]<<" ";
	}
	cout<<endl;

	for (int cf=0;cf<tamano;cf++)
	{
		cout<<" "<<col_0[cf]<<" ";
	}
	cout<<endl;

	cout << "number of lines: " << no_of_lines << endl;
	
	if (no_of_lines >= tamano)
	{	
		cout<<"imprimiendo la matriz deberas de datos"<<endl;
		for (int mf=0;mf <tamano;mf++){
			for (int cf=0;cf<tamano;cf++)
			{
				cout<<" "<<temp_data[mf][cf]<<" ";
			}
			cout<<endl;
		}
		break;
	}
    else{
    	int minimo=99999;//menor numero cubierto por alguna linea

    	for (int i = 0; i < tamano ; i++)
		{
			if(!fil_0[i]){
				for (int j = 0; j < tamano; j++)
					if(minimo>temp_data[i][j] and col_0[j]!=1 and fil_0[i]!=1){
						minimo=temp_data[i][j];
					}
			}

		}
		/*
		cout<<"imprimiendo la matriz deberas de datos"<<endl;
		for (int mf=0;mf <tamano;mf++){
			for (int cf=0;cf<tamano;cf++)
			{
				cout<<" "<<temp_data[mf][cf]<<" ";
			}
			cout<<endl;
		}*/

		cout<<"minimo valor"<<minimo<<endl;

		for (int i = 0; i < tamano ; i++)
		{
			if(!fil_0[i]){
				for (int j = 0; j < tamano; j++)
					temp_data[i][j]-=minimo;
			}
			if(col_0[i])
				for (int j = 0; j < tamano; j++)
					temp_data[j][i]+=minimo;
			}
		}
		count+=1;
		cout<<"Cantidad de elementos deveraz"<<no_of_lines<<endl;
		if (count==100)
			break;
	}

 }
 
void HungarianMethod::display(){
	int i, j;
	cout << "\nGiven Cost Metrix :\n";
	for (i = 0; i<tamano; i++)
		cout << "\t" << char(65 + i);
	cout << endl;
	for (i = 0; i<tamano; i++){
		cout << i + 1;
		for (j = 0; j<tamano; j++)
			cout << "\t" << data[i][j];
		cout << endl;
	}
	if (bal_stat != 0){
		cout << "\n\nhere the give cost metrix is not squar Matrix\n";
		cout << "so this is a unbalance problem and as a solution";
		cout<<"bal_stat"<<bal_stat;
		cout << "\n we have add an extra " << ((bal_stat == 1) ? "column" : "row") << " with 0 value in each\n";
	}
	cout << "\n\nOpportunity Matrix :\n";
	rowMinima(data);
	columnMinima(data);
	for (i = 0; i<tamano; i++){
		for (j = 0; j<tamano; j++)
			cout << "\t" << data[i][j];
		cout << endl;
	}
	int sum = 0;
	cout << "\n\nJobs\t:\tMachine\t:\tCost\n";
	for (i = 0; i<tamano; i++)
		for (j = 0; j<tamano; j++)
			if (allocation[i][j] != 0){
				cout << i + 1 << "\t:\t" << char(65 + j) << "\t:\t" << allocation[i][j];
				sum += allocation[i][j];
				cout << endl;
			}
	cout << "\nTotal Assignment Cost = " << sum << " RS.";
}
/*
int main(){
	int tamano;
	cout<<"Tamaño de la matriz";

	cin>>tamano;
	HungarianMethod hm(tamano);

	float ** data;//[tamano][tamano] = { 0 };

	incializacion(data,tamano);
	for(int i=0;i<tamano;i++){
		for(int j=0;j<tamano;j++){
			data[i][j]=240+tamano*10+j;
		}
	}

	float ** temp_data;//[tamano][tamano] = { 0 };
	incializacion(temp_data,tamano);
	hm.makeAllocation(data,temp_data);
	//hm.display();

	cin.clear();
	cin.ignore();
	cin.get();
	return 0;
}*/