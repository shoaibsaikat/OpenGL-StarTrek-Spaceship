/*--------------------------------------------------------------------
Name: Mina Shoaib Rahman Saikat
Roll: 0705096
Sec : B
CSE, BUET
---------------------------------------------------------------------*/
/*--------------------------------------------------------------------
Controls:
Left, right, up, down arrow for camera movement
x, X, y, Y, z, Z for spaceship movement
' '(space) for showing or hiding structure
---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define BLACK		0, 0, 0
#define PI			3.14159265
#define DISPHEIGHT	800
#define DISPWIDTH	600

bool isWire = false;
float transX, transY, transZ;
float theta, phi;


/*--------------------------------------------------------------------
draws the x, y and z axis
--------------------------------------------------------------------*/
void drawAxis() {
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	//Y axis
	glVertex3f(0, -150, 0);
	glVertex3f(0, 150, 0);

	//X axis
	glColor3f(0, 0, 1);
	glVertex3f(-150, 0, 0);
	glVertex3f(150, 0, 0);

	//Z axis
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, -150);
	glVertex3f(0, 0, 150);
	glEnd();
}

/*--------------------------------------------------------------------
up for upper circle radius, down for lower circle radius
length for length, N for how many points(i.e. the smoothness)
draws only the outer side of a sylinder, front and back circle
width can be different
--------------------------------------------------------------------*/
void r3RingCircle(float up, float down, float length, int N) {
	float angle;
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= N; ++i) {
		angle = 2 * PI * i / N;

		glVertex3f(cos(angle) * up, sin(angle) * up, 0);
		glVertex3f(cos(angle) * down, sin(angle) * down, length);
	}
	glEnd();
}

/*--------------------------------------------------------------------
x,y,z is for coordinates
up for upper circle radius, down for lower circle radius
length for length, N for how many points(i.e. the smoothness)
draws only the outer side of a sylinder, front and back circle
width can be different
NB. this function has x,y,z which is redundant, we can have
this effect by fltranslatef function, but I didn't remove it
cause, it will make me do some extra work
--------------------------------------------------------------------*/
void r3RingCircle(float x, float y, float z, float up, float down, float width, int N) {
	float angle;
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= N; ++i) {
		angle = 2 * PI * i / N;

		glVertex3f(x + cos(angle) * up, y + sin(angle) * up, z);
		glVertex3f(x + cos(angle) * down, y + sin(angle) * down, z + width);
	}
	glEnd();
}

/*--------------------------------------------------------------------
x,y,z are the coordinates
R is the radius, N is the # of points(i.e. the smoothness)
draws a cone
--------------------------------------------------------------------*/
void r3Cone(float x, float y, float z, float rad, float width, int N) {
	float angle;
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= N; ++i) {
		angle = 2 * PI * i / N;

		glVertex3f(x, y, z);
		glVertex3f(x + cos(angle) * rad, y + sin(angle) * rad, z + width);
	}
	glEnd();
}

/*--------------------------------------------------------------------
x,y,z are the coordinates
R is the radius, N is the # of points(i.e. the smoothness)
draws a sphere
--------------------------------------------------------------------*/
void drawHemisphere(float x, float y, float z, float R, int N) {
	// Array of latitudinal triangle strips, each parallel to the equator, stacked one
	// above the other from the equator to the north pole.
	float p, q;
	p = q = N;
	for (int j = 0; j < q; j++) {
		// One latitudinal triangle strip.
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i <= p; i++) {
			glVertex3f(x + R * cos((float)(j + 1) / q * PI / 2.0) * cos(2.0 * (float)i / p * PI),
				y + R * cos((float)(j + 1) / q * PI / 2.0) * sin(2.0 * (float)i / p * PI),
				z + R * sin((float)(j + 1) / q * PI / 2.0));
			glVertex3f(x + R * cos((float)j / q * PI / 2.0) * cos(2.0 * (float)i / p * PI),
				y + R * cos((float)j / q * PI / 2.0) * sin(2.0 * (float)i / p * PI),
				z + R * sin((float)j / q * PI / 2.0));
		}
		glEnd();
	}
}

/*--------------------------------------------------------------------
l for length, w for width and H for height
lower rectangle is smaller than upper
--------------------------------------------------------------------*/
void drawMainPiller(float l, float w, float H) {
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(l * 10, 0, 0);
	glVertex3f(l * 10, w * 5, 0);
	glVertex3f(0, w * 5, 0);

	glVertex3f(l * 10, 0, H);
	glVertex3f(l * 30, 0, H);
	glVertex3f(l * 30, w * 5, H);
	glVertex3f(l * 10, w * 5, H);

	glVertex3f(0, w * 5, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(l * 10, 0, H);
	glVertex3f(l * 10, w * 5, H);

	glVertex3f(l * 10, w * 5, 0);
	glVertex3f(l * 10, 0, 0);
	glVertex3f(l * 30, 0, H);
	glVertex3f(l * 30, w * 5, H);

	glVertex3f(0, 0, 0);
	glVertex3f(l * 10, 0, 0);
	glVertex3f(l * 30, 0, H);
	glVertex3f(l * 10, 0, H);

	glVertex3f(0, w * 5, 0);
	glVertex3f(l * 10, w * 5, 0);
	glVertex3f(l * 30, w * 5, H);
	glVertex3f(l * 10, w * 5, H);
	glEnd();
}

/*--------------------------------------------------------------------
l for length, w for width and H for height
lower and upper rectangles are of same size
--------------------------------------------------------------------*/
void drawSidePiller(float l, float w, float H) {
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(l * 5, 0, 0);
	glVertex3f(l * 5, w * 5, 0);
	glVertex3f(0, w * 5, 0);

	glVertex3f(l * 50, 0, H);
	glVertex3f(l * 55, 0, H);
	glVertex3f(l * 55, w * 5, H);
	glVertex3f(l * 50, w * 5, H);

	glVertex3f(0, w * 5, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(l * 50, 0, H);
	glVertex3f(l * 50, w * 5, H);

	glVertex3f(l * 5, w * 5, 0);
	glVertex3f(l * 5, 0, 0);
	glVertex3f(l * 55, 0, H);
	glVertex3f(l * 55, w * 5, H);

	glVertex3f(0, 0, 0);
	glVertex3f(l * 5, 0, 0);
	glVertex3f(l * 55, 0, H);
	glVertex3f(l * 50, 0, H);

	glVertex3f(0, w * 5, 0);
	glVertex3f(l * 5, w * 5, 0);
	glVertex3f(l * 55, w * 5, H);
	glVertex3f(l * 50, w * 5, H);
	glEnd();
}

void display() {
	glClearColor(BLACK, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		100 * cos(phi) * cos(theta),
		100 * cos(phi) * sin(theta),
		100 * (-sin(phi)),
		0, 0, 0, 0, 0, 1);

	if (!isWire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	drawAxis();

	glTranslatef(transX, transY, transZ);

	//head
	glPushMatrix();
	glTranslatef(0, 0, 20);
	glColor3f(.8, .2, .7);
	r3RingCircle(0, 45, 0, 12.5, 13, 3, 40);
	glColor3f(.4, .9, .4);
	r3Cone(0, 45, -3, 12.5, 3, 40);
	glColor3f(.2, .5, .7);
	r3Cone(0, 45, 6, 13, -3, 40);
	glColor3f(0.8, 0.3, 0);
	drawHemisphere(0, 45, 5.6, 2, 20);
	glColor3f(0.8, 0.3, 0);
	drawHemisphere(0, 45, -2.6, -1.2, 20);
	glPopMatrix();

	//main bar
	glPushMatrix();
	glTranslatef(2, 30.5, 0);
	glRotatef(90, 0, 0, 1);
	drawMainPiller(.3, .7, 20.5);
	glPopMatrix();

	//main sylinder
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 3, 10);
	glColor3f(1, 0, 0);
	drawHemisphere(0, 0, 24.7, 4, 20);
	glColor3f(.5, 0, 1);
	r3RingCircle(2, 4, 25, 20);
	glPopMatrix();

	//side bars
	glPushMatrix();
	glTranslatef(.5, 12, 0);
	glColor3f(.59, .8, 0);
	drawSidePiller(.3, 1.2, 35);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(.5, 18, 0);
	drawSidePiller(-.3, -1.2, 35);
	glPopMatrix();

	//side sylinders
	glPushMatrix();
	glTranslatef(17, -7, 35);
	glRotatef(-90, 1, 0, 0);
	glColor3f(1, 0, 0);
	drawHemisphere(0, 0, 24.7, 2, 15);
	glColor3f(0, .8, .59);
	r3RingCircle(2, 2, 25, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-17, -7, 35);
	glRotatef(-90, 1, 0, 0);
	glColor3f(1, 0, 0);
	drawHemisphere(0, 0, 24.7, 2, 15);
	glColor3f(0, .8, .59);
	r3RingCircle(2, 2, 25, 20);
	glPopMatrix();

	glutSwapBuffers();
}

void animate() {
	glutPostRedisplay();
}

void init(float h, float w) {
	theta = phi = 0;
	transX = transY = transZ = 0;

	glClearColor(BLACK, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, h / w, 0.1, 10000.0);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case ' ':
		isWire = !isWire;
		break;

		//axis movement
	case 'x':
		transX += 2;
		break;
	case 'X':
		transX -= 2;
		break;
	case 'y':
		transY += 2;
		break;
	case 'Y':
		transY -= 2;
		break;
	case 'z':
		transZ += 2;
		break;
	case 'Z':
		transZ -= 2;
		break;

	default:
		break;
	}
}

void specialKeyListener(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:
		phi -= .1;
		break;
	case GLUT_KEY_UP:
		phi += .1;
		break;

	case GLUT_KEY_RIGHT:
		theta += .1;
		break;
	case GLUT_KEY_LEFT:
		theta -= .1;
		break;
	default:
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(DISPHEIGHT, DISPWIDTH);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Star Trek Spaceship");
	printf("Press space to hide wires\n");

	init(DISPHEIGHT, DISPWIDTH);
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(display);
	glutIdleFunc(animate);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyListener);
	glutMainLoop();

	return 0;
}
