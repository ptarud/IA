/* 
 * File:   hc.cpp
 * Author: diepeerk
 * 
 * Created on 1 de junio de 2011, 15:41
 */

#include "hc.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#define MOVES 5

using namespace std;

hc::hc() {
}

hc::hc(const hc& orig) {
}

hc::~hc() {
}

void hc::setIterations(int iterations){
    this->iterations = iterations;
}

void hc::run(int restarts){
    
    g_best = new solution(*sol);
    vector<solution> mejores_soluciones;
    vector<solution> peores_soluciones;
    bool mejore;
    int iters = 0;
    int resets = 0;
    int maxIters = 20   ;
    
    for(int t = 0; t < iterations; t++){
        mejore = false;
        if(iters == maxIters && resets <= restarts){ //hago un restart
            resets++;
            float prob = (float)rand()/RAND_MAX;
            if(prob <= 0.33 && prob >= 0){
                int index = (int)rand()%mejores_soluciones.size();
                sol->copy(mejores_soluciones.at(index));
            }else if(prob > 0.33 && prob <= 0.66){
                int index = (int)rand()%peores_soluciones.size();
                sol->copy(peores_soluciones.at(index));
            }else if(prob > 0.66){
                sol->init();
            }
        }
        /*elijo aleatoreamente movimiento que aplicaré para crear vecindario*/
        int move = (int)rand()%(MOVES+1);
        /*aplico movimiento*/
        if(move == 0){ //aplico solution::swapHomes(t1,t2)
            int t1,t2;
            for(t1 = 0; t1 < sol->getN(); t1++){
                for(t2 = 0; t2 < sol->getN(); t2++){
                    solution sol2(*sol);
                    if(t1 != t2){
                        sol2.swapHomes(t1,t2);
                        if(sol2.isFeasible()){
                            /*si es mejor ó si es igual, pero con mejor fitness */
                            if(sol2.getDistance() < sol->getDistance() || ( sol2.getDistance() == sol->getDistance() && sol2.getFitness() < sol->getFitness() )){
                                mejore = true; 
                                iters = 0;
                                mejores_soluciones.push_back(sol2);
                                sol->copy(sol2);
                                /*paro los for*/
                                t1 = sol->getN();
                                t2 = sol->getN();
                                if(sol2.getDistance() < g_best->getDistance()){
                                    g_best->copy(sol2);
                                }
                            }else if(sol2.getDistance() > sol->getDistance()){ //soy peor
                                peores_soluciones.push_back(sol2);
                            }
                        }
                    }
                }
            }
            
        }else if(move == 1){ //aplico solution::swapRounds(r1,r2)
            int r1, r2;
            for(r1 = 0; r1 < 2*(sol->getN()-1)+sol->getK(); r1++){
                for(r2 = 0; r2 < 2*(sol->getN()-1)+sol->getK(); r2++){
                    solution sol2(*sol);
                    if(r1 != r2){
                        sol2.swapRounds(r1,r2);
                        if(sol2.isFeasible()){
                            if(sol2.getDistance() < sol->getDistance() || ( sol2.getDistance() == sol->getDistance() && sol2.getFitness() < sol->getFitness() )){ //soy mejor
                                mejore = true; 
                                iters = 0;
                                mejores_soluciones.push_back(sol2);
                                sol->copy(sol2);
                                /*paro los for*/
                                r1 = 2*(sol->getN()-1)+sol->getK();
                                r2 = 2*(sol->getN()-1)+sol->getK();
                                if(sol2.getDistance() < g_best->getDistance()){
                                    g_best->copy(sol2);
                                }
                            }else if(sol2.getDistance() > sol->getDistance()){ //soy peor
                                peores_soluciones.push_back(sol2);
                            }
                        }
                    }
                }
            }
        }else if(move == 2){ //aplico solution::swapByes(t1,t2)
            if(sol->getK() > 0){
                int t1,t2;
                for(t1 = 0; t1 < sol->getN(); t1++){
                    for(t2 = 0; t2 < sol->getN(); t2++){
                        solution sol2(*sol);
                        if(t1 != t2){
                            sol2.swapByes(t1,t2);
                            if(sol2.isFeasible()){
                                if(sol2.getDistance() < sol->getDistance() || ( sol2.getDistance() == sol->getDistance() && sol2.getFitness() < sol->getFitness() )){
                                    mejore = true; 
                                    iters = 0;
                                    mejores_soluciones.push_back(sol2);
                                    sol->copy(sol2);
                                    /*paro los for*/
                                    t1 = sol->getN();
                                    t2 = sol->getN();
                                    if(sol2.getDistance() < g_best->getDistance()){
                                        g_best->copy(sol2);
                                    }
                                }else if(sol2.getDistance() > sol->getDistance()){ //soy peor
                                    peores_soluciones.push_back(sol2);
                                }
                            }
                        }
                    }
                }
            }else{
                t--;
            }
        }else if(move == 3){ //aplico solution::partianSwapRounds(t1,r1,r2)
            for(int t1 = 0; t1 < sol->getN(); t1++){
                for(int r1 = 0; r1 < 2*(sol->getN()-1)+sol->getK(); r1++){
                    for(int r2 = 0; r2 < 2*(sol->getN()-1)+sol->getK(); r2++){
                        solution sol2(*sol);
                        if(r1 != r2){
                            sol2.partialSwapRounds(t1,r1,r2);
                            if(sol2.isFeasible()){
                                if(sol2.getDistance() < sol->getDistance() || ( sol2.getDistance() == sol->getDistance() && sol2.getFitness() < sol->getFitness() )){
                                    mejore = true; 
                                    iters = 0;
                                    mejores_soluciones.push_back(sol2); 
                                    sol->copy(sol2);
                                    /*paro los for*/
                                    t1 = sol->getN();
                                    r1 = 2*(sol->getN()-1)+sol->getK();
                                    r2 = 2*(sol->getN()-1)+sol->getK();
                                    if(sol2.getDistance() < g_best->getDistance()){
                                        g_best->copy(sol2);
                                    }
                                }else if(sol2.getDistance() > sol->getDistance()){ //soy peor
                                    peores_soluciones.push_back(sol2);
                                }
                            }
                        }
                    } 
                }
            }
        }else if(move == 4){ //aplico solution::swapTeams(t1,t2)
            int t1,t2;
            for(t1 = 0; t1 < sol->getN(); t1++){
                for(t2 = 0; t2 < sol->getN(); t2++){
                    solution sol2(*sol);
                    if(t1 != t2){
                       sol2.swapTeams(t1,t2);
                       if(sol2.isFeasible()){
                            /*si es mejor ó si es igual, pero con mejor fitness */
                            if(sol2.getDistance() < sol->getDistance() || ( sol2.getDistance() == sol->getDistance() && sol2.getFitness() < sol->getFitness() )){
                                mejore = true; 
                                iters = 0;
                                mejores_soluciones.push_back(sol2);
                                sol->copy(sol2);
                                /*paro los for*/
                                t1 = sol->getN();
                                t2 = sol->getN();
                                if(sol2.getDistance() < g_best->getDistance()){
                                    g_best->copy(sol2);
                                }
                            }else if(sol2.getDistance() > sol->getDistance()){ //soy peor
                                peores_soluciones.push_back(sol2);
                            }
                        }
                    }
                }
            }
        }else if(move == 5){
            int t1, t2, r;
            for(t1 = 1; t1 <= sol->getN(); t1++){
                for(t2 = 1; t2 <= sol->getN(); t2++){
                    for(r = 1; r <= 2*(sol->getN()-1)+sol->getK();r++){
                        solution sol2(*sol);
                        if(t1 != t2){
                           sol2.partialSwapTeams(t1,t2,r);
                           if(sol2.isFeasible()){
                               mejore = true; 
                               iters = 0; 
                               /*si es mejor ó si es igual, pero con mejor fitness */
                                if(sol2.getDistance() < sol->getDistance() || ( sol2.getDistance() == sol->getDistance() && sol2.getFitness() < sol->getFitness() )){
                                    mejores_soluciones.push_back(sol2);
                                    sol->copy(sol2);
                                    /*paro los for*/
                                    t1 = sol->getN()+1;
                                    t2 = sol->getN()+1;
                                    r = 2*(sol->getN()-1)+sol->getK()+1;
                                    if(sol2.getDistance() < g_best->getDistance()){
                                        g_best->copy(sol2);
                                    }
                                }else if(sol2.getDistance() > sol->getDistance()){ //soy peor
                                    peores_soluciones.push_back(sol2);
                                }
                            }
                        }
                    }
                } 
            }
        }
        if(!mejore){
            iters++;
        }
    }
    
}

void hc::setInitSol(solution *sol){
    this->sol = sol;
}

solution* hc::getGBest(){
    return g_best;
}