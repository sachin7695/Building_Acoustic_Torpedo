# Task 2: Simulate torpedo to follow a submarine

## Your Objective:
To develop a controller node that subscribes headings and distance data from hydrophones, process it using PID Controller algorithm and publish rudder angle and propeller velocity to surge towards the submarine and finally, send a detonate request signal to the warhead while approaching the submarine.

## About this Task:
This task aims to test your knowledge in ROS and basic robotics concepts such as:
- ROS Publisher & Subscriber
- ROS Service & Client
- PID Controller

## How should you proceed:
1. Write codes to subscribe from `/torpedo/hydrophones/headings` & `/torpedo/hydrophones/distance` topics and publish on `/torpedo/rudder/angle` & `/torpedo/propeller/velocity` topics. [ROS Publisher Subscriber Tutorial](http://wiki.ros.org/ROS/Tutorials/WritingPublisherSubscriber%28c%2B%2B%29)

2. Use the following rostopic commands to get details of the topic and debug your code.
  - `rostopic list`: list all available ros topics.
  - `rostopic info /topic_name`: get msg datatype, current Publishers & Subscribers.
  - `rostopic pub /topic_name`: publish data to a topic.
  - `rostopic echo /topic_name`: subscribe data from a topic.

  - `rosservice list`: list all available services
  - `rosservice call /service_name`: call a service with required parameters
  - Use **Tab key** to autocomplete commands and also suggest possible commands/arguements.

3. Write a PID Controller that takes **headings** as error input and generates **rudder angle** as output, thus aligning the torpedo towards the submarine. A constant value can be published to the **propeller velocity**, allowing the torpedo to surge forward.

4. When the torpedo is less than **2 meters** from the submarine, send a request detonation signal to the service `/torpedo/warhead/detonate`. [ROS Service Client Tutorial](http://wiki.ros.org/ROS/Tutorials/WritingServiceClient%28c%2B%2B%29)

## How to run:
1. Firstly, navigate to tiburon_ws from the terminal `cd ~/tiburon_ws` and then create a new C++ node with the `gedit controller.cpp` command.

2. Add the following lines at the end of [CMakeLists.txt](../torpedo/CMakeLists.txt) in the torpedo package and build the package with `catkin build torpedo`

  ```
  add_executable(controller src/controller.cpp)
  target_link_libraries(controller ${catkin_LIBRARIES})
  ```

3. To launch warzone world along with torpedo, run the following commands in separate terminals (use **CTRL+SHIFT+T** in terminal to create a new tab inside the terminal)
  - `roslaunch warzone arena.launch`
  - `roslaunch torpedo spawn.launch`
  - `rosrun torpedo controller`
