#include <QApplication>
#include <QSurfaceFormat> //追加
#include "glwidget.h" //追加

int main(int argc, char *argv[])
{
    QSurfaceFormat fmt;
    fmt.setVersion(4,0);//versionを指定
    fmt.setProfile(QSurfaceFormat::CoreProfile);//coreProfileを使う
    QSurfaceFormat::setDefaultFormat(fmt);//以上の設定を適応

    QApplication a(argc, argv);

    GLWidget w;
    w.show(); //表示
    return a.exec();
}

