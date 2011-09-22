/* 
 * File:   instance.cpp
 * Author: diepeerk
 * 
 * Created on 31 de mayo de 2011, 15:48
 */
#include "instance.h"
#include <fstream>
#include <cstdio>

using namespace std;

instance::instance() {
}

instance::instance(const instance& orig) {
}

instance::~instance() {
    for(int i = 0; i < N; i++){
        delete[] distance[i];
    }
    delete[] distance;
}

void instance::setN(int N){
    this->N = N;
}

void instance::read(char *file){
    ifstream data;
    
    data.open(file);
    /*asigno memoria a matriz distance[][]*/
    distance = new int*[N];
    for(int i = 0; i < N; i++){
        distance[i] = new int[N];
    }
    
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            data >> distance[i][j];
        }
    }
    
    data.close();
}

int **instance::getDistMatrix(){
    return distance;
}

int instance::getN()
{
    return N;
}