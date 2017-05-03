#include "autopilot_interface.h"
#include <iostream>

int main()
{
    Serial_Port* serial_port = new Serial_Port("COM10", 57600);
    Autopilot_Interface ai(serial_port);
    serial_port->start();
    ai.start();
    //ai.enable_offboard_control(); //Arduplane will fail offboard control you cant not change local or global ned position
    
    Time_Stamps recentTimeStamp;
    recentTimeStamp.reset_timestamps();

    bool servoCommandAccepted = false;
    bool waypointCommandAccepted = false;
    
    std::chrono::system_clock::time_point begging = std::chrono::system_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begging) < std::chrono::milliseconds(25000))
    {
        Mavlink_Messages messages = ai.current_messages;

            std::cout << "Mavlink Command: " << messages.mavlink_command_ack.command << " Result:: " << (int)messages.mavlink_command_ack.result << std::endl;
            recentTimeStamp.cmd_ack = messages.time_stamps.cmd_ack;
            if (messages.mavlink_command_ack.command == MAV_CMD_DO_SET_SERVO && messages.mavlink_command_ack.result == MAV_RESULT_ACCEPTED)
            {
                servoCommandAccepted = true;
            }
        

   
            //doesnt look like you can check what mission cmd this is responding to
            std::cout << "Mavlink Mission: " << (int)messages.mavlink_mission_ack.type << std::endl;
            recentTimeStamp.mission_cmd_ack = messages.time_stamps.mission_cmd_ack;
            if (messages.mavlink_mission_ack.type == MAV_MISSION_ACCEPTED)
            {
                waypointCommandAccepted = true;
            }
        

        if (true)//waypointCommandAccepted)
        {
			//you need to tell the pixhawk how many waypoints to accept
			mavlink_mission_count_t mission_count;
			mission_count.count = 1;
			ai.send_waypoint_count(mission_count);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

			//send waypoints
            mavlink_mission_item_t mission_item;
            mission_item.command = MAV_CMD_NAV_WAYPOINT;
            mission_item.param1 = 0;//hold time in decimal second IGNORED by ArduPlane
            mission_item.param2 = 0;//Acceptance radius in meters
            mission_item.param3 = 0;//0 to pass through WP if >0 radius in meters to pass by WP
            mission_item.param4 = 0;//Desired yaw angle NaN for unchanged
            mission_item.x = 0;//latitude //this seems wrong but param 5 is x
            mission_item.y = 0;//longitude
            mission_item.z = 0;//altitude
            mission_item.seq = 0;//waypoint number
            ai.send_mission_cmd(mission_item);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
			std::cout << "Sent" << std::endl;
        }

        if (true)
        {
            mavlink_command_int_t cmd_item;
            cmd_item.command = MAV_CMD_DO_SET_SERVO;
            cmd_item.param1 = 10;//server number
            cmd_item.param2 = 1500;//pwm microseconds 1000 to 2000 typicaly
            ai.send_command(cmd_item);
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
		
    }
   // ai.disable_offboard_control();
    ai.stop();
    serial_port->stop();

    std::cout << "Ending" << std::endl;
}

//error enums for Commands
//typedef enum MAV_RESULT
//{
//    MAV_RESULT_ACCEPTED=0, /* Command ACCEPTED and EXECUTED | */
//    MAV_RESULT_TEMPORARILY_REJECTED = 1, /* Command TEMPORARY REJECTED/DENIED | */
//        MAV_RESULT_DENIED = 2, /* Command PERMANENTLY DENIED | */
//        MAV_RESULT_UNSUPPORTED = 3, /* Command UNKNOWN/UNSUPPORTED | */
//        MAV_RESULT_FAILED = 4, /* Command executed, but failed | */
//        MAV_RESULT_ENUM_END = 5, /*  | */
//} MAV_RESULT;      


//error enums for Mission Commands
//typedef enum MAV_MISSION_RESULT
//{
//	MAV_MISSION_ACCEPTED = 0, /* mission accepted OK | */
//	MAV_MISSION_ERROR = 1, /* generic error / not accepting mission commands at all right now | */
//	MAV_MISSION_UNSUPPORTED_FRAME = 2, /* coordinate frame is not supported | */
//	MAV_MISSION_UNSUPPORTED = 3, /* command is not supported | */
//	MAV_MISSION_NO_SPACE = 4, /* mission item exceeds storage space | */
//	MAV_MISSION_INVALID = 5, /* one of the parameters has an invalid value | */
//	MAV_MISSION_INVALID_PARAM1 = 6, /* param1 has an invalid value | */
//	MAV_MISSION_INVALID_PARAM2 = 7, /* param2 has an invalid value | */
//	MAV_MISSION_INVALID_PARAM3 = 8, /* param3 has an invalid value | */
//	MAV_MISSION_INVALID_PARAM4 = 9, /* param4 has an invalid value | */
//	MAV_MISSION_INVALID_PARAM5_X = 10, /* x/param5 has an invalid value | */
//	MAV_MISSION_INVALID_PARAM6_Y = 11, /* y/param6 has an invalid value | */
//	MAV_MISSION_INVALID_PARAM7 = 12, /* param7 has an invalid value | */
//	MAV_MISSION_INVALID_SEQUENCE = 13, /* received waypoint out of sequence | */
//	MAV_MISSION_DENIED = 14, /* not accepting any mission commands from this communication partner | */
//	MAV_MISSION_RESULT_ENUM_END = 15, /*  | */
//} MAV_MI

//https://pixhawk.ethz.ch/mavlink/ 