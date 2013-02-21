
//include header file for glfw library so that we can use OpenGL
#include <GL/glfw.h>
#include <stdlib.h>  //needed for exit function
#include <iostream>
using namespace std;

//Initializes 3D rendering
void initializeRendering()
{
    glfwInit();
    //Makes 3D drawing work when something is in front of something else
	//glEnable(GL_DEPTH_TEST);
}

//Called when a key is pressed
void GLFWCALL handleKeypress(int key,int press) //The key that was pressed
{
	switch (key) {
		case GLFW_KEY_ESC: //Escape key
			exit(0); //Exit the program
	}
}

//Called when the window is resized
void GLFWCALL handleResize(int width,int height)
{
    //Tell OpenGL how to convert from coordinates to pixel values
    glViewport( 0, 0, width, height );

    glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
    //Set the camera perspective
    glLoadIdentity(); //reset the camera
    gluPerspective( 45.0f,                      //camera angle
                (GLfloat)width/(GLfloat)height, //The width to height ratio
                 1.0f,                          //The near z clipping coordinate
                100.0f );                       //The far z clipping coordinate
}
void display()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black

    //Clear information from last draw
    glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

    glBegin(GL_QUADS); //Begin quadrilateral coordinates

	//Trapezoid
	glVertex3f(-0.7f, -1.5f, -5.0f);
	glVertex3f(0.7f, -1.5f, -5.0f);
	glVertex3f(0.4f, -0.5f, -5.0f);
	glVertex3f(-0.4f, -0.5f, -5.0f);

	glEnd(); //End quadrilateral coordinates

	glBegin(GL_TRIANGLES); //Begin triangle coordinates

	//Pentagon
	glVertex3f(0.5f, 0.5f, -5.0f);
	glVertex3f(1.5f, 0.5f, -5.0f);
	glVertex3f(0.5f, 1.0f, -5.0f);

	glVertex3f(0.5f, 1.0f, -5.0f);
	glVertex3f(1.5f, 0.5f, -5.0f);
	glVertex3f(1.5f, 1.0f, -5.0f);

	glVertex3f(0.5f, 1.0f, -5.0f);
	glVertex3f(1.5f, 1.0f, -5.0f);
	glVertex3f(1.0f, 1.5f, -5.0f);

	//Triangle
	glVertex3f(-0.5f, 0.5f, -5.0f);
	glVertex3f(-1.0f, 1.5f, -5.0f);
	glVertex3f(-1.5f, 0.5f, -5.0f);

	glEnd(); //End triangle coordinates
    glfwSwapBuffers();

}
int main()
{
   // int     width, height;
    //int     frame = 0;
    bool    running = true;

    initializeRendering();
    if( !glfwOpenWindow( 512, // width of window
                         512, //height of window
                          1,  //redbits
                          0,  //greenbits
                          0,  //bluebits
                          0,  //alphabits
                          0,  //depthbits
                          0, //stencilbits
                          GLFW_WINDOW ) //mode
        ) //return false if window is not created
    {
        glfwTerminate(); //terminating glfw window
        return 0;
    }

    glfwSetWindowTitle("codeincodeblock.blogspot.com - basic shape");
    glfwSetWindowSizeCallback(handleResize); //callback function of GLFW to handle window resize
    glfwSetKeyCallback(handleKeypress); //callback function to handle keypress
    while(running) // infinite loop to draw object again and again
    {              // because once object is draw then window is terminated
        display();
        running = glfwGetWindowParam( GLFW_OPENED ); //when glfw window is opened then it return true
                                                     //if closed then return false
    }
    return 0;
}
