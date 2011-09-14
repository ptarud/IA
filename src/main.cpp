/* 
 * File:   main.cpp
 * Author: diepeerk
 *
 * Created on 31 de mayo de 2011, 15:47
 */

#include <cstdlib>
#include "instance.h"
#include "solution.h"
#include "hc.h"
#include <cstdio>
#include <ctime>

using namespace std;

int main(int argc, char** argv) {

    srand(time(NULL));
    srand(rand());
    if(argc == 9){
        instance instancia;
        hc algoritmo;
        instancia.setN(atoi(argv[1]));
        instancia.read(argv[8]);
        
        solution *solucion = new solution(instancia.getN(),atoi(argv[2])); //N,K
        solucion->setDistanceMatrix(instancia.getDistMatrix());
        solucion->setLU(atoi(argv[3]),atoi(argv[4]));
        solucion->setBO(atoi(argv[5]),atoi(argv[6]));
        solucion->init();
        printf("HILL CLIMBING Alguna Mejora con restarts\n");
        printf("\tParametros:\n");
        printf("\t\tN = %d, K = %d, L = %d, U = %d, B = %d, O = %d, Instancia = %s, Iteraciones = %d\n\n",solucion->getN(),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),atoi(argv[6]),argv[8],atoi(argv[7]));
        
        algoritmo.setInitSol(solucion);
        algoritmo.setIterations(atoi(argv[7]));
        algoritmo.run();
        printf("MEJOR SOLUCION\n");
        solution *gbest = algoritmo.getGBest();
        gbest->printSolution();
        
        delete gbest;
        delete solucion;
        return 1;
    }else{
        printf("Argumentos inválidos\n");
        printf("./ia <num equipos> <K> <L> <U> <B> <O> <iteraciones> <archivo>\n");
        return -1;
    }
}

