// 参考
// https://qiita.com/reqko/items/62bce521ec740e5f1774
// https://qiita.com/mocobt/items/b43b3fe5f7d4e4eedaa5

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector>
#include <QtOpenGL>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions{
private:
    GLuint m_program;

    GLuint m_vao;
    GLuint m_vbo;

    QVector<GLfloat> m_vertices;
	QOpenGLVertexArrayObject m_object;

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
    //コンストラクタの次に呼ばれる
    void initializeGL(){
        initializeOpenGLFunctions();//初期化
        glClearColor(0.0f,0.0f,0.0f,1.0f);

        m_program = glCreateProgram();
        //vertex shaderの作成
        GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vshader,1,&vshader_src,0);
        glCompileShader(vshader);
        //fragment shaderの作成
        GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fshader,1,&fshader_src,0);
        glCompileShader(fshader);
        //プログラムにアタッチ
        glAttachShader(m_program,vshader);
        glAttachShader(m_program,fshader);
        //リンク
        glLinkProgram(m_program);
        //削除
        glDeleteShader(vshader);
        glDeleteShader(fshader);

        //vaoの作成
//        glGenVertexArrays(1,&m_vao);
		m_object.create();
//        glBindVertexArray(m_vao);
		m_object.bind();
        //vboの作成
        glGenBuffers(1,&m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
        glBufferData(GL_ARRAY_BUFFER,m_vertices.size()*sizeof(GLfloat), m_vertices.constData(),GL_STATIC_DRAW);
        //vertex shaderのコードで頂点座標のロケーションは0に指定済
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(0);
    }

    void paintGL(){
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(m_program);
//        glBindVertexArray(m_vao);
		m_object.bind();
        glDrawArrays(GL_TRIANGLES,0,m_vertices.size()/3); //ドローコール
    }
};
#endif // GLWIDGET_H
