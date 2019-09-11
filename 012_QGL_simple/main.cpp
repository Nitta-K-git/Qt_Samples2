#include <QApplication>
#include <QGLWidget>
#include <gl/GLU.h>
#include <QVector3D>

class GLWidget : public QGLWidget{
private:
	QVector<QVector3D> m_vertices;
	
public:
	GLWidget(QWidget *parent = nullptr):QGLWidget(parent){
		m_vertices << QVector3D(+0.0f, +1.0f, +0.0f)
				   << QVector3D(+1.0f, -1.0f, +0.0f)
				   << QVector3D(-1.0f, -1.0f, +0.0f)
					  ;
	}
	void initializeGL(){
		//		// Initialize OpenGL Backend
		//		initializeGLFunctions();
	}
	void resizeGL(int w, int h){
		glViewport( 0, 0, (GLint)w, (GLint)h);
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( 45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0 );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
	}
	void paintGL(){
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		
		glTranslatef(0,0,-5);
		glBegin(GL_TRIANGLES);
		for(auto v: m_vertices){
			glVertex3f(v.x(), v.y(), v.z());
		}
		glEnd();
	}
	~GLWidget(){
	}
};

int main(int argc, char *argv[]){
	QSurfaceFormat fmt;
	fmt.setVersion(4,0);//versionを指定
	fmt.setProfile(QSurfaceFormat::CoreProfile);//coreProfileを使う
	QSurfaceFormat::setDefaultFormat(fmt);//以上の設定を適応
	
	QApplication a(argc, argv);
	
	GLWidget w;
	w.show(); //表示
	return a.exec();
}
