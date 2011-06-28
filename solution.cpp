/* 
 * File:   solution.cpp
 * Author: diepeerk
 * 
 * Created on 31 de mayo de 2011, 18:31
 */

#include "solution.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;
//using namespace utils;

solution::solution(int N, int K) {
    this->N = N;
    this->K = K;
    /*asigno memoria a matriz sol[N][2*(N-1)+K]*/
    sol = new int*[N];
    for(int i = 0; i < N; i++){
        sol[i] = new int[2*(N-1)+K];
    }
    /*la lleno de "basura"*/
    for(int i = 0; i < N; i++){
        for(int j = 0; j < 2*(N-1)+K; j++){
            sol[i][j] = -(N+100);
        }
    }
}

solution::solution(const solution& orig) {
    
    this->B = orig.B;
    this->K = orig.K;
    this->L = orig.L;
    this->N = orig.N;
    this->O = orig.O;
    this->U = orig.U;
    this->distMatrix = orig.distMatrix;
    
    /*asigno memoria a matriz sol[][]*/
    sol = new int*[N];
    for(int i = 0; i < N; i++){
        sol[i] = new int[2*(N-1)+K];
    }
    /*la copio*/
    for(int i = 0; i < N; i++){
        for(int j = 0; j < 2*(N-1)+K; j++){
            sol[i][j] = orig.sol[i][j];
        }
    }
    /*asigno memoria a matriz LU[]*/
    LU = new int[U-L+1];
    /*la copio*/
    for(int i = 0; i <=U-L;i++){
        LU[i] = orig.LU[i];
    }
}

void solution::copy(solution orig){
    this->B = orig.B;
    this->K = orig.K;
    this->L = orig.L;
    this->N = orig.N;
    this->O = orig.O;
    this->U = orig.U;
    this->distMatrix = orig.distMatrix;
    
    /*copio matriz sol[][]*/
    for(int i = 0; i < N; i++){
        for(int j = 0; j < 2*(N-1)+K; j++){
            sol[i][j] = orig.sol[i][j];
        }
    }
    /*copio matriz LU*/
    for(int i = 0; i <=U-L ;i++){
        LU[i] = orig.LU[i];
    }
}

solution::~solution() {
    
    /*elimino memoria reservada durante la ejecucion*/
    for(int i = 0; i < N; i++){
        delete[] sol[i];
    }
    delete[] sol;
    
    delete[] LU;
}

int **solution::getSolution(){
    return sol;
}

int solution::getDistance(){
    int distTeam[N];
    for(int i = 0; i < N; i++){
        int ult = i+1;
        distTeam[i] = 0;
        if(sol[i][0] < 0){ //si parte de visita
            distTeam[i]+= distMatrix[i][abs(sol[i][0])-1]; //sumo desde donde estoy a donde tengo q llegar
            ult = sol[i][0]; //ultimo es donde partio
        }else{ //si parte de local
            ult = i+1; //ultimo es su "pais"
        }
        for(int j = 1; j < 2*(N-1)+K;j++){
            if(ult > 0 && sol[i][j] < 0){ //si estoy en casa y tengo que viajar
                distTeam[i]+= distMatrix[i][abs(sol[i][j])-1];
                ult = sol[i][j];
            }else if(ult < 0 && sol[i][j] > 0){ //si estoy en algun pais y vuelvo a casa
                distTeam[i]+= distMatrix[abs(ult)-1][i];
                ult = i+1;
            }else if(ult < 0 && sol[i][j] < 0){ //si estoy en algun pais y tengo q viajar
                distTeam[i]+= distMatrix[abs(ult)-1][abs(sol[i][j])-1];
                ult = sol[i][j];
            }
        }
        if(ult < 0){ //finalmente si estoy en otro pais, vuelvo a casa
            distTeam[i]+= distMatrix[abs(ult)-1][i];
        }
    }
    
    int distance = 0;
    for(int i = 0; i < N; i++){
        distance+= distTeam[i];
    }
    
    return distance;
}

void solution::printSolution(){
    for(int i = 0; i < N ; i ++){
        printf("\n");
        for(int j = 0; j < 2*(N-1) + K; j++){
            printf("%d\t",sol[i][j]);
        }
    }
    printf("\n\nDistance = %d\n",getDistance());
    printf("Fitness = %d\n",getFitness());
}

void solution::setDistanceMatrix(int **distMatrix){
    this->distMatrix = distMatrix;
}

bool solution::isFeasible(){
    /*veo cumplimiento de parametros L,U*/
    int i,j;
    int roadTrip, homeStand;
    for(i = 0; i < N; i++){
        roadTrip = 0;
        homeStand = 0;
        int r = -1;
        /*elijo el primero cuando sol[i][j] no es cero*/
        do{
           r++; 
        }while(sol[i][r] == 0);
        int ult = sol[i][r];
        if(ult > 0){
            homeStand++;
        }else if(ult < 0){
            roadTrip++;
        }
        for(j = r+1; j < 2*(N-1)+K;j++){
            if(ult > 0 && sol[i][j] > 0){ //si sigo jugando de local
                homeStand++;
                if(homeStand > LU[U-L]){
                    return false;
                }
                ult = sol[i][j];
            }else if(ult < 0 && sol[i][j] < 0){ //si sigo jugando de visita
                roadTrip++;
                if(roadTrip > LU[U-L]){
                    return false;
                }
                ult = sol[i][j];
            }else if(ult > 0 && sol[i][j] < 0){ //si estaba jugando de local y ahora viajo para jugar de visita
                bool entre = false;
                for(int k = 0; k <=U-L; k++){
                    if(LU[k] == homeStand){
                        entre = true;
                    } 
                }
                if(!entre){
                    return false;
                }
                homeStand = 0;
                roadTrip = 1;
                ult = sol[i][j];
            }else if(ult < 0 && sol[i][j] > 0){ //si estaba jugando de visita y ahora viajo para jugar de local
                bool entre = false;
                for(int k = 0; k<=U-L; k++){
                    if(LU[k] == roadTrip){
                        entre = true;
                    }
                }
                if(!entre){
                    return false;
                }
                roadTrip = 0;
                homeStand = 1;
                ult = sol[i][j];
            }
        }
    }
    
    return true;
        
}

void solution::swapHomes(int t1, int t2){
        
    int r1, r2;
    /*busco r1 y r2*/
    for(int j = 0; j < 2*(N-1)+K; j++){
        if(sol[t1][j] == (t2+1)){
            r1 = j;
        }else if(sol[t1][j] == -(t2+1)){
            r2 = j;
        }
    }
    /*hago el cambio*/
    int aux;
    aux = sol[t1][r1];
    sol[t1][r1] = sol[t1][r2];
    sol[t1][r2] = aux;
    aux = sol[t2][r1];
    sol[t2][r1] = sol[t2][r2];
    sol[t2][r2] = aux;
}

void solution::swapRounds(int r1, int r2){
    
    for(int i = 0; i < N; i++){
        int aux;
        aux = sol[i][r1];
        sol[i][r1] = sol[i][r2];
        sol[i][r2] = aux;
    }
}

void solution::swapTeams(int t1, int t2){
    
   for(int j = 0; j < 2*(N-1)+K; j++){
        if(abs(sol[t1][j]) != t2+1 && abs(sol[t2][j]) != t1+1){
            /*hago el swap*/
            int aux = sol[t1][j];
            sol[t1][j] = sol[t2][j];
            sol[t2][j] = aux;
            
            /*hago el arreglo dependiendo de como sean los numeros*/
            if((sol[t1][j] < 0 && sol[t2][j] < 0) || (sol[t1][j] > 0 && sol[t2][j] > 0)){
                aux = sol[abs(sol[t1][j])-1][j];
                sol[abs(sol[t1][j])-1][j] = sol[abs(sol[t2][j])-1][j];
                sol[abs(sol[t2][j])-1][j] = aux;
            }else if(sol[t1][j] != 0 && sol[t2][j] == 0){
                if(sol[t1][j] > 0){
                    sol[abs(sol[t1][j])-1][j] = -(t1+1);
                }else if(sol[t1][j] < 0){
                    sol[abs(sol[t1][j])-1][j] = t1+1;
                }
            }else if(sol[t1][j] == 0 && sol[t2][j] != 0){
                if(sol[t2][j] > 0){
                    sol[abs(sol[t2][j])-1][j] = -(t2+1);
                }else if(sol[t2][j] < 0){
                    sol[abs(sol[t2][j])-1][j] = t2+1;
                }
            }else if(sol[t1][j] > 0 && sol[t2][j] < 0){
                sol[abs(sol[t1][j])-1][j] = -(t1+1);
                sol[abs(sol[t2][j])-1][j] = t2+1;
            }else if(sol[t1][j] < 0 && sol[t2][j] > 0){
                sol[abs(sol[t1][j])-1][j] = t1+1;
                sol[abs(sol[t2][j])-1][j] = -(t2+1);
            }
        }
    }
}

void solution::swapByes(int t1, int t2){

    for(int j = 0; j < 2*(N-1)+K; j++){
        if(sol[t1][j] == 0 && sol[t2][j] == 0){
            float prob = (float)rand()/RAND_MAX;
            if(prob > 0.5){ //elijo el positivo del t2
                for(int k = 0; k < 2*(N-1)+K;k++){
                    if(sol[t1][k] == t2+1){
                        int aux;
                        aux = sol[t1][j];
                        sol[t1][j] = sol[t1][k];
                        sol[t1][k] = aux;
                        aux = sol[t2][j];
                        sol[t2][j] = sol[t2][k];
                        sol[t2][k] = aux;
                        return;
                    }
                }
            }else{ //elijo el negativo del t2
                for(int k = 0; k < 2*(N-1)+K;k++){
                    if(sol[t1][k] == -(t2+1)){
                        int aux;
                        aux = sol[t1][j];
                        sol[t1][j] = sol[t1][k];
                        sol[t1][k] = aux;
                        aux = sol[t2][j];
                        sol[t2][j] = sol[t2][k];
                        sol[t2][k] = aux;
                        return;
                    }
                }
            
            }
        }
    
    }

}

void solution::partialSwapRounds(int t, int r1, int r2){
    
    if(abs(sol[t][r1]) == abs(sol[t][r2])){ //si son los mismos equipos, swapeo las de equipos iguales en las columnas r1 y r2
        for(int i = 0; i < N; i++){
            if(abs(sol[i][r1]) == abs(sol[i][r2])){
                int aux;
                aux = sol[i][r1];
                sol[i][r1] = sol[i][r2];
                sol[i][r2] = aux;
            }
        }
    }else{ //si son equipos distontos, swapeo las de equipos distontos en las columnas r1 y r2
        for(int i = 0; i < N; i++){
            if(abs(sol[i][r1]) != abs(sol[i][r2])){
                int aux;
                aux = sol[i][r1];
                sol[i][r1] = sol[i][r2];
                sol[i][r2] = aux;
            }
        }
    }
}

int solution::getN(){
    return N;
}

int solution::getK(){
    return K;
}

void solution::setLU(int L, int U){
    this->L = L;
    this->U = U;
    LU = new int[U-L+1];
    int count = L;
    for(int i = 0; i <= U-L; i++){
        LU[i] = count;
        count++;
    }
}

void solution::setBO(int B, int O){
    this->B = B;
    this->O = O;
}

int solution::getFitness(){
    int fitness = 0;
    int ult;
    int days = 0;//dias consecutivos sin dias libres
    int daysOff = 0; //dias libres consecutivos
    for(int i = 0; i < N; i++){
        ult = sol[i][0];
        if(ult != 0){
            days++;
        }else{
            daysOff++;
        }
        for(int j = 1; j < 2*(N-1)+K; j++){
            if(ult != 0 && sol[i][j] != 0){ //si juega
                days++;
            }else if(ult != 0 && sol[i][j] == 0){//si tiene dia libre
                if(days > B){ //penalizo
                    fitness+=500;
                }
                days = 0;
            }else if(ult == 0 && sol[i][j] == 0){
                daysOff++;
            }else if(ult == 0 && sol[i][j] != 0){
                if(daysOff > O){
                    fitness+=100;
                }
                daysOff = 0;
            }
            
            ult = sol[i][j];
        }
    }
    
    return fitness;
}

void solution::assign(int i, int j, int val){
    if(sol[i][j] == -(N+100)){ //si no está asignado
        sol[i][j] = val;
        if(val > 0){
            assign(abs(val)-1,j,-(i+1));
        }else if(val < 0){
            assign(abs(val)-1,j,(i+1));
        }
    }
}

void solution::init(){
    int polygon[N-1];
    int aux[N-1];
    int n = (int)rand()%N +1;
    
    int count = 1;
    for(int i = 0; i < N-1; i++){
        if(count == n)
            count++;
        polygon[i] = count;
        count++;
    }
   
    for(int j = 0; j < N-1; j++){
        /*calculo probabilidad para ver si asigno en casa o en visita*/
        float prob = (float)rand()/RAND_MAX;
        if(prob > 0.5){ //asigno como loal
            assign(n-1,j,polygon[0]);
        }else{ //asigno como visita
            assign(n-1,j,-polygon[0]);
        }
        for(int l = 2; l <= N/2; l++){
            prob = (float)rand()/RAND_MAX;
            if(prob > 0.5){ //asigno como local
                assign(polygon[l-1]-1,j,polygon[N-l]);
            }else{ //asigno como visita
                assign(polygon[l-1]-1,j,-polygon[N-l]);
            }
        }
        /*hago el giro en sentido horario del poligono*/
        aux[0] = polygon[N-2];
        for(int k = 0; k < N-2; k++){
            aux[k+1] = polygon[k];
        }
        for(int k = 0; k < N-1; k++){
            polygon[k] = aux[k];
        }
    }
    /*hago el espejo*/
    for(int i = 0; i < N; i++){
        int count = 0;
        for(int j = 2*(N-1)-1; j>=N-1; j--){
            sol[i][j] = -sol[i][count];
            count++;
        }
    }
    
    
    /*asigno byes al final de la sol inicial*/
    for(int i = 0; i < N; i++){
        for(int j = 2*(N-1); j < 2*(N-1)+K; j++){
            sol[i][j] = 0;
        }
    }
}

void solution::init2(){
    
    int j = 0;
    sol[0][j] = 6;
    sol[1][j] = 5;
    sol[2][j] = -4;
    sol[3][j] = 3;
    sol[4][j] = -2;
    sol[5][j] = -1;
    j++;
    sol[0][j] =-2;
    sol[1][j] = 1;
    sol[2][j] = 5;
    sol[3][j] = 6;
    sol[4][j] = -3;
    sol[5][j] = -4;
    j++;
    sol[0][j] = 4;
    sol[1][j] = -3;
    sol[2][j] = 2;
    sol[3][j] = -1;
    sol[4][j] = 6;
    sol[5][j] = -5;
    j++;
    sol[0][j] = 3;
    sol[1][j] = -6;
    sol[2][j] = -1;
    sol[3][j] = -5;
    sol[4][j] = 4;
    sol[5][j] = 2;
    j++;
    sol[0][j] = -5;
    sol[1][j] = 4;
    sol[2][j] = 6;
    sol[3][j] = -2;
    sol[4][j] = 1;
    sol[5][j] = -3;
    j++;
     sol[0][j] = -4;
    sol[1][j] = 3;
    sol[2][j] = -2;
    sol[3][j] = 1;
    sol[4][j] = -6;
    sol[5][j] = 5;
    j++;
    sol[0][j] = -3;
    sol[1][j] = 6;
    sol[2][j] = 1;
    sol[3][j] = 5;
    sol[4][j] = -4;
    sol[5][j] = -2;
    j++;
    sol[0][j] = 5;
    sol[1][j] = -4;
    sol[2][j] = -6;
    sol[3][j] = 2;
    sol[4][j] = -1;
    sol[5][j] = 3;
    j++;
    sol[0][j] = 2;
    sol[1][j] = -1;
    sol[2][j] = -5;
    sol[3][j] = -6;
    sol[4][j] = 3;
    sol[5][j] = 4;
    j++;
    sol[0][j] = -6;
    sol[1][j] = -5;
    sol[2][j] = 4;
    sol[3][j] = -3;
    sol[4][j] = 2;
    sol[5][j] = 1;
    
}

void solution::partialSwapTeams(int team1, int team2, int round){
    bool* swaprounds=new bool[2*(N-1)+K];

    for(int i=0;i<2*(N-1)+K;i++){
        swaprounds[i]=false;
    }
    swaprounds[round-1]=true;

    //Identificación de rondas para swap
    searchRoundSwap(sol[team1-1][round-1],team2,team1,swaprounds);
    searchRoundSwap(sol[team2-1][round-1],team1,team2,swaprounds);

    //Haciendo swap
    for(int i=0;i<2*(N-1)+K;i++){
        if(swaprounds[i]){
            if(abs(sol[team1-1][i])!=team2)
                 swapRound2(team1,team2,i+1);
        }
    }
    delete[] swaprounds;
    swaprounds=NULL;

}

//Busca las rondas para hacer swap para el partialSwapTeams
// team: equipo con localidad (signo) que hay que buscar
// dteam: equipo de  destino sin signo (swapped)
// steam: equipo swap sin signo (swapped)
void solution::searchRoundSwap(int team, int dteam, int steam, bool*& swapRounds){
    int searchteam=team;
    for(int i=0;i<2*(N-1)+K;i++){
        if(!swapRounds[i] && sol[dteam-1][i]==searchteam){
            swapRounds[i]=true;
            searchRoundSwap(sol[steam-1][i],dteam,steam,swapRounds);
        }
    }

}
//Cambia partidos en una ronda para partialSwapTeams
void solution::swapRound2(int team1, int team2, int round){
    if(team1!=team2){
        int steam1=sol[team1-1][round-1];
        int steam2=sol[team2-1][round-1];
        sol[team1-1][round-1]=steam2;
        sol[team2-1][round-1]=steam1;

        sol[abs(steam2)-1][round-1]=(steam2/abs(steam2))*(-1)*team1;
        sol[abs(steam1)-1][round-1]=(steam1/abs(steam1))*(-1)*team2;
    }

}