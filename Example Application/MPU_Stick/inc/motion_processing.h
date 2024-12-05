
#ifndef INC_MOTION_PROCESSING_H_
#define INC_MOTION_PROCESSING_H_

#include <stdint.h>
#include "MPU9250.h"
#include "IQmathLib.h"
#include "ADCP.h"

#define ROLL        0
#define PITCH       1
#define YAW         2

#define MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_LESSER_THAN         0
#define MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_GREATER_THAN        1
#define MOTION_DETECTION_THRESHOLD_TYPE_LESSER_THAN                  2
#define MOTION_DETECTION_THRESHOLD_TYPE_GREATER_THAN                 3
#define MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_WITHIN_BOUNDS  4
#define MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_OUTSIDE_BOUNDS 5

struct _motion_processing
{
   uint32_t _align;

   /* Roll, pitch and yaw can vary between -180 and +180. */

    _iq12 roll_accel;
    _iq12 pitch_accel;
    _iq12 yaw_accel;
    _iq12 roll_gyro;
    _iq12 pitch_gyro;
    _iq12 yaw_gyro;
    _iq12 roll_filter;
    _iq12 pitch_filter;
    _iq12 yaw_filter;

    _iq12 step_time;

    /* Filter constants. */

    _iq12 roll_filter_constant;
    _iq12 pitch_filter_constant;

    /* Motion detection parameters. */

    /* Accelerometer thresholds are mentioned in milli-Gs. */
    /* Gyroscope thresholds are mentioned in milli-DPS. */
    /* Angle thresholds are mentioned in degrees. */

    _iq12 accelerometer_low_threshold[3];       //  Lower threshold (for windowed mode) limits / threshold for accelerometer.
    _iq12 gyroscope_low_threshold[3];           //  Lower threshold (for windowed mode) limits / threshold for gyroscope.
    _iq12 angle_low_threshold[3];               //  Lower threshold (for windowed mode) limits / threshold for angles.

    _iq12 accelerometer_high_threshold[3];      //  Upper threshold limit for accelerometer.
    _iq12 gyroscope_high_threshold[3];          //  Upper threshold limit for gyroscope.
    _iq12 angle_high_threshold[3];              //  Upper threshold limit for angle.

    /* Motion processing detection parameters. */

    union motion_detection
    {
        uint32_t all;

        struct _motion_detection_segments
        {
            uint32_t acc_x      : 1;
            uint32_t acc_y      : 1;
            uint32_t acc_z      : 1;
            uint32_t gyr_x      : 1;
            uint32_t gyr_y      : 1;
            uint32_t gyr_z      : 1;
            uint32_t roll       : 1;
            uint32_t pitch      : 1;
            uint32_t yaw        : 1;
            uint32_t acc_x_threshold_type   :   3;
            uint32_t acc_y_threshold_type   :   3;
            uint32_t acc_z_threshold_type   :   3;
            uint32_t gyr_x_threshold_type   :   3;
            uint32_t gyr_y_threshold_type   :   3;
            uint32_t gyr_z_threshold_type   :   3;
            uint32_t roll_threshold_type    :   3;
            uint32_t pitch_threshold_type   :   3;
            uint32_t yaw_threshold_type     :   3;
        }motion_detection_segments;
    }motion_detection;

}MP;

void MP_Init();
void MP_update();

#endif /* INC_MOTION_PROCESSING_H_ */
