#include "definitions.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

/** EXTERNAL VARIABLES **/

extern GLdouble _window_ratio;
extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

extern object3d *_first_object;
extern object3d *_selected_object;

extern camara *iniCamara;
extern camara *selectedCamara;

extern luz *lLuces[8];
extern int luzSmooth;

extern int modoP;


/**
 * @brief Function to draw the axes
 */
void draw_axes()
{
    /*Draw X axis*/
    glColor3f(KG_COL_X_AXIS_R,KG_COL_X_AXIS_G,KG_COL_X_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(KG_MAX_DOUBLE,0,0);
    glVertex3d(-1*KG_MAX_DOUBLE,0,0);
    glEnd();
    /*Draw Y axis*/
    glColor3f(KG_COL_Y_AXIS_R,KG_COL_Y_AXIS_G,KG_COL_Y_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,KG_MAX_DOUBLE,0);
    glVertex3d(0,-1*KG_MAX_DOUBLE,0);
    glEnd();
    /*Draw Z axis*/
    glColor3f(KG_COL_Z_AXIS_R,KG_COL_Z_AXIS_G,KG_COL_Z_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,0,KG_MAX_DOUBLE);
    glVertex3d(0,0,-1*KG_MAX_DOUBLE);
    glEnd();
}


/**
 * @brief Callback reshape function. We just store the new proportions of the window
 * @param width New width of the window
 * @param height New height of the window
 */
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    /*  Take care, the width and height are integer numbers, but the ratio is a GLdouble so, in order to avoid
     *  rounding the ratio to integer values we need to cast width and height before computing the ratio */
    _window_ratio = (GLdouble) width / (GLdouble) height;
}

void disLuZ() {
    int i;
    for (i = 0; i < 8; i++) {
        if(glIsEnabled(GL_LIGHT0+i)) {
            //luz *l = lLuces[i];
            glLightfv(GL_LIGHT0+i, GL_POSITION, lLuces[i]->posicion);
            glLightfv(GL_LIGHT0+i, GL_DIFFUSE, lLuces[i]->lDifusa);
            glLightfv(GL_LIGHT0+i, GL_AMBIENT, lLuces[i]->lAmbiental);
            glLightfv(GL_LIGHT0+i, GL_SPECULAR, lLuces[i]->lEspecular);

            if (lLuces[i]->tipo == 1 || lLuces[i]->tipo == 2) {
                glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, lLuces[i]->lAtenuacion[0]);
                glLightf(GL_LIGHT0+i, GL_LINEAR_ATTENUATION, lLuces[i]->lAtenuacion[1]);
                glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, lLuces[i]->lAtenuacion[2]);

                if(lLuces[i]->tipo == 2) {
                    glLightf(GL_LIGHT0+i, GL_SPOT_CUTOFF, lLuces[i]->corte);
                    glLightfv(GL_LIGHT0+i, GL_SPOT_DIRECTION, lLuces[i]->lDireccion);
                    glLightf(GL_LIGHT0+i, GL_SPOT_EXPONENT, lLuces[i]->exponente);
                }
            }
            
        }
    }
    
}

void disMaterial(object3d *objeto) {
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, objeto->matPTR->brillo);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto->matPTR->mDifusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto->matPTR->mAmbiente);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto->matPTR->mEspecular);

}

/**
 * @brief Callback display function
 */
void display(void) {
    GLint v_index, v, f;
    object3d *aux_obj = _first_object;

    /* Clear the screen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Define the projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(modoP == 0){
            
        glOrtho(-selectedCamara->params[0]*50.0, selectedCamara->params[0]*50.0,
                 -selectedCamara->params[1]*50.0, selectedCamara->params[1]*50.0,
                 selectedCamara->params[2], selectedCamara->params[3]);
        

    }else
    {
        glFrustum(-selectedCamara->params[0], selectedCamara->params[0],
                 -selectedCamara->params[1], selectedCamara->params[1],
                 selectedCamara->params[2], selectedCamara->params[3]);

    }
    

    /* Now we start drawing the object */
    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    /*First, we draw the axes*/
    draw_axes();

    glLoadMatrixd(selectedCamara->mcsr);

    disLuZ();


    /*Now each of the objects in the list*/
    while (aux_obj != 0) {

        /* Select the color, depending on whether the current object is the selected one or not */
        if (aux_obj == _selected_object){
            glColor3f(KG_COL_SELECTED_R,KG_COL_SELECTED_G,KG_COL_SELECTED_B);
        }else{
            glColor3f(KG_COL_NONSELECTED_R,KG_COL_NONSELECTED_G,KG_COL_NONSELECTED_B);
        }

        disMaterial(aux_obj);

        /* Draw the object; for each face create a new polygon with the corresponding vertices*/


       /*
       cargar mcsr
       push matriz mcsr
       mult obj
       pop
       */

      
        glPushMatrix();
      
        glMultMatrixd(aux_obj->mtrxPTR->mat);
      

        for (f = 0; f < aux_obj->num_faces; f++) {
            if(glIsEnabled(GL_LIGHTING)){
                glBegin(GL_POLYGON);
            
            } else {
                glBegin(GL_LINE_LOOP);
            }

            //flat
            if(luzSmooth == 0) {
                vector3 vec = aux_obj->face_table[f].normal;
                glNormal3d(aux_obj->face_table[f].normal.x, aux_obj->face_table[f].normal.y, aux_obj->face_table[f].normal.z);
            }
            
            for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                v_index = aux_obj->face_table[f].vertex_table[v];
                
                //smooth
                if(luzSmooth == 1){
                    vector3 vecS = aux_obj->vertex_table[v_index].normalV;
                    glNormal3d(aux_obj->vertex_table[v_index].normalV.x, aux_obj->vertex_table[v_index].normalV.y, aux_obj->vertex_table[v_index].normalV.z);
                }

                glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                        aux_obj->vertex_table[v_index].coord.y,
                        aux_obj->vertex_table[v_index].coord.z);

            }
            glEnd();
        }
        glPopMatrix();
        aux_obj = aux_obj->next;
    }
    
    /*Do the actual drawing*/
    
    glFlush();
    glutSwapBuffers();
}
