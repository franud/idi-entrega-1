
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersQuadrat();
  creaBuffersTriangle();
}

void MyGLWidget::pintaMuntanya ()  // Caldrà modificar aquest mètode per pintar la muntanya
{
  // pintem un triangle
  glBindVertexArray(VAOTriangle);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MyGLWidget::modelTransformBarraGronxador () {
  glm::mat4 TGBarraGronxador = glm::mat4 (1.0f);
  TGBarraGronxador = glm::translate (TGBarraGronxador, glm::vec3 (0.0f, -0.7f, 0.0f));
  TGBarraGronxador = glm::rotate (TGBarraGronxador, glm::radians (angleGir), glm::vec3 (0.0f, 0.0f, 1.0f));
  TGBarraGronxador = glm::scale (TGBarraGronxador, 2.0f * glm::vec3 (1.4f, 0.2f, 0.0f));
  TGBarraGronxador = glm::translate (TGBarraGronxador, glm::vec3 (0.25f, 0.5f, 0.0f));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGBarraGronxador[0][0]);
}


/* Creo que esta solución no es la más elegante. Si un profesor corrige esto manualmente, los movimientos son: 
1. Mover la base al centro de coordenadas
2. Escalo por dos el cuadrado (porque sus lados no miden 1x1 y para facilitar los calculos)
3. Muevo el asiento a su posición cuando la barra está con 0 grados de inclinación
4. Lo muevo 0.7 en el eje Y que es donde estaria el asiento si la barra estuviese con el centro en (0,0)
5. Hago en giro de angleGir grados
6. Lo muevo -0.7 en el eje Y para ponerlo en su posición

Me gustaría saber cual es la forma en la que lo quereis vosotros, este es mi correo:
jose.francisco.udaeta@estudiantat.upc.edu

Gracias
 */
void MyGLWidget::modelTransformSeient (glm::vec3 posBase) {
  glm::mat4 TGBarraGronxador = glm::mat4 (1.0f);
  TGBarraGronxador = glm::translate (TGBarraGronxador, glm::vec3 (0.0f, -0.7f, 0.0f));  
  TGBarraGronxador = glm::rotate (TGBarraGronxador, glm::radians (angleGir), glm::vec3 (0.0f, 0.0f, 1.0f));
  TGBarraGronxador = glm::translate (TGBarraGronxador, glm::vec3 (0.0f, 0.7f, 0.0f));
  TGBarraGronxador = glm::translate (TGBarraGronxador, posBase);
  TGBarraGronxador = glm::scale (TGBarraGronxador, 2.0f * glm::vec3 (0.2f, 0.1f, 0.0f));
  TGBarraGronxador = glm::translate (TGBarraGronxador, glm::vec3 (0.25f, 0.5f, 0.0f));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGBarraGronxador[0][0]);
}

void MyGLWidget::pintaGronxador ()  
{ // Caldrà modificar aquest mètode per pintar el gronxador sencer, la barra i els dos seients
  // pintem un quadrat 
  glBindVertexArray(VAOQuadrat);
  //Barra Gronxador
  modelTransformBarraGronxador ();
  send_color (0.2f, 0.7f, 0.2f);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  //Seient Esquerra
  send_color (0.4f, 0.4f, 0.4f);
  modelTransformSeient (glm::vec3 (-0.6f, -0.5f, 0.0f));
  glDrawArrays(GL_TRIANGLES, 0, 6);
  //Seient Dreta
  send_color (0.4f, 0.4f, 0.4f);
  modelTransformSeient (glm::vec3 (0.6f, -0.5f, 0.0f));
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Pintem muntanya
  modelTransformMuntanya ();
  send_color (0.5f, 0.3f, 0.1f);
  pintaMuntanya();

  modelTransformBaseGronxador ();
  send_color (0.7f, 0.0f, 0.0f);
  pintaBaseGronxador();
  
  // Pintem gronxador
  //transformGirGronxador(); //No m'ha fet falta la funció pero potser tenia que haber posat aquí el codi del gir...
  pintaGronxador();

  modelTransformNeu();
  send_color (1.0f, 1.0f, 1.0f);
  pintaNeu();
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Left:
      if (angleGir < 25.0f) {
        angleGir = angleGir + 5.0f;
      }
      break;
    
    case Qt::Key_Right:
      if (angleGir > -25.0f) {
        angleGir = angleGir - 5.0f;
      }
      break;
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::transformGirGronxador ()
{
  GirGronxador = glm::mat4(1.0);  
  // Codi per a la TG necessària
  // .....  
}

void MyGLWidget::creaBuffersQuadrat ()
{
  glm::vec3 Vertices[6];  // vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-0.5, -0.5, 0);
  Vertices[1] = glm::vec3( 0, -0.5, 0);
  Vertices[2] = glm::vec3(-0.5, 0, 0);
  Vertices[3] = glm::vec3( 0, -0.5, 0);
  Vertices[4] = glm::vec3( 0, 0, 0);
  Vertices[5] = glm::vec3( -0.5, 0, 0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar el tronc
  glGenVertexArrays(1, &VAOQuadrat);
  glBindVertexArray(VAOQuadrat);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersTriangle ()
{
  glm::vec3 Vertices[3];  // Quatre vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3( 0, 0, 0);
  Vertices[1] = glm::vec3( 1, 0, 0);
  Vertices[2] = glm::vec3( 0.5, 1, 0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar el tronc
  glGenVertexArrays(1, &VAOTriangle);
  glBindVertexArray(VAOTriangle);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim els identificadors dels uniforms
  transLoc = glGetUniformLocation (program->programId(), "TG");
  colorLoc = glGetUniformLocation (program->programId(), "color");
  //...
}

void MyGLWidget::pintaNeu () {
  pintaMuntanya();
}

void MyGLWidget::modelTransformNeu () {
  glm::mat4 TGNeu;
  TGNeu = glm::mat4 (1.0f);
  TGNeu = glm::translate (TGNeu, glm::vec3 (-0.2, 0.8f, 0.0f));
  TGNeu = glm::scale (TGNeu, glm::vec3 (0.4f, 0.4f, 1.0f));
  TGNeu = glm::translate (TGNeu, glm::vec3 (-0.5f, -1.0f, 0.0f));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGNeu[0][0]); 
}

void MyGLWidget::modelTransformMuntanya () {
  glm::mat4 TGmountain;
  TGmountain = glm::mat4(1.0f);
  TGmountain = glm::translate(TGmountain, glm::vec3 (-1.0f, -1.0f, 0.0f));
  TGmountain = glm::scale (TGmountain, glm::vec3 (1.6f, 1.8f, 1.0f)); 

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGmountain[0][0]);
}

void MyGLWidget::send_color (float r, float g, float b) {
  glUniform3f (colorLoc, r, g, b);
}

void MyGLWidget::pintaBaseGronxador () {
  pintaMuntanya ();
}

void MyGLWidget::modelTransformBaseGronxador () {
  glm::mat4 TGBaseGronxador;
  TGBaseGronxador = glm::mat4(1.0f);
  TGBaseGronxador = glm::translate (TGBaseGronxador, glm::vec3 (-0.1f, -1.0f, 0.0f));
  TGBaseGronxador = glm::scale (TGBaseGronxador, glm::vec3 (0.2f, 0.3f, 0.0f));

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGBaseGronxador[0][0]);
}