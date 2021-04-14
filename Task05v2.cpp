#include <GL/glut.h>
#include <math.h>

#define PI 3.1415927

static GLfloat angle_x = 0.0, angle_y = 0.0;
static int current_color = 0, current_x = 0, current_y = 0;

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void draw_cyl(int slices, int stacks, float radius, float height)
{
	double slice = height / slices;
	double stack = 2.0 * PI / stacks;

	for (int i = 0; i < slices; ++i)
	{
		GLfloat z0 = 0.5 * height - i * slice;
		GLfloat z1 = z0 - slice;

		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= stacks; ++j)
		{
			double a = j * stack;
			GLfloat x = radius * cos(a);
			GLfloat y = radius * sin(a);

			glNormal3f(x / radius, y / radius, 0.0);
			glTexCoord2f(j / (GLfloat) stacks, i / (GLfloat) slices);
			glVertex3f(x, y, z0);

			glNormal3f(x / radius, y / radius, 0.0);
			glTexCoord2f(j / (GLfloat) stacks, (i + 1) / (GLfloat) slices);
			glVertex3f(x, y, z1);
		}
		glEnd();
	}
}

void rotate(void)
{
	angle_x += 1.0;
	if (angle_x > 360.0) {
		angle_x -= 360.0;
	}
	glutPostRedisplay();
}

void change_color(void)
{
	switch (current_color) {
		case 0:
		case 1:
		case 2:
			current_color++;
			break;
		case 3:
			current_color = 0;
			break;
		default: 
			break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'A':
		case 'a':
			rotate();
			break;
		case 'C':
		case 'c':
			change_color();
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
	}
}

void motion(int x, int y)
{
	if (x > current_x) {
		angle_x += 1.0;
		if (angle_x > 360.0) {
			angle_x -= 360.0;
		}
		glutPostRedisplay();
		current_x = x;
	}

	if (x < current_x) {
		angle_x -= 1.0;
		if (angle_x < 0.0) {
			angle_x += 360.0;
		}
		glutPostRedisplay();
		current_x = x;
	}

	if (y > current_y) {
		angle_y += 1.0;
		if (angle_y > 360.0) {
			angle_y -= 360.0;
		}
		glutPostRedisplay();
		current_y = y;
	}

	if (y < current_y) {
		angle_y -= 1.0;
		if (angle_y < 0.0) {
			angle_y += 360.0;
		}
		glutPostRedisplay();
		current_y = y;
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (current_color) {
		case 0:
			glColor3f(1.0, 0.0, 0.0);
			break;
		case 1:
			glColor3f(0.0, 1.0, 0.0);
			break;
		case 2:
			glColor3f(0.0, 0.0, 1.0);
			break;
		case 3: 
			glColor3f(1.0, 1.0, 1.0);
			break;
		default:
			break;
	}

	glLoadIdentity();
		glTranslatef(0.0, 0.0, -3.0);
		glRotatef(angle_x, 0.0, 1.0, 0.0);
		glRotatef(angle_y, 1.0, 0.0, 0.0);
		draw_cyl(32, 32, 0.5, 1.0);
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.5, 20.0);

	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Cylinder 3D Render");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);

	glutMainLoop();
	return 0;
}