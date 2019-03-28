#include <math.h>                                        //For expf()
#include <cstdio>                                    //GLUT library
#include <GL/glew.h>
//#include <GLUT/glut.h>									//GLUT library
#include <GLFW/glfw3.h>
#include "Quad.h"                                        //the Quad class definition


float Xmin = -5, Xmax = 5;                            //Range of variable x that we sample
float Ymin = -5, Ymax = 5;                            //Range of variable y that we sample
int Nx = 100, Ny = 100;                            //Number of samples taken along the x and y axes

float dx = (Xmax - Xmin) / (Nx - 1);                            //Size of a cell along the x axis
float dy = (Ymax - Ymin) / (Ny - 1);                            //Size of a cell along the y axis


float fov = 60;                                        //Perspective projection parameters
float aspect = 1;
float z_near = 0.1;
float z_far = 30;

float eye_x = 10, eye_y = 10, eye_z = 10;                //Modelview (camera extrinsic) parameters
float c_x = 0, c_y = 0, c_z = 0;
float up_x = 0, up_y = 0, up_z = 1;


float f(float x, float y)                                //A simple two-variable function to plot
{                                                        //The function is samples
    float ret = 5 * sin(1 / 0.2 / (x * x + y * y));
    return ret;
}


void viewing(GLFWwindow *window, int W, int H)                                //Window resize function, sets up viewing parameters (GLUT callback function)
{
    glMatrixMode(
            GL_MODELVIEW);                            //1. Set the modelview matrix (including the camera position and view direction)
    glLoadIdentity();
    gluLookAt(eye_x, eye_y, eye_z, c_x, c_y, c_z, up_x, up_y, up_z);

    glMatrixMode(GL_PROJECTION);                        //2. Set the projection matrix
    glLoadIdentity();
    gluPerspective(fov, float(W) / H, z_near, z_far);

    glViewport(0, 0, W,
               H);                                //3. Set the viewport to the entire size of the rendering window
}


void draw(GLFWwindow *window)                                                //Render the height plot (GLUT callback function)
{
    glClearColor(1, 1, 1, 1);                                //1. Clear the frame and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    for (float x = Xmin; x <= Xmax - dx; x += dx)                    //2. Render the height plot as a set of quads
        for (float y = Ymin; y <= Ymax - dy; y += dy) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glColor3f(0, 1, 0);                                //   Draw the quad filled with green
            Quad q1;
            q1.addPoint(x, y, f(x, y));
            q1.addPoint(x + dx, y, f(x + dx, y));
            q1.addPoint(x + dx, y + dy, f(x + dx, y + dy));
            q1.addPoint(x, y + dy, f(x, y + dy));
            q1.draw();

            glColor3f(0, 0, 0);                                //   Draw the quad outline in black
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            Quad q2;
            q2.addPoint(x, y, f(x, y));
            q2.addPoint(x + dx, y, f(x + dx, y));
            q2.addPoint(x + dx, y + dy, f(x + dx, y + dy));
            q2.addPoint(x, y + dy, f(x, y + dy));
            q2.draw();
        }

    glfwSwapBuffers(window);                                    //3. Rendering ready, ask GLUT to show the back buffer
}


int main(int argc, char *argv[])                            //Main program
{
    if (!glfwInit()) {
        printf("GLFW initialization failed");
        glfwTerminate();
        return 1;
    }

    //Setting OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 0);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);


    GLFWwindow *mainWindow = glfwCreateWindow(500, 500, "Test Window", nullptr,
                                              nullptr);
    if (!mainWindow) {
        printf("GLFW window creation failed");
        glfwTerminate();
        return 1;
    }

    // Get Buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("GLEW initialisation failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

//    glutInit(&argc, argv);                                //1. Initialize the GLUT toolkit
//    glutInitDisplayMode(GLUT_RGB |
//                        GLUT_DOUBLE);            //2. Ask GLUT to create next windows with a RGB framebuffer and a Z-buffer too
//    glutInitWindowSize(500,
//                       500);                            //3. Tell GLUT how large are the windows we want to create next
//    glutCreateWindow("2. Another graph");                //4. Create our window

    glfwSetWindowSizeCallback(mainWindow, viewing);
    viewing(mainWindow, 500, 500);
    while (!glfwWindowShouldClose(mainWindow)) {
        draw(mainWindow);
    }

//    glutDisplayFunc(draw);                                //5. Add a drawing callback to the window
//    glutReshapeFunc(viewing);                            //6. Add a resize callback to the window
//    glutMainLoop();                                        //7. Start the event loop that displays the graph and handles window-resize events

    return 0;
}




