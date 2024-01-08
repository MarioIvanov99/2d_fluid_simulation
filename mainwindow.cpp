
#include "mainwindow.h"

// constructor / destructor
mainwindow::mainwindow(QWidget *parent)
    : QWidget(parent)
    { // constructor

    // create menu bar
    menuBar = new QMenuBar(this);

    // create file menu
    fileMenu = menuBar->addMenu("&File");

    // create the action
    actionQuit = new QAction("&Quit", this);

    // leave signals & slots to the controller

    // add the item to the menu
    fileMenu->addAction(actionQuit);

    // create the window layout
    windowLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    // create main widget
    cubeWidget = new widget(this);
    windowLayout->addWidget(cubeWidget);

    // create Y slider
    densitySlider = new QSlider(Qt::Horizontal);
	// sets Y slider min and max
    densitySlider->setMinimum(200);
    densitySlider->setMaximum(400);
    densitySlider->setValue(300);
    windowLayout->addWidget(densitySlider);
	// create Z slider
    gasSlider = new QSlider(Qt::Horizontal);
	// sets Z slider min and max
    gasSlider->setMinimum(500);
    gasSlider->setMaximum(1500);
    gasSlider->setValue(1000);
    windowLayout->addWidget(gasSlider);
	// create door slider
    viscSlider = new QSlider(Qt::Horizontal);
	// sets door slider min and max
    viscSlider->setMinimum(100);
    viscSlider->setMaximum(700);
    viscSlider->setValue(400);
    windowLayout->addWidget(viscSlider);
  // create door slider 2
    massSlider = new QSlider(Qt::Horizontal);
  // sets door slider 2 min and max
    massSlider->setMinimum(2);
    massSlider->setMaximum(10);
    massSlider->setValue(5);
    windowLayout->addWidget(massSlider);
  // create speed slider
    hSlider = new QSlider(Qt::Horizontal);
  // sets door slider min and max
    hSlider->setMinimum(15);
    hSlider->setMaximum(30);
    hSlider->setValue(20);
    windowLayout->addWidget(hSlider);


    ptimer = new QTimer(this);

    ptimer->start(20);

    connect(ptimer, SIGNAL(timeout()),  cubeWidget, SLOT(updatePos()));
	  //connects sliders to their various update functions
    connect(densitySlider, SIGNAL(valueChanged(int)),  cubeWidget, SLOT(updateDensity(int)));
    connect(gasSlider, SIGNAL(valueChanged(int)),  cubeWidget, SLOT(updateGas(int)));
    connect(viscSlider, SIGNAL(valueChanged(int)),  cubeWidget, SLOT(updateVsicosity(int)));
    connect(massSlider, SIGNAL(valueChanged(int)),  cubeWidget, SLOT(updateMass(int)));
    connect(hSlider, SIGNAL(valueChanged(int)),  cubeWidget, SLOT(updateH(int)));


    } // constructor

mainwindow::~mainwindow()
    { // destructor
    delete ptimer;
    delete densitySlider;
    delete gasSlider;
    delete viscSlider;
    delete massSlider;
    delete hSlider;
    delete cubeWidget;
    delete windowLayout;
    delete actionQuit;
    delete fileMenu;
    delete menuBar;
    } // destructor

// resets all the interface elements
void mainwindow::ResetInterface()
    { // ResetInterface()

    //don't use the slider for now

    //	nVerticesSlider->setValue(thePolygon->nVertices);

    // now force refresh
    cubeWidget->update();
    update();
    } // ResetInterface()
