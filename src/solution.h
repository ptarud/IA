/* 
 * File:   solution.h
 * Author: ptarud
 *
 * Created on 31 de mayo de 2011, 18:31
 */

#ifndef SOLUTION_H
#define	SOLUTION_H

class solution {
public:
    solution(int,int);
    solution(const solution& orig);
    virtual ~solution();
    void init(); //genera solucion inicial espejo random por metodo del poligono
    void init2(); //pruebas
    int **getSolution();
    int getDistance();
    int getFitness();
    int getN();
    int getK();
    void printSolution();
    void setDistanceMatrix(int**);
    bool isFeasible();
    void copy(solution);
    void setLU(int,int);
    void setBO(int,int);
    /*movimientos ( los teams van de 0 a N-1 y los rounds van de 0 a 2*(N-1)+K-1) )*/
    void swapHomes(int,int); //0
    void swapRounds(int,int); //1
    void swapByes(int,int); //2
    void partialSwapRounds(int,int,int); //3
    void swapTeams(int,int); //4
    void partialSwapTeams(int,int,int);//5
    void searchRoundSwap(int,int,int,bool*&); //metodo usado en partialSwapTeams
    void swapRound2(int,int,int); //idem
private:
    int **sol; //solucion
    int N,K; //N: numero de equipos y K: numero de byes
    int L,U;
    int *LU; //HomeStand y RoundTrip esta en el rango [L,U]
    int B,O; //numero juegos consecutivos < B ; numero de dias libres conseuctivos < O
    int **distMatrix; //matriz de distancia (instancia)
    void assign(int,int,int); //asigna equipo en sol[i][j]
};

#endif	/* SOLUTION_H */

