/* Use the following line on a Windows machine:
 *  #include <GL/glut.h>
 *   */
/* This line is for Max OSX  */
#include <GLUT/glut.h>
#include <OpenGL/gl3.h>

#include "common_header.h"
#include "shaders.h"

float fTriangle[9]; // Data to render triangle (3 vertices, each has 3 floats)
float fQuad[12]; // Data to render quad using triangle strips (4 vertices, each has 3 floats)
float fTriangleColor[9];
float fQuadColor[12];

UINT uiVBO[4];
UINT uiVAO[2];

CShader shVertex, shFragment;
CShaderProgram spMain;

/*! GLUT display callback function */
void display(void);
/*! GLUT window reshape callback function */
void reshape(int, int);


void initScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Setup triangle vertices
	fTriangle[0] = -0.4f; fTriangle[1] = 0.1f; fTriangle[2] = 0.0f;
	fTriangle[3] = 0.4f; fTriangle[4] = 0.1f; fTriangle[5] = 0.0f;
	fTriangle[6] = 0.0f; fTriangle[7] = 0.7f; fTriangle[8] = 0.0f;

	// Setup triangle color
	fTriangleColor[0] = 1.0f; fTriangleColor[1] = 0.0f; fTriangleColor[2] = 0.0f;
	fTriangleColor[3] = 0.0f; fTriangleColor[4] = 1.0f; fTriangleColor[5] = 0.0f;
	fTriangleColor[6] = 0.0f; fTriangleColor[7] = 0.0f; fTriangleColor[8] = 1.0f;

	// Setup quad vertices
	fQuad[0] = -0.2f; fQuad[1] = -0.1f; fQuad[2] = 0.0f;
	fQuad[3] = -0.2f; fQuad[4] = -0.6f; fQuad[5] = 0.0f;
	fQuad[6] = 0.2f; fQuad[7] = -0.1f; fQuad[8] = 0.0f;
	fQuad[9] = 0.2f; fQuad[10] = -0.6f; fQuad[11] = 0.0f;

	// Setup quad color
	fQuadColor[0] = 1.0f; fQuadColor[1] = 0.0f; fQuadColor[2] = 0.0f;
	fQuadColor[3] = 0.0f; fQuadColor[4] = 1.0f; fQuadColor[8] = 0.0f;
	fQuadColor[6] = 0.0f; fQuadColor[7] = 0.0f; fQuadColor[5] = 1.0f;
	fQuadColor[9] = 1.0f; fQuadColor[10] = 1.0f; fQuadColor[11] = 0.0f;

	glGenVertexArrays(2, uiVAO); // Generate two VAOs, one for triangle and one for quad
	glGenBuffers(4, uiVBO); // And four VBOs

	// Setup whole triangle
	glBindVertexArray(uiVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Setup whole quad
	glBindVertexArray(uiVAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuadColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Load shaders and create shader program

	shVertex.loadShader("shader.vert", GL_VERTEX_SHADER);
	shFragment.loadShader("shader.frag", GL_FRAGMENT_SHADER);

	spMain.createProgram();
	spMain.addShaderToProgram(&shVertex);
	spMain.addShaderToProgram(&shFragment);

	spMain.linkProgram();
	spMain.useProgram();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	/* set the window size to 512 x 512 */
	glutInitWindowSize(512, 512);

	/* set the display mode to Red, Green, Blue and Alpha
	 *      allocate a depth buffer
	 *           enable double buffering
	 *                */
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	/* create the window (and call it Lab 1) */
	glutCreateWindow("Lab 1");

	/* set the glut display callback function
	 *      this is the function GLUT will call every time
	 *           the window needs to be drawn
	 *                */
	glutDisplayFunc(display);

	/* set the glut reshape callback function
	 *      this is the function GLUT will call whenever
	 *           the window is resized, including when it is
	 *                first created
	 *                     */
	glutReshapeFunc(reshape);

	/* set the default background color to black */
	glClearColor(0,0,0,1);

	/* enter the main event loop so that GLUT can process
	 *      all of the window event messages
	 *           */
	glutMainLoop();

	return 0;
}

/*! glut display callback function.  Every time the window needs to be drawn,
 *  glut will call this function.  This includes when the window size
 *   changes, or when another window covering part of this window is
 *    moved so this window is uncovered.
 *     */
void display()
{
	// We just clear color
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(uiVAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(uiVAO[1]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glutSwapBuffers();
}

/*! glut reshape callback function.  GLUT calls this function whenever
 *  the window is resized, including the first time it is created.
 *   You can use variables to keep track the current window size.
 *    */
void reshape(int width, int height)
{
	/* tell OpenGL we want to display in a recangle that is the 
	 *      same size as the window
	 *           */
	glViewport(0,0,width,height);

	/* switch to the projection matrix */
	glMatrixMode(GL_PROJECTION);

	/* clear the projection matrix */
	glLoadIdentity();

	/* set the camera view, orthographic projection in 2D */
	gluOrtho2D(0,width,0,height);

	/* switch back to the model view matrix */
	glMatrixMode(GL_MODELVIEW);
}

void releaseScene()
{
	spMain.deleteProgram();

	shVertex.deleteShader();
	shFragment.deleteShader();
}
