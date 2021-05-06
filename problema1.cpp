#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <ctime>
#include <random>
#include <mutex>


using namespace std;

mutex mtx;

constexpr int FLOAT_MIN = 10;
constexpr int FLOAT_MAX = 100;

atomic<float> valor_atomico = -1;

void comp(vector<float> contenedor,int inicio, int final, float& valor_a_retornar)
{
    //Usamos un lock para bloquear la zona critica de la funcion
    mtx.lock();
    //Va a iterar el vector contenedor, el cual contiene las porcion del vector de numeros que le toca al hilo.
    for( ; inicio < final; inicio++ )
    {
        //En este condicional compara el valor de cada posicion del vector contenedor con result que al iniciar es un numero pequeño.
        //Si el numero del vector contenedor es mayor a valor_a_retornar

        if(valor_a_retornar < contenedor.at(inicio))
        {
            //Busco en la lista este valor y lo asigno a valor_a_retornar
            valor_a_retornar = contenedor.at(inicio);
            //cout << endl;
            //cout << "Maximo Local: " << valor_a_retornar << " ";
        }

    }
    mtx.unlock();
    //Liberamos este lock para que se pueda usar por otro hilo

}
float encontrar_el_valor_maximo(const vector<float>& numeros, int num_hilos)
{

    //Se crea un vector de hilos.
    vector<thread*> todos_hilos;
    //Se crea un vector de numeros, en donde se guardara los maximos obtenidos en cada particion.
    vector <float> numeros_maximos;

    //Numero que determinara las porciones que sera partido el vector principal.
    int porciones = (int)(numeros.size()/num_hilos);

    for(int i = 0;i < num_hilos; i++)

    {
        numeros_maximos.push_back(0);

        //Dos condicionales para determinar a que hilo le toca cada porcion del vector principal.
        if(i == (num_hilos-1))
        {
            //Se realiza un push_back en el vector de hilos un nuevo hilo creado.
            todos_hilos.push_back(new thread(comp, numeros, (i*porciones),numeros.size(),ref(numeros_maximos.at(i))));
            //Se realiza un join de espera para que no exista ningun conflicto entre los hilos.
            todos_hilos.at(i)->join();
        }
        else {
            //Se realiza un push_back en el vector de hilos un nuevo hilo creado.
            todos_hilos.push_back(new thread(comp, numeros, (i * porciones), ((i + 1) * porciones), ref(numeros_maximos.at(i))));
            //Se realiza un join de espera para que no exista ningun conflicto entre los hilos.
            todos_hilos.at(i)->join();
        }
    }

    float numero_maximo = 0;
    comp(numeros_maximos,0,numeros_maximos.size(),numero_maximo);

    return numero_maximo;
}
void problema_1(int cantidad_de_hilos, const vector<float>& contenedor_de_numeros){


    float maximo = encontrar_el_valor_maximo(contenedor_de_numeros, cantidad_de_hilos);
    cout<< endl << "Maximo global: " << maximo << endl;

}


////////////////////////////////////////////////////////////////////////////////////////

void comp_2(vector<float> contenedor,int inicio, int final, float& valor_atomico )
{
    for( ; inicio < final; inicio++ )
    {
        if(valor_atomico< contenedor.at(inicio) )
        {
            valor_atomico = contenedor.at(inicio);
            //cout << endl;
            //cout << "Maximo Local: " << valor_atomico << " ";
        }

    }
}


float encontrar_el_valor_maximo_2(const vector<float>& numeros, int num_hilos)
{


    vector<thread*> todos_hilos;
    vector <float> maximos;
    int intervalo = (int)(numeros.size()/num_hilos);

    for(int i = 0;i < num_hilos; i++)

    {
        maximos.push_back(0);

        if(i == (num_hilos-1))
        {
            todos_hilos.push_back(new thread(comp_2, numeros, (i*intervalo),numeros.size(),ref(maximos.at(i))));
            todos_hilos.at(i)->join();

        }
        else {
            todos_hilos.push_back(new thread(comp_2, numeros, (i * intervalo), ((i + 1) * intervalo), ref(maximos.at(i))));
            todos_hilos.at(i)->join();
        }
    }


    comp_2(maximos, 0, maximos.size(), reinterpret_cast<float &>(valor_atomico));

    return valor_atomico;
}
void problema_2(int cantidad_de_hilos_2, const vector<float>&  contenedor_de_numeros_2){


    float maximo = encontrar_el_valor_maximo_2(contenedor_de_numeros_2, cantidad_de_hilos_2);
    cout<< endl <<"Maximo global: " << maximo<< endl;


}


void calcular_tiempo_1(int n, int cantidad_de_elementos_del_vector){

    vector<float> cn;

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distr(FLOAT_MIN, FLOAT_MAX);

    for (int i = 0; i < cantidad_de_elementos_del_vector; ++i) {
        float numero_aleatorio  = distr(eng);
        cn.push_back(numero_aleatorio);
    }

    unsigned t0, t1;
    t0=clock();
    problema_1(n,cn);
    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Tiempo de ejecucion usando locks: " << time << endl;

}



void calcular_tiempo_2(int n, int cantidad_de_elementos_del_vector){

    vector<float> cn;

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distr(FLOAT_MIN, FLOAT_MAX);

    for (int i = 0; i < cantidad_de_elementos_del_vector; ++i) {
        float numero_aleatorio  = distr(eng);
        cn.push_back(numero_aleatorio);
    }


    unsigned t0, t1;
    t0=clock();
    problema_2(n,cn);
    t1 = clock();

    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Tiempo de ejecucion usando variables atomicas: " << time << endl;
}


int main()
{

    cout << endl << "Usando 1 hilo y cantidad de 1000 " << endl;
    calcular_tiempo_1(1,1000);
    cout << endl;
    calcular_tiempo_2(1,1000);


    cout << endl  << "Usando 2 hilos y cantidad de 5000: " << endl;
    calcular_tiempo_1(2,5000);
    cout << endl;
    calcular_tiempo_2(2,5000);

    cout << endl <<  "Usando 4 hilos y cantidad de 10000: " << endl;
    calcular_tiempo_1(4,10000);
    cout << endl;
    calcular_tiempo_2(4,10000);

    cout << endl <<"Usando 8 hilos y cantidad de 15000: " << endl;
    calcular_tiempo_1(8,15000);
    cout << endl;
    calcular_tiempo_2(8,15000);






    //Conclusiones
    //Usando 1 hilo y cantidad de 1000
    //
    //Maximo global: 99.9914
    //Tiempo de ejecucion usando locks: 0.002
    //
    //
    //Maximo global: 99.9914
    //Tiempo de ejecucion usando variables atomicas: 0.003
    //Usando 2 hilos y cantidad de 5000:
    //
    //Maximo global: 99.9914
    //Tiempo de ejecucion usando locks: 0.002
    //
    //
    //Maximo global: 99.9914
    //Tiempo de ejecucion usando variables atomicas: 0.003
    //Usando 4 hilos y cantidad de 10000:
    //
    //Maximo global: 99.9914
    //Tiempo de ejecucion usando locks: 0.002
    //
    //
    //Maximo global: 99.9914
    //Tiempo de ejecucion usando variables atomicas: 0.002
    //Usando 8 hilos y cantidad de 15000:
    //
    //Maximo global: 99.9914
    //Tiempo de ejecucion usando locks: 0.002
    //
    //
    //Maximo global: 99.9914
    //Tiempo de ejecucion usando variables atomicas: 0.002
    //Conclusión:
    //Se puede observar que con las pruebas realizadas en algunos casos el enfoque en usar locks demorba un tiempo menos que
    //al utilizar un efoque de variables atómicas.


}