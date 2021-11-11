#include <ignition/math.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/gazebo.hh>
#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_srvs/Trigger.h>

namespace gazebo {
    class WarZone : public WorldPlugin {
    public:
        void Load(physics::WorldPtr _parent, sdf::ElementPtr _sdf) {
            world = _parent;
            sdf = _sdf;

            // world->InsertModelFile("model://sea_mine");
            world->InsertModelFile("model://submarine");

            if (!ros::isInitialized()) {
              int argc = 0;
              char **argv = NULL;
              ros::init(argc, argv, world->Name());
            }

            ros::NodeHandle *nh = new ros::NodeHandle;
            pub_headings = nh->advertise<std_msgs::Float32>("/torpedo/hydrophones/headings", 1000, this);
            pub_distance = nh->advertise<std_msgs::Float32>("/torpedo/hydrophones/distance", 1000, this);

            service_detonate = nh->advertiseService("/torpedo/warhead/detonate", &WarZone::OnDetonate, this);
            updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&WarZone::OnUpdate, this, _1));
            std::cout<<"Plugin Load Complete"<<std::endl;
        }

    public:
        void OnUpdate(const common::UpdateInfo &_info) {
          if (!sdf->HasElement("submarine"))
            gzerr << "missing <submarine> element\n";

          model_submarine = world->ModelByName(sdf->GetElement("submarine")->Get<std::string>());

          if (model_submarine) {
            pos_sub = model_submarine->RelativePose().Pos();

            if (!sdf->HasElement("depth"))
              gzerr << "missing <depth> element\n";
            double depth = sdf->GetElement("depth")->Get<double>();

            if (pos_sub.Z() != depth)
              model_submarine->SetRelativePose(ignition::math::Pose3d(0, 0, depth, 0, 0, 0));

            if (!sdf->HasElement("radius"))
              gzerr << "missing <radius> element\n";
            float R = sdf->GetElement("radius")->Get<float>();

            if (!sdf->HasElement("angular_vel"))
              gzerr << "missing <angular_vel> element\n";
            float W = sdf->GetElement("angular_vel")->Get<float>();

            float t = _info.simTime.Float();
            model_submarine->SetLinearVel(ignition::math::Vector3d(-R * W * sin(W * t), -R * W * cos(W * t), 0));
            model_submarine->SetAngularVel(ignition::math::Vector3d(0, 0, -W));

            if (!sdf->HasElement("torpedo"))
              gzerr << "missing <torpedo> element\n";

            model_torpedo = world->ModelByName(sdf->GetElement("torpedo")->Get<std::string>());

            if (model_torpedo) {
              pos_tor = model_torpedo->RelativePose().Pos();
              distance.data = pos_tor.Distance(pos_sub);
              headings.data = atan2(pos_sub.Y() - pos_tor.Y(), pos_sub.X() - pos_tor.X());

              pub_headings.publish(headings);
              pub_distance.publish(distance);
            }
          }
        }

        bool OnDetonate(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res) {
          if (distance.data < 10) {
            res.success = true;
            res.message = "Detonation Successful";
            world->RemoveModel(model_torpedo);
            world->RemoveModel(model_submarine);
          }
          else {
            res.success = false;
            res.message = "Detonation Failed";
          }
          return true;
        }

    private:
        physics::WorldPtr world;
        sdf::ElementPtr sdf;
        ros::Publisher pub_distance, pub_headings;
        ros::ServiceServer service_detonate;
        ignition::math::Vector3<double> pos_sub, pos_tor;
        physics::ModelPtr model_submarine, model_torpedo;
        std_msgs::Float32 distance, headings;
        event::ConnectionPtr updateConnection;
    };

    // Register this plugin with the simulator
    GZ_REGISTER_WORLD_PLUGIN(WarZone)
}
