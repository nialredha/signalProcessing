// C program to demonstrate drawing a circle using OpenGL

#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

// function to initialize 
void myInit(void)
{
	// making background color black
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// making picture green (in RGB mode)
	glColor3f(0.0, 1.0, 0.0);

	// breadth of picture boundary is 1 pixel
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// setting window dimension in X- and Y- direction
	gluOrtho2D(-780, 780, -420, 420);
}

// function to display
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);

	float x, y, i;

	// iterate y up to 2PI incrementing angle
	for (i = 0; i<(2*M_PI); i += 0.001)
	{
		x = 200 * cos(i);
		y = 200 * sin(i);

		glVertex2i(x,y);
	}

	glEnd();
	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	// giving window size in X- and Y- direction
	glutInitWindowSize(1366, 768);
	glutInitWindowPosition(0, 0);

	// giving name to window
	glutCreateWindow("Circle");
	myInit();

	glutDisplayFunc(display);
	glutMainLoop();
}
