#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "display.h"
#include "io.h"
#include "definitions.h"

/** GLOBAL VARIABLES **/

GLdouble _window_ratio;                     /*Control of window's proportions */
GLdouble _ortho_x_min,_ortho_x_max;         /*Variables for the control of the orthographic projection*/
GLdouble _ortho_y_min ,_ortho_y_max;        /*Variables for the control of the orthographic projection*/
GLdouble _ortho_z_min,_ortho_z_max;         /*Variables for the control of the orthographic projection*/

object3d * _first_object= 0;                /*List of objects*/
object3d * _selected_object = 0;            /*Object currently selected*/

camara *iniCamara = 0;                
camara *selectedCamara = 0;


material *lMateriales[4];

int luzSmooth = 1;
luz *selectedLuz = 0;
luz *lLuces[8];


//Materiales: bronce, plata, oro, obsidiana

void inicializarMateriales() {
    material *bronce = (material*)malloc(sizeof(material));
    lMateriales[0] = bronce;

    lMateriales[0]->mAmbiente[0] = 0.2125;
    lMateriales[0]->mAmbiente[1] = 0.1275;
    lMateriales[0]->mAmbiente[2] = 0.054;
    lMateriales[0]->mAmbiente[3] = 1.0;

    lMateriales[0]->mDifusa[0] = 0.714;
    lMateriales[0]->mDifusa[1] = 0.4284;
    lMateriales[0]->mDifusa[2] = 0.18144;
    lMateriales[0]->mDifusa[3] = 1.0;

    lMateriales[0]->mEspecular[0] = 0.393548;
    lMateriales[0]->mEspecular[1] = 0.271906;
    lMateriales[0]->mEspecular[2] = 0.166721;
    lMateriales[0]->mEspecular[3] = 1.0;

    lMateriales[0]->brillo =128 * 0.2;


    material *plata= (material*)malloc(sizeof(material));
    lMateriales[1] = plata;

    lMateriales[1]->mAmbiente[0] = 0.19225;
    lMateriales[1]->mAmbiente[1] = 0.19225;
    lMateriales[1]->mAmbiente[2] = 0.19225;
    lMateriales[1]->mAmbiente[3] = 1.0;

    lMateriales[1]->mDifusa[0] = 0.50754;
    lMateriales[1]->mDifusa[1] = 0.50754;
    lMateriales[1]->mDifusa[2] = 0.50754;
    lMateriales[1]->mDifusa[3] = 1.0;

    lMateriales[1]->mEspecular[0] = 0.508273;
    lMateriales[1]->mEspecular[1] = 0.508273;
    lMateriales[1]->mEspecular[2] = 0.508273;
    lMateriales[1]->mEspecular[3] = 1.0;

    lMateriales[1]->brillo = 0.4;


    material *oro= (material*)malloc(sizeof(material));
    lMateriales[2] = oro;

    lMateriales[2]->mAmbiente[0] = 0.24725;
    lMateriales[2]->mAmbiente[1] = 0.1995;
    lMateriales[2]->mAmbiente[2] = 0.0745;
    lMateriales[2]->mAmbiente[3] = 1.0;

    lMateriales[2]->mDifusa[0] = 0.75164;
    lMateriales[2]->mDifusa[1] = 0.60648;
    lMateriales[2]->mDifusa[2] = 0.22648;
    lMateriales[2]->mDifusa[3] = 1.0;

    lMateriales[2]->mEspecular[0] = 0.628281;
    lMateriales[2]->mEspecular[1] = 0.555802;
    lMateriales[2]->mEspecular[2] = 0.555802;
    lMateriales[2]->mEspecular[3] = 1.0;

    lMateriales[2]->brillo = 0.4;


    material *obsidiana = (material*)malloc(sizeof(material));
    lMateriales[3] = obsidiana;

    lMateriales[3]->mAmbiente[0] = 0.05375;
    lMateriales[3]->mAmbiente[1] = 0.05;
    lMateriales[3]->mAmbiente[2] = 0.06625;
    lMateriales[3]->mAmbiente[3] = 1.0;

    lMateriales[3]->mDifusa[0] = 0.18275;
    lMateriales[3]->mDifusa[1] = 0.17;
    lMateriales[3]->mDifusa[2] = 0.22525;
    lMateriales[3]->mDifusa[3] = 1.0;

    lMateriales[3]->mEspecular[0] = 0.332741;
    lMateriales[3]->mEspecular[1] = 0.328634;
    lMateriales[3]->mEspecular[2] = 0.346435;
    lMateriales[3]->mEspecular[3] = 1.0;

    lMateriales[2]->brillo = 0.3;

      printf("Materiales inicializados con exito\n");


}

//Inicializar Luces por defecto
void inicializarLuces() {
    
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    //Reservar espacio para las luces
    int i;
    for(i = 0; i < 8; i++){
        luz *l = (luz *) malloc(sizeof(luz));
        l->inicializada = 0;
        l->num = i;

        glDisable(GL_LIGHT0+i);

        lLuces[i] = l;
    
    }
    //crear sol
    glEnable(GL_LIGHT0);
    lLuces[0]->tipo = 0;
    lLuces[0]->inicializada = 1;

    lLuces[0]->posicion[0] = 0.0;
    lLuces[0]->posicion[1] = 1.0;
    lLuces[0]->posicion[2] = 0.0;
    lLuces[0]->posicion[3] = 0.0;

    lLuces[0]->lAmbiental[0] = 0.2;
    lLuces[0]->lAmbiental[1] = 0.2;
    lLuces[0]->lAmbiental[2] = 0.2;
    lLuces[0]->lAmbiental[3] = 1.0;

    lLuces[0]->lEspecular[0] = 0.6;
    lLuces[0]->lEspecular[1] = 0.6;
    lLuces[0]->lEspecular[2] = 0.6;
    lLuces[0]->lEspecular[3] = 1.0;

    lLuces[0]->lDifusa[0] = 0.6;
    lLuces[0]->lDifusa[1] = 0.6;
    lLuces[0]->lDifusa[2] = 0.6;
    lLuces[0]->lDifusa[3] = 1.0;

    //primera bombilla
    glEnable(GL_LIGHT1);
    lLuces[1]->tipo = 1;
    lLuces[1]->inicializada = 1;

    lLuces[1]->posicion[0] = 5;
    lLuces[1]->posicion[1] = 5.0;
    lLuces[1]->posicion[2] = 5.0;
    lLuces[1]->posicion[3] = 1.0;

    lLuces[1]->lAmbiental[0] = 0.75;
    lLuces[1]->lAmbiental[1] = 0.75;
    lLuces[1]->lAmbiental[2] = 0.75;
    lLuces[1]->lAmbiental[3] = 1.0;

    lLuces[1]->lEspecular[0] = 0.75;
    lLuces[1]->lEspecular[1] = 0.75;
    lLuces[1]->lEspecular[2] = 0.75;
    lLuces[1]->lEspecular[3] = 1.0;

    lLuces[1]->lDifusa[0] = 0.75;
    lLuces[1]->lDifusa[1] = 0.75;
    lLuces[1]->lDifusa[2] = 0.75;
    lLuces[1]->lDifusa[3] = 1.0;

    lLuces[1]->lAtenuacion[0] = 0.4;
    lLuces[1]->lAtenuacion[1] = 0.3;
    lLuces[1]->lAtenuacion[2] = 0.2;

    //segunda bombilla
    /*
    //PON 2?????
    glEnable(GL_LIGHT1);
    lLuces[2]->tipo = 1;
    lLuces[2]->inicializada = 1;

    lLuces[2]->posicion[0] = 20.0;
    lLuces[2]->posicion[1] = 20.0;
    lLuces[2]->posicion[2] = 0;
    lLuces[2]->posicion[3] = 1.0;

    lLuces[2]->lAmbiental[0] = 0.65;
    lLuces[2]->lAmbiental[1] = 0.65;
    lLuces[2]->lAmbiental[2] = 0.65;
    lLuces[2]->lAmbiental[3] = 1.0;

    lLuces[2]->lEspecular[0] = 0.65;
    lLuces[2]->lEspecular[1] = 0.65;
    lLuces[2]->lEspecular[2] = 0.65;
    lLuces[2]->lEspecular[3] = 1.0;

    lLuces[2]->lDifusa[0] = 0.65;
    lLuces[2]->lDifusa[1] = 0.65;
    lLuces[2]->lDifusa[2] = 0.65;
    lLuces[2]->lDifusa[3] = 1.0;

    lLuces[2]->lAtenuacion[0] = 0.4;
    lLuces[2]->lAtenuacion[1] = 0.3;
    lLuces[2]->lAtenuacion[2] = 0.2;
*/
    //El sol es la luz por defecto
    selectedLuz = lLuces[0];

printf("Luz creada con exito\n");
   
}




/** GENERAL INITIALIZATION **/
void initialization (){

    /*Initialization of all the variables with the default values*/
    _ortho_x_min = KG_ORTHO_X_MIN_INIT;
    _ortho_x_max = KG_ORTHO_X_MAX_INIT;
    _ortho_y_min = KG_ORTHO_Y_MIN_INIT;
    _ortho_y_max = KG_ORTHO_Y_MAX_INIT;
    _ortho_z_min = KG_ORTHO_Z_MIN_INIT;
    _ortho_z_max = KG_ORTHO_Z_MAX_INIT;

    _window_ratio = (GLdouble) KG_WINDOW_WIDTH / (GLdouble) KG_WINDOW_HEIGHT;
     
    glClearColor(KG_COL_BACK_R, KG_COL_BACK_G, KG_COL_BACK_B, KG_COL_BACK_A);
    glPolygonMode(GL_FRONT, GL_FILL);

    // crear la primera camara

     printf("%s\n","Alocando espacio para la primera Camara");
     selectedCamara = (camara *) malloc(sizeof (camara));
     selectedCamara->next = 0;
     
     printf("%s\n","Comenzando creacion de la primera Camara");
        //cambiando----------------------------------------------------------------------------------------------------------
     //glMatrixMode(GL_MODELVIEW);
     gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
     glGetDoublev(GL_MODELVIEW_MATRIX, selectedCamara->mcsr);

            //ajustar la inversa

            selectedCamara->imcsr[0] = selectedCamara->mcsr[0];
            selectedCamara->imcsr[1] = selectedCamara->mcsr[4];
            selectedCamara->imcsr[2] = selectedCamara->mcsr[8];
            selectedCamara->imcsr[3] = selectedCamara->mcsr[3];

            selectedCamara->imcsr[4] = selectedCamara->mcsr[1];
            selectedCamara->imcsr[5] = selectedCamara->mcsr[5];
            selectedCamara->imcsr[6] = selectedCamara->mcsr[9];
            selectedCamara->imcsr[7] = selectedCamara->mcsr[7];

            selectedCamara->imcsr[8] = selectedCamara->mcsr[2];
            selectedCamara->imcsr[9] = selectedCamara->mcsr[6];
            selectedCamara->imcsr[10] = selectedCamara->mcsr[10];
            selectedCamara->imcsr[11] = selectedCamara->mcsr[11];

            selectedCamara->imcsr[12] = 0.0;
            selectedCamara->imcsr[13] = 0.0;
            selectedCamara->imcsr[14] = 20.0;
            selectedCamara->imcsr[15] = selectedCamara->mcsr[15];

            selectedCamara->params[0] = 0.1;
            selectedCamara->params[1] = 0.1;
            selectedCamara->params[2] = 0.1;
            selectedCamara->params[3] = 1000.0;

            iniCamara = selectedCamara;
        printf("%s\n","Camara creada con exito");

   
    
    


    
    /*Definition of the background color*/
    //glClearColor(KG_COL_BACK_R, KG_COL_BACK_G, KG_COL_BACK_B, KG_COL_BACK_A);

    /*Definition of the method to draw the objects*/
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


/** MAIN FUNCTION **/
int main(int argc, char** argv) {

    /*First of all, print the help information*/
    print_help();
    printHelpCamara();

    /* glut initializations */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(1000, 750); //KG_WINDOW_WIDTH      KG_WINDOW_HEIGHT
    glutInitWindowPosition(KG_WINDOW_X, KG_WINDOW_Y);
    glutCreateWindow(KG_WINDOW_TITLE);
    //NUEVOoooOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    if(luzSmooth == 1){
        glShadeModel(GL_SMOOTH);
    } else {
        glShadeModel(GL_FLAT);
    }

    /* set the callback functions */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SKeyboard);

    /* this initialization has to be AFTER the creation of the window */
    initialization();
    inicializarMateriales();
    inicializarLuces();
    

     

    /* start the main loop */
    glutMainLoop();
    return 0;
}
