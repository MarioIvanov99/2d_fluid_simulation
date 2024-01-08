#include <GL/glu.h>
#include <GL/glut.h>
#include <QGLWidget>
#include "widget.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


// constructor
widget::widget(QWidget *parent):
  QGLWidget(parent),
  dense(300.0),
  gas(1000.0),
  viscosity(400.0),
  mass(2.5),
  h(20.0),
  deltat(0.0005)
    { // constructor


    } // constructor


// called when OpenGL context is set up
void widget::initializeGL()
    { // initializeGL()
    // set the widget background colour
    glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	  glEnable(GL_POINT_SMOOTH);
	  glPointSize(h/2.0);
	  glMatrixMode(GL_PROJECTION);
    SetParticles();

    } // initializeGL()


// called every time the widget is resized
void widget::resizeGL(int w, int h)
    { // resizeGL()
    // set the viewport to the entire widget

    } // resizeGL()

// called to create the vector of particles
void widget::SetParticles(){

  if(particles.size() > 0)
    particles.clear();

  for (float i = 20; i < 300; i += h/2.0){

    for (float j = 150; j <= 300; j += h/2.0){

        Particle particle; // Particle to put inside the vector
        particle.density = 0.0;
        particle.pressure = 0.0;
        particle.pos.setX(j);
        particle.pos.setY(i);
    	particles.push_back(particle);

    }

  }

}

// called to integrate through leapfrog
void widget::leapfrog(){

  for(int i = 0; i < particles.size(); i++){

    //Leapfrog integration
    particles[i].velocity += deltat*particles[i].acceleration/2.0;
    particles[i].pos += deltat*particles[i].velocity;
    particles[i].acceleration = particles[i].force/particles[i].density;
    particles[i].velocity += deltat*particles[i].acceleration/2.0;

    //Collision with sides
    //Reverse velocity
    //This is done so that particles dont stick to the sides
    if (particles[i].pos.x()-h < 0.0){

      particles[i].velocity.setX(particles[i].velocity.x() * -0.5);
    	particles[i].pos.setX(h);

    }
    if(particles[i].pos.x()+h > 600){

      particles[i].velocity.setX(particles[i].velocity.x() * -0.5);
    	particles[i].pos.setX(600-h);

    }
    if(particles[i].pos.y()-h < 0.0)
    {
      particles[i].velocity.setY(particles[i].velocity.y() * -0.5);
    	particles[i].pos.setY(h);

    }
    if(particles[i].pos.y()+h > 450){

      particles[i].velocity.setY(particles[i].velocity.y() * -0.5);
    	particles[i].pos.setY(600-h);

    }

  }

}

// calculate the poly6 kernel
float widget::poly6(float h2, float r2){

  return 4.0/(M_PI * pow(sqrt(h2), 8.0))*pow(h2-r2, 3.0); //2D equivalent

}

// calculate the poly6 kernel gradient
float widget::poly6grad(float h, float r){

  float h2 = h*h;
  float r2 = r*r;

  return -24.0/(M_PI * pow(h, 8.0))*pow(h2-r2, 2.0); //2D equivalent

}

// calculate the spiky kernel gradient
float widget::spiky(float h, float r){

  return -30.0/(M_PI * pow(h, 5.0))*pow(h-r, 2.0); //2D equivalent

}

// calculate the poly6 kernel laplacian
float widget::poly6lap(float h, float r){

  float h2 = h*h;
  float r2 = r*r;
  return -24.0/(M_PI * pow(h, 8.0))*(3*h2-7*r2);

}

// calculate the viscosity kernel laplacian
float widget::visc(float h, float r){

  return 20.f/(M_PI * pow(h, 5.f))*(h-r);

}

// calculate a prarticles pressure and density
void widget::DensityAndPressure(){

  float h2 = h*h;

  for(int i = 0; i < particles.size(); i++){

    particles[i].density = 0.0;

    for(int j = 0; j < particles.size(); j++){

    	QVector2D r = particles[j].pos - particles[i].pos;
    	float r2 = QVector2D::dotProduct(r, r);

    	if (sqrt(r2)>=0 && sqrt(r2)<=sqrt(h2)){ //Otherwise 0

    		particles[i].density += mass*poly6(h2 ,r2);

    	}

    }

    particles[i].pressure = gas*(particles[i].density - dense);

  }

}

void widget::TotalForce(){

  QVector2D gravity(0.0, -9.8);

	for(int i = 0; i < particles.size(); i++){

		QVector2D pressureForce;
		QVector2D viscForce;
    QVector2D tensionForce;
    QVector2D normal;
    float lap = 0.0;
    float sigma = 10000000.0;
    QVector2D gravForce = gravity*mass/particles[i].density;

		for(int j = 0; j < particles.size(); j++){

			QVector2D rvec = particles[j].pos - particles[i].pos;
			float r = sqrt(QVector2D::dotProduct(rvec, rvec));

			if(r>0 && r<h){

        if(task1) // Pressure equation with the poly6 kernel
				  pressureForce += rvec*mass*(particles[i].pressure+particles[j].pressure)/(2.0*particles[j].density)*poly6grad(h, r);
        if(task2) // Pressure equation with the spiky kernel
          pressureForce += rvec.normalized()*mass*(particles[i].pressure+particles[j].pressure)/(2.0*particles[j].density)*spiky(h, r);
				if(task3) // Viscosity equation with the viscosity kernel
				  viscForce +=  mass*(particles[j].velocity-particles[i].velocity)/particles[j].density*visc(h, r);
        if(task4){ //Surface tension with the poly6 kernel
          normal += rvec*mass/particles[j].density*poly6grad(h, r);
          lap += mass/particles[j].density*poly6lap(h, r);
        }

			}

		}

    pressureForce = -pressureForce;
    viscForce *= viscosity;
    tensionForce = -sigma*lap*normal.normalized();
		particles[i].force = pressureForce + viscForce + tensionForce + gravForce;

	}

}

void widget::updatePos(){

  QWidget::setFocus(); //For key event hanfling
  if(run){ // Paused at the start

    DensityAndPressure();
  	TotalForce();
  	leapfrog();
    this->repaint();

  }

}

void widget::updateDensity(int a){

  dense = (float) a;

}

void widget::updateGas(int a){

  gas = (float) a;

}

void widget::updateVsicosity(int a){

  viscosity = (float) a;

}

void widget::updateMass(int a){

  mass = (float) a/2.0;

}

void widget::updateH(int a){

  h = (float) a;

}

void widget::keyPressEvent(QKeyEvent *event){
  if(event->key() == Qt::Key_R){
      run = false;
      SetParticles();
      this->repaint();
    }
  if(event->key() == Qt::Key_1){
    run = true;
    task1 = true;
    task2 = false;
    task3 = false;
    task4 = false;
  }
  if(event->key() == Qt::Key_2){
    run = true;
    task1 = false;
    task2 = true;
    task3 = false;
    task4 = false;
  }
  if(event->key() == Qt::Key_3){
    run = true;
    task1 = false;
    task2 = true;
    task3 = true;
    task4 = false;
  }
  if(event->key() == Qt::Key_4){
    run = true;
    task1 = false;
    task2 = true;
    task3 = true;
    task4 = true;
  }

}

// called every time the widget needs painting
void widget::paintGL()
    { // paintGL()

    glClear(GL_COLOR_BUFFER_BIT);
  	glLoadIdentity();
    glViewport(0, 0, 600, 450);
  	glOrtho(0, 600, 0, 450, 0, 1);

  	glColor3f(0.4, 0.4, 1.0);

  	glBegin(GL_POINTS);
  	for (int i = 0; i < particles.size(); i++){

  		glVertex2f(particles[i].pos.x(), particles[i].pos.y());

  	}
  	glEnd();


    // flush to screen
    glFlush();

    } // paintGL()
