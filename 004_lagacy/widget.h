#ifndef WIDGET_H
#define WIDGET_H

#include <QGLWidget>
#include <QVector3D>
#include <QMatrix4x4>
#include <string>
#include <vector>
#include <map>

class QGLViewerWidget : public QGLWidget{
	Q_OBJECT
public:
	QGLViewerWidget( QWidget* _parent=0 );
	virtual ~QGLViewerWidget();
	
private: // inherited
	void initializeGL();
	void paintGL();
	void resizeGL( int w, int h );
};


#endif // WIDGET_H
