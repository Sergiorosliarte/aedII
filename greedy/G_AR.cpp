#include <iostream>
//#include <sys/time.h>
#define MAX 1
#define MIN 2

using namespace std;

//Sergio Ros Liarte - G1.2 - 26648692H
//Jose María López Serrano - G1.2 - 49972371H
//Curso 2022-2023, AED II
//Profesor: Francisco José Montoya

/*Solución al problema G de Avance Rápido de la práctica 2 de AED II.
*/

//Tipo de dato abstracto que representa la solución al problema.
struct Solucion {
    int * conchas;
    int peso;
};

/*Funciones de selección, escogen el siguiente candidato a formar parte de la solución.
 * Min se utiliza cuando estamos buscando la solución mínima en peso (las conchas que
 * nos devuelven), y Max se utiliza cuando buscamos la solución maxima (las conchas que damos).
 * El algoritmo simplemente va recorriendo buscando el elemento con mejor proporción
 * valor/peso (o peso/valor, respectivamente a Min y Max), siempre comprobando que
 * no se haya utilizado antes
 */
int seleccionaMin(int pesos[], int valores[], int N, bool *elegidos) 
{
	float max = -1.0;
    int j = 0;
    float proporcion = 0.0;
    for (int i = 0; i < N; i++) 
    {
		proporcion = (float) valores[i] / (float) pesos[i];
		if (proporcion > max && !elegidos[i]) 
		{
			j = i;
            max = proporcion;
        }
    }
    
    return j;
}

int seleccionaMax(int pesos[], int valores[], int N, bool *elegidos) 
{
	float max = -1.0;
    int j = 0;
    float proporcion = 0.0;
    for (int i = 0; i < N; i++) 
    {
		proporcion = (float) pesos[i] / (float) valores[i];
		if (proporcion > max && !elegidos[i]) 
		{
			j = i;
            max = proporcion;
        }
    }
    
    return j;
}

/* Funcion principal del algoritmo voraz. Sigue el esquema clásico
 * del problema de optimización del cambio de monedas. Primero inicializa
 * el array de solución a 0, y un array de booleanos para saber que elementos
 * hemos usado o descartado ya. Después, y mientras que el valor acumulado no
 * llegue al objetivo, va obteniendo un nuevo candidato, comprueba si es factible
 * incorporarlo a la solucion, y si es asi incorpora tantas conchas de ese tipo
 * como quepan en la solución. Tanto si lo ha descartado como si no lo marca como usado.
 */ 
Solucion cambio(int pesos[], int valores[], int V, int N, int type) {
    int * x = new int[N];
    bool * elegidos = new bool[N];
    for (int i = 0; i < N; i++)
    {
		x[i] = 0;
		elegidos[i] = false;
    }
    int j, act = 0;
	
	if (type == 1)
	{
		while (act != V) 
		{
			j = seleccionaMax(pesos, valores, N, elegidos);
			if (valores[j] <= (V-act))
			{
				x[j] = (V - act) / valores[j];
				act += (x[j] * valores[j]);
			}
			elegidos[j] = true;
		}
	}
	else
	{
		while (act != V) 
		{
			j = seleccionaMin(pesos, valores, N, elegidos);
			if (valores[j] <= (V-act))
			{
				x[j] = (V - act) / valores[j];
				act += (x[j] * valores[j]);
			}
			elegidos[j] = true;
		}
	}

    Solucion sol;
    sol.peso = 0;
    sol.conchas = x;
     for (int i = 0; i < N; i++)
			sol.peso += sol.conchas[i] * pesos[i];
    delete [] elegidos;
    return sol;
}

/* Método principal. Lee el numero de casos a resolver y para esos T casos
 * lee N y V, el array de valores y el array de pesos. Calcula la solución al
 * problema e imprime la solución. 
 * Estan comentadas las instrucciones de la libreria time que hemos utilzado
 * para medir el tiempo de ejecución y realizar el estudio experimental.
 */ 
int main() {
	struct timeval ti, tf;
	double tiempo1;
	double tiempo2;
    int T, N, V;
    cin >> T;

    for (int i = 0; i < T; i++) {
        cin >> N >> V;
        
        int valores[N] = {};
        for (int i = 0; i < N; i++) 
			cin >> valores[i];
			
		int pesos[N] = {};
		for (int i = 0; i < N; i++) 
			cin >> pesos[i];
			
		//Obtención del tiempo antes de ejecutar el algoritmo voraz
		//gettimeofday(&ti, NULL);
        
        Solucion maxPeso = cambio(pesos, valores, V, N, MAX);
        
        //Obtención del tiempo después de ejecutar el algoritmo
		//gettimeofday(&tf, NULL);
		
		//Cálculo del tiempo de ejecución del algoritmo de maximizacion en microsegundos
		//tiempo1 = (tf.tv_sec - ti.tv_sec)*1000000 + (tf.tv_usec - ti.tv_usec);
        
        
        //Obtención del tiempo antes de ejecutar el algoritmo voraz
		//gettimeofday(&ti, NULL);
		
        Solucion minPeso = cambio(pesos, valores, V, N, MIN);
        
        //Obtención del tiempo después de ejecutar el algoritmo
		//gettimeofday(&tf, NULL);
		
		//Cálculo del tiempo de ejecución del algoritmo de minimizacion en microsegundos
		//tiempo2 = (tf.tv_sec - ti.tv_sec)*1000000 + (tf.tv_usec - ti.tv_usec);
		
        cout << maxPeso.peso - minPeso.peso << endl;
		for (int i = 0; i < N-1; i++) 
			cout << maxPeso.conchas[i] << " ";
		cout<< maxPeso.conchas[N-1] << endl;// << "T: " << tiempo1 << endl;
		
		for (int i = 0; i < N-1; i++) 
			cout << minPeso.conchas[i] << " ";
		cout<< minPeso.conchas[N-1] << endl;// << "T: " << tiempo2 << endl;
        

        delete [] maxPeso.conchas;
        delete [] minPeso.conchas;
    }
}
