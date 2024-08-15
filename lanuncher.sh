gnome-terminal -- bash -c "
                            roslaunch rosbridge_server rosbridge_websocket.launch; 
                            exec bash
                          "

sleep 5

gnome-terminal -- bash -c "
                            rosrun tf2_web_republisher tf2_web_republisher; 
                            exec bash
                          "
 
sleep 5

gnome-terminal -- bash -c "
                            roslaunch px4_gazebo laea_gazebo_lidar.launch; 
                            exec bash
                          "

sleep 2

gnome-terminal -- bash -c "
                            roslaunch rtp_gazebo rtp_receiver.launch ; 
                            exec bash
                          "

sleep 2

gnome-terminal -- bash -c "
                            roslaunch rtp_gazebo rtp_sender.launch;
                            exec bash
                          "

sleep 2

gnome-terminal -- bash -c "
                            roslaunch px4_gazebo controller.launch;
                            exec bash
                          "

sleep 5

gnome-terminal -- bash -c "
                            roslaunch octomap_server scan_mapping.launch; 
                            exec bash
                          "

sleep 5

gnome-terminal -- bash -c "
                            roslaunch exploration_manager explore_test.launch; 
                            exec bash
                          "

sleep 5

gnome-terminal -- bash -c "
                            roslaunch exploration_manager rviz_alg.launch; 
                            exec bash
                        "
                      
sleep 5

# gnome-terminal -- bash -c "
#                             roslaunch kinect_publisher kinect_publisher.launch; 
#                             exec bash
#                         "

# sleep 5

# gnome-terminal -- bash -c "
#                             roslaunch kinect_publisher rtabmap.launch; 
#                             exec bash
#                         "
                      
# sleep 5

gnome-terminal -- bash -c "
                            rosrun mavros mavsys mode -c OFFBOARD;
                            exec bash
                          "

sleep 5

gnome-terminal -- bash -c "
                            rosrun mavros mavsafety arm;
                            exec bash
                          "

sleep 5

# gnome-terminal -- bash -c "
#                             roslaunch rtp_gazebo slam_sender.launch;
#                             exec bash
#                           "

# sleep 4

# gnome-terminal -- bash -c "
#                             roslaunch rtp_gazebo monitor_receiver.launch;
#                             exec bash
#                           "

