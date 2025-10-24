# Micro-ROS on RIOT Examples
This repository provides example applications demonstrating both a publisher and a subscriber for Micro-ROS on RIOT.

Micro-ROS nodes do not provide a full DDS implementation; they use Micro XRCE DDS.
The Micro XRCE DDS client on the Micro-ROS node connects to the Micro XRCE DDS agent,
which then acts on its behalf.
The client and agent can be connected using UDP or a serial connection.

## Setting up the Transport and Agent
Use the respective module to set which transport you want to use:
`USEMODULE += micro_ros_udp_transport` or `USEMODULE += micro_ros_serial_transport`.

### Serial Transport
The easiest way to use the serial transport is via a USB UART bridge.
After connecting the bridge, set the UART device and baud rate via `make menuconfig`.
Now start the Micro-ROS agent:
```
docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:kilted serial --dev "/dev/ttyUSB0"
```
When connecting multiple nodes to one agent, use the multiserial transport for the agent:
```
docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:kilted multiserial --devs "/dev/ttyUSB0 /dev/ttyUSB1"
```

### UDP Transport
Set the port and agent IP via `make menuconfig`.

When Micro-ROS nodes communicate via IEEE 802.15.4 or Bluetooth Low Energy,
you have to set up a border router. You can use the gnrc_border_router example in RIOT for that.

To start the agent run:
```
docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:kilted udp6 --port 2025
```

## Running the example
To run the example, ensure the transport is configured and run:
```
make all flash term
```

The publisher example publishes a `std_msgs/msg/Int32` message to the `int32_msg` topic every second, and the subscriber listens to incoming messages on the same topic.

If only one of the two examples is running, it can be helpful to publish or listen to the topic from the command line.
To enter the shell of the docker container running the agent, run:
```
docker exec -it <agent container> /bin/bash
```
To list all topics being published or subscribed to, run:
```
ros2 topic list
```
To publish a `std_msgs/msg/Int32` message to the `int32_msg` topic, run:
```
ros2 topic pub /int32_msg --once std_msgs/msg/Int32 "{data: 42}"
```
To listen to all incoming messages on the `int32_msg` topic, run:
```
ros2 topic echo /int32_msg
```