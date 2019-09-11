#include <QApplication>
//#include "window.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions{
private:
	// OpenGL State Information
	QOpenGLBuffer m_vertex;
	QOpenGLVertexArrayObject m_object;
	QOpenGLShaderProgram *m_program;
	
	// Private Helpers
	//  void printVersionInformation();
	QVector<GLfloat> m_vertices;
	
	const char *vshader_src =
			"#version 400 core\n"
			"layout(location = 0) in vec4 position;\n"
			"void main(){\n"
			"gl_Position = position;\n"
			"}\n";
	const char *fshader_src =
			"#version 400 core\n"
			"out vec4 color;\n"
			"void main(){\n"
			"color = vec4(1.0,1.0,1.0,1.0);\n"
			"}\n";
public:
	GLWidget(QWidget *parent = nullptr):QOpenGLWidget(parent){
		m_vertices << -0.5f << -0.5f << 0.0f //三角形
                   <<  0.0f <<  0.5f << 0.0f
                   <<  0.5f << -0.5f << 0.0f;
	}
	void initializeGL(){
		// Initialize OpenGL Backend
		initializeOpenGLFunctions();
//		printVersionInformation();
		
		// Set global information
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		// Application-specific initialization
		{
			// Create Shader (Do not release until VAO is created)
			m_program = new QOpenGLShaderProgram();
//			m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/simple.vert");
//			m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/simple.frag");
			m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vshader_src);
			m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fshader_src);
			m_program->link();
			m_program->bind();
			
			// Create Buffer (Do not release until VAO is created)
			m_vertex.create();
			m_vertex.bind();
			m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
			m_vertex.allocate(m_vertices.constData(), sizeof(GLfloat)*m_vertices.size());
			
			// Create Vertex Array Object
			m_object.create();
			m_object.bind();
//			m_program->enableAttributeArray(0);
//			m_program->enableAttributeArray(1);
//			m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
//			m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
			
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
			glEnableVertexAttribArray(0);
			// Release (unbind) all
			m_object.release();
			m_vertex.release();
			m_program->release();
		}
		
	}
	void resizeGL(int width, int height){}
	void paintGL(){
		// Clear
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Render using our shader
		m_program->bind();
		{
			m_object.bind();
			glDrawArrays(GL_TRIANGLES, 0, m_vertices.size()/3);
			m_object.release();
		}
		m_program->release();
	}
	~GLWidget(){
		// Actually destroy our OpenGL information
		m_object.destroy();
		m_vertex.destroy();
		delete m_program;
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
