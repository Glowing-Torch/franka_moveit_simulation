#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include "main_window.hpp"

// 获取屏幕的宽度和高度
int main(int argc, char **argv)
{
    ros::init(argc, argv, "qt_ros_service_client");
    QApplication app(argc, argv);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    ROS_INFO("SCREEN_WIDTH%d,screen_height%d", screenWidth, screenHeight);
    MainWindow window;
    window.resize(screenWidth * 0.3, 0.4 * screenHeight);
    window.show();

    return app.exec();
}
