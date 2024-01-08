#ifndef __GL_POLYGON_WIDGET_H__
#define __GL_POLYGON_WIDGET_H__ 1

#include <QGLWidget>
#include <iostream>
#include <vector>
#include <QVector2D>
#include <QKeyEvent>

typedef struct{ //Particle struct

	QVector2D pos, velocity, acceleration, force;
	float density, pressure;

}Particle;

class widget: public QGLWidget
    { //

    Q_OBJECT

    public:
    widget(QWidget *parent);

    public slots:
    // called by the timer in the main window
    void updatePos();
		// update density
		void updateDensity(int);
		// update gas
		void updateGas(int);
		// update viscosity
		void updateVsicosity(int);
		// update mass
		void updateMass(int);
		// update h
		void updateH(int);

    protected:
    // called when OpenGL context is set up
    void initializeGL();
    // called every time the widget is resized
    void resizeGL(int w, int h);
    // called every time the widget needs painting
    void paintGL();
    // called to create the vector of particles
    void SetParticles();
		// called to integrate through leapfrog
    void leapfrog();
		// calculate a prarticles pressure and density
    void DensityAndPressure();
		// calculate the poly6 kernel
		float poly6(float, float);
		// calculate the poly6 kernel gradient
		float poly6grad(float, float);
		// calculate the poly6 kernel laplacian
		float poly6lap(float, float);
		// calculate the spiky kernel gradient
		float spiky(float, float);
		// calculate the viscosity kernel laplacian
		float visc(float, float);
		// total force
    void TotalForce();
		//Key handling
		void keyPressEvent(QKeyEvent *event);

    private:

    float dense, gas, mass, viscosity, h, deltat;
		bool run=false, task1, task2, task3, task4;
    std::vector<Particle> particles;

    }; // class PlaneWidget

#endif
