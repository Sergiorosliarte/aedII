#include <iostream>
#include <sys/time.h>
using namespace std;

//Sergio Ros Liarte - G1.2 - 26648692H
//Jose María López Serrano - G1.2 - 49972371H
//Curso 2022-2023, AED II
//Profesor: Francisco José Montoya


/*Solución al problema 2 de la práctica 1 de AED II - Divide y vencerás.
 * Para generar casos de prueba se debe pasar al programa una entrada
 * (bien por teclado o en un fichero de texto plano) con el siguiente
 * formato:
	k
	t1 n1 m1
		...
	tk nk mk
*/
/* Siendo k el número de casos que se quieren probar, y después
 * una línea por cada caso, donde t es el tipo de problema que
 * se quiere resolver (peor,mejor caso o caso promedio): mejor caso = 1,
 * caso promedio = 2, peor caso = 3. n es el tamaño que se quiere que
 * tengan las cadenas A y B, y m es el tamaño de la subcadena que se quiere
 * encontrar
 */


/* Decidimos definir m como variable global porque, una vez en DyV,
 * es una variable de solo lectura, sin embargo si la usamos como
 * variable global es un parámetro más que habría que pasar a los
 * subproblemas y complicaría la recursividad. Al ser una variable global
 * el código queda más limpio.
 */
int m = 0;

/* Función que genera el peor caso, este es, no existe ninguna
 * secuencia de m caracteres 'a' en una cadena y 'z' en la otra,
 * para garantizarlo simplemente generamos cadenas entre 'a' e 'y',
 * así que en ningún momento se llega a 25 de diferencia.
 */
void Generador_peor (string &a, string &b, int n)
{
	srand(time(NULL));
	char r;
	for (int i = 0; i < n; i++)
	{
		r = 'a' + rand()%25;
		a += r;
		r = 'a' + rand()%25;
		b += r;
	}
}

/* Función que genera el caso promedio. Simplemente dos cadenas
 * completamente aleatorias de tamaño n.
 */
void Generador_promedio (string &a, string &b, int n)
{
	srand(time(NULL));
	char r;
	for (int i = 0; i < n; i++)
	{
		r = 'a' + rand()%26;
		a += r;
		r = 'a' + rand()%26;
		b += r;
	}
}

/*Función que genera el mejor caso. Primero crea m caracteres 'a'
 * consecutivos al principio de la cadena a y m 'z' en la b, y el resto
 * lo genera aleatoriamente.
 */
void Generador_mejor (string &a, string &b, int n)
{
	for (int i = 0; i < m; i++)
	{
		a += 'a';
		b += 'z';
	}
	
	srand(time(NULL));
	char r;
	for (int i = m; i < n; i++)
	{
		r = 'a' + rand()%26;
		a += r;
		r = 'a' + rand()%26;
		b += r;
	}
}

//Tipo de dato abstracto que representa la solución al problema (o subproblema).
struct Solucion
{
    int pos, dif;
};

/* Función que obtiene caracter por paracter la diferencia en valor
 * absoluto entre los n elementos de a y de b. Esto es, precalcula el 
 * array D de diferencias.
 */
int* PrecalcularDiferencias (string a, string b)
{
	int *D = new int[a.size()];
	for (int i = 0; i< (int)a.size(); i++)
		D[i] = abs((int) a[i] - (int) b[i]);
	return D;
}

/*Función para determinar si un subproblema es suficientemente pequeño
 * como para usar solución directa. Como se explica en la memoria, el
 * límite lo fijamos en 2m porque si fuera m se podrían generar algunas
 * cadenas de menos de m caracteres que no servirían como soluciones,
 * pero no interesa tampoco coger tamaños más grandes.
 */
bool Pequeno (int inicio, int fin)
{
    if (fin-inicio+1 <= 2*m)
        return true;

    else return false;
}

/*Algoritmo de solución directa. Este algoritmo calcula la suma de las
 * diferencias de los primeros m elementos del subproblema y a partir de ahí
 * va comprobando si eliminando el primero por la izquierda y añadiendo uno
 * por la derecha se obtendría una diferencia mayor, hasta llegar al
 * final de la cadena. Al final devuelve la posición de inicio
 * y la diferencia total de la mejor subcadena de m elementos.
 */
Solucion SolucionDirecta (int D[], int inicio, int fin)
{
    Solucion s;
    int suma = 0;
    for (int k = inicio; k < inicio+m; k++)
        suma += D[k];
    
    s.dif = suma;
    s.pos = inicio;
    int i = inicio;
    int j = inicio + m;

    while (j <= fin)
    {
        suma = suma + D[j] - D[i];
        if (suma > s.dif)
        {
            s.dif = suma;
            s.pos = i+1;
        }
        i++;
        j++;
    }
    return s;
}

/*Algoritmo para combinar soluciones. Compara si la primera o la segunda
 * solución son soluciones óptimas para reducir el tiempo de ejecución
 * si se diera el caso (una subcadena de tamaño m y diferencia m*25), y
 * después, si no se da, compara ambas con la mejor solución de los 2*m-2
 * elementos del medio del problema, estos son en los que se puede haber 
 * cortado una solución válida al dividir el problema en 2 subproblemas
 * iguales por la mitad, quedando una parte al final del subproblema 
 * izquierdo y otra al principio del derecho.
 */ 
Solucion CombinarSoluciones (Solucion sol1, Solucion sol2, int c1, int c2, int D[])
{
		//solución de los 2m-2 elementos del centro
		Solucion solMedio = SolucionDirecta(D, c1, c2);
		if (sol1.dif >= sol2.dif)
		{
			if (sol1.dif >= solMedio.dif)
				return sol1;
				
			else return solMedio;
		}
		else
		{
			if (sol2.dif >= solMedio.dif)
				return sol2;
			
			else return solMedio;
		}
}

/*Algoritmo recursivo principal del método de DyV. Primero se comprueba
 * si el problema es suficientemente pequeño y si lo es se aplica
 * solución directa. Si no lo es se divide en dos subproblemas y se
 * calcula recursivamente la solución de ambos. Una vez hecho esto se
 * combinan su soluciones.
 */
Solucion DivideYVenceras (int D[], int inicio, int fin)
{
	if (Pequeno(inicio, fin))
		return SolucionDirecta(D, inicio, fin);
	
	else 
	{
		//pm es el punto medio del problema
		int pm = (inicio+fin)/2;
		Solucion sol1 = DivideYVenceras(D, inicio, pm);
		if (sol1.dif == m*25)
			return sol1;
		
		Solucion sol2 = DivideYVenceras(D, pm+1, fin);
		if (sol2.dif == m*25)
			return sol2;
		
		return CombinarSoluciones (sol1, sol2, pm-m+2, pm+m-1, D);
	}
}



int main () 
{
	struct timeval ti, tf;
	double tiempo;
	// K - Nº de casos que se quieren resolver
	int k;
	cin>>k;
	//Bucle para resolver los k problemas
	for (int i = 0; i < k; i++)
	{
		/*Cada linea de entrada tiene el formato t n m,
		 siendo t el tipo de problema que se quiere resolver
		 (mejor caso, caso promedio o peor caso)
		 n el tamaño de las cadenas y m el de la subcadena m
		 que se quiere encontrar.*/
		int n, t;
		cin>>t>>n>>m;
		string a = "";
		string b = "";
		switch (t)
		{
			case (1):
				Generador_mejor(a,b,n);
				break;
			case (2):
				Generador_promedio(a,b,n);
				break;
			case (3):
				Generador_peor(a,b,n);
				break;
		}
		//Funcion que calcula el array de diferencias para realizar el algoritmo
		int *D = PrecalcularDiferencias(a,b);
		
		//Código para comprobar que funciona el algoritmo.
		/*for (int j=0; j<(int)a.size(); j++)
			cout<<D[j]<<" ";
		cout<<endl;
		Solucion r = SolucionDirecta(D,0,n-1);
		cout<<"Posición: "<<r.pos<<", Diferencia: "<<r.dif<<"."<<endl;*/
		
		//Obtención del tiempo antes de ejecutar divide y vencerás
		gettimeofday(&ti, NULL);
		
		//Primera llamada del algoritmo recursivo
		Solucion s = DivideYVenceras(D,0,n-1);
		
		//Obtención del tiempo después de ejecutar el algoritmo
		gettimeofday(&tf, NULL);
		
		//Cálculo del tiempo de ejecución del algoritmo de D-y-V en microsegundos
		tiempo = (tf.tv_sec - ti.tv_sec)*1000000 + (tf.tv_usec - ti.tv_usec);
		//Salida
		cout<<i+1<<".: "<<"Posición: "<<s.pos+1<<", Diferencia: "<<s.dif
			<<", Tiempo de ejecución: "<<tiempo<<" us."<<endl;
		delete[] D;
	}
   return 0;
}
