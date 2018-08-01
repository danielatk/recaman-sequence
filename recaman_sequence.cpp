#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265358979
#define NUM_ITER 60

using namespace std;

GLint window_width  = 800;
GLint window_height = 600;
vector<double> X;	//vector that saves the x coordinates of each point in the sequence
vector<double> Y;	//vector that saves the y coordinates of each point in the sequence
vector<int> sequence;	//vector that saves numbers in récaman's sequence

//function that calculates the terms in Recaman's sequence
void generate_sequence(int iterations){
	sequence.push_back(1);
	int step;
	int in;	//variable that indicates if generated number has already been seen in sequence
	for(int i = 1; i<iterations; i++){
		in = 0;
		step = i + 1;
		for(int j = 0; j<sequence.size(); j++){
			if(sequence.at(j) == sequence.at(i-1) - step){
				in = 1;
				break;
			}
		}
		if(!in && ((sequence.at(i-1) - step) > 0)){
			X.push_back(X.at(0) + 7*(sequence.at(i-1) - step - 1));
			Y.push_back(Y.at(0));
			sequence.push_back(sequence.at(i-1) - step);
		}else{
			X.push_back(X.at(0) + 7*(sequence.at(i-1) + step - 1));
			Y.push_back(Y.at(0));
			sequence.push_back(sequence.at(i-1) + step);
		}
	}
}

void draw_points(){
	glBegin(GL_POINTS);
	for(int i = 0; i < sequence.size(); i++){
		glVertex2f(X.at(i), Y.at(i));
		printf("%.2f %.2f\n", X.at(i), Y.at(i));
	}
	glEnd();
}

//function that receives as input the x and y coordinates for the center of the semicircle before translation, the radius of the semicircle, the number of segments to be used to draw the semicircle and a variable that indicates if the semicircle should be positive or negative in reference to the y coordinate of the center point
void draw_semicircle(double c_x, double c_y, double radius, int num_segments, int positive){ //change so that line isn't drawn
	double s_x, s_y;	//x and y coordinates for the semicircle
	double theta;
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < num_segments; i++){
		theta = PI * i/num_segments;
		if(positive){
			s_x = radius * cos(theta);
			s_y = radius * sin(theta);
		}else{
			s_x = -radius * cos(theta);
			s_y = -radius * sin(theta);
		}
		glVertex2f(s_x + c_x, s_y + c_y);
	}
	glEnd();
}

//draws a semicircle between each two consecutive points in the sequence, alternating between positive and negative y values for the semicircle, with reference to the base y value
void draw_sequence(){
	double radius;	//radius of the semicircle formed between each pair of points in the sequence
	double s_x, s_y;	//x and y coordinates for the semicircle
	double c_x, c_y;	//x and y coordinates for the center of the semicircle
	double theta;
	int num_segments;
	int positive;	//variable that indicates if the semicircle should be positive or negative in reference to the y coordinate of the center point
	for(int i = 1; i < sequence.size(); i++){
		radius = abs(X.at(i) - X.at(i-1))/2;
		c_x = (X.at(i) + X.at(i-1))/2;
		c_y = (Y.at(i) + Y.at(i-1))/2;
		if(i%2 == 0) positive = 0;
		else positive = 1;
		num_segments = (int)10*radius;
		draw_semicircle(c_x, c_y, radius, num_segments, positive);
	}

	//glutSwapBuffers();
}	

void recaman_sequence(int iterations){
	generate_sequence(iterations);
	draw_points();
	draw_sequence();
}

void display (void){
	double x, y;
	
	glPointSize(5);
	glColor3f(1.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_POINTS);
	for (int i = 0; i < X.size(); i++){
		x = X.at(i);
		y = Y.at(i);
		glVertex2f(x , y);
	}
	glEnd();

	glutSwapBuffers();
	//glFlush();
}

void mouse(int bin, int state , int x , int y){
	if(bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		y = window_height - y;

		X.push_back(x);
		Y.push_back(y);

		glPointSize(3);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POINTS);
			glVertex2f(x , y);
		glEnd();
		recaman_sequence(NUM_ITER);
		glutSwapBuffers();
	}	
}

void reshape(GLint width, GLint height){
	window_width  = width;
	window_height = height;
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y){
	switch (key){
	case 27:	// ESCAPE key
		exit (0);
		break;
	}
}

int main (int argc,char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize(window_width,window_height);
	/*Set the posotion of window*/
	glutInitWindowPosition(0,0);
	glutCreateWindow("Recaman's Sequence");

	glClearColor (0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport( 0,0, window_width, window_height);
	glMatrixMode( GL_PROJECTION );
	glOrtho( 0.0, window_width, 0.0, window_height, 1.0, -1.0 );

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); 

	/* initialize viewing values */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	
	glutMainLoop();
}
