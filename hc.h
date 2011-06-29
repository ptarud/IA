/* 
 * File:   hc.h
 * Author: diepeerk
 *
 * Created on 1 de junio de 2011, 15:41
 */
#include "solution.h" 
#include <list>

#ifndef HC_H
#define	HC_H

using namespace std;

class hc {
public:
    hc();
    hc(const hc& orig);
    virtual ~hc();
    void setIterations(int);
    void run();
    void setInitSol(solution*);
    solution* getGBest();
private:
    int iterations;
    solution *sol, *g_best;
};

#endif	/* HC_H */

