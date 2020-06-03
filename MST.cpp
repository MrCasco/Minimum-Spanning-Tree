#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <chrono>
#include <algorithm>

using namespace std::chrono;
using namespace std;

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

class comparator {
public:
  int operator() (const Nodo& n1, const Nodo& n2){
    return n1.weight > n2.weight;
  }
};

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

  void leeGrafo(string archivo, int operation){
    fstream myfile(archivo+".txt", std::ios_base::in);
    int a, b, c;
    myfile >> a >> b;
    nodos = a;
    aristas = b;
    lista.resize(nodos);
    visited.resize(nodos);
    if (operation == 0) {
      while (myfile >> a >> b >> c) {
        lista[a].push_back(Nodo(a, b, c));
        lista[b].push_back(Nodo(b, a, c));
      }
    }
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
      sort(weights.begin(), weights.end(), sorter());
    }
  }



  float prim(string archivo) {
    leeGrafo(archivo, 0);
    auto start = high_resolution_clock::now();
    int nodoActual = lista[0][0].parent;
    visited[nodoActual] = 1;
    costo = 0;
    for (int i = 0; i < nodos-1; i++) {
      for (int j = 0; j < lista[nodoActual].size(); j++) {
        if (visited[lista[nodoActual][j].name] == 0) {
          heap.push(lista[nodoActual][j]);
        }
      }
      if (visited[heap.top().name] == 1) {
        while (visited[heap.top().name] == 1) {
          heap.pop();
        }
      }
      camino.push_back("("+to_string(heap.top().parent)+", "+to_string(heap.top().name)+", "+to_string(heap.top().weight)+")"+"\n");
      costo += heap.top().weight;
      nodoActual = heap.top().name;
      visited[nodoActual] = 1;
      heap.pop();
    }
    auto stop = high_resolution_clock::now();
    printWay();
    std::cout << "El costo total es de: " << costo << '\n';
    auto duration = duration_cast<microseconds>(stop - start);
    return duration.count();
  }

  float kruskalDFS(string archivo){
    leeGrafo(archivo, 1);
    auto start = high_resolution_clock::now();
    int nodoActual = weights[0].name;
    costo = 0;
    explore(nodoActual);
    printWay();
    auto stop = high_resolution_clock::now();
    std::cout << "El costo total es de: " << costo << '\n';
    auto duration = duration_cast<microseconds>(stop - start);
    return duration.count();
  }

  void explore(int nodo){
    visited[nodo] = 1;
    for (int i = 1; i < weights.size(); i++) {
      if (visited[weights[i].name] == 0 && visited[weights[i].parent] == 1) {
        camino.push_back("("+to_string(weights[i].parent)+", "+to_string(weights[i].name)+", "+to_string(weights[i].weight)+")"+"\n");
        costo += weights[i].weight;
        visited[weights[i].parent] = 1;
        explore(weights[i].name);
      }
      if (camino.size() == nodos-1) {
        break;
      }
    }
  }

  float kruskalUF(string archivo){
    leeGrafo(archivo, 2);
    auto start = high_resolution_clock::now();
    visited[weights[0].parent] = 1;
    costo = 0;
    for (int i = 0; i < leaders.size(); i++) {
      leaders[i] = i;
    }
    for (int i = 0; i < weights.size(); i++) {
      if (find(weights[i].parent) != find(weights[i].name)) {
        leaders[find(weights[i].parent)] = weights[i].name;
        costo += weights[i].weight;
        camino.push_back("("+to_string(weights[i].parent)+", "+to_string(weights[i].name)+", "+to_string(weights[i].weight)+")"+"\n");
      }
      if (camino.size() == nodos-1) {
        break;
      }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    printWay();
    std::cout << "Costo: " << costo << '\n';
    return duration.count();
  }

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
};

int main() {
  MST mst;
  std::cout << mst.prim("P3Edges0") << '\n';
}
