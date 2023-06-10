#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <math.h>
#include "arbolBinarioBusqueda.h"

float A = (sqrt(5) - 1) / 2;

/**********************/
/*** Funciones hash ***/
/**********************/

// Método de la división
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
int h1(long k, int n) {
  return k%n;
}


// Método de la multiplicación
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
int h2(long k, int n) {
  float a = (float)k*A;
  a -= (long)a; // Esta línea implementa la operación módulo 1 (%1)
  
  return n*a;
}

// Double hashing
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
// i: número del intento
int double_hashing(long k, int n, int i) {
  // Utilizando como primer método el método de la multiplicación y luego el
  // método de la división
  return (h2(k, n) + i*(h1(k, n)+1)) % n;
}

using namespace std;
vector<string> split(string str, char pattern);

int main(int argc, char const *argv[])
{	
	srand(time(NULL)); 
	int n = 29000; // numero de casos
	ifstream archivo1;
	archivo1.open("universities_followers_2022.csv", ios::app);
	//user_id;user_name;
	string a;
	vector<string> b;
	vector<string> user_name;
	vector<long> user_id;
	// hashing cerrado
	vector<long> ht_hashing_cerrdado_dh(n);
	vector<long> ht_hashing_cerrdado_dh2(n);
	//hashing abierto
	vector<vector<long>> ht_hashing_abierto(n);
	vector<vector<long>> ht_hashing_abierto2(n);
	//Sacar primera linea
	getline(archivo1,a,'\n');

	//Crear vectores con Id y Username
	for(int i = 0; i<n;i++){
		getline(archivo1,a,'\n');
		b = split(a, ';');
		long z = stol(b[1]);
		user_id.push_back(z);
		user_name.push_back(b[2]);
	}

	//llenar tablas hash con -1
	for (int i = 0; i < n; ++i)
	{
		ht_hashing_cerrdado_dh[i] = -1;
		ht_hashing_cerrdado_dh2[i] = -1;
	}



// -----------------+++++++-------------------------------/

	string outfile_name =  "n29k_results.csv";
  	ofstream outfile(outfile_name);
  	string column_names = "n,time[ms]\n";
  	outfile << column_names;
	//hashing abierto para entrada entero
	double mm_total_time = 0;
  	auto start_time = std::chrono::high_resolution_clock::now();
	for(int i = 0; i<n;i++){
		long k = user_id[i];

		int hash_value = h1(k, n);

		ht_hashing_abierto[hash_value].push_back(k);	
	}
	auto end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time<< ","  <<"Ashing Abierto Entrada Long"<<endl;



		//hashing abierto para entrada string
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for(int i = 0; i<n;i++){
		//coidigo pasar string a long
		long k = 0;
		for (char c:user_name[i]){
			k += (char) c; 
		}

		int hash_value = h1(k, n);

		ht_hashing_abierto2[hash_value].push_back(k);
		
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time << "," <<"Ashing Abierto Entrada String"<<endl;

// -----------------+++++++-------------------------------/
	//hashing cerrado para entrada string
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for(int i = 0; i<n;i++){

		long k = 0;
		for (char c:user_name[i]){
			k += (char) c; 
		}

		int intentos = 0;

		int hash_value = double_hashing(k, n, intentos);

		while(ht_hashing_cerrdado_dh[hash_value] != -1 && intentos < n){
			intentos += 1;
			hash_value = double_hashing(k, n, intentos);
		}
			//Si encontramos una casilla libre:
		if (ht_hashing_cerrdado_dh2[hash_value] == -1){
			ht_hashing_cerrdado_dh2[hash_value] = k;
		}
		
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time << "," <<"Ashing Cerrado Entrada String"<<endl;

	//hashing cerrado para entrada long
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for(int i = 0; i<n;i++){
		long k = user_id[i];
		int intentos = 0;

		int hash_value = double_hashing(k, n, intentos);

		while(ht_hashing_cerrdado_dh[hash_value] != -1 && intentos < n){
			intentos += 1;
			hash_value = double_hashing(k, n, intentos);
		}
			//Si encontramos una casilla libre:
		if (ht_hashing_cerrdado_dh[hash_value] == -1){
			ht_hashing_cerrdado_dh[hash_value] = k;
		}
		
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time<< ","  <<"Ashing Cerrado Entrada Long"<<endl;

// -----------------+++++++-------------------------------/
	// Arbol binario insertar long
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();

	ArbolBinarioBusqueda arbol;
	for(int i = 0; i < n; i++){
		//coidigo pasar string a long
		// Insertar userid
		bool insertado = arbol.insertar(user_id[i]);

	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time << "," <<"Arbol Entrada Long"<<endl;

	// Arbol binario insertar string
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	ArbolBinarioBusqueda arbol2;
	for(int i = 0; i < n; i++){
		//coidigo pasar string a long
		long nuevo_valor = 0;
		for (char c:user_name[i]){
			nuevo_valor += (char) c; 
		}
		// Insertar username
		bool insertado = arbol2.insertar(nuevo_valor);

	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time<< ","  <<"Arbol Entrada String"<<endl;

//-----------------        +++++++         -------------------------------// 
	// BUSQUEDA on datos que existen 
	// Datos para buscar
	// para buscar datos que si existan se usaran valores en el vector donde se almacenaron los datos al principio
//-----------------------------------
/* 
	// Arbol entrada long
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for (int i = 500; i < 601; ++i)
	{
		arbol.buscar(user_id[i]);
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time<< ","  <<"Buesqueda en Arbol Entrada Long"<<endl;

//------------------------------------
	// Arbol entrada string
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for (int i = 500; i < 601; ++i)
	{
		long nuevo_valor = 0;
		for (char c:user_name[i]){
			nuevo_valor += (char) c; 
		}
		arbol2.buscar(nuevo_valor);
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time << "," <<"Buesqueda en Arbol Entrada String"<<endl;
//-----------------------------------------------------
	//hashing abierto para entrada entero
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for(int i = 500; i < 601;i++){
		long k = user_id[i];

		int hash_value = h1(k, n);
		for(int j = 0; j < ht_hashing_abierto.size(); j++){
			if(ht_hashing_abierto[hash_value][j] == k){
				//Encontrado!!
			}
		}
		
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time << "," <<"Buesqueda ht abierto Entrada Long"<<endl;
//------------------------------------------------------
	//hashing abierto para entrada string
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for(int i = 500; i < 601; i++){
		//coidigo pasar string a long
		long k = 0;
		for (char c:user_name[i]){
			k += (char) c; 
		}

		int hash_value = h1(k, n);
		for(int j = 0; j < ht_hashing_abierto2.size(); j++){
			if(ht_hashing_abierto2[hash_value][j] == k){
				//Encontrado!!
			}
		}
		
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time << "," <<"Buesqueda ht abierto Entrada String"<<endl;
//-----------------------------------------------------------------------
		//hashing cerrado para entrada string
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for(int i = 550; i<601;i++){

		long k = 0;
		for (char c:user_name[i]){
			k += (char) c; 
		}

		int intentos = 0;

		int hash_value = double_hashing(k, n, intentos);

		while(ht_hashing_cerrdado_dh[hash_value] != -1 && intentos < n){
			intentos += 1;
			hash_value = double_hashing(k, n, intentos);
		}
			//Si encontramos una casilla libre:
		if (ht_hashing_cerrdado_dh2[hash_value] == k){
			//Encontrado!!
		}
		
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time << "," <<"Buesqueda ht cerrado Entrada String"<<endl;
//---------------------------------------------------------------------------------
	//hashing cerrado para entrada long
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for(int i = 500; i<601;i++){
		long k = user_id[i];
		int intentos = 0;

		int hash_value = double_hashing(k, n, intentos);

		while(ht_hashing_cerrdado_dh[hash_value] != -1 && intentos < n){
			intentos += 1;
			hash_value = double_hashing(k, n, intentos);
		}
			//Si encontramos una casilla libre:
		if (ht_hashing_cerrdado_dh[hash_value] == k){
			//Encontrado!!
		}
		
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time << "," <<"Buesqueda ht cerrado Entrada Long"<<endl;

//---------------------------------------------------------------------------------

	//Busqueda para datos que no estan en la tabla
	// Se le agregan 100 datos mas a los vectores donde se almacenan los id y usernames asegurando asi que no esteb almacenadas
	//Crear vectores con Id y Username
	for(int i = 0; i<100;i++){
		getline(archivo1,a,'\n');
		b = split(a, ';');
		long z = stol(b[1]);
		user_id.push_back(z);
		user_name.push_back(b[2]);
	}

	//------------------------
		// Arbol entrada long
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for (int i = n; i < n+100; ++i)
	{
		arbol.buscar(user_id[i]);
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time<< ","  <<"Buesqueda en Arbol Entrada Long no listado"<<endl;

//------------------------------------
	// Arbol entrada string
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for (int i = n; i < n+100; ++i)
	{
		long nuevo_valor = 0;
		for (char c:user_name[i]){
			nuevo_valor += (char) c; 
		}
		arbol2.buscar(nuevo_valor);
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time << "," <<"Buesqueda en Arbol Entrada String no listado"<<endl;
//-----------------------------------------------------
	//hashing abierto para entrada entero
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for(int i = n; i < n+100; ++i){

		long k = user_id[i];

		int hash_value = h1(k, n);
		if(ht_hashing_abierto[hash_value].size() != 0){
			for(int j = 0; j < ht_hashing_abierto.size(); j++){
				if(ht_hashing_abierto[hash_value][j] == k){
					//Encontrado!!
				}
			}
		}
		
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time << "," <<"Buesqueda ht abierto Entrada Long no listado"<<endl;
//------------------------------------------------------
	//hashing abierto para entrada string
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for(int i = n; i < n+100; ++i){
		//coidigo pasar string a long
		long k = 0;
		for (char c:user_name[i]){
			k += (char) c; 
		}

		int hash_value = h1(k, n);

		if (ht_hashing_abierto2[hash_value].size() != 0)
		{
			for(int j = 0; j < ht_hashing_abierto2.size(); j++){
				if(ht_hashing_abierto2[hash_value][j] == k){
					//Encontrado!!
				}
			}
		}

		
		
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time<< ","  <<"Buesqueda ht abierto Entrada String no listado"<<endl;
//-----------------------------------------------------------------------
		//hashing cerrado para entrada string
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for(int i = n; i < n+100; ++i){

		long k = 0;
		for (char c:user_name[i]){
			k += (char) c; 
		}

		int intentos = 0;

		int hash_value = double_hashing(k, n, intentos);

		while(ht_hashing_cerrdado_dh[hash_value] != -1 && intentos < n){
			intentos += 1;
			hash_value = double_hashing(k, n, intentos);
		}
			//Si encontramos una casilla libre:
		if (ht_hashing_cerrdado_dh2[hash_value] == k){
			//Encontrado!!
		}
		
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time << "," <<"Buesqueda ht cerrado Entrada String no listado"<<endl;
//---------------------------------------------------------------------------------
	//hashing cerrado para entrada long
	mm_total_time = 0;
  	start_time = std::chrono::high_resolution_clock::now();
	for(int i = n; i < n+100; ++i){
		long k = user_id[i];
		int intentos = 0;

		int hash_value = double_hashing(k, n, intentos);

		while(ht_hashing_cerrdado_dh[hash_value] != -1 && intentos < n){
			intentos += 1;
			hash_value = double_hashing(k, n, intentos);
		}
			//Si encontramos una casilla libre:
		if (ht_hashing_cerrdado_dh[hash_value] == k){
			//Encontrado!!
		}
		
	}
	end_time = std::chrono::high_resolution_clock::now();
	mm_total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	outfile << n << "," << mm_total_time << "," <<"Buesqueda ht cerrado Entrada Long no listado"<<endl;
*/
	outfile.close(); 
	return 0;
}




vector<string> split(string str, char pattern){
	int posInt = 0;
	int posFound = 0;
	string splitted;
	vector<string> resultado;

	while(posFound >= 0){
		posFound = str.find(pattern, posInt);
		splitted = str.substr(posInt, posFound - posInt);
		posInt = posFound + 1;
		resultado.push_back(splitted);
	}
	return resultado;
}