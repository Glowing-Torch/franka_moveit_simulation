#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <ros/ros.h>
#include <QProcess>
#include <std_msgs/String.h>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void callRosService();
    void startRosNode();
    void stopRosNode();

private:
    void initializeControllerState();
    QPushButton *serviceButton;
    QPushButton *startRosButton;
    QPushButton *stopRosButton;

    QLabel *responseLabel;
    QVBoxLayout *layout;
    QProcess *rosProcess; // 用于管理 ROS 进程
    ros::NodeHandle nh;
    ros::ServiceClient client;
    ros::ServiceClient listClient;
    bool isrunning;
};

#endif