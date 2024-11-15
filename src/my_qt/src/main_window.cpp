#include "main_window.hpp"
#include <QMessageBox>
#include <controller_manager_msgs/SwitchController.h>
#include <controller_manager_msgs/ListControllers.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置窗口标题
    setWindowTitle("Qt ROS");
    // isrunning = false;
    // 初始化 ROS 服务客户端
    listClient = nh.serviceClient<controller_manager_msgs::ListControllers>("/controller_manager/list_controllers");
    client = nh.serviceClient<controller_manager_msgs::SwitchController>("/controller_manager/switch_controller");
    // 创建按钮
    rosProcess = new QProcess(this);
    startRosButton = new QPushButton("Start Simulation", this);
    stopRosButton = new QPushButton("Stop Simulation", this);

    // 设置布局
    layout = new QVBoxLayout;
    layout->addWidget(startRosButton);
    layout->addWidget(stopRosButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // 连接按钮点击信号到槽函数
    connect(startRosButton, &QPushButton::clicked, this, &MainWindow::startRosNode);
    connect(stopRosButton, &QPushButton::clicked, this, &MainWindow::stopRosNode);
}
MainWindow::~MainWindow()
{

}
void MainWindow::initializeControllerState()
{
    isrunning=false;
    controller_manager_msgs::ListControllers srv;
    listClient.waitForExistence(ros::Duration(3));
    if (listClient.call(srv))
    {
        for (const auto &controller : srv.response.controller)
        {
            if (controller.name == "effort_joint_trajectory_controller" && controller.state == "running")
            {
                isrunning = true;
                break;
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "Service Error", "Failed to get controller status!");
    }
}
void MainWindow::startRosNode()
{
    QString command = "roslaunch panda_moveit_config demo_gazebo.launch";
    rosProcess->start("bash", QStringList() << "-c" << command);

    if (!rosProcess->waitForStarted())
    {
        QMessageBox::warning(this, "ROS Node Error", "Failed to start ROS node!");
    }
    else
    {
        QMessageBox::information(this, "ROS Node", "ROS Node started successfully!");
        initializeControllerState();
        if (isrunning)
        {
            serviceButton = new QPushButton("Stop Controller", this);
        }
        else
        {
            serviceButton = new QPushButton("Start Controller", this);
        }
        responseLabel = new QLabel("Response: ", this);
        layout->addWidget(serviceButton);
        layout->addWidget(responseLabel);
        connect(serviceButton, &QPushButton::clicked, this, &MainWindow::callRosService);
    }
}

void MainWindow::stopRosNode()
{
    if (rosProcess->state() == QProcess::Running)
    {
        rosProcess->terminate(); // 发送 SIGTERM 信号
        QProcess pkillGzserver;
        pkillGzserver.start("pkill", QStringList() << "-f" << "gzserver");
        pkillGzserver.waitForFinished();

        QProcess pkillGzclient;
        pkillGzclient.start("pkill", QStringList() << "-f" << "gzclient");
        pkillGzclient.waitForFinished();
        if (!rosProcess->waitForFinished(2000))
        {
            rosProcess->kill(); // 强制终止进程
        }
        QMessageBox::information(this, "ROS Node", "ROS Node stopped successfully!");
        responseLabel->deleteLater();
        serviceButton->deleteLater();
    }
    else
    {
        QMessageBox::information(this, "ROS Node", "ROS Node is not running!");
    }
}

void MainWindow::callRosService()
{
    controller_manager_msgs::SwitchController srv;
    initializeControllerState();
    if (isrunning)
    {
        srv.request.stop_controllers.push_back("effort_joint_trajectory_controller");
        srv.request.start_controllers.clear();
    }
    else
    {
        srv.request.start_controllers.push_back("effort_joint_trajectory_controller");
        srv.request.stop_controllers.clear();
    }
    srv.request.strictness = controller_manager_msgs::SwitchController::Request::STRICT; // 使用严格模式
    srv.request.start_asap = true;
    // srv.request.timeout = 2.0; // 超时时间为 2 秒
    if (client.call(srv))
    {
        ROS_INFO("Controller status: %s", isrunning ? "true" : "false"); // 显示服务调用结果
        if (srv.response.ok)
        {
            responseLabel->setText("Response: Success - ");
            if (isrunning)
            {
                QMessageBox::information(this, "Mode Switch", "Controller has stopped");
                serviceButton->setText("Start Controller");
            }
            else
            {
                QMessageBox::information(this, "Mode Switch", "Controller has started");
                serviceButton->setText("Stop Controller");
            }
            isrunning = !isrunning;
        }
        else
        {
            responseLabel->setText("Response: Failed - ");
        }
    }
    else
    {
        // 显示错误消息
        QMessageBox::warning(this, "Service Error", "Failed to call service!");
    }
}