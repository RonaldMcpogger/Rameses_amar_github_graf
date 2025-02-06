#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"




float x = 0.f,
y = 0.f,
z = -1.f;

float 
Sx=1.f, 
Sy=1.f, 
Sz=1.f;

float ax_X=0.f,
ax_Y=1.f, 
ax_Z=0.f;

float thetx=0.f;
float thety = 0.f;

float thetz = 0.f;



void key_Callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if (key == GLFW_KEY_D )
    {
        x += 0.01f;
    }

    if (key == GLFW_KEY_A)
    {
        x -= 0.01f;
    }

    if (key == GLFW_KEY_W)
    {
        y += 0.01f;
    }

    if (key == GLFW_KEY_S)
    {
        y -= 0.01f;
    }

    if (key == GLFW_KEY_RIGHT)
    {
        thetx+= 10.f;
        
    }

    if (key == GLFW_KEY_LEFT)
    {
        thetx -= 10.f;
       
        

    }

    if (key == GLFW_KEY_UP)
    {
        thety -= 10.f;
        
        
    }

    if (key == GLFW_KEY_DOWN)
    {
        thety += 10.f;
        
    }
    if (key == GLFW_KEY_Q)
    {
        Sx -= .1f;
        Sy -= .1f;
        Sz -= .1f;


    }
    if (key == GLFW_KEY_E)
    {
        Sx += .1f;
        Sy += .1f;
        Sz += .1f;


    }
    if (key == GLFW_KEY_Z)
    {
        z -= .1f;
    }
    if (key == GLFW_KEY_C)
    {
        z += .1f;
    }
}

int main(void)
{
    GLFWwindow* window;

    

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    float windowWidth = 700.f;
    float windowHeight = 700.f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "Rameses P. Amar: BUNNY", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //Initialize Glad
    gladLoadGL();
    glfwSetKeyCallback(window, key_Callback);

    std::fstream vertSrc("Shaders/Sam.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/Sam.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();



    std::string path = "3D/bunny.obj";

   
    std::vector<tinyobj::shape_t> shapes;
    std::vector < tinyobj::material_t> material;
    std::string warning, error;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);


        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &f, NULL);
        glCompileShader(fragShader);

        GLuint shaderProg = glCreateProgram();
        glAttachShader(shaderProg, vertexShader);
        glAttachShader(shaderProg, fragShader);
        glLinkProgram(shaderProg);

    tinyobj::attrib_t attributes; //positions, texture data, and etc.

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    //screen space
    //mix,mixy,maxx,maxy call glviewport after gladloadgl
    glViewport(0, 0, 700,700 );
    std::vector<GLuint> mesh_indices;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    GLfloat vertices[]
    {
        0.f, 0.5f, 0.f,
        -0.5f, 0.0f, 0.f,
        0.5f, 0.f, 0.f
    };

    GLuint indices[]
    {
        0, 1, 2
    };

    //ID of VAO & VBO & EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //current VAO = null
    glBindVertexArray(VAO);
    //current VAO = VAO

    //Current VBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Current VBO = VBO
    //current VAO.VBO.append(VBO)

    //Data
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * attributes.vertices.size(), //Size of buffer in bytes
        &attributes.vertices[0], //Array 
        GL_STATIC_DRAW //Static Objects for moving object need to use GL_DYNAMIC_DRAW
    );

    //Describes how to read data
    glVertexAttribPointer(
        //0 Position Data
        0, //Attrib Index-Index of VBO
        3, // X , Y , Z
        GL_FLOAT, //Array of GL floats
        GL_FALSE, //is_normalized
        3 * sizeof(GLfloat), //size of components in bytes
        (void*)0 //stride value
    );

    //current VBO = VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    //enables attrib index 0
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glm::mat4 identity_matrix=glm::mat4(1.0f);

    //setup projection matrix:
    // ortho:
   // glm::mat4 projectionMatrix = glm::ortho(-2.f, 2.f, -2.f, 2.f, -1.f, 1.f);
    //fov:

    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.f), 
        windowHeight/windowWidth,
        0.1f,
        100.f);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        //camera
        glm::vec3 cameraPos = glm::vec3(0+x, 0, 5.f);
        glm::mat4 cameraPositionMatrix = glm::translate(glm::mat4(1.0f), cameraPos * -1.0f);

        //orientation
        glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.f, 0));
        glm::vec3 cameraCenter = glm::vec3(0, 3.f+z, 0);

        //FOrward
        glm::vec3 F = cameraCenter - cameraPos;
        F = glm::normalize(F);

        glm::vec3 R = glm::cross(F, worldUp);

        glm::vec3 U = glm::cross(R, F);
        glm::mat4 cameraOrientation = glm::mat4(1.);

        //matrix [column][row]
        //[row] 1 or [n][0] = R
        cameraOrientation[0][0] = R.x;
        cameraOrientation[1][0] = R.y;
        cameraOrientation[2][0] = R.y;

        cameraOrientation[0][1] = U.x;
        cameraOrientation[1][1] = U.y;
        cameraOrientation[2][1] = U.z;

        cameraOrientation[0][2] = -F.x;
        cameraOrientation[1][2] = -F.y;
        cameraOrientation[2][2] = -F.z;


        glm::mat4 viewMatrix = (cameraOrientation * cameraPositionMatrix);

        //linear transform
        glm::mat4 transformMatrix = glm::translate(identity_matrix, glm::vec3(x, y,z));

         transformMatrix = glm::scale(transformMatrix, glm::vec3(Sx,Sy, Sz));


         transformMatrix = glm::rotate(transformMatrix, glm::radians(thetx), glm::normalize(glm::vec3(0, 1, 0)));
         transformMatrix = glm::rotate(transformMatrix, glm::radians(thety), glm::normalize(glm::vec3(1, 0, 0)));



         unsigned int viewLoc = glad_glGetUniformLocation(shaderProg, "view");
         glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));


         unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
         glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        
         unsigned int  transformloc = glGetUniformLocation(shaderProg, "transform");

         glUniformMatrix4fv(transformloc, 1, GL_FALSE, glm::value_ptr(transformMatrix));


        glUseProgram(shaderProg);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);


        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

