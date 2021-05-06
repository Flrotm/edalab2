#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

//Se definen las variables globales y el arreglo de hilos

mutex mtx;

int top = 0;
const int max_size = 10000;
unsigned int cantidad_de_hilos = 3;
vector<thread*> todos_hilos;

//Creamos la cola y una funcion que retorna su tamano
char data [max_size];
int size(){
    return top+1;
}

//Fuincion para verificar si la pila esta vacia
bool empty(){
    if(top == -1){
        return true;
    }
    else{
        return false;
    }
}
//Top de la pila
char Top(){
    return data[top];
}
//Push en la pila
void Push(char elemento){
    mtx.lock();
    if(top+1 < max_size){
        data[++top] = elemento;
    }
    mtx.unlock();
}
//Pop de la pila
void Pop(){

    if(!empty()){
        top--;

    }

}
//En esta funcion es donde cada thread hace push a sus elementos
//Le pasamos un vector con los contenidos que queremos insertar en la pila
//Calculamos el tamaño de cada particion y emepezamos a iterar segun la cantidad de hilos
//Cada vez que hay que insertar una particion se crea un thread y se le pasa cada elemento a la pila

void Push_con_hilos(vector<char> elementos){
    int particion_size=elementos.size()/cantidad_de_hilos;
    cout<<"Cada hilo inserta "<<particion_size<<" elementos"<<endl;
    int cont=0;
    for(int i = 1;i <= cantidad_de_hilos; i++)
    {

        cout<<"EL HILO "<<i<<" HA INSERTADO:"<<endl;
        if(i == cantidad_de_hilos) {
            for (int j = cont; j <particion_size*i; ++j) {
                todos_hilos.push_back(new thread(Push, elementos[cont]));
                cout <<elementos[cont]<<endl;
                cont+=1;
            }

        }
        else {
            for (int k = cont; k < particion_size*i; ++k) {
                todos_hilos.push_back(new thread(Push, elementos[cont]));
                cout <<elementos[cont]<<endl;
                cont+=1;
            }


        }
    }

}

//Sacamos los elementos de la pila y comprobamos manualmente
void Pop_con_hilos(int size){

    for(int i = 0;i < size; i++){
        cout<<"POP: "<<Top()<<" ";
        Pop();
    }
}


int main() {
    vector<char> elemens;
    elemens.push_back('A');
    elemens.push_back('A');
    elemens.push_back('B');
    elemens.push_back('B');
    elemens.push_back('C');
    elemens.push_back('C');
    Push_con_hilos(elemens);
    Pop_con_hilos(elemens.size());


    return 0;
}
