#include "definitions.h"
#include "load_obj.h"
#include <GL/glut.h>
#include <stdio.h>
#include "display.h"
#include "math.h"
#define M_PI 3.14159265358979323846

extern object3d * _first_object;
extern object3d * _selected_object;

extern camara *iniCamara;
extern camara *selectedCamara;
camara *auxCamara = 0;

extern luz *lLuces[8];
extern luz *selectedLuz;
extern int luzSmooth;


extern material *lMateriales[4];

extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

int modo = 0;
int GLOBAL = 0;
int borrar = 0;
int mCamara = 1;
int vuelo = 1;
int modoP = 1;
int mLuces = 0;

double aux[3]={0,0,0};


/**
 * @brief This function just prints information about the use
 * of the keys
 */
void print_help(){
    printf("KbG Irakasgaiaren Praktika. Programa honek 3D objektuak \n");
    printf("aldatzen eta bistaratzen ditu.  \n\n");
    printf("\n\n");
    printf("FUNTZIO NAGUSIAK \n");
    printf("<?>\t\t Laguntza hau bistaratu \n");
    printf("<ESC>\t\t Programatik irten \n");
    printf("<F>\t\t Objektua bat kargatu\n");
    printf("<TAB>\t\t Kargaturiko objektuen artean bat hautatu\n");
    printf("<DEL>\t\t Hautatutako objektua ezabatu\n");
    printf("<CTRL + ->\t Bistaratze-eremua handitu\n");
    printf("<CTRL + +>\t Bistaratze-eremua txikitu\n");

    printf("LUCES:\n");
    printf("<F9>\t\t Activar/desactivar la iluminacion\n");
    printf("<F1-F8>\t\t Encender/apagar la fuente correspondiente\n");
    printf("<1-8\t\t Seleccionar la fuente correspondiente\n");
    printf("<0>\t\t Asignar tipo de fuente a la luz seleccionada\n");
    printf("<Insert>\t\t Cambiar de tipo de iluminacion: FLAT/SMOOTH\n");

    printf("<K, k>\t\t Cambiar a modo camara\n");
    printf("<O, o>\t\t Cambiar a modo objeto\n");
    printf("<A,a>\t\t Aplicar transformaciones a las fuentes de luz\n");
    printf("\n\n");


}

void printHelpObjetos() {
    printf("CONTROL OBJETOS \n\n");

    printf("Las transformaciones se realizan con las flechas y F11, F12");
    printf("<M, m>\t\t Trasladar \n");
    printf("<B, b>\t\t Rotar \n");   
    printf("<T,t>\t\t Escalar \n\n");

    printf("<G, g>\t\t Transformaciones globales \n");
    printf("<L, l>\t\t Transformaciones locales \n"); 
    printf("<Z, z>\t\t Revertir el ultimo cambio \n\n");
    printf("\n\n");

}


void printHelpCamara() {
    printf("CONTROLES DE LA CAMARA \n\n");

    printf("Las transformaciones se realizan con las flechas y F11, F12 \n");
    printf("<M, m>\t\t Trasladar \n");
    printf("<B, b>\t\t Rotar \n");   
    printf("<T,t>\t\t Cambiar volumen de vision \n\n");

    printf("<N,n>\t\t Crea una camara nueva estandandarizada \n\n");
    printf("<c>\t\t Cambiar de camara \n");
    printf("<C>\t\t Visualizar lo que ve el objeto seleccionado \n");
    printf("<G, g>\t\t Modo analisis \n");
    printf("<L, l>\t\t Modo vuelo \n"); 
    printf("<P, p>\t\t Cambiar tipo de proyeccion: perspectiva/paralela \n");  
    printf("<P, p>\t\t Cambiar tipo de proyeccion: perspectiva/paralela \n");
    printf("\n\n");

}

void printHelpIlum() {
    printf("CONTROLES DE LA ILUMINACION \n\n");
    
    printf("Las transformaciones se realizan con las flechas y F11, F12");
    printf("<M, m>\t\t Trasladar \n");
    printf("<B, b>\t\t Rotar \n\n");   

    printf("<+>\t\t Incrementar angulo de apertura (solo focos) \n");
    printf("<->\t\t Decrementar angulo de apertura (solo focos) \n\n");
    printf("\n\n");
}

//actualizar la matriz inversa a la de cambio de sistema de referencia
void actualizaInversa( camara * c) {
    
    
    c->imcsr[0] = c->mcsr[0];
    c->imcsr[1] = c->mcsr[4];
    c->imcsr[2] = c->mcsr[8];
    

    c->imcsr[4] = c->mcsr[1];
    c->imcsr[5] = c->mcsr[5];
    c->imcsr[6] = c->imcsr[9];
    

    c->imcsr[8] = c->mcsr[2];
    c->imcsr[9] = c->mcsr[6];
    c->imcsr[10] = c->mcsr[10];
    
 }

 //actualziar la matrizz de cambio de sistema de referencia
void actualizarMCSR(){


    selectedCamara->mcsr[0] = selectedCamara->imcsr[0];
    selectedCamara->mcsr[1] = selectedCamara->imcsr[4];
    selectedCamara->mcsr[2] = selectedCamara->imcsr[8];

    selectedCamara->mcsr[4] = selectedCamara->imcsr[1];
    selectedCamara->mcsr[5] = selectedCamara->imcsr[5];
    selectedCamara->mcsr[6] = selectedCamara->imcsr[9];

    selectedCamara->mcsr[8] = selectedCamara->imcsr[2];
    selectedCamara->mcsr[9] = selectedCamara->imcsr[6];
    selectedCamara->mcsr[10] = selectedCamara->imcsr[10];

    int eX = selectedCamara->imcsr[12]*selectedCamara->imcsr[0] + selectedCamara->imcsr[13]*selectedCamara->imcsr[1] + selectedCamara->imcsr[14]*selectedCamara->imcsr[2];
    int eY = selectedCamara->imcsr[12]*selectedCamara->imcsr[4] + selectedCamara->imcsr[13]*selectedCamara->imcsr[5] + selectedCamara->imcsr[14]*selectedCamara->imcsr[6];
    int eZ = selectedCamara->imcsr[12]*selectedCamara->imcsr[8] + selectedCamara->imcsr[13]*selectedCamara->imcsr[9] + selectedCamara->imcsr[14]*selectedCamara->imcsr[10];
    selectedCamara->mcsr[12] = -eX;
    selectedCamara->mcsr[13] = -eY;
    selectedCamara->mcsr[14] = -eZ;


}

//Crea una camara asociada al objeto
 void actualizarObj(){

     printf("%s\n", "Creando Camara de un Objeto");

     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();

     gluLookAt(_selected_object->mtrxPTR->mat[12], _selected_object->mtrxPTR->mat[13], _selected_object->mtrxPTR->mat[14],
                _selected_object->mtrxPTR->mat[12] + _selected_object->mtrxPTR->mat[8], _selected_object->mtrxPTR->mat[13] + _selected_object->mtrxPTR->mat[9], _selected_object->mtrxPTR->mat[14] + _selected_object->mtrxPTR->mat[10] ,
                _selected_object->mtrxPTR->mat[4], _selected_object->mtrxPTR->mat[5], _selected_object->mtrxPTR->mat[6]);

     glGetDoublev(GL_MODELVIEW_MATRIX, _selected_object->miCamara->mcsr);

     _selected_object->mtrxPTR->mat[12] = _selected_object->mtrxPTR->mat[12];
     _selected_object->mtrxPTR->mat[13] = _selected_object->mtrxPTR->mat[13];
     _selected_object->mtrxPTR->mat[14] = _selected_object->mtrxPTR->mat[14];

     actualizaInversa(_selected_object->miCamara);

     printf("%s\n", "Fin de la creacion de la Camara");

 }


//Coloca la camara mirando en direccion al objeto seleccionado
 void empezarAnalisis(){

     printf("%s\n", "Colocando la Camara mirando al objeto");

     glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
     gluLookAt(selectedCamara->imcsr[12], selectedCamara->imcsr[13], selectedCamara->imcsr[14],
                _selected_object->mtrxPTR->mat[12],  _selected_object->mtrxPTR->mat[13], _selected_object->mtrxPTR->mat[14],
                selectedCamara->imcsr[4], selectedCamara->imcsr[5], selectedCamara->imcsr[6]
     ); 
     glGetDoublev(GL_MODELVIEW_MATRIX, selectedCamara->mcsr);
     actualizaInversa(selectedCamara);

     printf("%s\n", "En posicion");
    
    }

//Crea una camara con los valores dados
 void crearCamara( camara * c, double eX, double eY, double eZ, double pX, double pY, double pZ,double uX, double uY, double uZ){

     printf("%s\n", "Creando una nueva Camara");

     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();

     gluLookAt(eX, eY, eZ, pX, pY, pZ, uX, uY, uZ);
     glGetDoublev(GL_MODELVIEW_MATRIX, c->mcsr);

            //ajustar la inversa

            
            c->imcsr[3] = 0.0;          
            c->imcsr[7] = 0.0;
            c->imcsr[11] = 0.0;

            c->imcsr[12] = 0.0;
            c->imcsr[13] = 0.0;
            c->imcsr[14] = 20.0;
            c->imcsr[15] = 1.0;


            c->params[0] = 0.1;
            c->params[1] = 0.1;
            c->params[2] = 0.1;
            c->params[3] = 1000.0;

            actualizaInversa(c);
     printf("%s\n", "Camara creada");

 }

//Rota la camara la cantidad de grados dada sobre el eje proporcionado
void rotacionLibre(double grados, double x, double y, double z){

    printf("%s\n", "Rotando la camara en modo Vuelo");

    point3 aux;
    aux.x = selectedCamara->imcsr[12];
    aux.y = selectedCamara->imcsr[13];
    aux.z = selectedCamara->imcsr[14];

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(grados,x,y,z);
    glMultMatrixd(selectedCamara->imcsr);
    glGetDoublev(GL_MODELVIEW_MATRIX, selectedCamara->imcsr );

    selectedCamara->imcsr[12] = aux.x;
    selectedCamara->imcsr[13] = aux.y;
    selectedCamara->imcsr[14] = aux.z;

    printf("%s\n", "Fin de la rotacion");


}

//Rota la cama alrededor del objeto
void rotacionLigada(double grados, double x, double y, double z){

    printf("%s\n", "Rotacion de la Camara mirando al objeto");

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(_selected_object->mtrxPTR->mat[12], _selected_object->mtrxPTR->mat[13], _selected_object->mtrxPTR->mat[14]);
    glRotated(grados,x,y,z);
    glTranslated(-_selected_object->mtrxPTR->mat[12], -_selected_object->mtrxPTR->mat[13], -_selected_object->mtrxPTR->mat[14]);

    glMultMatrixd(selectedCamara->imcsr);
    glGetDoublev(GL_MODELVIEW_MATRIX, selectedCamara->imcsr );

    printf("%s\n", "Fin de la rotacion");

}

//Mueve la camara en modo vuelo en eje dado
void volar(double x, double y, double z){


    selectedCamara->imcsr[12] = selectedCamara->imcsr[12] + x;
    selectedCamara->imcsr[13] = selectedCamara->imcsr[13] + y;
    selectedCamara->imcsr[14] = selectedCamara->imcsr[14] + z;


}


//Borra un objeto borrando primero todas sus caras y vertices
void deletePoligon(object3d *oPTR){


	int i;
    for( i=0; i>oPTR->num_faces; i++){

            	free( _selected_object->face_table[i].vertex_table);
    }
    free(oPTR->face_table);
    free(oPTR->vertex_table);
    free(oPTR);

}

/////////////////////////////////Luces

//Crea un foco estandandariado
void crearFoco(object3d *oPTR) {
    oPTR->focoPTR = (luz *) malloc(sizeof(luz));
    glEnable(GL_LIGHT2);

    oPTR->focoPTR->tipo = 2;
    oPTR->focoPTR->inicializada = 1;

    oPTR->focoPTR->lAmbiental[0] = 0.8;
    oPTR->focoPTR->lAmbiental[1] = 0.8;
    oPTR->focoPTR->lAmbiental[2] = 0.8;
    oPTR->focoPTR->lAmbiental[3] = 1.0;

    oPTR->focoPTR->lDifusa[0] = 0.8;
    oPTR->focoPTR->lDifusa[1] = 0.8;
    oPTR->focoPTR->lDifusa[2] = 0.8;
    oPTR->focoPTR->lDifusa[3] = 1.0;

    oPTR->focoPTR->lEspecular[0] = 0.8;
    oPTR->focoPTR->lEspecular[1] = 0.8;
    oPTR->focoPTR->lEspecular[2] = 0.8;
    oPTR->focoPTR->lEspecular[3] = 1.0;

    oPTR->focoPTR->posicion[0] = oPTR->mtrxPTR->mat[12];
    oPTR->focoPTR->posicion[1] = oPTR->mtrxPTR->mat[13];
    oPTR->focoPTR->posicion[2] = oPTR->max.z;
    oPTR->focoPTR->posicion[3] = 1.0;

    oPTR->focoPTR->lAtenuacion[0] = 0.3;
    oPTR->focoPTR->lAtenuacion[1] = 0.2;
    oPTR->focoPTR->lAtenuacion[2] = 0.1;

    oPTR->focoPTR->lDireccion[0] = oPTR->mtrxPTR->mat[8];
    oPTR->focoPTR->lDireccion[0] = oPTR->mtrxPTR->mat[9];
    oPTR->focoPTR->lDireccion[0] = oPTR->mtrxPTR->mat[10];

    oPTR->focoPTR->corte = 50.0;

    oPTR->focoPTR->exponente = 0.0;

}

//Actualiza el foco despues de una transformacio
void actualizarFoco(){
    _selected_object->focoPTR->posicion[0] = _selected_object->mtrxPTR->mat[12];
    _selected_object->focoPTR->posicion[1] = _selected_object->mtrxPTR->mat[13];
    _selected_object->focoPTR->posicion[2] = _selected_object->mtrxPTR->mat[14];

    _selected_object->focoPTR->lDireccion[0] = _selected_object->mtrxPTR->mat[8];
    _selected_object->focoPTR->lDireccion[1] = _selected_object->mtrxPTR->mat[9];
    _selected_object->focoPTR->lDireccion[2] = _selected_object->mtrxPTR->mat[10];

}

//Cambia el estado de las luces
void encenderApagarLuz(int l){
    //luz creada
    if(lLuces[l]->inicializada) {

        int luz = GL_LIGHT0 + l;
        if(glIsEnabled(luz)) {
            //apagar luz
            glDisable(luz);
            printf("hemos apagado la luz %d\n", l);
        } else {
            //encender luz
            glEnable(luz);
            printf("hemos encendido la luz %d\n", l);
        }

    } else {
        printf("Esta luz no esta creada\n");

    }
}

// Calcula el vector normal del plano que contiene los 2 vectores dados
vector3 vNormal(vector3 v1, vector3 v2) {
    
    vector3 res;
    res.x = (v1.y*v2.z)-(v2.y*v1.z);
    res.y = -(v1.x*v2.z)+(v2.x*v1.z);
    res.z = (v1.x*v2.y)-(v2.x*v1.y);

    return res;

}




//Calcula todos los vectores normales normales de un objeto
void vNoarmlesObjeto (object3d * objeto)
{
	int i;
    int j;
	face *cara;
	vector3 v1;
    vector3 v2;
    vector3 normal;
	double mod;
	
	for(i=0; i < objeto->num_vertices; i++)
	{
		objeto->vertex_table[i].normalV.x = 0;
		objeto->vertex_table[i].normalV.y=0;
		objeto->vertex_table[i].normalV.z=0;
	}
		
	
	for(j=0; j < objeto->num_faces; j++)
	{
		cara = &(objeto->face_table[j]);
		

		v1.x = (objeto->vertex_table[cara->vertex_table[0]].coord.x - objeto->vertex_table[cara->vertex_table[1]].coord.x);
        v1.y = (objeto->vertex_table[cara->vertex_table[0]].coord.y-objeto->vertex_table[cara->vertex_table[1]].coord.y);
        v1.z = (objeto->vertex_table[cara->vertex_table[0]].coord.z-objeto->vertex_table[cara->vertex_table[1]].coord.z);
		

		v2.x = (objeto->vertex_table[cara->vertex_table[2]].coord.x-objeto->vertex_table[cara->vertex_table[1]].coord.x);
        v2.y = (objeto->vertex_table[cara->vertex_table[2]].coord.y-objeto->vertex_table[cara->vertex_table[1]].coord.y);
        v2.z = (objeto->vertex_table[cara->vertex_table[2]].coord.z-objeto->vertex_table[cara->vertex_table[1]].coord.z);
		
		//calcular_vector_normal(&normal, v2, v1);
        normal = vNormal(v1,v2);
		
		//mod = mod_vector(normal.x, normal.y, normal.z);
        mod = sqrt((normal.x*normal.x)+(normal.y*normal.y)+(normal.z*normal.z));
		
		cara->normal.x = normal.x/mod;
		cara->normal.y = normal.y/mod;
		cara->normal.z = normal.z/mod;
		
        //Calcular el vector normal de los vertices
		for(i=0; i < cara->num_vertices; i++)
		{
			objeto->vertex_table[cara->vertex_table[i]].normalV.x +=  cara->normal.x;
			objeto->vertex_table[cara->vertex_table[i]].normalV.y +=  cara->normal.y;
			objeto->vertex_table[cara->vertex_table[i]].normalV.z +=  cara->normal.z;
		}
	}
	
    //pasar todos vectores a unitarios
	for(i=0; i < objeto->num_vertices; i++)
	{
		mod = sqrt((objeto->vertex_table[i].normalV.x*objeto->vertex_table[i].normalV.x)+
                    (objeto->vertex_table[i].normalV.y*objeto->vertex_table[i].normalV.y)+
                    (objeto->vertex_table[i].normalV.z*objeto->vertex_table[i].normalV.z)
                    );
		
		objeto->vertex_table[i].normalV.x = objeto->vertex_table[i].normalV.x / mod;
		objeto->vertex_table[i].normalV.y = objeto->vertex_table[i].normalV.y / mod;
		objeto->vertex_table[i].normalV.z = objeto->vertex_table[i].normalV.z / mod;
	}
}

//traslada la luz en el eje y lado dados
void lTrasladar(char eje, char lado) {
    int tipo = selectedLuz->tipo;

    printf("x %lf", selectedLuz->posicion[0]);
    printf("y %lf", selectedLuz->posicion[1]);
    printf("z %lf", selectedLuz->posicion[2]);
    printf("\n");

    if(tipo == 0) {
        printf("Este tipo de traslaciones no se pueden aplicar al sol\n");

    } else {
        if(eje == 'x'){
            if(lado == '+') {
                selectedLuz->posicion[0] += 0.1;
            } else {
                selectedLuz->posicion[0] -= 0.1;
            }
        } else if(eje = 'y') {
            if(lado == '+') {
                selectedLuz->posicion[1] += 0.1;
            } else {
                selectedLuz->posicion[1] -= 0.1;
            }
        } else {
            if(lado == '+') {
                selectedLuz->posicion[1] += 0.1;
            } else {
                selectedLuz->posicion[1] -= 0.1;
            }
        }

    }
}

void lRotar(char eje, double g){
    double rad = g * (M_PI/180.0);
    double x;
    double y;
    double z;
    if(selectedLuz->tipo !=0){
        if(selectedLuz->tipo == 2 && GLOBAL == 0) {
            x = selectedLuz->lDireccion[0];
            y = selectedLuz->lDireccion[1];
            z = selectedLuz->lDireccion[2];

            if(eje == 'x') {
                selectedLuz->lDireccion[1] = cos(rad)*y - sin(rad)*z;
                selectedLuz->lDireccion[2] = sin(rad)*y + cos(rad)*z;

            } else if(eje == 'y') {
                selectedLuz->lDireccion[0] = cos(rad)*x + sin(rad)*z;
                selectedLuz->lDireccion[2] = -sin(rad)*x + cos(rad)*z;

            } else {
                selectedLuz->lDireccion[0] = cos(rad)*x - sin(rad)*y;
                selectedLuz->lDireccion[1] = sin(rad)*x + cos(rad)*y;
            }
        } else {
            x = selectedLuz->posicion[0];
            y = selectedLuz->posicion[1];
            z = selectedLuz->posicion[2];

            if(eje == 'x') {
                selectedLuz->posicion[1] = cos(rad)*y - sin(rad)*z;
                selectedLuz->posicion[2] = sin(rad)*y + cos(rad)*z;

            } else if(eje == 'y') {
                selectedLuz->posicion[0] = cos(rad)*x + sin(rad)*z;
                selectedLuz->posicion[2] = -sin(rad)*x + cos(rad)*z;

            } else {
                selectedLuz->posicion[0] = cos(rad)*x - sin(rad)*y;
                selectedLuz->posicion[1] = sin(rad)*x + cos(rad)*y;
            }
        }
    }else
    {
        printf("No se puede rotar el sol");
    }
    
}





/**
 * @brief Callback function to control the basic keys
 * @param key Key that has been pressed
 * @param x X coordinate of the mouse pointer when the key was pressed
 * @param y Y coordinate of the mouse pointer when the key was pressed
 */
void keyboard(unsigned char key, int x, int y) {

    char* fname = malloc(sizeof (char)*128); /* Note that scanf adds a null character at the end of the vector*/
    int read = 0;
    lMatrix *auxMatrixptr = (lMatrix *) malloc(sizeof (lMatrix));
    char c;
    double X;
    double Y;
    double Z;
    int m;
    object3d *auxiliar_object = 0;
    GLdouble wd,he,midx,midy;
    camara *nCamara = 0;

    switch (key) {
    case 'f':
    case 'F':
        /*Ask for file*/
        printf("%s", KG_MSSG_SELECT_FILE);
        scanf("%s", fname);
        /*Allocate memory for the structure and read the file*/
        auxiliar_object = (object3d *) malloc(sizeof (object3d));
        read = read_wavefront(fname, auxiliar_object);
        switch (read) {
        /*Errors in the reading*/
        case 1:
            printf("%s: %s\n", fname, KG_MSSG_FILENOTFOUND);
            break;
        case 2:
            printf("%s: %s\n", fname, KG_MSSG_INVALIDFILE);
            break;
        case 3:
            printf("%s: %s\n", fname, KG_MSSG_EMPTYFILE);
            break;
        /*Read OK*/
        case 0:

             auxiliar_object->mtrxPTR= (lMatrix *)malloc(sizeof(lMatrix));
             auxiliar_object->mtrxPTR->next = 0;
		   

             glMatrixMode(GL_MODELVIEW_MATRIX);
        	 glLoadIdentity();
        	 glGetDoublev(GL_MODELVIEW_MATRIX,auxiliar_object->mtrxPTR->mat);
            
            vNoarmlesObjeto(auxiliar_object);
            printf("Vectores normales calculados \n");

            //Por defecto sera del material obsidiana
            auxiliar_object->matPTR = lMateriales[3];
            printf("Material asignado \n");

            //Creamos el foco del obejto
            crearFoco(auxiliar_object);
            lLuces[2] = auxiliar_object->focoPTR;
            printf("Foco asociado al objeto creado \n");
            
            //Crear la camara asociada al objeto
            auxiliar_object->miCamara= (camara *)malloc(sizeof(camara));
            crearCamara(auxiliar_object->miCamara,
            auxiliar_object->mtrxPTR->mat[12],auxiliar_object->mtrxPTR->mat[13],auxiliar_object->mtrxPTR->mat[14],
            auxiliar_object->mtrxPTR->mat[12]+auxiliar_object->mtrxPTR->mat[8],auxiliar_object->mtrxPTR->mat[13]
             +auxiliar_object->mtrxPTR->mat[9],auxiliar_object->mtrxPTR->mat[14]+auxiliar_object->mtrxPTR->mat[10],
            auxiliar_object->mtrxPTR->mat[4],auxiliar_object->mtrxPTR->mat[5],auxiliar_object->mtrxPTR->mat[6]
            );
            printf("Camara asociada al objeto creada \n"); 
            
            auxiliar_object->next = _first_object;
            _first_object = auxiliar_object;
            _selected_object = _first_object;

            printf("%s\n",KG_MSSG_FILEREAD);
            borrar=1;
            break;
        }
        break;

    case 9: /* <TAB> */
        if (_selected_object)
        {
            _selected_object = _selected_object->next;
            //si el siguiente objeto es nulo volver al primero
            if (_selected_object == 0){
                _selected_object = _first_object;
            }
            if(vuelo == 0){
                empezarAnalisis();
            }
            if(selectedLuz == lLuces[2] ){
                //actualzizar el foco
                selectedLuz = _selected_object->focoPTR;
                lLuces[2] = _selected_object->focoPTR;
            }
        }else{
            printf("Debes cargar un objeto");
        }
        break;

    case 127: /* <SUPR> */
        if(_selected_object!=0){
       	 if (_selected_object == _first_object)
        	{
                //Si es el primero objeto pasar al siguiente y si no hay siguiente dejar el parametro borrar a 0
            	_first_object = _first_object->next;
            	deletePoligon(_selected_object);
            	_selected_object = _first_object;
            	if (_selected_object==0)
            	{
            		borrar=0;
            	}

        	} else {
                //Pasar al objeto anterior
            	auxiliar_object = _first_object;
            	while (auxiliar_object->next != _selected_object)
                auxiliar_object = auxiliar_object->next;
            	auxiliar_object->next = _selected_object->next;
            	deletePoligon(_selected_object);
            	_selected_object = auxiliar_object;
        }
    }
        break;


    case '-':
    //  Aumentamos el volumen de vision aumentando zoom
        if ((glutGetModifiers() == GLUT_ACTIVE_CTRL) || (mCamara == 1 && mLuces == 0)){
            	selectedCamara->params[1] = selectedCamara->params[1] / 0.5;
        		selectedCamara->params[0] = selectedCamara->params[0] / 0.5;
        		selectedCamara->params[2] = selectedCamara->params[2] * 0.5;
        		selectedCamara->params[3] = selectedCamara->params[3] / 0.5;
        		printf("Disminuir el ZOOM\n");
        } else if (mCamara == 0 && mLuces == 0 ){
        	if (_selected_object)
        	{
        		//escalar hacia abajo todo el objeto
        		printf("Disminuir la escala\n");
        		glMatrixMode(GL_MODELVIEW);
		    	if (GLOBAL == 0){
			    	glLoadMatrixd(_selected_object->mtrxPTR->mat);
                }
                else{
                        glLoadIdentity();
                }
        		glScaled(0.9,0.9,0.9);
        		if ( GLOBAL == 1){
                     glMultMatrixd(_selected_object->mtrxPTR->mat);
                }
		    	glGetDoublev(GL_MODELVIEW_MATRIX, auxMatrixptr->mat);
		    	auxMatrixptr->next = _selected_object->mtrxPTR;
                _selected_object->mtrxPTR =  auxMatrixptr;
		    	actualizarFoco();
        	}else{
        		printf("Carga un objeto primero\n");
        	}
        } else { 
            //Redocir el corte de la luz
        	    if (selectedLuz->tipo == 2 )
        	    {
        	    		if (selectedLuz->corte > 1.0)
        	    		{
                            if (selectedLuz->corte==180.0){
                                    selectedLuz->corte = 90.0;
                            }
                            else{
                                    selectedLuz->corte-=1.0;
                            }
		   	    		
        	    		}else{
        	    			printf("No se puede reducir mas el angulo de apertura\n");
        	    		}
        	    }else{
        	    	printf("La luz debe ser un foco\n");
        	    }
        }
        break;

    case '+':


    	if ((glutGetModifiers() == GLUT_ACTIVE_CTRL) || (mCamara == 1 && mLuces == 0)){
            //Reducimos el volumen de vision haciendo zoom
            	selectedCamara->params[1] = selectedCamara->params[1] * 0.5;
        		selectedCamara->params[0] = selectedCamara->params[0] * 0.5;
        		selectedCamara->params[2] = selectedCamara->params[2] / 0.5;
        		selectedCamara->params[3] = selectedCamara->params[3] * 0.5;
        		printf(" Aumentar el ZOOM\n");
        } else if (mCamara == 0 && mLuces == 0 ){
        	if (_selected_object)
        	{
        		//Aumenta la escala
        		printf("Aumentar la escala\n");
        		glMatrixMode(GL_MODELVIEW);
		    	if (GLOBAL == 0){
			    	glLoadMatrixd(_selected_object->mtrxPTR->mat);
                }
                else{
                        glLoadIdentity();
                }
        		glScaled(1.1,1.1,1.1);
        		if ( GLOBAL == 1){
                     glMultMatrixd(_selected_object->mtrxPTR->mat);
                }
		    	glGetDoublev(GL_MODELVIEW_MATRIX, auxMatrixptr->mat);
		    	auxMatrixptr->next = _selected_object->mtrxPTR;
                _selected_object->mtrxPTR =  auxMatrixptr;
		    	actualizarFoco();
        	}else{
        		printf("Carga un objeto primero\n");
        	}
        } else { 
            //Aumentar el corte de la camara
        	    if (selectedLuz->tipo == 2 )
        	    {
        	    		if (selectedLuz->corte < 180.0)
        	    		{
                            if (selectedLuz->corte==90.0){
                                    selectedLuz->corte = 180.0;
                            }
                            else{
                                    selectedLuz->corte+=1.0;
                            }
		   	    		
        	    		}else{
        	    			printf("No se puede reducir mas el angulo de apertura\n");
        	    		}
        	    }else{
        	    	printf("La luz debe ser un foco\n");
        	    }
        }
        break;

    case '?':
        print_help();
        if(mCamara == 0 && mLuces == 0) {
            printHelpObjetos();
        } else if(mCamara == 1 && mLuces == 0) {
            printHelpCamara();
        } else {
            printHelpIlum();
        }
        break;

    case 'z':
        //Desacer los cambios del objeto
    	if(borrar == 1 && _selected_object->mtrxPTR->next !=0 && _selected_object->mtrxPTR->mat !=0 ){
        	_selected_object->mtrxPTR = _selected_object->mtrxPTR->next;
            glutPostRedisplay();
        	
        }else{
        	printf("%s\n", "Porfavor carga un objeto o realiza alguna transformacion antes de borrar" );
        }

        break;
    case 'M':
    case 'm':
        //Activar traslaciones modo traslacion
    	printf("%s\n"," Pasamos a modo traslacion" );
        modo = 0;
        break;

    case 'K':
    case 'k':
        //Activar el modo camara
    	printf("%s\n"," Pasamos a modo Camara" );
        printHelpCamara();
        mCamara = 1;
        mLuces = 0;

        break;

    case 'P':
    case 'p':
        //Alterna el tipo de Proyeccion 0 Paralela 1 Perspectiva
    	printf("%s\n"," Cambiamos el modo de Proyeccion" );
        if(modoP == 0){
            modoP = 1;;

        }else
        {
            modoP = 0;

        }
        

        break;
    case 'O':
    case 'o':
        //Activar el modo objeto
    	printf("%s\n"," Pasamos a modo Objeto" );
        printHelpObjetos();
        mCamara = 0;
        mLuces = 0;

        break;

    case 'c':
        //Cambiar de camara
    	printf("%s\n"," Pasamos a la siguiente Camara" );
        if(selectedCamara->next != 0){  
            selectedCamara = selectedCamara->next;
        }else
        {
            selectedCamara = iniCamara;
        }
        if(vuelo == 0){
            empezarAnalisis();
        }
        
        break;


    case 'C':
            //Alternar entre la camara del objeto y la previamente seleccionada
    		if (_selected_object != 0)
    		{
	    		if (auxCamara == 0)
	    		{
		    		auxCamara = selectedCamara;
		    		selectedCamara = _selected_object->miCamara;
		    		actualizarObj();
		    		printf("Camara de objeto actualizada\n");
		    	} else {
		    		selectedCamara = auxCamara;
		    		auxCamara = 0;
		    		printf("Volvemos a la camara inicial\n");
		    	}
	    	}else{
	    		printf("Carga un objeto primero\n");
	    	}
    		break;

    case 'B':
    case 'b':
        //Activar el modo rotacion
    	printf("%s\n"," Pasamos a modo rotacion" );
        modo = 1;
        break;

    case 'T':
    case 't':
        //Activar el modo escalado
    	printf("%s\n"," Pasamos a modo escalado" );
        modo = 2;
        break;

    case 'G':
    case 'g':
    	
        //Activar el modo analisis si estas en modo camara o pasar al modo global si no
        if(mCamara == 1){
            if(_selected_object != 0){
            vuelo = 0;
            printf("%s\n"," Modo Analisis" );
            
            empezarAnalisis();
            }else
            {
                printf("Carga un objeto primero\n");
            }
            
            
        }else{
            printf("%s\n"," GLOBAL en modo GLOBAL" );
            
            GLOBAL = 1;
        }
        break;

    case 'L':
    case 'l':
    	//Activar el modo vuelo si estas en modo camara o poner el modo local
        if(mCamara == 1){
            printf("%s\n"," Camara en modo Vuelo" );
            vuelo = 1;
        }else{
            printf("%s\n"," GLOBAL en modo LOCAL" );
        GLOBAL = 0;
        }
        
        break;

    case 'N':
    case 'n':
        //Creacion de una nueva camara
        nCamara = (camara  *) malloc(sizeof (camara));
        crearCamara(nCamara, 0.0, 0.0, 20.0,
                             0.0, 0.0, 0.0,
                             0.0, 1.0, 0.0);
        nCamara->next = iniCamara;
        iniCamara = nCamara;
        selectedCamara = iniCamara;

        break;

    case 'A':
    case 'a':
    printf("%s\n"," Pasamos a modo Luces" );
    printHelpIlum();
    //Activar el modo luces
        if(mLuces == 1) {
            mLuces = 0;
        } else {
            mLuces = 1;
        }
        break;
    
    case 'U':
    case 'u':
    //Cambiar el material asociado al objeto
        printf("Los mateeriales son los siguientes\n");
        printf("1.- Bronce\n");
        printf("2.- Plata\n");
        printf("3.- Oro\n");
        printf("4.- Obsidiana\n");
        printf("Introduce el numero correspondiente al material para cambiarlo\n");
        do {
            read = scanf("%d", &m);
            getchar();
            if(read!=1){
                printf("Vuelve a intentarlo\n");
            }
        }while(read!=1);

        if( m>=1 && m<5){
        _selected_object->matPTR = lMateriales[m-1];
        }else
        {
            printf("Ese material no esta disponible\n");
        }
        

        break;


    case 27: /* <ESC> */
        exit(0);
        break;

    case '0':
        //Siempre que no sea una las tres primeras luces reescribira o inicializara la luz seleccionada
        if(selectedLuz == lLuces[0] || selectedLuz == lLuces[1] || selectedLuz == lLuces[2] ){
            printf("Esta luz no puede ser modificada ni reescrita\n");
        }else{

            if(selectedLuz->inicializada == 1){
                printf("Se sobreescribira la luz seleccionada\n");
            }
            printf("Si desea crear un foco pulse 'f' si desea crear una bombilla pulse 'b\n");
            scanf(" %c", &c);
            getchar();
            if(c == 'f'){
                selectedLuz->tipo = 2;
                selectedLuz->inicializada = 1;
                glEnable(GL_LIGHT0+selectedLuz->num);
                selectedLuz->exponente = 0.0;

                printf("Introduce la posicion de la luz\n");
                do {
                    printf("1: "); 
                    read = scanf("%lf", &X); 
                    getchar(); 
                    if(read!=1) {
                        printf("Vuelve a intentarlo\n");
                    }
                } while(read!=1);

                do {
                    printf("2: "); 
                    read = scanf("%lf", &Y); 
                    getchar(); 
                    if (read!=1){ 
                        printf("Vuelve a intentarlo\n");
                    }
                } while(read!=1);

                do {
                    printf("3: "); 
                    read = scanf("%lf", &Z); 
                    getchar(); 
                    if (read!=1){
                        printf("Vuelve a intentarlo\n");
                    }
                } while(read!=1);

                selectedLuz->posicion[0]=X;
                selectedLuz->posicion[1]=Y;
                selectedLuz->posicion[2]=Z;

                printf("Introduce la direccion del foco\n");

                do {
                    printf("1: "); 
                    read = scanf("%lf", &X); 
                    getchar(); 
                    if (read!=1){
                        printf("Error en la lectura. Vuelve a introducirlo\n");
                    }
                } while(read!=1);

                do {
                    printf("2: "); 
                    read = scanf("%lf", &Y); getchar(); 
                    if (read!=1){
                        printf("Error en la lectura. Vuelve a introducirlo\n");
                    }
                } while(read!=1);

                do {
                    printf("3: "); 
                    read = scanf("%lf", &Z); 
                    getchar(); 
                    if (read!=1){ 
                        printf("Error en la lectura. Vuelve a introducirlo\n");
                    }
                } while(read!=1);
                
                selectedLuz->lDireccion[0]=X;
                selectedLuz->lDireccion[1]=Y;
                selectedLuz->lDireccion[2]=Z;
               

                selectedLuz->lAmbiental[0] = 0.8;
                selectedLuz->lAmbiental[1] = 0.8;
                selectedLuz->lAmbiental[2] = 0.8;
                selectedLuz->lAmbiental[3] = 1.0;

                selectedLuz->lDifusa[0] = 0.8;
                selectedLuz->lDifusa[1] = 0.8;
                selectedLuz->lDifusa[2] = 0.8;
                selectedLuz->lDifusa[3] = 1.0;

                selectedLuz->lEspecular[0] = 0.8;
                selectedLuz->lEspecular[1] = 0.8;
                selectedLuz->lEspecular[2] = 0.8;
                selectedLuz->lEspecular[3] = 1.0;

                selectedLuz->lAtenuacion[0] = 0.3;
                selectedLuz->lAtenuacion[1] = 0.2;
                selectedLuz->lAtenuacion[2] = 0.1;

                selectedLuz->corte = 20.0;
                

            }else if( c == 'b'){
                
                selectedLuz->inicializada = 1;
                glEnable(GL_LIGHT0+selectedLuz->num);
                selectedLuz->tipo = 1;
                selectedLuz->inicializada = 1;

                printf("Introduce la posicion de la luz\n");
                do {
                    printf("1: "); 
                    read = scanf("%lf", &X); 
                    getchar(); 
                    if (read!=1){ 
                            printf("Vuelve a intentarlo\n");
                    }
                } while(read!=1);
                do {printf("2: "); read = scanf("%lf", &Y); getchar(); if (read!=1) printf("Vuelve a intentarlo\n");} while(read!=1);
                do {printf("3: "); read = scanf("%lf", &Z); getchar(); if (read!=1) printf("Vuelve a intentarlo\n");} while(read!=1);
                selectedLuz->posicion[0]=X;
                selectedLuz->posicion[1]=Y;
                selectedLuz->posicion[2]=Z;

                selectedLuz->lAmbiental[0] = 0.75;
                selectedLuz->lAmbiental[1] = 0.75;
                selectedLuz->lAmbiental[2] = 0.75;
                selectedLuz->lAmbiental[3] = 1.0;


                selectedLuz->lEspecular[0] = 0.75;
                selectedLuz->lEspecular[1] = 0.75;
                selectedLuz->lEspecular[2] = 0.75;
                selectedLuz->lEspecular[3] = 1.0;

                selectedLuz->lDifusa[0] = 0.75;
                selectedLuz->lDifusa[1] = 0.75;
                selectedLuz->lDifusa[2] = 0.75;
                selectedLuz->lDifusa[3] = 1.0;

                selectedLuz->lAtenuacion[0] = 0.4;
                selectedLuz->lAtenuacion[1] = 0.3;
                selectedLuz->lAtenuacion[2] = 0.2;

            }else{
                printf("No se ha reconocido la tecla intentelo de nuevo \n");
            }
        }
        break;
    case '1': 
	case '2': 
	case '3': 
	case '4': 
	case '5': 
	case '6': 
	case '7':
	case '8':
    //Elegir la luz 

		selectedLuz = lLuces[atoi(&key)-1];
		printf("Has elegido la luz %d ",atoi(&key));
        if(selectedLuz->inicializada == 1){
            printf("Todo ha ido ido correctamente \n");
        }else{
            printf("Sin embargo todavia esta por inicializarse\n");
        }
        
		break;

        

    default:
        /*In the default case we just print the code of the key. This is usefull to define new cases*/
        printf("%d %c\n", key, key);
        break;
    }
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}

void SKeyboard( int key, int x, int y) {

	




//CONTROL DE TECLAS


    if(borrar==1){

    if(mCamara == 0 && mLuces == 0){

        
        //matriz que te deja visualizar el mundo
	    glMatrixMode(GL_MODELVIEW);

	    if (GLOBAL == 0) {

            //cargar matriz de objeto
		    glLoadMatrixd(_selected_object->mtrxPTR->mat);

	    } else {

            //trabajando en modo global, cargar matriz identidad
		    glLoadIdentity();
	    }
    

    //Switch transformaciones para los objetos
	switch (key){

        case GLUT_KEY_F1:
        case GLUT_KEY_F2:
        case GLUT_KEY_F3:
        case GLUT_KEY_F4:
        case GLUT_KEY_F5:
        case GLUT_KEY_F6:
        case GLUT_KEY_F7:
        case GLUT_KEY_F8:
            //Activar o desactivar la luz vinculada a la tecla
            encenderApagarLuz(key-1);
        break;

            //activar/desactivar luces
        case GLUT_KEY_F9:
            if(glIsEnabled(GL_LIGHTING)){
                //desactivar luces
                glDisable(GL_LIGHTING);
                printf("Luces desactivadas \n");
            } else {
                //activar luces
                glEnable(GL_LIGHTING);
                printf("Luces activaddas\n");

            }
        break;
        
        case GLUT_KEY_INSERT:
            //flat
            if(luzSmooth == 0){
                glShadeModel(GL_SMOOTH);
                luzSmooth = 1;
                printf("Iluminacion de tipo suave\n");
                glutPostRedisplay();


            } else { //smooth
                glShadeModel(GL_FLAT);
                luzSmooth = 0;
                printf("Iluminacion de tipo liso\n");
                

            }  
        break;



	   case GLUT_KEY_LEFT:


            if(modo == 0){
                
        	    glTranslated(-1, 0, 0);     

            } else if(modo == 1){

                glRotated(15, 0, -1, 0);
                    
            } else if (modo == 2) {

                glScaled(0.9 ,1, 1);

            }
        break;


        case GLUT_KEY_RIGHT:

        if(modo == 0){

            glTranslated(1, 0, 0);  

        } else if (modo == 1) {

            glRotated(15, 0, 1, 0);

        } else if (modo == 2) {

            glScaled(1.1 ,1 ,1);

        }
        break;


        case GLUT_KEY_UP:

        if(modo == 0){
           
        	glTranslated(0, 1 ,0);
            

        } else if (modo == 1) {
            
            glRotated(15, 1, 0, 0);
                    

        } else if (modo == 2) {

        	glScaled(1, 1.1, 1);    

        }
        break;


        case GLUT_KEY_DOWN:

            if(modo == 0){
                
                glTranslated(0, -1 ,0);
                
            } else if (modo == 1) {

                glRotated(15, -1, 0, 0);          

            } else if (modo == 2) {

                glScaled(1, 0.9 ,1);

            }
            break;


        case GLUT_KEY_F11:

            if (modo == 0) {

                glTranslated(0, 0 ,-1);
                
            } else if(modo == 1){

                glRotated(15, 0, 0, -1);

            } else if (modo == 2) {

                glScaled(1, 1, 0.9);
                
            }
            break;


        case GLUT_KEY_F12:

            if(modo == 0){

                glTranslated(0, 0 ,1);
                

            } else if(modo == 1){
        	
                printf("%s\n"," Rotando objeto" );
                glRotated(15, 0, 0, 1);
        
            } else if(modo == 2){

        	    glScaled(1, 1, 1.1);
            
            }
            break;
    }//FIN SWITCH OBJETOS

    if (GLOBAL==1)
    {
        glMultMatrixd(_selected_object->mtrxPTR->mat);	
    }
        lMatrix *newMatriz = (lMatrix *) malloc(sizeof (lMatrix));
        glGetDoublev(GL_MODELVIEW_MATRIX, newMatriz->mat);
        newMatriz->next = _selected_object->mtrxPTR;
        _selected_object->mtrxPTR = newMatriz;
    }//fin if
    
    else if(mCamara == 1 && mLuces == 0){
        
    //Switch para las transformaciones de la camara
    switch (key){

        case GLUT_KEY_F1:
        case GLUT_KEY_F2:
        case GLUT_KEY_F3:
        case GLUT_KEY_F4:
        case GLUT_KEY_F5:
        case GLUT_KEY_F6:
        case GLUT_KEY_F7:
        case GLUT_KEY_F8:
            encenderApagarLuz(key-1);
        break;

            //activar/desactivar luces
        case GLUT_KEY_F9:
            if(glIsEnabled(GL_LIGHTING)){
                //desactivar luces
                glDisable(GL_LIGHTING);
                printf("Luces desactivadas \n");
            } else {
                //activar luces
                glEnable(GL_LIGHTING);
                printf("Luces activaddas\n");

            }
        break;
        
        case GLUT_KEY_INSERT:
            //flat
            if(luzSmooth == 0){
                glShadeModel(GL_SMOOTH);
                luzSmooth = 1;
                printf("Iluminacion de tipo suave\n");
                glutPostRedisplay();


            } else { //smooth
                glShadeModel(GL_FLAT);
                luzSmooth = 0;
                printf("Iluminacion de tipo liso\n");
                

            }  
        break;
       

        case GLUT_KEY_LEFT:

            if(modo == 0){

                if(vuelo == 1 ){

                    volar(-selectedCamara->imcsr[0], -selectedCamara->imcsr[1],-selectedCamara->imcsr[2] );

                } else{
                    printf("No se pueden trasladar en el eje X en analisis\n");
                }
            } else if(modo == 1){
            
                if(vuelo == 1){
                    rotacionLibre(-15.0, selectedCamara->imcsr[4], selectedCamara->imcsr[5],selectedCamara->imcsr[6]);
                } else{

                    rotacionLigada(-15.0, selectedCamara->imcsr[4], selectedCamara->imcsr[5],selectedCamara->imcsr[6]);
                
                }

            } else{

                if(selectedCamara->params[0] > 0.2){
                    selectedCamara->params[0] -= 0.1;
                }else{
                    printf("el volumen no puede reducirse mas\n");
                }

        }
        break;
    
        case GLUT_KEY_RIGHT:

            if(modo == 0){

                if(vuelo == 1 ){

                    volar(selectedCamara->imcsr[0], selectedCamara->imcsr[1],selectedCamara->imcsr[2] );

                } else{
                    printf("No se pueden trasladar en el eje X en analisis\n");
                }
            } else if(modo == 1){
                
                if(vuelo == 1){
                    rotacionLibre(15.0, selectedCamara->imcsr[4], selectedCamara->imcsr[5],selectedCamara->imcsr[6]);
                } else{
                    rotacionLigada(15.0, selectedCamara->imcsr[4], selectedCamara->imcsr[5],selectedCamara->imcsr[6]);
                    
                }

            } else{
                    selectedCamara->params[0] += 0.1;
                

            }
        break;


        case GLUT_KEY_UP:

            if(modo == 0){

                if(vuelo == 1 ){

                    volar( selectedCamara->imcsr[4], selectedCamara->imcsr[5], selectedCamara->imcsr[6] );

                } else{
                    printf("No se pueden trasladar en el eje Y en analisis\n");
                }
            } else if(modo == 1){
                
                if(vuelo == 1){
                    rotacionLibre(15.0, selectedCamara->imcsr[0], selectedCamara->imcsr[1],selectedCamara->imcsr[2]);
                } else{
                    rotacionLigada(15.0, selectedCamara->imcsr[0], selectedCamara->imcsr[1],selectedCamara->imcsr[2]);
                    
                }

            } else{

                    selectedCamara->params[1] += 0.1;
                

            }
        break;
    
        case GLUT_KEY_DOWN:

            if(modo == 0){

                if(vuelo == 1 ){

                    volar( -selectedCamara->imcsr[4], -selectedCamara->imcsr[5], -selectedCamara->imcsr[6] );

                } else{
                    printf("No se pueden trasladar en el eje X en analisis\n");
                }
            } else if(modo == 1){
                
                if(vuelo == 1){
                    rotacionLibre(-15.0, selectedCamara->imcsr[0], selectedCamara->imcsr[1],selectedCamara->imcsr[2]);
                }else{
                    rotacionLigada(-15.0, selectedCamara->imcsr[0], selectedCamara->imcsr[1],selectedCamara->imcsr[2]);
                    
                }

            } else{
                    if(selectedCamara->params[1] > 0.2){
                    selectedCamara->params[1] -= 0.1;
                    }else{
                    printf("el volumen no puede reducirse mas\n");
                }
                

        }
        break;

        case GLUT_KEY_F11:

            if(modo == 0){

                volar( -selectedCamara->imcsr[8], -selectedCamara->imcsr[9], -selectedCamara->imcsr[10] );

                
            } else if(modo == 1){
                
                if(vuelo == 1){
                    rotacionLibre(15.0, selectedCamara->imcsr[8], selectedCamara->imcsr[9],selectedCamara->imcsr[10]);
                } else{
                    rotacionLigada(15.0, selectedCamara->imcsr[8], selectedCamara->imcsr[9],selectedCamara->imcsr[10]);
                    
                }

            } else{

                    selectedCamara->params[2] += 0.1;
                    selectedCamara->params[3] += 0.1;
                

            }
        break;
    
        case GLUT_KEY_F12:

            if(modo == 0){

                volar( selectedCamara->imcsr[8], selectedCamara->imcsr[9], selectedCamara->imcsr[10] );

                
            } else if(modo == 1){
                
                if(vuelo == 1){
                    rotacionLibre(-15.0, selectedCamara->imcsr[8], selectedCamara->imcsr[9],selectedCamara->imcsr[10]);
                } else{
                    rotacionLigada(-15.0, selectedCamara->imcsr[8], selectedCamara->imcsr[9],selectedCamara->imcsr[10]);
                    
                }

            } else{
                    if(selectedCamara->params[2] > 0.2){
                    selectedCamara->params[2] -= 0.1;
                    }else{
                    printf("el volumen no puede reducirse mas\n");
                }
                

            }
        break;
    
    }//fin switch

        

    }// fin else if de la camara
    else {
        //Luces

        //Switch de las transformaciones de las luces
        switch (key)
        {

        case GLUT_KEY_F1:
        case GLUT_KEY_F2:
        case GLUT_KEY_F3:
        case GLUT_KEY_F4:
        case GLUT_KEY_F5:
        case GLUT_KEY_F6:
        case GLUT_KEY_F7:
        case GLUT_KEY_F8:
            encenderApagarLuz(key-1);
        break;

            //activar/desactivar luces
        case GLUT_KEY_F9:
            if(glIsEnabled(GL_LIGHTING)){
                //desactivar luces
                glDisable(GL_LIGHTING);
                printf("Luces desactivadas \n");
            } else {
                //activar luces
                glEnable(GL_LIGHTING);
                printf("Luces activaddas\n");

            }
        break;
        
        case GLUT_KEY_INSERT:
            //flat
            if(luzSmooth == 0){
                glShadeModel(GL_SMOOTH);
                luzSmooth = 1;
                printf("Iluminacion de tipo suave\n");
                glutPostRedisplay();


            } else { //smooth
                glShadeModel(GL_FLAT);
                luzSmooth = 0;
                printf("Iluminacion de tipo liso\n");
                

            }  
        break;
        
        case GLUT_KEY_LEFT:
            if(modo == 0){ //traslacion
                lTrasladar('x', '-');

            } else { //rotacion
                lRotar('y', -15);
            }
            break;
        
        case GLUT_KEY_RIGHT:
            if(modo == 0){ //traslacion
                lTrasladar('x', '+');

            } else { //rotacion
                lRotar('y', 15);
            }
            break;
        case GLUT_KEY_UP:
            if(modo == 0){ //traslacion
                lTrasladar('y', '+');

            } else { //rotacion
                lRotar('x', 15);

            }
            break;
        
        case GLUT_KEY_DOWN:
            if(modo == 0){ //traslacion
                lTrasladar('y', '-');

            } else { //rotacion
                lRotar('-', -15);
            }
            break;

        case GLUT_KEY_F11:
            if(modo == 0){ //traslacion
                lTrasladar('z', '-');

            } else { //rotacion
                lRotar('z', -15);


            }
            break;
        
        case GLUT_KEY_F12:
            if(modo == 0){ //traslacion
                lTrasladar('z', '+');

            } else { //rotacion
                lRotar('z', 15);
            }
            break;
        
        }
       
    }//fin switch
    
    actualizarMCSR();
    glutPostRedisplay();
    }else{
        printf("Carga un objeto primero \n");
    }
        

}//FIN TECLADO ESPECIAL



