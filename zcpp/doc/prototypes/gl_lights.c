// A simple demonstration of OpenGL lighting
// g++ gl_lights.c -framework OpenGL -framework GLUT -o test && ./test

#include <iostream.h>
#include <iomanip.h>
#include <GLUT/glut.h> // #include <gl/glut.h>

// Set the initial size of the window, in pixels.
// These values may be changed by the resize_window() callback.
// They are used by the mouse_movement() callback.
int window_width = 900;
int window_height = 600;

// Vectors for colours and properties of materials.
const GLfloat red[] = { 0.8f, 0.0, 0.0, 1.0 };
const GLfloat blue[] = { 0.0, 0.2f, 1.0, 1.0 };
const GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat polished[] = { 100.0 };
const GLfloat dull[] = { 0.0 };

// Position the light at the origin.
const GLfloat light_pos[] = { 0.0, 0.0, 0.0, 1.0 };

// Translation values for light.
GLfloat move_x = 0.0;
GLfloat move_y = 0.0;
GLfloat move_z = 0.0;

// Attenuation factors for light.
GLfloat const_att = 1.0;

void display () {
	// Display callback function.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Move model into view
	glTranslatef(0.0, 0.0, -20.0);

	// Position the light and show where it is
	glPushMatrix();
		glTranslatef(move_x, move_y, move_z);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, const_att);
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
		glDisable(GL_LIGHTING);
		glColor3d(0.9, 0.9, 0.5);
		glutSolidSphere(0.1, 10, 10);
		glEnable(GL_LIGHTING);
	glPopMatrix();

	// Display a dull, red sphere on the left
	glTranslatef(-5.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glMaterialfv(GL_FRONT, GL_SHININESS, dull);
	glutSolidSphere(3.0, 40, 40);

	// Display a bright, blue sphere on the right
	glTranslatef(10.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, polished);
	glutSolidSphere(3.0, 40, 40);

	glutSwapBuffers();
	glFlush();
}

void mouse_movement (int x, int y) {

	// Moving moouse right moves light ... right.
  move_x = - 5.0 + (10.0 * x) / window_width;

	// Moving mouse up moves light upwards.
	move_y = 5.0 - (10.0 * y) / window_height;
	glutPostRedisplay();
}

void resize_window (int w, int h) {
	// Allow window to be resized without losing the model.
	window_width = w;
	window_height = h;

	// Use entire window to display the model.
	glViewport(0, 0, window_width, window_height);

	// Adjust aspect-ratio to match new window dimensions.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35.0, GLfloat(window_width)/GLfloat(window_height), 1.0, 50.0);
	glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y) {
	switch (key) {
	case '<':
		const_att *= 0.9f;
		break;
	case '>':
		const_att /= 0.9f;
		break;
	case 'n': // near
	  move_z = move_z + 1.0;
    break;
  case 'f': // far
    move_z = move_z - 1.0;
    break;
	if (key == 27)
		exit(0);
	}
	cout << "CA " << setprecision(3) << const_att << endl;
	glutPostRedisplay();

}

int main (int argc, char *argv[]) {

	// Initialize OpenGL and enter loop.
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Simple Lighting Demonstration");

	// Initialize the light.
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	// Request hidden surface elimination and register callbacks.
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(display);
	glutMotionFunc(mouse_movement);
	glutReshapeFunc(resize_window);
	glutKeyboardFunc(keyboard);

	// Provide user assistance and start GLUT.
	cout <<
		"Move mouse up (down) to move light up (down)." << endl <<
		"Move mouse right (left) to move light towards (away from) you." << endl <<
		"Press ESC to exit." << endl;

	glutMainLoop();
	return 0;
}
