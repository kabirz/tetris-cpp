#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    QApplication::setApplicationName("俄罗斯方块");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("Tetris");
    
    // 创建并显示主窗口
    MainWindow window;
    window.show();
    
    return app.exec();
}