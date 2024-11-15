#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <ros/ros.h>
#include <std_msgs/String.h>

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
private slots:
    void callRosService();

private:
    QPushButton *serviceButton;
    QLabel *responseLabel;

    ros::NodeHandle nh;
    ros::ServiceClient client;
};

#endif