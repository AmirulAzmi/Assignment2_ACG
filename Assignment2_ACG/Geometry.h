#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Matrix.h"
using namespace std;

//----------------------------------------------
struct Vector3{
public:
	GLfloat x, y, z;
	Vector3(){ x = y = z = 0; }
    Vector3(GLfloat _x, GLfloat _y, GLfloat _z) : x(_x), y(_y), z(_z) {}
	void setVector3(float x, float y, float z){ 
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Mat4x1 toMat(){
		return Mat4x1(this->x, this->y, this->z);
	}
	void setVector3(Mat4x1 matrix){
		this->x = matrix.mat[0][0];
		this->y = matrix.mat[1][0];
		this->z = matrix.mat[2][0];
	}
	Vector3 operator-(const Vector3 &other){
		Vector3 newVector;
		newVector.x = this->x - other.x;
		newVector.y = this->y - other.y;
		newVector.z = this->z - other.z;
		return newVector;
	}
	Vector3 operator=(const Vector3 &other){
		Vector3 newVector;
		newVector.x = other.x;
		newVector.y = other.y;
		newVector.z = other.z;
		return newVector;
	}
	Vector3 operator*(const Vector3 &other){
		Vector3 newVector;
		newVector.x = (this->y * other.z) - (this->z * other.y);
		newVector.y = (this->z * other.x) - (this->x * other.z);
		newVector.z = (this->x * other.y) - (this->y * other.x);
		return newVector;
	}
};

Vector3 calcNormal(Vector3 a, Vector3 b){
	return a*b;
}
Vector3 calcNormal3(float u1, float u2, float u3, float v1, float v2, float v3){
	Vector3 a(u1, u2, u3);
	Vector3 b(v1, v2, v3);
	return a*b;
}

//-------------------------------------------------
class Plane{
private:
	int no_vertex;
	Vector3* vertex;
public:
	Plane(){
		
		this->no_vertex = 0;
		this->vertex = nullptr;
	}
	Plane(int n, Vector3 vertices[]){
		
		if(n>2){
			this->no_vertex = n;
			vertex = new Vector3[n];
			for(int i=0; i<n; i++){
				vertex[i] = vertices[i];
			}
		}
	}
	Vector3* getVertices(){ return vertex; }
	~Plane(){ 
		delete [] vertex;
		this->vertex = nullptr;
		this->no_vertex = 0;
	}
	void drawPlane(){
		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
			glNormal3f(0,0,1);
			for(int i=0; i<no_vertex; i++){
				glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z);
			}
		glEnd();
	}
	void createPolygon(int n_vertex, float radius){
		no_vertex = n_vertex;
		vertex = new Vector3[n_vertex];
		for(int i=0; i<n_vertex; i++){
			vertex[i].x = radius * sin( i*(360/n_vertex)*(PI/180) );
			vertex[i].y = radius * cos( i*(360/n_vertex)*(PI/180) );
		}
	}
	void sweepPolygon(float sweep_length){
		Vector3 *sides;
		int n = no_vertex*2;
		sides = new Vector3[n];
		// draw back polygon
		glBegin(GL_POLYGON);
			glNormal3f(0,0,-1);
			for(int i=0; i<no_vertex; i++){
				glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z-sweep_length);
			}
		glEnd();
		// draw front polygon
		glBegin(GL_POLYGON);
			glNormal3f(0,0,1);
			for(int i=0; i<no_vertex; i++){
				glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z+sweep_length);
			}
		glEnd();
		
		// draw sides polygon
		Vector3 normal;
		glBegin(GL_QUAD_STRIP);
			for(int i=0; i<no_vertex; i++){
				//glNormal3f((vertex[i].x+vertex[i+1].x)/2, (vertex[i].y+vertex[i+1].y)/2, 0);
				glNormal3f(vertex[i].x, vertex[i].y, 0);
				glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z-sweep_length);
				glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z+sweep_length);
				
			}
			
			glNormal3f(vertex[0].x, vertex[0].y, 0);
			glVertex3f(vertex[0].x, vertex[0].y, vertex[0].z-sweep_length);
			glVertex3f(vertex[0].x, vertex[0].y, vertex[0].z+sweep_length);
		glEnd();
	}
	void translatePlane(float tx, float ty, float tz){
		MatrixOperation move;
		for(int i=0; i<no_vertex; i++){
			vertex[i].setVector3( move.translateMatrix( vertex[i].toMat(), tx, ty, tz) );
		}
	}
	void scalePlane(float sx, float sy, float sz){
		MatrixOperation resize;
		for(int i=0; i<no_vertex; i++){
			vertex[i].setVector3( resize.scaleMatrix( vertex[i].toMat(), sx, sy, sz) );
		}
	}
	void rotatePlane(float degree, char axis){
		MatrixOperation rotate;
		for(int i=0; i<no_vertex; i++){
			vertex[i].setVector3( rotate.rotateMatrix( vertex[i].toMat(), degree, axis) );
		}
	}
	void scaleAtVector3(Vector3 fixed, float sx, float sy, float sz){
		MatrixOperation resize;
		for(int i=0; i<no_vertex; i++){
			vertex[i].setVector3( resize.scaleAtVector3Matrix( vertex[i].toMat(), fixed.x, fixed.y, fixed.z, sx, sy, sz) );
		}
	}
	void rotateAtVector3(Vector3 pivot, float degree, char axis){
		MatrixOperation rotate;
		for(int i=0; i<no_vertex; i++){
			vertex[i].setVector3( rotate.rotateAtPivotMatrix( vertex[i].toMat(), pivot.x, pivot.y, pivot.z, degree, axis) );
		}
	}
};
//-------------------------------------------
class Cube{
private:
	Vector3 vertex[8];
public:
	Cube(){
	//void createCube(float length, float height, float width){
		vertex[0].setVector3(-1,  1,  1);
		vertex[1].setVector3(-1, -1,  1);
		vertex[2].setVector3(-1, -1, -1);
		vertex[3].setVector3(-1,  1, -1);
		vertex[4].setVector3( 1,  1,  1);
		vertex[5].setVector3( 1, -1,  1);
		vertex[6].setVector3( 1, -1, -1);
		vertex[7].setVector3( 1,  1, -1);
	}/*		+-------------------+
			|	   3+------+7	|
			|	   /|     /|	|
			|	 0+------+4|	|
			|	  |2+----|-+6	|
			|	  |/     |/		|
			|	 1+------+5		|
			+-------------------+		*/
	void drawCube(){
		Vector3 faceUp[4] = {vertex[0], vertex[4], vertex[7], vertex[3]};
		Vector3 faceBt[4] = {vertex[1], vertex[2], vertex[6], vertex[5]};
		Vector3 faceRt[4] = {vertex[4], vertex[5], vertex[6], vertex[7]};
		Vector3 faceLf[4] = {vertex[0], vertex[3], vertex[2], vertex[1]};
		Vector3 faceBc[4] = {vertex[3], vertex[7], vertex[6], vertex[2]};
		Vector3 faceFr[4] = {vertex[0], vertex[1], vertex[5], vertex[4]};
		drawFace(faceUp, 0,1,0);
		drawFace(faceBt, 0,-1,0);
		drawFace(faceRt, 1,0,0);
		drawFace(faceLf,-1,0,0);
		drawFace(faceBc, 0,0,-1);
		drawFace(faceFr, 0,0,1);
	}
	void drawFace(Vector3 face[], float normalX, float normalZ, float normalY){
		glBegin(GL_QUADS);
		glNormal3f(normalX, normalZ, normalY);
		for(int i=0; i<4; i++){
			glVertex3f(face[i].x, face[i].y, face[i].z);
		}
		glEnd();
	}
	void translateCube(float tx, float ty, float tz){
		MatrixOperation move;
		for(int i=0; i<8; i++){
			vertex[i].setVector3( move.translateMatrix( vertex[i].toMat(), tx, ty, tz) );
		}
	}
	void scaleCube(float sx, float sy, float sz){
		MatrixOperation resize;
		for(int i=0; i<8; i++){
			vertex[i].setVector3( resize.scaleMatrix( vertex[i].toMat(), sx, sy, sz) );
		}
	}
	void rotateCube(float degree, char axis){
		MatrixOperation rotate;
		for(int i=0; i<8; i++){
			vertex[i].setVector3( rotate.rotateMatrix( vertex[i].toMat(), degree, axis) );
		}
	}
	void scaleAtVector3(Vector3 fixed, float sx, float sy, float sz){
		MatrixOperation resize;
		for(int i=0; i<8; i++){
			vertex[i].setVector3( resize.scaleAtVector3Matrix( vertex[i].toMat(), fixed.x, fixed.y, fixed.z, sx, sy, sz) );
		}
	}
	void rotateAtVector3(Vector3 pivot, float degree, char axis){
		MatrixOperation rotate;
		for(int i=0; i<8; i++){
			vertex[i].setVector3( rotate.rotateAtPivotMatrix( vertex[i].toMat(), pivot.x, pivot.y, pivot.z, degree, axis) );
		}
	}
};
//-------------------------------------------------
class Sphere{
private:	
	Vector3 *vertices;
	int no_vertex;
	int space;
	float *c_z; //length center to z
	bool wireframe;
public:
	Sphere(){
		space = 10;
		no_vertex = (90 / space) * (360 / space) * 4;
		vertices = new Vector3[no_vertex * 2];
		c_z = new float[no_vertex];
		wireframe = false;
	}
	~Sphere(){
		delete [] vertices;
		vertices = nullptr;
		delete [] c_z;
		c_z = nullptr;
	}
	Vector3* getVertices(){
		return vertices;
	}
	void DrawSphere (){
		// draw sphere
		glBegin (GL_TRIANGLE_STRIP);
			for (int i = 0; i < no_vertex*2; i++){
				glNormal3f (vertices[i].x, vertices[i].y, vertices[i].z);
				glVertex3f (vertices[i].x, vertices[i].y, vertices[i].z);
			}
		glEnd();

		if(wireframe){
			glColor3f(0,0,0);
			glBegin (GL_LINE_STRIP);
				for (int i = 0; i < no_vertex*2; i++){
					glVertex3f (vertices[i].x, vertices[i].y, vertices[i].z);
				}
			glEnd();
			glColor3f(1,1,1);
		}
	}
	void CreateSphere (double R, double H, double K, double Z) {
		int n = 0;
		double a, b;
		//create hemisphere
		for( b = 0; b <= 90-space; b+=space){
			for( a = 0; a <= 360 - space; a += space){
				vertices[n].x = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
				vertices[n].y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
				vertices[n].z = R * cos((b) / 180 * PI) - Z;
				n++;

				vertices[n].x = R * sin((a) / 180 * PI) * sin((b + space) / 180 * PI ) - H;
				vertices[n].y = R * cos((a) / 180 * PI) * sin((b + space) / 180 * PI ) + K;
				vertices[n].z = R * cos((b + space) / 180 * PI) - Z;

				n++;

				vertices[n].x = R * sin((a + space) / 180 * PI) * sin((b) / 180 * PI ) - H;
				vertices[n].y = R * cos((a + space) / 180 * PI) * sin((b) / 180 * PI ) + K;
				vertices[n].z = R * cos((b) / 180 * PI) - Z;
				n++;

				vertices[n].x = R * sin((a + space) / 180 * PI) * sin((b + space) / 180 * PI) - H;
				vertices[n].y = R * cos((a + space) / 180 * PI) * sin((b + space) / 180 * PI) + K;
				vertices[n].z = R * cos((b + space) / 180 * PI) - Z;
				n++;
			}
		}
		//duplicate hemishere
		for (int i = 0; i < no_vertex; i++){
			vertices[i+no_vertex].x = -vertices[i].x;
			vertices[i+no_vertex].y = -vertices[i].y;
			vertices[i+no_vertex].z = -vertices[i].z;
		}
	}
	void translateSphere(float tx, float ty, float tz){
		MatrixOperation move;
		for(int i=0; i<no_vertex*2; i++){
			vertices[i].setVector3( move.translateMatrix( vertices[i].toMat(), tx, ty, tz) );
		}
	}
	void scaleSphere(float sx, float sy, float sz){
		MatrixOperation resize;
		for(int i=0; i<no_vertex*2; i++){
			vertices[i].setVector3( resize.scaleMatrix( vertices[i].toMat(), sx, sy, sz) );
		}
	}
	void rotateSphere(float degree, char axis){
		MatrixOperation rotate;
		for(int i=0; i<no_vertex*2; i++){
			vertices[i].setVector3( rotate.rotateMatrix( vertices[i].toMat(), degree, axis) );
		}
	}
	void scaleAtVector3(Vector3 fixed, float sx, float sy, float sz){
		MatrixOperation resize;
		for(int i=0; i<no_vertex*2; i++){
			vertices[i].setVector3( resize.scaleAtVector3Matrix( vertices[i].toMat(), fixed.x, fixed.y, fixed.z, sx, sy, sz) );
		}
	}
	void rotateAtVector3(Vector3 pivot, float degree, char axis){
		MatrixOperation rotate;
		for(int i=0; i<no_vertex*2; i++){
			vertices[i].setVector3( rotate.rotateAtPivotMatrix( vertices[i].toMat(), pivot.x, pivot.y, pivot.z, degree, axis) );
		}
	}
	void toggleWireframe(){
		if(!wireframe){
			wireframe = true;
		}
		else wireframe = false;
	}
};
//----------------------------------------------
#endif