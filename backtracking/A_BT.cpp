#include <iostream>
//#include <sys/time.h>
using namespace std;

//Sergio Ros Liarte - G1.2 - 26648692H
//Jose María López Serrano - G1.2 - 49972371H
//Curso 2022-2023, AED II
//Profesor: Francisco José Montoya


/*Solución al problema A de backtracking de la práctica 2 de AED II.*/

//Variables que representan el numero de trabajadores y el de trabajos.
//Decidimos que fueran globales para no pasar tantos parámetros a las funciones.
int nw;
int nt;

/* Función generica "solución". Devuelve verdadero si esta en el último nivel
 * y el trabajador al que se ha asignado la tarea actual tiene capacidad para hacerla.
 */
bool solucion(int nivel, int *s, int *capacidades)
{
    return nivel == nt && capacidades[s[nivel-1]]!=-1;
}

/* Función de poda, ayuda a que el algoritmo sea más eficiente. Para cada rama,
 * calcula que el beneficio máximo teórico que se podría obtener por esa rama
 * para que criterio compruebe que sea superior al máximo almacenado.
 */ 
int poda(int b, int nivel, int *cotas) {
    int max = 0;
    for(int i = nivel; i <= nt; i++)
        max += cotas[i-1];
        
    return max+b;
}

/* Función generica criterio. Comprueba si no se ha llegado al último nivel,
 * si no se ha pasado asignando tareas de más a un trabajador y si el trabajador actualmente
 * asignado a la tarea actual es capaz de realizarla. Si todo esto es verdad comrpueba
 * que el máximo beneficio que se pueda obtener por esta rama supera al máximo
 * actual, y en caso de que no lo sea poda esta rama, es decir, no la explora.
 */
bool criterio(int nivel, int b, int BOA, int *s, int **beneficios, int *capacidades, int *cotas){
    if(nivel == nt || capacidades[s[nivel-1]] < 0 || beneficios[s[nivel-1]][nivel-1] == 0)
        return false;
        
    else
    {
        int p = poda(b, nivel+1, cotas);
        if (p > BOA)
			return true;
			
		else return false;
    }
}

/* Función generica masHermanos. Devuelve verdadero si un nodo tiene más hermanos.
 */
bool masHermanos(int nivel, int *s)
{
    return s[nivel-1] < nw-1;
}
 
/*Función generica "generar". Genera un nuevo nodo del arbol, distinguiendo
 * si es un nodo de un nivel nuevo o es un nodo hermano de otro del mismo nivel.
 */
void generar(int nivel, int &b, int *s, int **beneficios, int *capacidades)
{

    s[nivel-1]++;
    
    if(s[nivel-1] == 0)
    {
	  b += beneficios[s[nivel-1]][nivel-1];
	  capacidades[s[nivel-1]]--;
    }
    
    else
    {
    	b = b + beneficios[s[nivel-1]][nivel-1] - beneficios[s[nivel-1]-1][nivel-1];
        capacidades[s[nivel-1]]--;
        capacidades[s[nivel-1]-1]++;
    }
}
 
 /* Función generica retroceder. Deshace la última decisión tomada y sube un
 * nivel en el árbol.
 */
void retroceder(int &nivel, int &b, int *s, int **beneficios, int *capacidades)
{
    b -= beneficios[s[nivel-1]][nivel-1];
    capacidades[s[nivel-1]]++;
    s[nivel-1]=-1;
    nivel--;
}

/* Función principal. Sígue el esquema teorico del problema de backtracking 
 * por optimización. Hasta que se llegue al nivel 0, el bucle genera un hermano,
 * comprueba si es solución, después si se puede seguir bajando y por último si tiene más hermanos.
 */
int asigna(int **beneficios, int *capacidades, int *cotas)
{
	//Inicializacion de variables
	int *s = new int[nt];
	for(int i = 0;i<nt;i++)
        s[i]=-1;
        
    int nivel = 1;
    int b = 0;
    int BOA = 0;
    
    do 
    {
		//Genera un nuevo nodo del arbol.
        generar(nivel, b, s, beneficios, capacidades);
        
        //Si el nuevo nodo es una solucion más optima que la que se tenía, actualiza.
        if(solucion(nivel, s, capacidades) && b > BOA)
            BOA = b;
            
        //Si cumple el criterio para seguir bajando, aumenta el nivel.
        if (criterio(nivel, b, BOA, s, beneficios, capacidades, cotas))
            nivel++;
            
        else
        {
			//Mientras que no queden hermanos y no se llegue a la raíz, sube un nivel.
            while(!masHermanos(nivel, s) && nivel>0)
                retroceder(nivel, b, s, beneficios, capacidades);
        }
    } while(nivel > 0);
    
    //Se libera la memoria del array solución y se devuelve la soluciñon más óptima.
    delete [] s;
    return BOA;
}

int main() 
{
	//struct timeval ti, tf;
	//double tiempo;
    int T;
    cin >> T;
    //Se lee el numero de casos de la entrada.
    for(int t = 0; t < T; t++){
		//Se leen nw y nt.
        cin >> nw >> nt;
        
        //Se declaran los arrays de beneficios, cotas y capacidades.
        int *capacidades = new int[nw];
        
        int *cotas = new int[nt];
        
        int **beneficios = new int*[nw];
        for(int i = 0; i < nw; i++)
            beneficios[i]=new int[nt];
        
        for(int i = 0; i <nw; i++){
            for(int j = 0; j < nt;j++)
                cin >> beneficios[i][j];
		}
		
        for(int i = 0; i < nw; i++)
            cin >> capacidades[i];
            
        for(int i = 0; i < nt; i++) 
        {
            int max = 0;
            for(int j = 0; j < nw; j++) 
            {
                if (beneficios[j][i] > max)
                    max = beneficios[j][i];
            }
            cotas[i] = max;
        }/*Cotas es un array que guarda el maximo de cada columna del array
        beneficios para poder obtener facilmente cotas superiores del beneficio
        * de una rama para hacer la poda.
		*/
		//Obtención del tiempo antes de ejecutar el algoritmo voraz
		//gettimeofday(&ti, NULL);
        
        int sol = asigna(beneficios, capacidades, cotas);
        
        //Obtención del tiempo después de ejecutar el algoritmo
		//gettimeofday(&tf, NULL);
		
		//Cálculo del tiempo de ejecución del algoritmo de maximizacion en microsegundos
		//tiempo = (tf.tv_sec - ti.tv_sec)*1000000 + (tf.tv_usec - ti.tv_usec);
		//Se llama a la función y se imprime el resultado.
		
        cout << sol << endl; // "Tiempo: " << tiempo << endl;
        
        //Se libera toda la memoria dinámica.
        for(int i = 0; i < nw; i++)
			delete [] beneficios[i];
			
		delete [] beneficios;
		delete [] cotas;
		delete [] capacidades;

    }
}
