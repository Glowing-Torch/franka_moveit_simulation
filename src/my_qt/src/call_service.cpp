#include <ros/ros.h>
#include <controller_manager_msgs/SwitchController.h>

int main(int argc, char **argv)
{
    // 初始化 ROS 节点
    ros::init(argc, argv, "switch_controller_client");
    ros::NodeHandle nh;

    // 创建服务客户端
    ros::ServiceClient client = nh.serviceClient<controller_manager_msgs::SwitchController>("/controller_manager/switch_controller");

    // 等待服务可用
    if (!client.waitForExistence(ros::Duration(5.0))) {
        ROS_ERROR("Service /controller_manager/switch_controller not available");
        return 1;
    }

    // 创建服务请求对象
    controller_manager_msgs::SwitchController srv;
    
    // 设置要启动和停止的控制器
    srv.request.start_controllers.push_back(""); // 留空表示不启动任何控制器
    srv.request.stop_controllers.push_back("effort_joint_trajectory_controller");
    srv.request.strictness = controller_manager_msgs::SwitchController::Request::STRICT; // 使用严格模式
    srv.request.start_asap = true;
    srv.request.timeout = 2.0; // 超时时间为 2 秒

    // 调用服务并处理响应
    if (client.call(srv)) {
        if (srv.response.ok) {
            ROS_INFO("Successfully switched controllers.");
        } else {
            ROS_WARN("Failed to switch controllers.");
        }
    } else {
        ROS_ERROR("Failed to call service /controller_manager/switch_controller");
    }

    return 0;
}
