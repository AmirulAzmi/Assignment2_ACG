#include <iostream>
#include <cmath>
//#include <string>
#include <GL\glut.h>
#include "Geometry.h"
using namespace std;

//functions
void Init();
void Menu();
void display();
void reshape(int width, int height);
void inputKeyboard(unsigned char key, int x, int y);
void inputSpecial(int key, int x, int y);
void inputUpSpecial(int key, int x, int y);
void inputMouse(int button, int state, int x, int y);
void motion(int x, int y);

//Global variable
bool isClicked, isDrawEdge, keyUp, keyDown, keyLeft, keyRight, keyShift;
int windowWidth = 800, windowHeight = 600;
float angleX, angleY, angleZ, rotate_degree = 10, mouseX, mouseY, sweep_length;
Vector3 cameraPos(0,0,10);
Vector3 lightPos(1,1,1);
float pt_x, pt_y, pt_z; // Vector3 input
enum objDisplay{ CUSTOM, CUBE, POLYGON, SWEEP_POLYGON, SPHERE };
enum objTransform{ TRANSLATE, SCALE, ROTATE, SCALE_AT_POINT, ROTATE_AT_POINT, MOVE_LIGHT };
int geometry_used = CUSTOM, poly_vertices, transform_ops = TRANSLATE;
Cube cb;
Plane polygon;
Sphere sp;

int main(int argc, char **argv){
	//--------------------------------------------
	cout << "ADVANCE COMPUTER GRAPHICS\n\n";
	cout << "-- Right click: Choose object\n";
	cout << "-- Press `w' or `s' to zoom in/out\n";
	cout << "-- Press `Esc' to exit program\n";
	//---------------------------------------------
	//initialize glut
	glutInit(&argc, argv);

	//use 2 display buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	//setting for window - size, position, title
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(500, 50);
	glutCreateWindow("Transformation");

	Init(); //initialize variable
	Menu(); //menu (right click)

	//set display() as function to be rendered
	glutDisplayFunc(display);
	
	//set display() as idle function
	glutIdleFunc(display);
	
	glutReshapeFunc(reshape);
	
	//keyboard and mouse
	glutKeyboardFunc(inputKeyboard);
	glutSpecialFunc(inputSpecial);
	glutSpecialUpFunc(inputUpSpecial);
	glutMouseFunc(inputMouse);
	glutMotionFunc(motion);
	
	//Enter Looping
	glutMainLoop();

	return 0;
}
// -- INITIALIZE -------------------------------
void reset(){
	angleX = 30;
	angleY = -30;
	angleZ = mouseX = mouseY = 0.0;
	//camera_x = 0;
	//camera_y = 0;
	//camera_z = 10;
	pt_x = pt_y = pt_z = 0;
	sweep_length = 1.5;
	poly_vertices = 4;
	isClicked = keyUp = keyDown = keyShift = keyLeft = keyRight = false;
}
void Init(){
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { lightPos.x, lightPos.y, lightPos.z, 0.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(3.0);
	reset();
}

// -- MENU ------------------------------------
void MenuOperation(int value){
	if(value==geometry_used) reset();
	geometry_used = value;
	switch(value){
	case CUSTOM:
		break;
	case CUBE:
		//cb.createCube(2,2,2);
		system("cls");
		cout << "Cube\n";
		cout << "-- Press `w' or `s' to zoom in/out\n";
		cout << "-- Use `arrow' key to translate the cube position.\n";
		cout << "-- Click mouse's left button and drag to change view angle\n";
		cout << "-- Transform object: Right click --> Transform --> [Choose transformation]\n";
		break;
	case POLYGON:
		polygon.createPolygon(poly_vertices, 2);
		system("cls");
		cout << "Polygon (same side length)\n";
		cout << "-- Press `w' or `s' to zoom in/out\n";
		cout << "-- Use `arrow' key to translate the polygon position.\n";
		cout << "-- Use `+' key to add vertex and `-' to reduce vertex.\n";
		cout << "-- Click mouse's left button and drag to change view angle\n";
		break;
	case SWEEP_POLYGON:
		//polygon.createPolygon(poly_vertices, 2);
		system("cls");
		cout << "Sweep Polygon\n";
		cout << "-- Press `w' or `s' to zoom in/out\n";
		cout << "-- Use `arrow' key to translate the object position.\n";
		cout << "-- Use `+' key to add vertex and `-' to reduce vertex.\n";
		cout << "-- Use `[' and `]' to control sweep length\n";
		cout << "-- Click mouse's left button and drag to change view angle\n";
		break;
	case SPHERE:
		sp.CreateSphere(1.5,0,0,0);
		system("cls");
		cout << "Sphere\n";
		cout << "-- Press `w' or `s' to zoom in/out\n";
		cout << "-- Use `arrow' key to translate the sphere position.\n";
		cout << "-- Click mouse's left button and drag to change view angle\n";
		cout << "-- Transform object: Right click --> Transform --> [Choose transformation]\n";
		break;
	case 99: 
		exit(0); 
		break;
	}
	Menu();
}
void SubMenuOperation(int value){
	transform_ops = value;
	switch(value){
	case TRANSLATE:
		cout << "\nTranslate\n";
		cout << "-- Use arrow key\n";
		break;
	case SCALE:
		cout << "\nScale\n";
		cout << "-- press [x] or [y] or [z] to increase size\n";
		cout << "-- press [Shift + x] or [Shift + y] or [Shift + z] to decrease size\n";
		break;
	case ROTATE:
		cout << "\nRotate\n";
		cout << "-- press [x] or [y] or [z] to rotate (anti-clockwise)\n";
		cout << "-- press [Shift + x] or [Shift + y] or [Shift + z] to rotate (clockwise)\n";
		break;
	case SCALE_AT_POINT:
		cout << "\nScale at fixed Vector3\n";
		cout << "-- press [x] or [y] or [z] to increase size\n";
		cout << "-- press [Shift + x] or [Shift + y] or [Shift + z] to decrease size\n";
		cout << "Input Point Coordinate: \n > ";
		cin >> pt_x;
		cout << " > ";
		cin >> pt_y;
		cout << " > ";
		cin >> pt_z;
		cout << "Scale at Point (" << pt_x << ", " << pt_y << ", " << pt_z << ")\n";
		cout << "-- press [x] or [y] or [z] to increase size\n";
		cout << "-- press [Shift + x] or [Shift + y] or [Shift + z] to decrease size\n";
		break;
	case ROTATE_AT_POINT:
		cout << "\nRotate at pivot Point\n";
		cout << "-- press [x] or [y] or [z] to rotate (anti-clockwise)\n";
		cout << "-- press [Shift + x] or [Shift + y] or [Shift + z] to rotate (clockwise)\n";
		cout << "Input Vector3 Coordinate: \n > ";
		cin >> pt_x;
		cout << " > ";
		cin >> pt_y;
		cout << " > ";
		cin >> pt_z;
		cout << "Rotate at Point (" << pt_x << ", " << pt_y << ", " << pt_z << ")\n";
		cout << "-- press [x] or [y] or [z] to rotate (anti-clockwise)\n";
		cout << "-- press [Shift + x] or [Shift + y] or [Shift + z] to rotate (clockwise)\n";
		break;
	case MOVE_LIGHT:
		cout << "Input Light Coordinate: \n > ";
		cin >> lightPos.x;
		cout << " > ";
		cin >> lightPos.y;
		cout << " > ";
		cin >> lightPos.z;
		cout << "Rotate at Vector3 (" << pt_x << ", " << pt_y << ", " << pt_z << ")\n";
		cout << "-- press [x] or [y] or [z] to rotate (anti-clockwise)\n";
		cout << "-- press [Shift + x] or [Shift + y] or [Shift + z] to rotate (clockwise)\n";
		break;
	}
}
void Menu() {
	int submenu = glutCreateMenu(SubMenuOperation);
	glutAddMenuEntry("Translate", TRANSLATE);
	glutAddMenuEntry("Scale", SCALE);
	glutAddMenuEntry("Rotate", ROTATE);
	glutAddMenuEntry("Scale at fixed point", SCALE_AT_POINT);
	glutAddMenuEntry("Rotate at pivot point", ROTATE_AT_POINT);
	glutAddMenuEntry("Move light point", MOVE_LIGHT);

	glutCreateMenu(MenuOperation);
	glutAddMenuEntry("Cube", CUBE);
	glutAddMenuEntry("Polygon", POLYGON);
	if(geometry_used == POLYGON){
		glutAddMenuEntry("Extrude Polygon", SWEEP_POLYGON);
	}
	glutAddMenuEntry("Sphere", SPHERE);
	if(geometry_used != CUSTOM)
		glutAddSubMenu("Transform", submenu);
	glutAddMenuEntry("Reset", geometry_used);
	glutAddMenuEntry("Exit", 99);
	glutAttachMenu(GLUT_RIGHT_BUTTON); 
}

//--- DISPLAY ---------------------------------
void ruler(){
	//x-axis
	glColor3f(1.0, 0.2, 0.2); //red
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(4.5, 0.0, 0.0);
	glEnd();
	
	//y-axis
	glColor3f(0.2, 1.0, 0.2); //green
	glBegin(GL_LINES);
		glVertex3f(0.0, 4.5, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	//z-axis
	glColor3f(0.2, 0.2, 1.0); //blue
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 4.5);
	glEnd();
	glColor3f(1, 1, 1);
}
void update(){
	// draw
	if(geometry_used == CUBE){
		cb.drawCube();
	}
	if(geometry_used == POLYGON){
		polygon.drawPlane();
	}
	if(geometry_used == SWEEP_POLYGON){
		polygon.sweepPolygon(sweep_length);
	}
	if(geometry_used == SPHERE){
		sp.DrawSphere();
	}
	//----------
	if(transform_ops == SCALE_AT_POINT || transform_ops == ROTATE_AT_POINT){
		glPointSize(10);
		glColor3f(1,1,0);
		glBegin(GL_POINTS); glVertex3f(pt_x, pt_y, pt_z); glEnd();
		glColor3f(1,1,1);
	}
	
	float translate_value = 0.01;
	//-- TRANSLATE --------------------------------------
	//press arrrow up
	if(keyUp && !keyShift){
		if(geometry_used == SPHERE){
			sp.translateSphere(0.0, translate_value, 0.0);
		}
		if(geometry_used == CUBE){
			cb.translateCube(0.0, translate_value, 0.0);
		}
		if(geometry_used == POLYGON || geometry_used == SWEEP_POLYGON){
			polygon.translatePlane(0.0, translate_value, 0.0);
		}
	}
	//press arrow down
	if(keyDown && !keyShift){
		if(geometry_used == SPHERE){
			sp.translateSphere(0.0, -translate_value, 0.0);
		}
		if(geometry_used == CUBE){
			cb.translateCube(0.0, -translate_value, 0.0);
		}
		if(geometry_used == POLYGON || geometry_used == SWEEP_POLYGON){
			polygon.translatePlane(0.0, -translate_value, 0.0);
		}
	}
	//press arrow left
	if(keyLeft){
		if(geometry_used == SPHERE){
			sp.translateSphere(-translate_value, 0.0, 0.0);
		}
		if(geometry_used == CUBE){
			cb.translateCube(-translate_value, 0.0, 0.0);
		}
		if(geometry_used == POLYGON || geometry_used == SWEEP_POLYGON){
			polygon.translatePlane(-translate_value, 0.0, 0.0);
		}
	}
	//press arrow right
	if(keyRight){
		if(geometry_used == SPHERE){
			sp.translateSphere(translate_value, 0.0, 0.0);
		}
		if(geometry_used == CUBE){
			cb.translateCube(translate_value, 0.0, 0.0);
		}
		if(geometry_used == POLYGON || geometry_used == SWEEP_POLYGON){
			polygon.translatePlane(translate_value, 0.0, 0.0);
		}
	}
	//press arrow up + shift key
	if(keyShift && keyUp){
		if(geometry_used == SPHERE){
			sp.translateSphere(0.0, 0.0, -translate_value);
		}
		if(geometry_used == CUBE){
			cb.translateCube(0.0, 0.0, -translate_value);
		}
		if(geometry_used == POLYGON || geometry_used == SWEEP_POLYGON){
			polygon.translatePlane(0.0, 0.0, -translate_value);
		}
	}
	//press arrow down + shift key
	if(keyShift && keyDown){
		if(geometry_used == SPHERE){
			sp.translateSphere(0.0, 0.0, translate_value);
		}
		if(geometry_used == CUBE){
			cb.translateCube(0.0, 0.0, translate_value);
		}
		if(geometry_used == POLYGON || geometry_used == SWEEP_POLYGON){
			polygon.translatePlane(0.0, 0.0, translate_value);
		}
	}
}
void display(){
	//clear previous loop drawing
	glClearColor(0.7, 0.5, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//reset drawing location
	glLoadIdentity();
	
	//set camera (GLdouble)(posX,posY,posZ, centerX,centerY,centerZ, upX,upY,upZ)
	gluLookAt(cameraPos.x,cameraPos.y,cameraPos.z, 0,0,0, 0,1,0);

	glPushMatrix();
	glRotatef(angleX, 1,0,0);
	glPushMatrix();
	glRotatef(angleY, 0,1,0);
	glPushMatrix();
	glRotatef(angleZ, 0,0,1);
	//-start-drawing------------------
	
	update();
	ruler();

	//-end-drawing--------------------
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//send drawing to window
	glutSwapBuffers();
}
void reshape (int width, int height){
	//Set the viewport same as window size
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); 
	
	//Switch to the projection matrix - to manipulate how the scene is viewed  
	glMatrixMode(GL_PROJECTION);

	//Reset the projection matrix to the identity matrix
	glLoadIdentity();

	//Set the FieldOfView's angle(in degrees), aspect ratio, and near & far planes
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0);

	//Switch back to the model view matrix, so that we can start drawing shapes correctly 
	glMatrixMode(GL_MODELVIEW);  
}
void transform(char axis, float scale_value, float theta){
	if(axis == 'X') axis = 'x';
	if(axis == 'Y') axis = 'y';
	if(axis == 'Z') axis = 'z';
	//--Cube--------------------------------------
	if(geometry_used == CUBE){
		if(transform_ops==SCALE){
			if(axis=='x') { cb.scaleCube(scale_value, 1, 1); }
			else if(axis=='y') { cb.scaleCube(1, scale_value, 1); }
			else if(axis=='z') { cb.scaleCube(1, 1, scale_value); }
		}
		if(transform_ops==ROTATE){
			cb.rotateCube(theta, axis);
		}
		if(transform_ops==SCALE_AT_POINT){
			if(axis=='x') { cb.scaleAtVector3(Vector3(pt_x, pt_y, pt_z), scale_value, 1, 1); }
			else if(axis=='y') { cb.scaleAtVector3(Vector3(pt_x, pt_y, pt_z), 1, scale_value, 1); }
			else if(axis=='z') { cb.scaleAtVector3(Vector3(pt_x, pt_y, pt_z), 1, 1, scale_value); }
		}
		if(transform_ops==ROTATE_AT_POINT){
			cb.rotateAtVector3(Vector3(pt_x, pt_y, pt_z), theta, axis);
		}
	}
	//Sphere
	if(geometry_used == SPHERE){
		if(transform_ops==SCALE){
			if(axis=='x') { sp.scaleSphere(scale_value, 1, 1); }
			else if(axis=='y') { sp.scaleSphere(1, scale_value, 1); }
			else if(axis=='z') { sp.scaleSphere(1, 1, scale_value); }
		}
		if(transform_ops==ROTATE){
			sp.rotateSphere(theta, axis);
		}
		if(transform_ops==SCALE_AT_POINT){
			if(axis=='x') { sp.scaleAtVector3(Vector3(pt_x, pt_y, pt_z), scale_value, 1, 1); }
			else if(axis=='y') { sp.scaleAtVector3(Vector3(pt_x, pt_y, pt_z), 1, scale_value, 1); }
			else if(axis=='z') { sp.scaleAtVector3(Vector3(pt_x, pt_y, pt_z), 1, 1, scale_value); }
		}
		if(transform_ops==ROTATE_AT_POINT){
			sp.rotateAtVector3(Vector3(pt_x, pt_y, pt_z), theta, axis);
		}
	}
	if(geometry_used == POLYGON || geometry_used == SWEEP_POLYGON){
		if(transform_ops==SCALE){
			if(axis=='x') { polygon.scalePlane(scale_value, 1, 1); }
			else if(axis=='y') { polygon.scalePlane(1, scale_value, 1); }
			else if(axis=='z') { polygon.scalePlane(1, 1, scale_value); }
		}
		if(transform_ops==ROTATE){
			polygon.rotatePlane(theta, axis);
		}
		if(transform_ops==SCALE_AT_POINT){
			if(axis=='x') { polygon.scaleAtVector3(Vector3(pt_x, pt_y, pt_z), scale_value, 1, 1); }
			else if(axis=='y') { polygon.scaleAtVector3(Vector3(pt_x, pt_y, pt_z), 1, scale_value, 1); }
			else if(axis=='z') { polygon.scaleAtVector3(Vector3(pt_x, pt_y, pt_z), 1, 1, scale_value); }
		}
		if(transform_ops==ROTATE_AT_POINT){
			polygon.rotateAtVector3(Vector3(pt_x, pt_y, pt_z), theta, axis);
		}
	}
}
// -- MOUSE/KEYBOARD --------------
void inputKeyboard(unsigned char key, int x, int y){
	switch(key){
		//Esc btn
		case 27: exit(0); break;
		//Spacebar 
		case 8: reset(); glutPostRedisplay(); break;
		case 'w': //zoom in
			cameraPos.z -= 0.3;
			break;
		case 's': // zoom out
			cameraPos.z += 0.3;
			break;
		case 'a': // right
			//MoveCamera('h', 1);
			break;
		case 'd': // left
			//MoveCamera('h', -1);
			break;
		case '+':
			if(geometry_used == POLYGON || geometry_used==SWEEP_POLYGON){
				if(poly_vertices<45)
					polygon.createPolygon(++poly_vertices, 2);
				else { poly_vertices = poly_vertices; }
				cout << "no. vertices: " << poly_vertices << endl;
			}
			break;
		case '-':
			if(geometry_used == POLYGON || geometry_used==SWEEP_POLYGON){
				if(poly_vertices>3)
					polygon.createPolygon(--poly_vertices, 2);
				else { poly_vertices = 3; }
				cout << "no. vertices: " << poly_vertices << endl;
			}
			break;
		case '[':
			if(geometry_used==SWEEP_POLYGON)
				{ sweep_length -= 0.1; cout << "sweep length: " << sweep_length << endl; }
			break;
		case ']':
			if(geometry_used==SWEEP_POLYGON)
				{ sweep_length += 0.1; cout << "sweep length: " << sweep_length << endl; }
			break;
		case 'x':
		case 'y':
		case 'z':
			transform(key, 1.1, rotate_degree);
			break;
		case 'X':
		case 'Y':
		case 'Z':
			transform(key, 0.9, -rotate_degree);
			break;
		default: break;
	}
}
void inputSpecial(int key, int x, int y){
	int vertical = 0, horizontal = 0;
	switch(key){
		case GLUT_KEY_UP:
			keyUp = true;
			break;
		case GLUT_KEY_DOWN:
			keyDown = true;
			break;
		case GLUT_KEY_LEFT:
			keyLeft = true;
			break;
		case GLUT_KEY_RIGHT:
			keyRight = true;
			break;
		default:
			break;
	}
	if( glutGetModifiers() == GLUT_ACTIVE_SHIFT){
		keyShift = true;
	}
	update();
}
void inputUpSpecial(int key, int x, int y){
	switch(key){
		case GLUT_KEY_UP:
			keyUp = false;
			break;
		case GLUT_KEY_DOWN:
			keyDown = false;
			break;
		case GLUT_KEY_LEFT:
			keyLeft = false;
			break;
		case GLUT_KEY_RIGHT:
			keyRight = false;
			break;
		default:
			break;
	}
	if( glutGetModifiers() != GLUT_ACTIVE_SHIFT){
		keyShift = false;
	}
	update();
}
void inputMouse(int button, int state, int x, int y){
	isClicked = (GLUT_LEFT_BUTTON == GLUT_DOWN);
	mouseX = x;
	mouseY = y;
}
void motion(int x, int y){
	if(isClicked){
		angleY += (float)(x-mouseX)/4;
		angleX -= (float)(mouseY-y)/4;
	}
	angleY = (int)angleY % 360;
	angleX = (int)angleX % 360;
	mouseX = x;
	mouseY = y;
}
