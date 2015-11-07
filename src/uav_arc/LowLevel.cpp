//Program Name: LowLevel_Control.cpp
//Description : This program is encharge of execute the instructions requested by the sequencer

//including the header of ROS classes.

#include <ros/ros.h>
#include <stdlib.h>
#include <mavros_msgs/State.h>
#include <sensor_msgs/Imu.h>
#include <mavros_msgs/CommandBool.h>
#include <ros/service.h>

using namespace ros;
using namespace std;

int armado = 0;
bool check_guided = false;
mavros_msgs::State st;

void getImu_Data(const sensor_msgs::Imu &data){
	//obtiene la altura actual del VANT para corregir la
	ROS_INFO_STREAM("Datos: "<<data.linear_acceleration.z);
}

void makeSure_Armed(const mavros_msgs::State &msg){
	NodeHandle nh;
	bool success;
	//ServiceClient client = n.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming",1);
	if (msg.armed){
		armado = 1;
		ROS_INFO_STREAM("Vehicle armed "<< armado );		
	}else{		
		armado = 0;
		ROS_INFO_STREAM("Vehicle disarmed "<< armado);
		ServiceClient client = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
		mavros_msgs::CommandBool::Request req;
		mavros_msgs::CommandBool::Request res;
		req.value = 1;
		mavros_msgs::CommandBool srv;
		success = client.call(req, res);
	}
		//system("rosservice call /mavros/cmd/arming 1");				
		//The vehicle will be armed here
	if(success){
		//Manda a llamar el Takeoff
		if(msg.guided){
			ROS_INFO_STREAM("GUIDED MODE");
			//Obtiene la altura actual del vehiculo y calcula la diferencia entre la altura actual y la ideal (10 metros)
			check_guided = true;
		}else{
			ROS_INFO_STREAM("NOT GUIDED MODE");
			system("rosrun mavros mavsys mode -c GUIDED ");
			check_guided = true;
		}
		if (check_guided){
			Subscriber client_Imu = nh.subscribe("/mavros/imu/data", 1, &getImu_Data);	
			spin();
		}
	}



		/*if(client.call(req, res)){
			ROS_INFO_STREAM("Command has been sent "<< req);
			ROS_INFO_STREAM("Response saved on " << res);
		}else{
			ROS_INFO_STREAM("The command has failed");
		} */
}

int main(int argc, char **argv){
	//initialize the ROS system 
	init(argc, argv, "low_level_node");
	
	while(ok()){
		spinOnce();	
	}
	return 0;

	//Establish this program as a ROS node.
	NodeHandle nh;
	Rate rate(2);
	ROS_INFO_STREAM("los datos son :"<<argv[1]);
	//Si el argumento pasado es 1, entonces activa el armado del vehiculo. 
	//creating a subscribe object
	int valor = atoi(argv[1]);
	if(valor == 1){
		Subscriber subs = nh.subscribe("/mavros/state", 1, &makeSure_Armed);		
		spin();

	}else{
		ROS_INFO_STREAM("ENTRO al else");
	}
	
	rate.sleep();
	return 0;
}