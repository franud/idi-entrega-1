#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void creaBuffersQuadrat();
    void creaBuffersTriangle();
    void carregaShaders();

    void pintaMuntanya(); //Done
    void pintaBaseGronxador(); //Done
    void pintaGronxador();
    void pintaNeu();
    
    void modelTransformMuntanya(); //Done
    void modelTransformBaseGronxador(); //Done
    void modelTransformBarraGronxador();
    void modelTransformSeient(glm::vec3 posBase);
    void modelTransformNeu();
    void transformGirGronxador(); 
    void send_color (float R, float G, float B); //Done

    // program
    QOpenGLShaderProgram *program;
    // attribute locations
    GLuint vertexLoc;
    // uniform locations
    GLuint transLoc, colorLoc;
    // ...

    // VAOs
    GLuint VAOTriangle, VAOQuadrat;

    // viewport
    GLint ample, alt;

    // color objecte
    glm::vec3 colObj;
    // matriu per girar el gronxador i angle gir
    glm::mat4 GirGronxador;
    float angleGir = 0.0;
};
