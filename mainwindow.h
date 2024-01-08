#ifndef __GL_POLYGON_WINDOW_H__
#define __GL_POLYGON_WINDOW_H__ 1

#include <QGLWidget>
#include <QMenuBar>
#include <QSlider>
#include <QTimer>
#include <QBoxLayout>
#include "widget.h"

class mainwindow: public QWidget
    {
    public:


    // constructor / destructor
    mainwindow(QWidget *parent);
    ~mainwindow();

    // visual hierarchy
    // menu bar
    QMenuBar *menuBar;
        // file menu
        QMenu *fileMenu;
            // quit action
            QAction *actionQuit;

    // window layout
    QBoxLayout *windowLayout;

    // beneath that, the main widget
    widget *cubeWidget;
    // and a slider for the number of vertices
    QSlider *densitySlider;
    QSlider *gasSlider;
    QSlider *viscSlider;
    QSlider *massSlider;
    QSlider *hSlider;

    // a timer
    QTimer *ptimer;

    // resets all the interface elements
    void ResetInterface();
    };

#endif
