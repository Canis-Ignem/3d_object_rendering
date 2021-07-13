#ifndef IO_H
#define IO_H
#include "definitions.h"


void keyboard(unsigned char key, int x, int y);
void SKeyboard(int key, int x, int y);
void print_help();
void printHelpObjetos();
void printHelpCamara();
void printHelpIlum();

// AÑADIDAS
    //TECLADO
    void SKeyboard(int key, int x, int y);
    //CAMARA
    void actualizaInversa(object3d * c);
    void actualizarMCSR();
    void empezarAnalisis();
    void crearCamara( object3d * c, double eX, double eY, double eZ, double pX, double pY, double pZ,double uX, double uY, double uZ);
    void rotacionLibre(double grados, double x, double y, double z);
    void rotacionLigada(double grados, double x, double y, double z);
    void volar(double x, double y, double z);
    //OBJETO
    void deletePoligon(object3d *oPTR);
    void actualizarObj();

#endif // IO_H
