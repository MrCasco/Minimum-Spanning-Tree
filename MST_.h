#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <chrono>
#include <algorithm>

using namespace std::chrono;
using namespace std;

// Struct que contiene atributos útiles para la lista de adyacencia
class Nodo {
  public:
    int parent = 0;
    int name = 0;
    int weight = 0;

    Nodo(){
      parent = 0;
      name = 0;
      weight = 0;
    }

    Nodo(int newParent, int newName, int newWeight){
      name = newName;
      parent = newParent;
      weight = newWeight;
    }
};

//Objeto que ayuda a hacer el min heap
class comparator {
public:
  int operator() (const Nodo& n1, const Nodo& n2){
    return n1.weight > n2.weight;
  }
};

//Objeto que ayuda a ordenar la lista de Nodos por peso
class sorter{
public:
  bool operator() (const Nodo& a, const Nodo& b) {
    return a.weight < b.weight;
  }
};

class MST {
int nodos, aristas, costo;
// Lista de adyacencia
vector<vector<Nodo>> lista;
vector<Nodo> weights;
vector<int> visited;
vector<int> leaders;
vector <string> camino;
priority_queue<Nodo, vector<Nodo>, comparator> heap;

public:

  //Función que lee el archivo txt y llena la lista o vector de Nodos.
  void leeGrafo(string archivo, int operation){
    fstream myfile(archivo, std::ios_base::in);
    int a, b, c;
    myfile >> a >> b;
    nodos = a;
    aristas = b;
    lista.resize(nodos);
    visited.resize(nodos);
    // Llenar lista de adyacencia como si fuera no dirigida
    if (operation == 0) {
      while (myfile >> a >> b >> c) {
        lista[a].push_back(Nodo(a, b, c));
        lista[b].push_back(Nodo(b, a, c));
      }
    }
    // Lenar vector de pesos ordenados con aristas como si fuera no dirigido
    else if (operation == 1){
      while (myfile >> a >> b >> c) {
        weights.push_back(Nodo(a, b, c));
        weights.push_back(Nodo(b, a, c));
      }
      sort(weights.begin(), weights.end(), sorter());
    }
    else {
      leaders.resize(nodos);
      while (myfile >> a >> b >> c) {
        weights.push_back(Nodo(a, b, c));
      }
      for (int i = 0; i < leaders.size(); i++) {
        leaders[i] = i;
      }
      sort(weights.begin(), weights.end(), sorter());
    }
  }

  // Función que encuentra el MST por el método de Prim
  float prim(string archivo) {
    // Leer el grafo y llenar lista de adyacencia
    leeGrafo(archivo, 0);
    //Variable que indica inicio de la ejecución del código
    auto start = high_resolution_clock::now();
    int nodoActual = lista[0][0].parent;
    visited[nodoActual] = 1;
    costo = 0;
    // Ciclo para visitar toda la lista
    for (int i = 0; i < nodos-1; i++) {
      for (int j = 0; j < lista[nodoActual].size(); j++) {
        // Si el nodo candidato no está visitado, se mete al heap
        if (visited[lista[nodoActual][j].name] == 0) {
          heap.push(lista[nodoActual][j]);
        }
      }
      // Si se encuentra un nodo visitado, sacar la raíz del heap
      // hasta que sea uno no visitado
      if (visited[heap.top().name] == 1) {
        while (visited[heap.top().name] == 1) {
          heap.pop();
        }
      }
      // Añadir al camino la raíz del heap
      camino.push_back("("+to_string(heap.top().parent)+", "+to_string(heap.top().name)+", "+to_string(heap.top().weight)+")"+"\n");
      // Sumar el costo
      costo += heap.top().weight;
      // Cambiar el nodo actual al que pertenece en la raíz del heap
      nodoActual = heap.top().name;
      // Marcar el nuevo nodo como visitado
      visited[nodoActual] = 1;
      // Sacarlo del heap
      heap.pop();
    }
    // Detener el tiempo para obtener el tiempo de ejecución
    auto stop = high_resolution_clock::now();
    // Imprimir el camino que se tomó del MST
    printWay();
    // Imprimir costo final
    std::cout << "El costo total es de: " << costo << '\n';
    auto duration = duration_cast<microseconds>(stop - start);
    // Retornar tiempo de ejecución
    return duration.count();
  }

  // Función que encuentra el MST por el método de Kruskal
  float kruskalDFS(string archivo){
    // Llenar el vector ordenado por pesos de Nodos
    leeGrafo(archivo, 1);
    // Iniciar temporizador
    auto start = high_resolution_clock::now();
    // Iniciar costo en 0
    costo = 0;
    // Explorar el primer nodo del vector ordenado
    explore(weights[0].name);
    // Imprimir el camino
    printWay();
    // Detener temporizador
    auto stop = high_resolution_clock::now();
    // Imprimir costo
    std::cout << "El costo total es de: " << costo << '\n';
    auto duration = duration_cast<microseconds>(stop - start);
    // Retornar tiempo de ejecución
    return duration.count();
  }

  // Función recursiva que explora el nodo que se le pase como parametro
  void explore(int nodo){
    // Marcar como visitado el nuevo nodo
    visited[nodo] = 1;
    // Ciclo que recorre el vector de objetos Nodo ordenados
    for (int i = 1; i < weights.size(); i++) {
      // Si el nuevo nodo no está visitado, pero el padre sí, agregar arista al camino
      // Esta es la condicional para detectar ciclos
      if (visited[weights[i].name] == 0 && visited[weights[i].parent] == 1) {
        camino.push_back("("+to_string(weights[i].parent)+", "+to_string(weights[i].name)+", "+to_string(weights[i].weight)+")"+"\n");
        costo += weights[i].weight;
        visited[weights[i].parent] = 1;
        // Visitar el nuevo nodo que se acaba de agregar al camino
        explore(weights[i].name);
      }
      // Si se han metido n-1 aristas, detener la exploración
      if (camino.size() == nodos-1) {
        break;
      }
    }
  }

  // Función que encuentra el MST por el método de Kruskal Union Find
  float kruskalUF(string archivo){
    // Llenar y ordenar vector de Nodos
    leeGrafo(archivo, 2);
    // Iniciar temporizador
    auto start = high_resolution_clock::now();
    // Iniciar costo en 0
    costo = 0;
    // For que recorre todo el vector de objetos Nodo ordenados
    for (int i = 0; i < weights.size(); i++) {
      // Si el lider del vector padre es diferente al del hijo, añadir arista
      if (find(weights[i].parent) != find(weights[i].name)) {
        // Actualizar lider del padre
        leaders[find(weights[i].parent)] = weights[i].name;
        // Actualizar costo
        costo += weights[i].weight;
        // Añadir arista al camino
        camino.push_back("("+to_string(weights[i].parent)+", "+to_string(weights[i].name)+", "+to_string(weights[i].weight)+")"+"\n");
      }
      // Si el vector de aristas es igual a nodos-1, se ha encontrado el MST
      if (camino.size() == nodos-1) {
        break;
      }
    }
    // Detener temporizador
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    // Imprimir camino
    printWay();
    // Imprimir costo
    std::cout << "Costo total: " << costo << '\n';
    // Retornar tiempo de ejecución
    return duration.count();
  }

  // Función que encuentra y devuelve el lider de x nodo
  int find(int name){
    int nodoActual = name, lider;
    for (int i = 0; i < leaders.size(); i++) {
      lider = nodoActual;
      nodoActual = leaders[nodoActual];
      if (nodoActual == lider) {
        name = lider;
        break;
      }
    }
    return name;
  }

  // Función que imprime el camino seleccionado para el MST
  void printWay(){
    for (int i = 0; i < camino.size(); i++) {
      std::cout << camino[i];
    }
  }
};
