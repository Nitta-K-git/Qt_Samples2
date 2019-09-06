#include <iostream>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QMainWindow>
#include <QApplication>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions{
public:
    MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) { }
	QMatrix4x4 m_proj;
protected:
    void initializeGL() override
    {
        // Set up the rendering context, load shaders and other resources, etc.:
		initializeOpenGLFunctions();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    void resizeGL(int w, int h) override
    {
        // Update projection matrix and other size related settings:
        m_proj.setToIdentity();
        m_proj.perspective(45.0f, w / float(h), 0.01f, 100.0f);
    }

    void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
	MainWindow(){
		QOpenGLWidget *widget = new QOpenGLWidget(this);
//		QSurfaceFormat format;
//		format.setDepthBufferSize(24);
//		format.setStencilBufferSize(8);
//		format.setVersion(3, 2);
//		format.setProfile(QSurfaceFormat::CoreProfile);
//		widget->setFormat(format); // must be called before the widget or its parent window gets shown
	}
};

int main(int argc, char *argv[]){
	QApplication app(argc, argv);
    std::cout << "hello" << std::endl;
//	MainWindow m;
//	m.show();
    return app.exec();
}
