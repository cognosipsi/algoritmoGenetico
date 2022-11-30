#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#define ARCHIVO "comunasvecinas.csv"
#define ARCHIVO2 "costoporcomuna.csv"
using namespace std;

typedef struct comuna{
    int id;
    string nombre;
    int nVecinas;
    double costo;
}comuna;

// Cantidad de individuos por generacion
#define N_POBLACION 36

// Genes validos
const string GENES = "01";

// Funcion objetivo
const string OBJETIVO = "010001000000000100101001001010000000";

// Funcion para generar numeros aleatorios en un rango determinado
int rand(int start, int end)
{
	int rango = (end-start)+1;
	int rand_int = start+(rand()%rango);
	return rand_int;
}

// Funcion para crear genes aleatorios para la mutacion
char genesMutados()
{
	int largo = GENES.size();
	int r = rand(0, largo-1);
	return GENES[r];
}

// Funcion para crear cromosomas o string de genes
string create_gnome()
{
	int largo = OBJETIVO.size();
	string gnome = "";
	for(int i = 0;i<largo;i++)
		gnome += genesMutados();
	return gnome;
}

// Clase de un individuo de la poblacion
class Individuo
{
public:
	string cromosoma;
	int fitness;
	Individuo(string cromosoma);
	Individuo mate(Individuo padre2);
	int cal_fitness();
};

Individuo::Individuo(string cromosoma)
{
	this->cromosoma = cromosoma;
	fitness = cal_fitness();
};

//Algoritmo genetico
Individuo Individuo::mate(Individuo par2) {
	// Cromosoma para la descendencia
	string cromosomaHija = "";

	int largo = cromosoma.size();
	for(int i = 0;i<largo;i++)
	{
		//Entrecruzamiento

        // Probabilidad aleatoria p
		float p = rand(0, 36)/36;

		// Si p es menor a 0.45, se inserta el gen
		// del padre 1
		if(p < 0.45)
			cromosomaHija += cromosoma[i];

		// Si p se encuentre entre 0.45 y 0.90, se inserta
		// el gen del padre 2
		else if(p < 0.90)
			cromosomaHija += par2.cromosoma[i];


        //Mutacion

		// En otro caso, se insertan genes aleatorios (mutacion)
        //para mantener una diversidad
		else
			cromosomaHija += genesMutados();
	}
	return Individuo(cromosomaHija);
};

// Calculo de fitness, es decir, el numero de
// caracteres que difieren del string original
int Individuo::cal_fitness()
{
	int largo = OBJETIVO.size();
	int fitness = 0;
	for(int i = 0;i<largo;i++)
	{
		if(cromosoma[i] != OBJETIVO[i])
			fitness++;
	}
	return fitness;	
};

bool operator<(const Individuo &ind1, const Individuo &ind2)
{
	return ind1.fitness < ind2.fitness;
}

//Codigo principal

int main (void) {
    int i;
    int j = 2;
    comuna vRegion[35];

    for(i=0; i<=35; i++) {
        if (i == 30) {
            j++;
        }
        vRegion[i].id = j;
        j++;
    }

    //contador de vecinos
	ifstream archivo(ARCHIVO);
    string linea;

    int cont = 0;
    char delimitador = ',';
    j=0;

    while (getline (archivo, linea)){
        stringstream stream(linea);
        string numero;
        int i;
        for(i=0;i<=35;i++){
            getline(stream, numero, delimitador);
            if(numero == "1"){
                cont++;
            }
        }
        vRegion[j].nVecinas = cont;
        j++;
        cont = 0;
    }
    archivo.close();

   //contador de costos
    ifstream archivo1(ARCHIVO2);

    i = 0;

    while (getline (archivo1, linea)){
        stringstream stream(linea);
        string num;
        getline(stream, vRegion[i].nombre, delimitador);
        getline(stream, num, delimitador);
        double numero = std::stod(num);
        vRegion[i].costo = numero;
        i++;
        /*cout << nombre << ", " << numero << endl;*/
    }
    archivo1.close();


    j = 2;

    for(i=0; i<=35; i++) {
        if (i == 30) {
            j++;
        }
        vRegion[i].id = j;
        j++;
    }

    srand((unsigned)(time(0)));

	// generacion actual
	int generacion = 0;

	vector<Individuo> poblacion;
	bool found = false;

	// Creacion de poblacion  inicial
	for(int i = 0;i<N_POBLACION;i++)
	{
		string gnome = create_gnome();
		poblacion.push_back(Individuo(gnome));
	}

	
	while(! found)
	{
		// Ordenamiento de poblacion segun su fitness en orden ascendente
		sort(poblacion.begin(), poblacion.end());

        //Al alcanzar el individuo con fitness 0 se termina el bucle
        //ya que habriamos llegado a la solucion inicial
		if(poblacion[0].fitness <= 0)
		{
			found = true;
			break;
		}

		// Sino se genera una nueva descendencia
		vector<Individuo> nueva_generacion;

		// Elitismo, el 10% de la poblacion con mejor fitness
        //pasa a la siguiente generacion
		int s = (10*N_POBLACION)/36;
		for(int i = 0;i<s;i++) {
            nueva_generacion.push_back(poblacion[i]);
        }

		// Del 50% de la poblacion de mejor fitness
		// se escogen para producir una nueva descendencia a futuro
		s = (90*N_POBLACION)/36;
		for(int i = 0;i<s;i++) {
			int largo = poblacion.size();
			int r = rand(0, 18);
			Individuo padre1 = poblacion[r];
			r = rand(0, 18);
			Individuo padre2 = poblacion[r];
			Individuo descendencia = padre1.mate(padre2);
			nueva_generacion.push_back(descendencia);
		}
		poblacion = nueva_generacion;
		cout<< "Generacion: " << generacion << "\t";
		cout<< "Comunas: "<< poblacion[0].cromosoma <<"\t";		
        cout<< "Fitness: "<< poblacion[0].fitness << "\n";

		generacion++;
	}
	cout<< "Generacion: " << generacion << "\t";
	cout<< "Comunas: "<< poblacion[0].cromosoma <<"\t";
	cout<< "Fitness: "<< poblacion[0].fitness << "\n";
};