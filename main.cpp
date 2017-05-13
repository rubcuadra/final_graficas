//
//  main.cpp
//  finalMoi
//
//  Created by Ruben Cuadra on 5/11/17.
//  Copyright © 2017 Ruben Cuadra. All rights reserved.
//

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include "loader/glm.h"
#include "texturizer.h"

#include <stdlib.h>
#include <png.h>
#include <iostream>

GLfloat color_material1[4] = {1,0,0,0};
GLfloat color_material2[4] = {0,1,0,0};

GLfloat ambient[] =  {0.05, 0.05, 0, 1}; //Amarillo Leve
GLfloat position[] = {3.0, 3.0, 5.0, 1.0};
GLfloat mat_diffuse[] ={0.3, 0.3, 0.3, 1.0};
GLfloat mat_specular[] ={0.0, 0.0, 0.0, 1.0};
GLfloat mat_shininess[] = {0.0};

GLfloat mat_cili[] ={0.0, 1.0, 1.0, 0.85};
GLfloat mat_asta[] ={0.8, 0.5, 0.2, 1};
GLfloat mat_bandera[] ={1, 1, 1, 1};

GLfloat angle_x = 0.0;
GLfloat angle_y = 0.0;
GLfloat zoom_z = 1;
float offset = 0.0;
int down = 0, lastx, lasty;

GLMmodel *pmodel = NULL;    /* the loaded model */
Texturizer * texture;

GLfloat ctrlpoints[4][4][3] =
{
    {
        {-1.5, -1.5, 0.0},
        {-0.5, -1.5, 1},
        {0.5, -1.5, 0.0},
        {1.5, -1.5, 0.5}},
    {
        {-1.5, -0.5, 0.0},
        {-0.5, -0.5, 1},
        {0.5, -0.5, 0.0},
        {1.5, -0.5, 0.5}},
    {
        {-1.5, 0.5, 0.0},
        {-0.5, 0.5, 1},
        {0.5, 0.5, 0.0},
        {1.5, 0.5, 0.5}},
    {
        {-1.5, 1.5, 0.0},
        {-0.5, 1.5, 1},
        {0.5, 1.5, 0.0},
        {1.5, 1.5, 0.5}}
};

GLfloat texpts[2][2][2] = {{{0.0, 0.0}, {0.0, 1.0}},{{1.0, 0.0}, {1.0, 1.0}}};

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (45.0, (GLdouble)w/(GLdouble)h, 3.0, 8.0);
    glMatrixMode(GL_MODELVIEW);
}

void menu(int value)
{
    switch (value) {
        case 0:
            zoom_z += 0.1;
            break;
        case 1:
            zoom_z -= 0.1;
            break;
        case 2:
            offset += 0.1;
            break;
        case 3:
            offset -= 0.1;
            break;
    }
    glutPostRedisplay();
}

/* ARGSUSED1 */
void
motion(int x, int y)
{
    if (down)
    {
        angle_x = lastx - x;
        angle_y = lasty - y;
        //glRotatef(lastx - x, 0, 1, 0);
        //lastx = x;
        glutPostRedisplay();
    }
}

/* ARGSUSED3 */
void
mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            lastx = x;
            down = 1;
        }
        else
        {
            down = 0;
        }
    }
}

void initlights(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
        glRotatef(angle_x, 0, 1, 0);
        glRotatef(angle_y, 1, 0, 0);
        glScalef(zoom_z, zoom_z, zoom_z);
    
        glPushMatrix(); //Pintar Cilindro
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_cili ); //Color del material
            glTranslatef(0,0.9,0);
            glRotatef(90,1,0,0);
            GLUquadricObj* quadric = gluNewQuadric();
            gluQuadricNormals(quadric, GLU_SMOOTH);
            gluCylinder(quadric, 0.5, 0.5, 1, 15, 15);
        glPopMatrix();

        glDisable(GL_BLEND);
            glPushMatrix(); //Pintar mesa
                glScalef(1.5,1.5,1.5); 
                glmDraw( pmodel , GLM_SMOOTH | GLM_MATERIAL);
            glPopMatrix();

            glPushMatrix();
                glTranslatef(0,0.65,0);
                glMaterialfv(GL_FRONT, GL_AMBIENT, mat_asta ); //Color del material
                glScalef(0.25,1.5,0.25);
                glutSolidCube(1);
            glPopMatrix();

            glEnable(GL_TEXTURE_2D);
                glPushMatrix();
                    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_bandera ); //Color del material
                    glTranslatef(0.65,1.75,0);
                    glScalef(0.5,0.3,1);
                    glEvalMesh2(GL_FILL, 0, 25, 0, 25);
                glPopMatrix();
            glDisable(GL_TEXTURE_2D);

        glEnable (GL_BLEND);
            
    glPopMatrix();
    
    glutSwapBuffers();        //End
}

void myinit(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_AUTO_NORMAL);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glDepthMask (GL_FALSE);
    
    glEnable(GL_NORMALIZE);
    glEnable(GL_MAP2_TEXTURE_COORD_2);
    
    texture = new Texturizer("assets/mexico.png",true);
    initlights();       /* for lighted version only */
    //OBJ
    pmodel = glmReadOBJ( "assets/3d-model.obj" );   
    if (!pmodel)
    {
        printf("\nNo se pudo cargar el obj\n");
        exit(0);
    }
    glmUnitize(pmodel);
    glmVertexNormals(pmodel, 90.0, GL_TRUE);

    //puntos de control para la bandera
    glMapGrid2f(25, 0.0, 1.0, 25, 0.0, 1.0);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
            0, 1, 12, 4, &ctrlpoints[0][0][0]);
    //Textura para bandera
    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts[0][0][0]);

}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-4.0, 4.0, -4.0 * (GLfloat) h / (GLfloat) w,
                4.0 * (GLfloat) h / (GLfloat) w, -4.0, 4.0);
    else
        glOrtho(-4.0 * (GLfloat) w / (GLfloat) h,
                4.0 * (GLfloat) w / (GLfloat) h, -4.0, 4.0, -4.0, 4.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyDown(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'm':
            break;
        case 27:
            exit(0);
        default:
            break;
    }
}

void respuestas()
{
    //1)Menciona: a) ¿en qué año y dónde se inventa el 1er monitor?; b) ¿en qué año y cuál es el nombre de la empresa que hace la 1a versión de OpenGL (de IRIS GL)?; fue la empresa que más apoyó la industria de las gráficas por computadora en los 80s y 90s. (4 pts: 2c/u)
    std::cout<<"a) MIT 1950 b) Silicon Graphics\n";
    //2)¿Qué significan las siglas: HMD, LCD, GPU, OpenGL ARB, GLUT? (5 pts: 1 c/u)
    std::cout<<"Head Mounted Display,Liquid Cristal Display,Graphical Process Unit, OpenGL Architecture Review Board,,openGL Utility Toolkit\n";
    //3)Describe la forma 3D del espacio de color HSI, indicando dónde están los colores R, G, B, el negro, el blanco, el H, S e I. (6 pts)
    std::cout<<"a) El modelo HSI se suele representar como un cilindro que se basa en el cubo RGB, el color blanco se encuentra en la tapa superior mientras el negro en la tapa inferior; Se pinta un triangulo sobre una tapa, donde todos los vertices del triangulo tocan con la circunferencia de la tapa, en cada uno de estos puntos se pone un color, el inferior izquierdo es rojo, inferior derecho verde y el superior azul; Hue es el angulo sobre el centro de la tapa ,Saturation es la distancia desde el centro de la circunferencia and Lightness se encuentra en la altura del cilindro(Razon por la que es negro y blanco)\n";
    //4)¿Qué es un cuerpo rígido y cuáles son las 3 transformaciones homogéneas que más se usan para ese caso? (5 pts)
    std::cout<<"a) Figuras estaticas que no poseen movimiento alguno pero se puede simular al usar transformaciones, las cuales suelen ser Rotacion, Traslacion y Escalacion\n";
    //5)Menciona: a) 6 características importantes de las tarjetas de video; b) ¿qué es y para qué sirve el background process? y menciona con qué funciones de OpenGL se implementa (12 pts: 6c/u)
    std::cout<<"GPU, memoria de video, BIOS, RAM, Bandwitch de los  buses, drivers (marca/Soporte OpenGL)\n";
    std::cout<<"El background process nos ayuda a hacer simulaciones y es una funcion idle la cual se manda a llamar cuando no hay interaccion con la escena, sease clicks de teclado, movimientos del mouse,etc\n";
    //6)¿Cuáles son los 4 sistemas de coordenadas o de referencia que se manejan en gráficas por computadora para mapear una textura en una superficie (tipo Bezier) para que se vea bien la geometría y se despliegue en la ventana de GLUT?, indica para cada caso correctamente las letras en los ejes de cada uno (8 pts: 2c/u)
    std::cout<<"Parametrico(v,u), Textura(t,s) , Mundo(x,y,z), Ventana(xs,ys)\n";
    //7)Revisa los siguientes códigos de WebGL y explica cada línea de código con comentario * en cada inciso; también indica cómo se llama el shader de a) y el de b). (10 pts: 1c/u)
    
    //a) voidmain()
    //{
    //    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); //Estamos asignandole el color rojo sin transparencia al objeto
    //}
    //attribute vec4 a_Position;  //Aqui se esta diciendo que el programa tendra una variable de tipo vec4(Vector de 4 dimensiones) y se llamara a_Position
    //uniform mat4 u_ModelMatrix; //Decimos que el programa tendra una variable tipo mat4(Matriz de 4 dimensiones) con nombre u_ModelMatrix
    //void main()
    //{
    //  gl_Position = u_ModelMatrix * a_Position; //Se le esta asignando una nueva posicion al objeto, esto es el resultado de multiplicar el vector a_Position y la matriz u_ModelMatrix
    //}
    //c) gl.drawArrays(gl.TRIANGLES, 0, n);                                 //Pinta n triangulos en las coordendas que ya se pasaron al buffer
    //d) gl.uniformMatrix4fv(u_ModelMatrix, false, modelMatrix.elements);   //Hace un binding, le pasa lo que alla en modelMatrix.elements a un shader que tiene enlace con la variable u_ModelMatrix (Suponemos que se llama de la misma manera en el programa)
    //e) gl.clear(gl.COLOR_BUFFER_BIT);                                     //Limpia la pantalla(Canvas)
    //f) gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);          //Aqui es donde se le pasa un arreglo de vertices(Triangulos) al buffer y se le dice que usaremos un STATIC_DRAW
    std::cout<<"Los comentarios del codigo esta en los comentarios de ESTE codigo, a)Fragment Shader b) Vertex Shader \n";
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow(argv[0]);
    respuestas();
    myinit();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutCreateMenu(menu);
    glutAddMenuEntry("Zoom In", 1);
    glutAddMenuEntry("Zoom Out", 0);
    glutAddMenuEntry("Separar", 2);
    glutAddMenuEntry("Unir", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyDown);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}

