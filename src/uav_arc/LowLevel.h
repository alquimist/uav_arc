#include <ros/ros.h>
#include <mavros_msgs/State.h>
#include <stdlib.h>
#include <mavros_msgs/State.h>
#include <sensor_msgs/Imu.h>
#include <mavros_msgs/CommandBool.h>
#include <ros/service.h>

using namespace ros;

/*Declaracion de los prototipos de clase */
class LowLevel{
	private:
		NodeHandle _nh_;
		bool _isArmed;
		bool _isGuided;
		Subscriber _clt_sub_state;
		void setupSubscribers();
		void setupServices();
		
	public:
		LowLevel();
		LowLevel(NodeHandle nh);
		~LowLevel();
		bool isArmed();
		bool isGuided();
		bool uav_mav_arm();
		void sub_mav_state();
}


