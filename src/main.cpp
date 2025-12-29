#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

void configScene();
void renderScene();
void setLights (glm::mat4 P, glm::mat4 V);
void drawObjectMat(Model &model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model &model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);
void funScroll         (GLFWwindow* window, double xoffset, double yoffset);
void funCursorPos      (GLFWwindow* window, double xpos, double ypos);

// Shaders
   Shaders shaders;

// Modelos
   Model plane;
   Model sphere;

// Imagenes (texturas)
   Texture noEmissive;

   Texture sueloDiffuse, sueloNormal, sueloSpecular;
   Textures sueloTex;

   Texture paredLadrilloDiffuse, paredLadrilloNormal, paredLadrilloSpecular;
   Textures paredLadrilloTex;

   Texture paredAreniscaDiffuse, paredAreniscaNormal, paredAreniscaSpecular;
   Textures paredAreniscaTex;

   float alphaX =  0.0;
   float alphaY =  0.0;

   bool useNormals  = true;

   float posX = 0.0f;
   float posZ = 0.0f;

// Luces y materiales

#define   NLD 1
#define   NLP 1
#define   NLF 2
Light     lightG;
Light     lightD[NLD];
Light     lightP[NLP];
Light     lightF[NLF];

Material  gold;

// Viewport
   int w = 500;
   int h = 500;

int main() {

 // Inicializamos GLFW
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

 // Creamos la ventana
    GLFWwindow* window;
    window = glfwCreateWindow(w, h, "Proyecto Nicolás", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

 // Configuramos los CallBacks
    glfwSetFramebufferSizeCallback(window, funFramebufferSize);
    glfwSetKeyCallback      (window, funKey);
    glfwSetScrollCallback   (window, funScroll);
    glfwSetCursorPosCallback(window, funCursorPos);

 // Entramos en el bucle de renderizado
    configScene();
    while(!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void configScene() {

 // Test de profundidad
    glEnable(GL_DEPTH_TEST);

 // Transparencias
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Modelos
    plane.initModel("resources/models/plane.obj");
    sphere.initModel("resources/models/sphere.obj");


 // Imagenes (texturas)
   noEmissive.initTexture("resources/textures/black.png");

   sueloDiffuse.initTexture("resources/textures/Suelo_Diffuse.png");
   sueloNormal.initTexture("resources/textures/Suelo_Normal.png");
   sueloSpecular.initTexture("resources/textures/Suelo_Specular.png");

   sueloTex.diffuse  = sueloDiffuse.getTexture();
   sueloTex.normal   = sueloNormal.getTexture();
   sueloTex.specular = sueloSpecular.getTexture();
   sueloTex.emissive = noEmissive.getTexture();
   sueloTex.shininess  = 5.0f;


   paredLadrilloDiffuse.initTexture("resources/textures/ParedLadrillo_Diffuse.png");
   paredLadrilloNormal.initTexture("resources/textures/ParedLadrillo_Normal.png");
   paredLadrilloSpecular.initTexture("resources/textures/ParedLadrillo_Specular.png");

   paredLadrilloTex.diffuse = paredLadrilloDiffuse.getTexture();
   paredLadrilloTex.normal = paredLadrilloNormal.getTexture();
   paredLadrilloTex.specular = paredLadrilloSpecular.getTexture();
   paredLadrilloTex.emissive = noEmissive.getTexture();
   paredLadrilloTex.shininess  = 3.0f;

   paredAreniscaDiffuse.initTexture("resources/textures/ParedArenisca_Diffuse.png");
   paredAreniscaNormal.initTexture("resources/textures/ParedArenisca_Normal.png");
   paredAreniscaSpecular.initTexture("resources/textures/ParedArenisca_Specular.png");

   paredAreniscaTex.diffuse = paredAreniscaDiffuse.getTexture();
   paredAreniscaTex.normal = paredAreniscaNormal.getTexture();
   paredAreniscaTex.specular = paredAreniscaSpecular.getTexture();
   paredAreniscaTex.emissive = noEmissive.getTexture();
   paredAreniscaTex.shininess = 5.0f;



   gold.ambient   = glm::vec4(0.247250, 0.199500, 0.074500, 1.00);
   gold.diffuse   = glm::vec4(0.751640, 0.606480, 0.226480, 1.00);
   gold.specular  = glm::vec4(0.628281, 0.555802, 0.366065, 1.00);
   gold.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
   gold.shininess = 51.2;

   // Luz ambiental global
   lightG.ambient = glm::vec3(0.5, 0.5, 0.5);

   // Luces direccionales
   lightD[0].direction = glm::vec3(0.0, -1.0, 0.0);
   lightD[0].ambient   = glm::vec3( 0.0, 0.0, 0.0);
   lightD[0].diffuse   = glm::vec3( 0.0, 0.0, 0.0);
   lightD[0].specular  = glm::vec3( 0.0, 0.0, 0.0);

   // Luces posicionales
   lightP[0].position    = glm::vec3(posX, 3.0, posZ);
   lightP[0].ambient     = glm::vec3(0.2, 0.2, 0.2);
   lightP[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
   lightP[0].specular    = glm::vec3(0.9, 0.9, 0.9);
   lightP[0].c0          = 1.00;
   lightP[0].c1          = 0.22;
   lightP[0].c2          = 0.20;

   // Luces focales
   lightF[0].position    = glm::vec3(2.0,  1.0,  2.0);
   lightF[0].direction   = glm::vec3( 2.0, -2.0, -5.0);
   lightF[0].ambient     = glm::vec3( 0.2,  0.2,  0.2);
   lightF[0].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
   lightF[0].specular    = glm::vec3( 0.9,  0.9,  0.9);
   lightF[0].innerCutOff = 10.0;
   lightF[0].outerCutOff = lightF[0].innerCutOff + 5.0;
   lightF[0].c0          = 1.000;
   lightF[0].c1          = 0.090;
   lightF[0].c2          = 0.032;
   lightF[1].position    = glm::vec3( 2.0,  2.0,  -2.0);
   lightF[1].direction   = glm::vec3(0.0, 0.0, 0.0);
   lightF[1].ambient     = glm::vec3( 0.2,  0.2,  0.2);
   lightF[1].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
   lightF[1].specular    = glm::vec3( 0.9,  0.9,  0.9);
   lightF[1].innerCutOff = 5.0;
   lightF[1].outerCutOff = lightF[1].innerCutOff + 1.0;
   lightF[1].c0          = 1.000;
   lightF[1].c1          = 0.090;
   lightF[1].c2          = 0.032;

 // Materiales

}

void renderScene() {

 // Borramos el buffer de color
   glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();

 // Matriz P
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(60.0f), aspect, nplane, fplane);

 // Matriz V
   float x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
   float y = 10.0f*glm::sin(glm::radians(alphaY));
   float z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
   glm::vec3 eye   (  x,   y,   z);
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    glm::vec3 up    (0.0f, 1.0f, 0.0f);
    glm::mat4 V = glm::lookAt(eye, center, up);
    shaders.setVec3("ueye",eye);

   lightP[0].position = glm::vec3(posX, 3.0f, posZ);


 // Fijamos las luces
    setLights(P,V);

  // Flags
   shaders.setBool("uWithNormals",  useNormals);

 // Dibujamos la escena

   glm::mat4 S = glm::scale(I, glm::vec3(5.0f, 1.0f, 5.0f));

   glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 0.0f));

   glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

   // Dibujar con texturas
   drawObjectTex(plane, paredLadrilloTex, P, V, T*R*S);

   drawObjectTex(plane, sueloTex, P, V, S);

      T = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 5.0f, 0.0f));

      R = glm::rotate(I, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

      drawObjectTex(plane, paredAreniscaTex, P, V, T*R*S);

}

void setLights(glm::mat4 P, glm::mat4 V) {
   shaders.setLight("ulightG",lightG);
   for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
   for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
   for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

   for(auto & i : lightP) {
      glm::mat4 M = glm::translate(I,i.position) * glm::scale(I,glm::vec3(0.1));
      drawObjectMat(sphere, gold, P, V, M);
   }

   for(auto & i : lightF) {
      glm::mat4 M = glm::translate(I,i.position) * glm::scale(I,glm::vec3(0.025));
      drawObjectMat(sphere, gold, P, V, M);
   }
}

void drawObjectMat(Model &model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);

}

void drawObjectTex(Model &model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",false);
    shaders.setTextures("utextures",textures);
    model.renderModel(GL_FILL);

}

void funFramebufferSize(GLFWwindow* window, int width, int height) {

 // Configuracion del Viewport
    glViewport(0, 0, width, height);

 // Actualizacion de w y h
    w = width;
    h = height;

}

void funKey(GLFWwindow* window, int key, int scancode, int action, int mods) {

   if(action == GLFW_PRESS) {

      switch(key) {

      case GLFW_KEY_N:
         useNormals = !useNormals;
         std::cout << "[N] Normal Mapping: "
                   << (useNormals ? "ON\n" : "OFF\n");
         break;

      case GLFW_KEY_RIGHT: posZ += 0.5f; std::cout << "RIGHT\n"; break;
      case GLFW_KEY_LEFT:  posZ -= 0.5f; std::cout << "LEFT\n";break;
      case GLFW_KEY_UP:    posX += 0.5f; std::cout << "UP\n";break;
      case GLFW_KEY_DOWN:  posX -= 0.5f; std::cout << "DOWN\n";break;

      case GLFW_KEY_ESCAPE:
         glfwSetWindowShouldClose(window, true);
         break;
      default:
         break;
      }
   }
}


void funScroll(GLFWwindow* window, double xoffset, double yoffset) {

}

void funCursorPos(GLFWwindow* window, double xpos, double ypos) {

   if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE) return;

   float limY = 89.0;
   alphaX = 90.0f*(2.0*xpos/(float)w - 1.0);
   alphaY = 90.0f*(1.0 - 2.0*ypos/(float)h);
   if(alphaY<-limY) alphaY = -limY;
   if(alphaY> limY) alphaY =  limY;

}
