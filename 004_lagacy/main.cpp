#include "widget.h"

#include <QApplication>

#include <QMenu>
#include <QtWidgets>
#include <QMessageBox>

int main( int argc, char **argv )
{
	QApplication app(argc, argv);
	
	// create widget
	QMainWindow mainWin;
	QGLViewerWidget w(&mainWin);
	mainWin.setCentralWidget(&w);
	mainWin.resize(640, 480);
	mainWin.show();
	return app.exec();
}
