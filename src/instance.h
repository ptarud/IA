/* 
 * File:   instance.h
 * Author: diepeerk
 *
 * Created on 31 de mayo de 2011, 15:48
 */

#ifndef INSTANCE_H
#define	INSTANCE_H

class instance {
public:
    instance();
    instance(const instance& orig);
    virtual ~instance();
    void setN(int);
    void read(char*);
    int **getDistMatrix();
    int getN();
private:
    int N;
    int **distance;
};

#endif	/* INSTANCE_H */

