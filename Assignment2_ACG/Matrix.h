#ifndef MATRIX_H
#define MATRIX_H
#include <iomanip>
using namespace std;
#define PI 3.14159265

class Mat4x1{
public:
	float mat[4][1];

	//Constructors
	Mat4x1();
	Mat4x1(float[4][1]);
	Mat4x1(float, float, float);
	//~Mat4x1();

	//Methods
	void set(float matrix[4][1]);
	void printMatrix();
};

//Constructors
Mat4x1::Mat4x1(){
	for(int i=0; i<4; i++){
		for(int j=0; j<1; j++){
			mat[i][j] = 0;
		}
	}
}
Mat4x1::Mat4x1(float matrix[4][1]){ 
	set(matrix); 
}
Mat4x1::Mat4x1(float value1, float value2, float value3){
	mat[0][0] = value1;
	mat[1][0] = value2;
	mat[2][0] = value3;
	mat[3][0] = 1.0f;
}

//Methods
void Mat4x1::set(float matrix[4][1]){
	for(int i=0; i<4; i++){
		for(int j=0; j<1; j++){
			mat[i][j] = matrix[i][j];
		}		
	}
}
void Mat4x1::printMatrix(){
	for(int i=0; i<4; i++){
		for(int j=0; j<1; j++){
			cout << " | " << setw(2) << right << fixed << setprecision(0) << mat[i][j];
		}
		cout << " |\n";
	}
	cout << endl;
}

//----------------------------------------------------------------------
class Mat4x4{
public:
	float mat[4][4];
	//Constructors
	Mat4x4();
	Mat4x4(float matrix[4][4]);

	//~Mat4x4();

	//Methods
	void set(float matrix[4][4]);
	void printMatrix();

	//Operators overload
	Mat4x4 operator*(const Mat4x4 &matrix);
	Mat4x1 operator*(const Mat4x1 &matrix);
};

//Constructors
Mat4x4::Mat4x4(){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			mat[i][j] = 0;
		}
	}
}
Mat4x4::Mat4x4(float matrix[4][4]){ 
	set(matrix);
}

//Methods
void Mat4x4::set(float matrix[4][4]){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			mat[i][j] = matrix[i][j];
		}		
	}
}
void Mat4x4::printMatrix(){
			
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			cout << " | " << setw(2) << right << mat[i][j];
		}
		cout << " |\n";
	}
	cout << endl;
}
//Operators
Mat4x4 Mat4x4::operator*(const Mat4x4 &matrix){
	Mat4x4 C;
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			for(int k=0; k<4; k++){
				C.mat[i][j] += this->mat[i][k] * matrix.mat[k][j];
			}
		}
	}
	return C;
}
Mat4x1 Mat4x4::operator*(const Mat4x1 &matrix){
	Mat4x1 C;
	for(int i=0; i<4; i++){
		for(int j=0; j<1; j++){
			for(int k=0; k<4; k++){
				C.mat[i][j] += this->mat[i][k] * matrix.mat[k][j];
			}
		}
	}
	return C;
}
//---------------------------------------------------------

class MatrixOperation{
public:
	//Create transformation matrix
	Mat4x4 getTmatrix(float, float, float);	// tx, ty, tz
	Mat4x4 getSmatrix(float, float, float);	// sx, sy, sz
	Mat4x4 getRmatrix(float, char);	// degree, axis

	//Transformation
	Mat4x1 translateMatrix(Mat4x1, float, float, float); // P, tx, ty, tz
	Mat4x1 scaleMatrix(Mat4x1, float, float, float);	// P, sx, sy, sz
	Mat4x1 rotateMatrix(Mat4x1, float, char);	// P, degree, axis

	//Composite Transformation
	Mat4x1 scaleAtVector3Matrix(Mat4x1, float,float,float, float,float,float); // P, tx,ty,tz, sz,sy,sz
	Mat4x1 rotateAtPivotMatrix(Mat4x1, float,float,float, float,char); // P, tx,ty,tz, degree,axis
};

//Create transformation matrix
Mat4x4 MatrixOperation::getTmatrix(float tx, float ty, float tz){
	float tMat[4][4]={{1, 0, 0, tx},
					  {0, 1, 0, ty},
					  {0, 0, 1, tz},
					  {0, 0, 0, 1}};
	Mat4x4 tMatrix(tMat);
	return tMatrix;
}
Mat4x4 MatrixOperation::getSmatrix(float sx, float sy, float sz){
	float sMat[4][4]={{sx, 0, 0, 0},
					  {0, sy, 0, 0},
					  {0, 0, sz, 0},
					  {0, 0, 0, 1}};
	Mat4x4 sMatrix(sMat);
	return sMatrix;
}
Mat4x4 MatrixOperation::getRmatrix(float r, char axis){
	r = r*PI/180;
	float rxMat[4][4]={{1,   0   ,    0   , 0},
					   {0, cos(r), -sin(r), 0},
					   {0, sin(r),  cos(r), 0},
					   {0,   0   ,    0   , 1}};			 
	
	float ryMat[4][4]={{cos(r), 0, -sin(r), 0},
					   {   0  , 1,    0   , 0},
					   {sin(r), 0,  cos(r), 0},
					   {   0  , 0,    0   , 1}};
	
	float rzMat[4][4]={{cos(r), -sin(r), 0, 0},
					   {sin(r),  cos(r), 0, 0},
					   {   0  ,    0   , 1, 0},
					   {   0  ,    0   , 0, 1}};

	Mat4x4 rMatrix;
	if (axis == 'x'){
		rMatrix.set(rxMat);
	}
	else if (axis == 'y'){
		rMatrix.set(ryMat);
	}
	else if (axis == 'z'){
		rMatrix.set(rzMat);
	}
	else{
		//cout << "Error: axis input is wrong, choose 'x', 'y' or 'z' only." << endl;
	}
	return rMatrix;
}

//Transformation
Mat4x1 MatrixOperation::translateMatrix(Mat4x1 P, float tx, float ty, float tz){
	return (getTmatrix(tx, ty, tz) * P);
}
Mat4x1 MatrixOperation::scaleMatrix(Mat4x1 P, float sx, float sy, float sz){
	
	return (getSmatrix(sx, sy, sz) * P);
}
Mat4x1 MatrixOperation::rotateMatrix(Mat4x1 P, float degree, char axis){
	
	return (getRmatrix(degree, axis) * P);
}

//Composite Transformation
Mat4x1 MatrixOperation::scaleAtVector3Matrix(Mat4x1 P, float tx, float ty, float tz, float sx, float sy, float sz){
	
	return (getTmatrix(tx, ty, tz) * getSmatrix(sx, sy, sz) * getTmatrix(-tx, -ty, -tz) * P);
}
Mat4x1 MatrixOperation::rotateAtPivotMatrix(Mat4x1 P, float tx, float ty, float tz, float degree, char axis){
	
	return (getTmatrix(tx, ty, tz) * getRmatrix(degree, axis) * getTmatrix(-tx, -ty, -tz) * P);
}
#endif