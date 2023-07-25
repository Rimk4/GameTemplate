#include "menusystem.h"
#include <glut.h>




int main(int argc, char** argv)
{

    glutInit(&argc, argv);

    initMenu();

    glClearColor(0, 0, 0, 1);
    glutMainLoop();


	return 0;
}