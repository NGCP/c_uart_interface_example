#include "autopilot_interface.h"
#include <iostream>

int main()
{
    Serial_Port* serial_port = new Serial_Port("/dev/ttyACM0", 57600);
    Autopilot_Interface ai(serial_port);
    serial_port->start();
    ai.start();
    ai.enable_offboard_control();

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


        //https://pixhawk.ethz.ch/mavlink/ look of mav_cmd to get enum for comamnds and params
        // Prepare command for off-board mode
        mavlink_command_long_t com;
        com.command = MAV_CMD_NAV_TAKEOFF;//NOT A LONG, but it will internally convert to int any ways for example of long
        com.param5 = (float)100;
        com.param6 = (float)100;
        com.param7 = (float)100;
        ai.send_command(com);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

		mavlink_command_int_t com2;
		com2.command = MAV_CMD_NAV_WAYPOINT;
		com2.x = 10000000;
		com2.y = 20000000;
		com2.z = 30000000;
		ai.send_command(com2);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // ai.disable_offboard_control();
    ai.stop();
    serial_port->stop();

    std::cout << "Ending" << std::endl;
}
