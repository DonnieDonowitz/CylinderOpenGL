#include <GL/glut.h>
#include <stdio.h>

#define CYLINDER_RADIUS 1.0					//Raggio del cilindro.
#define CYLINDER_HEIGHT 2.0					//Altezza del cilindro.
#define CYLINDER_STACKS 15.0				//Numero di suddivisioni intorno all'asse z.
#define CYLINDER_SLICES 32.0				//Numero di suddivisioni lungo l'asse z.

static float my_x = 0.0, my_y = 0.0;                                    // my_x e my_y rappresentano le coordinate attuali del punto di vista. 
																		
static int current_color = 0, angle = 0, angle_x = 45, angle_y = 45;    /* Per lo switch di colori ho utilizzato una variabile intera chiamata current_color.
																		La variabile angle rappresenta l'angolo di rotazione del cilindro. 
																		angle_x e angle_y rappresentano gli angoli del sistema di coordinate utilizzato per la rappresentazione. */
GLuint list;

void init(void)
{
	GLUquadricObj *cylinder;

	glClearColor(0.0, 0.0, 0.0, 0.0);

	/* Ho deciso di usare una quadrica per snellire il codice. 
	Avrei potuto utilizzare la trigonometria per disegnare il cilindro, ma ho preferito sfruttare la libreria GLU, anche per familiarizzare con quest'ultima. */

	list = glGenLists(1);
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricNormals(cylinder, GLU_FLAT);
	
	glNewList(list, GL_COMPILE);
		gluCylinder(cylinder, CYLINDER_RADIUS, CYLINDER_RADIUS, CYLINDER_HEIGHT, CYLINDER_SLICES, CYLINDER_STACKS);
	glEndList();
}

static void change_color(void)			//Funzione per lo switch tra i colori.
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
}

void my_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (current_color) {			//Colori Rosso, Verde, Blue e Bianco.
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

	glShadeModel(GL_FLAT);
	
	glPushMatrix();
		glLoadIdentity();

		glRotatef(angle, 0.0, 0.0, 1.0);		//Imposto rotazione di angle gradi lungo l'asse z.
		glRotatef(angle_x, 0.0, 1.0, 0.0);		//Imposto rotazione di angle_x gradi lungo l'asse y.
		glRotatef(angle_y, 1.0, 0.0, 0.0);		//Imposto rotazione di angle_y gradi lungo l'asse x.

		glCallList(list);
	glPopMatrix();
	
	glFlush();
}

void my_reshape(int w, int h)   //Funzione di ridimensionamento dello scenario in base alla dimensione della finestra di esecuzione.
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();			//Carico la matrice di identità sulla matrice di proiezione attuale per evitare che operazioni precedenti influiscano sul risultato.
	if (w <= h) {
		glOrtho(-2.5, 2.5, -2.5*(GLfloat)h / (GLfloat)w, 2.5*(GLfloat)h / (GLfloat)w, -10.0, 10.0);			//Nuovo sistema di coordinate.
	}
	else {
		glOrtho(-2.5*(GLfloat)w / (GLfloat)h, 2.5*(GLfloat)w / (GLfloat)h, -2.5, 2.5, -10.0, 10.0);			//Nuovo sistema di coordinate.
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void my_motion(int x, int y)		//Funzione per il cambio di vista in base alla posizione del cursore rispetto al sistema di coordinate del cilindro.
{
	if (x > my_x) {
		angle_x = (angle_x + 1) % 360;
		glutPostRedisplay();
		my_x = x;
	}

	if (x < my_x) {
		angle_x--;
		if (angle_x < 0) {
			angle_x += 360;
		}
		glutPostRedisplay();
		my_x = x;
	}

	if (y > my_y) {
		angle_y = (angle_y + 1) % 360;
		glutPostRedisplay();
		my_y = y;
	}

	if (y < my_y) {
		angle_y--;
		if (angle_y < 0) {
			angle_y += 360;
		}
		glutPostRedisplay();
		my_y = y;
	}
}

void my_keyboard(unsigned char key, int x, int y)			//Funzione di gestione dei comandi da tastiera per rotazione e cambio colore.
{
	switch (key) {

		/* Premere 'c' o 'C' per cambiare colore, ruotando tra le 4 scelte.
		Premere 'a' o 'A' per ruotare il solido lungo l'asse z. 
		Premere 'ESC' per uscire. */

		case 'C':
		case 'c':
			change_color();
			glutPostRedisplay();
			break;
		case 'a':
		case 'A':
			angle = (angle + 1) % 360;
			glutPostRedisplay();
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
	}
}

int main(int argc, char** argv)
{
	//Inizializzo glut e la finestra di dimensioni 640 pizel x 480 pixel. Assegno un titolo (Cylinder 3D Render) e una posizione alla finestra rispetto allo schermo.

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Cylinder 3D Render");

	init();

	// Assegno le funzioni personalizzate per visualizzazione e animazione del cilindro alle routine di default di glut. 

	glutDisplayFunc(my_display);
	glutReshapeFunc(my_reshape);
	glutKeyboardFunc(my_keyboard);
	glutMotionFunc(my_motion);

	glutMainLoop();			//Main Loop.
	return 0;
}