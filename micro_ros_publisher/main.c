#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int32.h>

#include "modules.h"
#if IS_ACTIVE(MODULE_MICRO_ROS_SERIAL_TRANSPORT)
#include "microxrce_serial_transport.h"
#endif
#if IS_ACTIVE(MODULE_MICRO_ROS_UDP_TRANSPORT)
#include "microxrce_udp_transport.h"
#endif

#include <stdio.h>
#include <unistd.h>

#define INTERVAL 1000U

#define RCCHECK(fn) { \
    rcl_ret_t temp_rc = fn; \
    if((temp_rc != RCL_RET_OK)) { \
        printf("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc); \
        return 1; \
    } \
}

#define RCSOFTCHECK(fn) { \
    rcl_ret_t temp_rc = fn; \
    if((temp_rc != RCL_RET_OK)) {\
        printf("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc); \
    } \
}

static std_msgs__msg__Int32      msg;
static rcl_publisher_t           publisher;

void timer_callback(rcl_timer_t * timer, int64_t last_call_time);

int main(void)
{
    /* Micro ROS realted variables */
    rcl_allocator_t 	allocator;
    rclc_support_t 		support;
    rcl_node_t 			node;
    rclc_executor_t 	executor;
    rcl_timer_t         timer;


    int ret;
#if IS_ACTIVE(MODULE_MICRO_ROS_SERIAL_TRANSPORT)
    ret = serial_set_custom_transport(UART_DEV(CONFIG_SERIAL_TRANSPORT_DEV), CONFIG_SERIAL_TRANSPORT_BAUD);
#elif IS_ACTIVE(MODULE_MICRO_ROS_UDP_TRANSPORT)
    ret = udp_set_custom_transport(CONFIG_UDP_TRANSPORT_IP, CONFIG_UDP_TRANSPORT_PORT);
#else
    puts("No transport set up!");
#endif

    if (ret < 0) {
        puts("Error setting up the transport!");
    }

    allocator = rcl_get_default_allocator();

    /* create init_options */
    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

    /* create node */
    RCCHECK(rclc_node_init_default(&node, "riot_int32_publisher", "", &support));

    /* create publisher */
    RCCHECK(rclc_publisher_init_default(
        &publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "int32_msg"));

    /* create timer */
    RCCHECK(rclc_timer_init_default2(
        &timer,
        &support,
        RCL_MS_TO_NS(INTERVAL),
        timer_callback,
        true));

    /* create executor */
    executor = rclc_executor_get_zero_initialized_executor();
    RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
    RCCHECK(rclc_executor_add_timer(&executor, &timer));

    msg.data = 0;

    puts("Initialized the publisher. Spinning up the executor...");
    /* spin up the executor */
    rclc_executor_spin(&executor);

    RCCHECK(rcl_publisher_fini(&publisher, &node));
    RCCHECK(rcl_node_fini(&node));

    return 0;
}

void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{
    (void) last_call_time;
    if (timer != NULL) {
        RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
        printf("Sent: %ld\n", msg.data);
        msg.data++;
    }
}
