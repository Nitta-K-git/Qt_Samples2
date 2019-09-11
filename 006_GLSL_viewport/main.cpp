#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions{
private:
	// OpenGL State Information
	QOpenGLBuffer m_vertex;
	QOpenGLVertexArrayObject m_object;
	QOpenGLShaderProgram *m_program;
	QVector<GLfloat> m_vertices;
	QMatrix4x4 m_proj;
	QMatrix4x4 m_camera;
	QMatrix4x4 m_world;
	int m_projLoc;
	
	const char *vshader_src =
			"#version 400 core\n"
			"layout(location = 0) in vec4 position;\n"
			"uniform mat4 projMatrix;\n"
			"void main(){\n"
			"gl_Position = projMatrix * position;\n"
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
			m_projLoc = m_program->uniformLocation("projMatrix");
			
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
	void resizeGL(int w, int h){
		m_proj.setToIdentity();
//		m_proj.perspective(45.0f, GLfloat(w)/h, 0.01f, 100.0f);
//		QPoint p0(-width, height);
		m_proj.ortho(-w/100., w/100., -h/100., h/100., -1, 1);
	}
	void paintGL(){
		glClear(GL_COLOR_BUFFER_BIT);
		m_program->bind();
		m_object.bind();
		m_program->setUniformValue(m_projLoc, m_proj);
		
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size()/3);
		
		m_object.release();
		m_program->release();
	}
	~GLWidget(){
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
