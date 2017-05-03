#include "autopilot_interface.h"
#include <iostream>

int main()
{
    Serial_Port* serial_port = new Serial_Port("COM10", 57600);
    Autopilot_Interface ai(serial_port);
    serial_port->start();
    ai.start();
    
	//MAVLINK_MSG_ID_MISSION_COUNT

	int index = 0;
    std::chrono::system_clock::time_point begging = std::chrono::system_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begging) < std::chrono::milliseconds(25000))
        // while(true)
    {

        //typedef enum MAV_RESULT
        //{
        //    MAV_RESULT_ACCEPTED=0, /* Command ACCEPTED and EXECUTED | */
        //    MAV_RESULT_TEMPORARILY_REJECTED = 1, /* Command TEMPORARY REJECTED/DENIED | */
        //        MAV_RESULT_DENIED = 2, /* Command PERMANENTLY DENIED | */
        //        MAV_RESULT_UNSUPPORTED = 3, /* Command UNKNOWN/UNSUPPORTED | */
        //        MAV_RESULT_FAILED = 4, /* Command executed, but failed | */
        //        MAV_RESULT_ENUM_END = 5, /*  | */
        //} MAV_RESULT;       
        Mavlink_Messages messages = ai.current_messages;
        std::cout << "Mavlink Command: " << messages.mavlink_command_ack.command << " Result:: " << (int)messages.mavlink_command_ack.result << std::endl;
		std::cout << "Mavlink Mission: " << (int)messages.mavlink_mission_ack.type << std::endl;

        //https://pixhawk.ethz.ch/mavlink/ look of mav_cmd to get enum for comamnds and params
      
		/*
        mavlink_command_long_t com;
        com.command = MAV_CMD_DO_SET_MODE;
		com.param1 = MAV_MODE_AUTO_ARMED;
		ai.send_command(com);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
		*/
		

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
		//} MAV_MISSION_RESULT;

		mavlink_mission_count_t mission_count;
		mission_count.count = 1;
		ai.send_waypoint_count(mission_count);

		std::this_thread::sleep_for(std::chrono::milliseconds(150));

		mavlink_mission_item_t mission_item;
		mission_item.command = MAV_CMD_NAV_WAYPOINT;
		mission_item.param1 = 0;
		mission_item.param2 = 0;
		mission_item.param3 = 0;
		mission_item.param4 = 0;
		mission_item.x = 0;
		mission_item.y = 0;
		mission_item.z = 0;
		mission_item.seq = index++;
		ai.send_mission_cmd(mission_item);


		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		
    }
    ai.disable_offboard_control();
    ai.stop();
    serial_port->stop();

    std::cout << "Ending" << std::endl;
}
