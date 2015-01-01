// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Libraries/Shader.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL/SOIL.h>
#include <time.h>
#include <string.h>

float zoomY = 0.0f;
float zoomX = 0.0f;

// Function prototypes
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The MAIN function, from here we start our application and run our Program/Game loop
int main(int argc, char *argv[])
{
    bool compare = false;
    bool fps = false;
    for(int i =0; i < argc;i++){
        if(strcmp(argv[i],"-C")==0)
            compare = true;
        if(strcmp(argv[i],"-F")==0)
            fps = true;
    }
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    if(window == NULL) //Check if Window is created
    {
    	cout << "ERROR::GLFW Window is null:"
    		" Please make sure you have OpenGL3.2 or higher" << endl;
		return -1;
	}

    // Callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback); 

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, 800, 800);

    // Build Shader
    Shader shader("Shaders/vertex.vs", "Shaders/fragment.frag");

    //GLfloat vertices[] = {
    //  // First triangle
    //  1.0f, 1.0f,	// Top Right
    //  1.0f, -1.0f,	// Bottom Right
    //  -1.0f, 1.0f, // Top Left 
    //  // Second triangle
    //  1.0f, -1.0f, // Bottom Right
    //  -1.0f, -1.0f, // Bottom Left
    //  -1.0f, 1.0f // Top Left
    //}; 
    GLfloat vertices[] = {
      1.0f, 1.0f,	// Top Right
      1.0f, -1.0f,	// Bottom Right
      -1.0f, -1.0f,  // Bottom Left
      -1.0f, 1.0f	// Top Left 
    };
    GLuint indices[] = {
        0, 1, 3,	// First Triangle
        1, 2, 3		// Second Triangle
    };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind VAO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO

    // Wire mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLuint texture;
	glGenTextures(1, &texture);  
	glBindTexture(GL_TEXTURE_1D, texture); 
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    int width, height;
	unsigned char* image = SOIL_load_image("Pallets/1817.png", 
	  &width, &height, 0, SOIL_LOAD_RGB);  
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, width, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);   
	glBindTexture(GL_TEXTURE_1D, 0);


	//glUniform1i(glGetUniformLocation(shader.Program, "MaxIterations"), 1000);
    // Game loop
    while(!glfwWindowShouldClose(window))
    {
    	clock_t start = clock(); // Start timer __GPU__
        // Check and call events
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		glUniform1f(glGetUniformLocation(shader.Program, "CrealOf"), zoomY);
		glUniform1f(glGetUniformLocation(shader.Program, "CimagOf"), zoomX);

        // Drawing
        shader.Use();
        glUniform1i(glGetUniformLocation(shader.Program, "Pallet"), 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D, texture);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
        if(fps || compare)
            cout << "GPU/OpenGL Took " << clock() - start << "ms\n"; // Stop timer __GPU__
    }
    
    glfwTerminate();

    if(compare){
        //start timer for cpu
        clock_t start = clock(); // Start timer __CPU__

        //Mandel CPU

        /* screen ( integer) coordinate */
        int iX,iY;
        const int iXmax = 800; 
        const int iYmax = 800;
        /* world ( double) coordinate = parameter plane*/
        double Cx,Cy;
        const double CxMin=-1.0;
        const double CxMax=1.0;
        const double CyMin=-1.0;
        const double CyMax=1.0;
        /* */
        double PixelWidth=(CxMax-CxMin)/iXmax;
        double PixelHeight=(CyMax-CyMin)/iYmax;
        /* color component ( R or G or B) is coded from 0 to 255 */
        /* it is 24 bit color RGB file */
        const int MaxColorComponentValue=255; 
        FILE * fp;
        char *filename="new1.ppm";
        char *comment="# ";/* comment should start with # */
        static unsigned char color[3];
        /* Z=Zx+Zy*i  ;   Z0 = 0 */
        double Zx, Zy;
        double Zx2, Zy2; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */
        /*  */
        int Iteration;
        const int IterationMax=1000;
        /* bail-out value , radius of circle ;  */
        const double EscapeRadius=2;
        double ER2=EscapeRadius*EscapeRadius;
        /*create new file,give it a name and open it in binary mode  */
        fp= fopen(filename,"wb"); /* b -  binary mode */
        /*write ASCII header to the file*/
        fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,iXmax,iYmax,MaxColorComponentValue);
        /* compute and write image data bytes to the file*/
        for(iY=0;iY<iYmax;iY++)
        {
             Cy=CyMin + iY*PixelHeight;
             if (fabs(Cy)< PixelHeight/2) Cy=0.0; /* Main antenna */
             for(iX=0;iX<iXmax;iX++)
             {         
                        Cx=CxMin + iX*PixelWidth;
                        /* initial value of orbit = critical point Z= 0 */
                        Zx=0.0;
                        Zy=0.0;
                        Zx2=Zx*Zx;
                        Zy2=Zy*Zy;
                        /* */
                        for (Iteration=0;Iteration<IterationMax && ((Zx2+Zy2)<ER2);Iteration++)
                        {
                            Zy=2*Zx*Zy + Cy;
                            Zx=Zx2-Zy2 +Cx;
                            Zx2=Zx*Zx;
                            Zy2=Zy*Zy;
                        };
                        /* compute  pixel color (24 bit = 3 bytes) */
                        if (Iteration==IterationMax)
                        { /*  interior of Mandelbrot set = black */
                           color[0]=0;
                           color[1]=0;
                           color[2]=0;                           
                        }
                     else 
                        { /* exterior of Mandelbrot set = white */
                             color[0]= Iteration % 255; /* Red*/
                             color[1]= Iteration % 255;  /* Green */ 
                             color[2]= Iteration % 255;/* Blue */
                        };
                        /*write color to the file*/
                        fwrite(color,1,3,fp);
                }
        }
        fclose(fp);
        cout << "CPU Took " << clock() - start << "ms\n"; // Stop timer __CPU__
    }
    return 0; //Done
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cout << "ScrollX[" << xoffset << "] ScrollY[" << yoffset << "]" << endl;
	zoomX += yoffset / 10.0f;
	zoomY += xoffset / 10.0f;
}