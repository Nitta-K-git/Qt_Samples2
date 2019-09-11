/*
 * 複数のVAO,VBO,モデル変換行列を使用してオブジェクト毎に違う座標変換を実行する
*/

#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QKeyEvent>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions{
private:
	// OpenGL State Information
	QOpenGLBuffer m_vbo, m_vbo2;
	QOpenGLVertexArrayObject m_vao, m_vao2;
	QOpenGLShaderProgram *m_program;
	QVector<GLfloat> m_vertices, m_vertices2;
	QMatrix4x4 m_proj;
	QMatrix4x4 m_camera;
	QMatrix4x4 m_world, m_world2;
	int m_projLoc;
	int m_modelLoc;
	
	const char *vshader_src =
			"#version 400 core\n"
			"layout(location = 0) in vec4 position;\n"
			"uniform mat4 projMatrix;\n"
			"uniform mat4 modelMatrix;\n"
			"void main(){\n"
			"gl_Position = projMatrix * modelMatrix * position;\n"
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
		m_vertices2 << -0.5f << 0.5f << 0.0f //三角形
				   <<  0.0f <<  1.5f << 0.0f
					<<  0.5f << 0.5f << 0.0f;
		m_world.setToIdentity();
		m_world2.setToIdentity();
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
			m_modelLoc = m_program->uniformLocation("modelMatrix");
			// Create Buffer (Do not release until VAO is created)
			m_vbo.create();
			m_vbo.bind();
			m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
			m_vbo.allocate(m_vertices.constData(), sizeof(GLfloat)*m_vertices.size());
			
			// Create Vertex Array Object
			m_vao.create();
			m_vao.bind();
			//			m_program->enableAttributeArray(0);
			//			m_program->enableAttributeArray(1);
			//			m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
			//			m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
			
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
			glEnableVertexAttribArray(0);
			// Release (unbind) all
			m_vao.release();
			m_vbo.release();
			
			m_vbo2.create();
			m_vbo2.bind();
			m_vbo2.setUsagePattern(QOpenGLBuffer::StaticDraw);
			m_vbo2.allocate(m_vertices2.constData(), sizeof(GLfloat)*m_vertices2.size());
			m_vao2.create();
			m_vao2.bind();
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
			glEnableVertexAttribArray(0);
			m_vao2.release();
			m_vbo2.release();
			
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
		
		m_vao.bind();
		m_program->setUniformValue(m_projLoc, m_proj);
		m_program->setUniformValue(m_modelLoc, m_camera * m_world);
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size()/3);
		m_vao.release();
		
		m_vao2.bind();
		m_program->setUniformValue(m_projLoc, m_proj);
		m_program->setUniformValue(m_modelLoc, m_camera * m_world2);
		glDrawArrays(GL_TRIANGLES, 0, m_vertices2.size()/3);
		m_vao2.release();
		
		m_program->release();
	}
	~GLWidget(){
		m_vao.destroy();
		m_vbo.destroy();
		delete m_program;
	}
protected:
	void keyPressEvent(QKeyEvent *event){
		switch (event->key()) {
		case Qt::Key_Escape:
			break;
		case Qt::Key_Return:
			break;
		case Qt::Key_Enter:
			break;
		case Qt::Key_Home:
			break;
		case Qt::Key_Right:
			m_camera.translate(0.5, 0); break;
		case Qt::Key_Left:
			m_camera.translate(-0.5, 0); break;
		case Qt::Key_Down:
			m_camera.translate(0, -0.5); break;
		case Qt::Key_Up:
			m_camera.translate(0, 0.5); break;
		case Qt::Key_Space:
			m_world.rotate(10, 0,1,0);
			m_world2.rotate(20, 0,1,0);
		default:
			break;
		}
		update();
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
