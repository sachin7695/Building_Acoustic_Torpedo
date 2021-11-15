#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include <ros/ros.h>
#include <std_msgs/Float32.h>

using namespace gazebo;

class TorpedoPlugin : public ModelPlugin
{
  public:
    void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf) {
      model = _parent;
      sdf = _sdf;

      if (!ros::isInitialized()) {
        int argc = 0;
        char **argv = NULL;
        ros::init(argc, argv, "TorpedoPlugin");
      }
      nh = new ros::NodeHandle;


      std::string robot_namespace = "";
      if (_sdf->HasElement("namespace"))
        robot_namespace = _sdf->Get<std::string>("namespace");

      if (!_sdf->HasElement("propeller_joint"))
        gzerr << "plugin missing <propeller_joint> element\n";
      propeller_joint = model->GetJoint(_sdf->GetElement("propeller_joint")->Get<std::string>());

      if (!_sdf->HasElement("propeller_topic"))
        gzerr << "plugin missing <propeller_topic> element\n";
      std::string propeller_topic = _sdf->GetElement("propeller_topic")->Get<std::string>();

      if (!_sdf->HasElement("rudder_link"))
        gzerr << "plugin missing <rudder_link> element\n";
      rudder_link = model->GetLink(_sdf->GetElement("rudder_link")->Get<std::string>());

      if (!_sdf->HasElement("rudder_topic"))
        gzerr << "plugin missing <rudder_topic> element\n";
      std::string rudder_topic = _sdf->GetElement("rudder_topic")->Get<std::string>();


      pose = rudder_link->RelativePose();
      rudder_sub = nh->subscribe(robot_namespace + rudder_topic, 10, &TorpedoPlugin::RudderCallback, this);
      propeller_sub = nh->subscribe(robot_namespace + propeller_topic, 10, &TorpedoPlugin::PropellerCallback, this);

      this->updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&TorpedoPlugin::OnUpdate, this, _1));
      std::cout<<"TorpedoPlugin Loaded"<<std::endl;
    }

    void OnUpdate(const common::UpdateInfo &_info) {
      pose.Set(pose.Pos().X(), pose.Pos().Y(), pose.Pos().Z(), 0, 0, rudder_angle);
      rudder_link->SetRelativePose(pose);
      propeller_joint->SetVelocity(0, propeller_velocity);
    }

    void PropellerCallback(const std_msgs::Float32::ConstPtr &msg) {
      propeller_velocity = msg->data;
    }

    void RudderCallback(const std_msgs::Float32::ConstPtr &msg) {
      rudder_angle = msg->data;
    }

  private:
    physics::ModelPtr model;
    sdf::ElementPtr sdf;
    physics::JointPtr propeller_joint;
    physics::LinkPtr rudder_link;
    event::ConnectionPtr updateConnection;
    ignition::math::Pose3d pose;

    ros::NodeHandle *nh;
    ros::Subscriber rudder_sub, propeller_sub;
    float propeller_velocity, rudder_angle;
};
GZ_REGISTER_MODEL_PLUGIN(TorpedoPlugin)
