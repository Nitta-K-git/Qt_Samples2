#include <QApplication>
#include "widget.h"

QGLViewerWidget::QGLViewerWidget(QWidget *_parent)
	: QGLWidget(_parent) {
}

QGLViewerWidget::~QGLViewerWidget() {}

void QGLViewerWidget::initializeGL() {
	// OpenGL state
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glDisable(GL_DITHER);
	glEnable(GL_DEPTH_TEST);
}

void QGLViewerWidget::resizeGL(int _w, int _h) {
	glViewport(0, 0, _w, _h);
	updateGL();
}

void QGLViewerWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
}
