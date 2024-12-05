
#include "motion_processing.h"

void MP_Init()
{
    MP.roll_accel = 0;
    MP.pitch_accel = 0;
    MP.yaw_accel = 0;

    MP.roll_gyro = 0;
    MP.pitch_gyro = 0;
    MP.yaw_gyro = 0;

    MP.motion_detection.motion_detection_segments.acc_x = 0;
    MP.motion_detection.motion_detection_segments.acc_y = 0;
    MP.motion_detection.motion_detection_segments.acc_z = 0;
    MP.motion_detection.motion_detection_segments.gyr_x = 0;
    MP.motion_detection.motion_detection_segments.gyr_y = 0;
    MP.motion_detection.motion_detection_segments.gyr_z = 0;
    MP.motion_detection.motion_detection_segments.roll  = 0;
    MP.motion_detection.motion_detection_segments.pitch = 0;
    MP.motion_detection.motion_detection_segments.yaw   = 0;

    /* Load data into communication frames. */

    MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_ACCEL_X_THRESHOLD_TYPE =
    MP.motion_detection.motion_detection_segments.acc_x_threshold_type;

    MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_ACCEL_Y_THRESHOLD_TYPE =
    MP.motion_detection.motion_detection_segments.acc_y_threshold_type;

    MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_ACCEL_Z_THRESHOLD_TYPE =
    MP.motion_detection.motion_detection_segments.acc_z_threshold_type;

    MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_GYRO_X_THRESHOLD_TYPE =
    MP.motion_detection.motion_detection_segments.gyr_x_threshold_type;

    MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_GYRO_Y_THRESHOLD_TYPE =
    MP.motion_detection.motion_detection_segments.gyr_y_threshold_type;

    MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_GYRO_Z_THRESHOLD_TYPE =
    MP.motion_detection.motion_detection_segments.gyr_z_threshold_type;

    MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_PITCH_THRESHOLD_TYPE =
    MP.motion_detection.motion_detection_segments.pitch_threshold_type;

    MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_ROLL_THRESHOLD_TYPE =
    MP.motion_detection.motion_detection_segments.roll_threshold_type;

    MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_YAW_THRESHOLD_TYPE =
    MP.motion_detection.motion_detection_segments.yaw_threshold_type;

    IEEE754._float = _IQ12toF(MP.accelerometer_low_threshold[X_AXIS]);
    MD_ACCEL_X_THRESHOLD_LOW.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.accelerometer_high_threshold[X_AXIS]);
    MD_ACCEL_X_THRESHOLD_HIGH.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.accelerometer_low_threshold[Y_AXIS]);
    MD_ACCEL_Y_THRESHOLD_LOW.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.accelerometer_high_threshold[Y_AXIS]);
    MD_ACCEL_Y_THRESHOLD_HIGH.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.accelerometer_low_threshold[Z_AXIS]);
    MD_ACCEL_Z_THRESHOLD_LOW.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.accelerometer_high_threshold[Z_AXIS]);
    MD_ACCEL_Z_THRESHOLD_HIGH.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.gyroscope_low_threshold[X_AXIS]);
    MD_GYRO_X_THRESHOLD_LOW.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.gyroscope_high_threshold[X_AXIS]);
    MD_GYRO_X_THRESHOLD_HIGH.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.gyroscope_low_threshold[Y_AXIS]);
    MD_GYRO_Y_THRESHOLD_LOW.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.gyroscope_high_threshold[Y_AXIS]);
    MD_GYRO_Y_THRESHOLD_HIGH.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.gyroscope_low_threshold[Z_AXIS]);
    MD_GYRO_Z_THRESHOLD_LOW.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.gyroscope_high_threshold[Z_AXIS]);
    MD_GYRO_Z_THRESHOLD_HIGH.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.angle_low_threshold[ROLL]);
    MD_ROLL_THRESHOLD_LOW.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.angle_high_threshold[ROLL]);
    MD_ROLL_THRESHOLD_HIGH.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.angle_low_threshold[PITCH]);
    MD_PITCH_THRESHOLD_LOW.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.angle_high_threshold[PITCH]);
    MD_PITCH_THRESHOLD_HIGH.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.angle_low_threshold[YAW]);
    MD_YAW_THRESHOLD_LOW.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.angle_high_threshold[YAW]);
    MD_YAW_THRESHOLD_HIGH.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.roll_filter_constant);
    MP_ROLL_FILTER_CONSTANT.all = IEEE754._int;

    IEEE754._float = _IQ12toF(MP.pitch_filter_constant);
    MP_PITCH_FILTER_CONSTANT.all = IEEE754._int;
}

void MP_update()
{
    /* Motion processing. */

    MP.roll_gyro    += _IQ12mpy( _IQ12div(_IQ12(MPU9250.gyro[X_AXIS]), _IQ12(1000) ), MP.step_time );
    MP.pitch_gyro   += _IQ12mpy( _IQ12div(_IQ12(MPU9250.gyro[Y_AXIS]), _IQ12(1000) ), MP.step_time );
    MP.yaw_gyro     += _IQ12mpy( _IQ12div(_IQ12(MPU9250.gyro[Z_AXIS]), _IQ12(1000) ), MP.step_time );

    MP.pitch_accel = _IQ12mpy( _IQ12atan2PU(_IQ12(MPU9250.accel[X_AXIS]), _IQ12mag( _IQ12(MPU9250.accel[Y_AXIS]), _IQ12(MPU9250.accel[Z_AXIS]) )), _IQ12(180.0f));
    MP.roll_accel  =  _IQ12mpy( _IQ12atan2PU(_IQ12(MPU9250.accel[Y_AXIS]), _IQ12(MPU9250.accel[Z_AXIS]) ), _IQ12(180.0f));
    MP.yaw_accel = MP.yaw_gyro;

    MP.roll_filter = _IQ12mpy( MP.roll_accel, MP.roll_filter_constant ) + _IQ12mpy( MP.roll_gyro, _IQ12(1.0f) - MP.roll_filter_constant );
    MP.pitch_filter = _IQ12mpy( MP.pitch_accel, MP.pitch_filter_constant ) + _IQ12mpy( MP.pitch_gyro, _IQ12(1.0f) - MP.pitch_filter_constant );
    MP.yaw_filter = MP.yaw_gyro;

    /* Motion detection */

    /* Accelerometer X detection. */

    switch(MP.motion_detection.motion_detection_segments.acc_x_threshold_type)
    {
        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_LESSER_THAN):
        {
            if( _IQ12abs( _IQ12(MPU9250.accel[X_AXIS]) ) <= MP.accelerometer_low_threshold[X_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.acc_x = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_x = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_GREATER_THAN):
        {
            if( _IQ12abs( _IQ12(MPU9250.accel[X_AXIS]) ) >= MP.accelerometer_low_threshold[X_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.acc_x = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_x = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_LESSER_THAN):
        {
            if( _IQ12(MPU9250.accel[X_AXIS])  <= MP.accelerometer_low_threshold[X_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.acc_x = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_x = 0;
            }
        }
        break;


        case(MOTION_DETECTION_THRESHOLD_TYPE_GREATER_THAN):
        {
            if( _IQ12(MPU9250.accel[X_AXIS]) >= MP.accelerometer_low_threshold[X_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.acc_x = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_x = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_WITHIN_BOUNDS):
        {
            if( ( _IQ12(MPU9250.accel[X_AXIS]) >= MP.accelerometer_low_threshold[X_AXIS] )
                && ( _IQ12(MPU9250.accel[X_AXIS]) <= MP.accelerometer_high_threshold[X_AXIS] ) )
            {
                MP.motion_detection.motion_detection_segments.acc_x = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_x = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_OUTSIDE_BOUNDS):
        {
            if( ( _IQ12(MPU9250.accel[X_AXIS]) <= MP.accelerometer_low_threshold[X_AXIS] )
                || ( _IQ12(MPU9250.accel[X_AXIS]) >= MP.accelerometer_high_threshold[X_AXIS] ) )
            {
                MP.motion_detection.motion_detection_segments.acc_x = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_x = 0;
            }
        }
        break;
    }


    /* Accelerometer Y detection. */

    switch(MP.motion_detection.motion_detection_segments.acc_y_threshold_type)
    {
        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_LESSER_THAN):
        {
            if( _IQ12abs( _IQ12(MPU9250.accel[Y_AXIS]) ) <= MP.accelerometer_low_threshold[Y_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.acc_y = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_y = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_GREATER_THAN):
        {
            if( _IQ12abs( _IQ12(MPU9250.accel[Y_AXIS]) ) >= MP.accelerometer_low_threshold[Y_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.acc_y = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_y = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_LESSER_THAN):
        {
            if( _IQ12(MPU9250.accel[Y_AXIS])  <= MP.accelerometer_low_threshold[Y_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.acc_y = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_y = 0;
            }
        }
        break;


        case(MOTION_DETECTION_THRESHOLD_TYPE_GREATER_THAN):
        {
            if( _IQ12(MPU9250.accel[Y_AXIS]) >= MP.accelerometer_low_threshold[Y_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.acc_y = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_y = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_WITHIN_BOUNDS):
        {
            if( ( _IQ12(MPU9250.accel[Y_AXIS]) >= MP.accelerometer_low_threshold[Y_AXIS] )
                && ( _IQ12(MPU9250.accel[Y_AXIS]) <= MP.accelerometer_high_threshold[Y_AXIS] ) )
            {
                MP.motion_detection.motion_detection_segments.acc_y = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_y = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_OUTSIDE_BOUNDS):
        {
            if( ( _IQ12(MPU9250.accel[Y_AXIS]) <= MP.accelerometer_low_threshold[Y_AXIS] )
                || ( _IQ12(MPU9250.accel[Y_AXIS]) >= MP.accelerometer_high_threshold[Y_AXIS] ) )
            {
                MP.motion_detection.motion_detection_segments.acc_y = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_y = 0;
            }
        }
        break;
    }

    /* Accelerometer Z detection. */

    switch(MP.motion_detection.motion_detection_segments.acc_z_threshold_type)
    {
        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_LESSER_THAN):
        {
            if( _IQ12abs( _IQ12(MPU9250.accel[Z_AXIS]) ) <= MP.accelerometer_low_threshold[Z_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.acc_z = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_z = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_GREATER_THAN):
        {
            if( _IQ12abs( _IQ12(MPU9250.accel[Z_AXIS]) ) >= MP.accelerometer_low_threshold[Z_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.acc_z = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_z = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_LESSER_THAN):
        {
            if( _IQ12(MPU9250.accel[Z_AXIS])  <= MP.accelerometer_low_threshold[Z_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.acc_z = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_z = 0;
            }
        }
        break;


        case(MOTION_DETECTION_THRESHOLD_TYPE_GREATER_THAN):
        {
            if( _IQ12(MPU9250.accel[Z_AXIS]) >= MP.accelerometer_low_threshold[Z_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.acc_z = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_z = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_WITHIN_BOUNDS):
        {
            if( ( _IQ12(MPU9250.accel[Z_AXIS]) >= MP.accelerometer_low_threshold[Z_AXIS] )
                && ( _IQ12(MPU9250.accel[Z_AXIS]) <= MP.accelerometer_high_threshold[Z_AXIS] ) )
            {
                MP.motion_detection.motion_detection_segments.acc_z = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_z = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_OUTSIDE_BOUNDS):
        {
            if( ( _IQ12(MPU9250.accel[Z_AXIS]) <= MP.accelerometer_low_threshold[Z_AXIS] )
                || ( _IQ12(MPU9250.accel[Z_AXIS]) >= MP.accelerometer_high_threshold[Z_AXIS] ) )
            {
                MP.motion_detection.motion_detection_segments.acc_z = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.acc_z = 0;
            }
        }
        break;
    }




    /* Gyroscope X detection. */

    switch(MP.motion_detection.motion_detection_segments.gyr_x_threshold_type)
    {
        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_LESSER_THAN):
        {
            if( _IQ12abs( _IQ12(MPU9250.gyro[X_AXIS]) ) <= MP.gyroscope_low_threshold[X_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.gyr_x = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_x = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_GREATER_THAN):
        {
            if( _IQ12abs( _IQ12(MPU9250.gyro[X_AXIS]) ) >= MP.gyroscope_low_threshold[X_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.gyr_x = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_x = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_LESSER_THAN):
        {
            if( _IQ12(MPU9250.gyro[X_AXIS])  <= MP.gyroscope_low_threshold[X_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.gyr_x = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_x = 0;
            }
        }
        break;


        case(MOTION_DETECTION_THRESHOLD_TYPE_GREATER_THAN):
        {
            if( _IQ12(MPU9250.gyro[X_AXIS]) >= MP.gyroscope_low_threshold[X_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.gyr_x = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_x = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_WITHIN_BOUNDS):
        {
            if( ( _IQ12(MPU9250.gyro[X_AXIS]) >= MP.gyroscope_low_threshold[X_AXIS] )
                && ( _IQ12(MPU9250.gyro[X_AXIS]) <= MP.gyroscope_high_threshold[X_AXIS] ) )
            {
                MP.motion_detection.motion_detection_segments.gyr_x = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_x = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_OUTSIDE_BOUNDS):
        {
            if( ( _IQ12(MPU9250.gyro[X_AXIS]) <= MP.gyroscope_low_threshold[X_AXIS] )
                || ( _IQ12(MPU9250.gyro[X_AXIS]) >= MP.gyroscope_high_threshold[X_AXIS] ) )
            {
                MP.motion_detection.motion_detection_segments.gyr_x = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_x = 0;
            }
        }
        break;
    }


    /* Gyroscope Y detection. */

    switch(MP.motion_detection.motion_detection_segments.gyr_y_threshold_type)
    {
        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_LESSER_THAN):
        {
            if( _IQ12abs( _IQ12(MPU9250.gyro[Y_AXIS]) ) <= MP.gyroscope_low_threshold[Y_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.gyr_y = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_y = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_GREATER_THAN):
        {
            if( _IQ12abs( _IQ12(MPU9250.gyro[Y_AXIS]) ) >= MP.gyroscope_low_threshold[Y_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.gyr_y = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_y = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_LESSER_THAN):
        {
            if( _IQ12(MPU9250.gyro[Y_AXIS])  <= MP.gyroscope_low_threshold[Y_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.gyr_y = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_y = 0;
            }
        }
        break;


        case(MOTION_DETECTION_THRESHOLD_TYPE_GREATER_THAN):
        {
            if( _IQ12(MPU9250.gyro[Y_AXIS]) >= MP.gyroscope_low_threshold[Y_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.gyr_y = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_y = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_WITHIN_BOUNDS):
        {
            if( ( _IQ12(MPU9250.gyro[Y_AXIS]) >= MP.gyroscope_low_threshold[Y_AXIS] )
                && ( _IQ12(MPU9250.gyro[Y_AXIS]) <= MP.gyroscope_high_threshold[Y_AXIS] ) )
            {
                MP.motion_detection.motion_detection_segments.gyr_y = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_y = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_OUTSIDE_BOUNDS):
        {
            if( ( _IQ12(MPU9250.gyro[Y_AXIS]) <= MP.gyroscope_low_threshold[Y_AXIS] )
                || ( _IQ12(MPU9250.gyro[Y_AXIS]) >= MP.gyroscope_high_threshold[Y_AXIS] ) )
            {
                MP.motion_detection.motion_detection_segments.gyr_y = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_y = 0;
            }
        }
        break;
    }

    /* Gyroscope Z detection. */

    switch(MP.motion_detection.motion_detection_segments.gyr_z_threshold_type)
    {
        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_LESSER_THAN):
        {
            if( _IQ12abs( _IQ12(MPU9250.gyro[Z_AXIS]) ) <= MP.gyroscope_low_threshold[Z_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.gyr_z = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_z = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_GREATER_THAN):
        {
            if( _IQ12abs( _IQ12(MPU9250.gyro[Z_AXIS]) ) >= MP.gyroscope_low_threshold[Z_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.gyr_z = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_z = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_LESSER_THAN):
        {
            if( _IQ12(MPU9250.gyro[Z_AXIS])  <= MP.gyroscope_low_threshold[Z_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.gyr_z = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_z = 0;
            }
        }
        break;


        case(MOTION_DETECTION_THRESHOLD_TYPE_GREATER_THAN):
        {
            if( _IQ12(MPU9250.gyro[Z_AXIS]) >= MP.gyroscope_low_threshold[Z_AXIS] )
            {
                MP.motion_detection.motion_detection_segments.gyr_z = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_z = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_WITHIN_BOUNDS):
        {
            if( ( _IQ12(MPU9250.gyro[Z_AXIS]) >= MP.gyroscope_low_threshold[Z_AXIS] )
                && ( _IQ12(MPU9250.gyro[Z_AXIS]) <= MP.gyroscope_high_threshold[Z_AXIS] ) )
            {
                MP.motion_detection.motion_detection_segments.gyr_z = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_z = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_OUTSIDE_BOUNDS):
        {
            if( ( _IQ12(MPU9250.gyro[Z_AXIS]) <= MP.gyroscope_low_threshold[Z_AXIS] )
                || ( _IQ12(MPU9250.gyro[Z_AXIS]) >= MP.gyroscope_high_threshold[Z_AXIS] ) )
            {
                MP.motion_detection.motion_detection_segments.gyr_z = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.gyr_z = 0;
            }
        }
        break;
    }





    /* Roll detection. */

    switch(MP.motion_detection.motion_detection_segments.roll_threshold_type)
    {
        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_LESSER_THAN):
        {
            if( _IQ12abs( MP.roll_filter ) <= MP.angle_low_threshold[ROLL] )
            {
                MP.motion_detection.motion_detection_segments.roll = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.roll = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_GREATER_THAN):
        {
            if( _IQ12abs( MP.roll_filter ) >= MP.angle_low_threshold[ROLL] )
            {
                MP.motion_detection.motion_detection_segments.roll = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.roll = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_LESSER_THAN):
        {
            if( MP.roll_filter  <= MP.angle_low_threshold[ROLL] )
            {
                MP.motion_detection.motion_detection_segments.roll = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.roll = 0;
            }
        }
        break;


        case(MOTION_DETECTION_THRESHOLD_TYPE_GREATER_THAN):
        {
            if( MP.roll_filter >= MP.angle_low_threshold[ROLL] )
            {
                MP.motion_detection.motion_detection_segments.roll = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.roll = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_WITHIN_BOUNDS):
        {
            if( ( MP.roll_filter >= MP.angle_low_threshold[ROLL] )
                && ( MP.roll_filter <= MP.angle_high_threshold[ROLL] ) )
            {
                MP.motion_detection.motion_detection_segments.roll = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.roll = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_OUTSIDE_BOUNDS):
        {
            if( ( MP.roll_filter <= MP.angle_low_threshold[ROLL] )
                || ( MP.roll_filter >= MP.angle_high_threshold[ROLL] ) )
            {
                MP.motion_detection.motion_detection_segments.roll = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.roll = 0;
            }
        }
        break;
    }


    /* Pitch detection. */

    switch(MP.motion_detection.motion_detection_segments.pitch_threshold_type)
    {
        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_LESSER_THAN):
        {
            if( _IQ12abs( MP.pitch_filter ) <= MP.angle_low_threshold[PITCH] )
            {
                MP.motion_detection.motion_detection_segments.pitch = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.pitch = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_GREATER_THAN):
        {
            if( _IQ12abs( MP.pitch_filter ) >= MP.angle_low_threshold[PITCH] )
            {
                MP.motion_detection.motion_detection_segments.pitch = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.pitch = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_LESSER_THAN):
        {
            if( MP.pitch_filter  <= MP.angle_low_threshold[PITCH] )
            {
                MP.motion_detection.motion_detection_segments.pitch = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.pitch = 0;
            }
        }
        break;


        case(MOTION_DETECTION_THRESHOLD_TYPE_GREATER_THAN):
        {
            if( MP.pitch_filter >= MP.angle_low_threshold[PITCH] )
            {
                MP.motion_detection.motion_detection_segments.pitch = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.pitch = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_WITHIN_BOUNDS):
        {
            if( ( MP.pitch_filter >= MP.angle_low_threshold[PITCH] )
                && ( MP.pitch_filter <= MP.angle_high_threshold[PITCH] ) )
            {
                MP.motion_detection.motion_detection_segments.pitch = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.pitch = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_OUTSIDE_BOUNDS):
        {
            if( ( MP.pitch_filter <= MP.angle_low_threshold[PITCH] )
                || ( MP.pitch_filter >= MP.angle_high_threshold[PITCH] ) )
            {
                MP.motion_detection.motion_detection_segments.pitch = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.pitch = 0;
            }
        }
        break;
    }



    /* Yaw detection. */

    switch(MP.motion_detection.motion_detection_segments.yaw_threshold_type)
    {
        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_LESSER_THAN):
        {
            if( _IQ12abs( MP.yaw_filter ) <= MP.angle_low_threshold[YAW] )
            {
                MP.motion_detection.motion_detection_segments.yaw = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.yaw = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_GREATER_THAN):
        {
            if( _IQ12abs( MP.yaw_filter ) >= MP.angle_low_threshold[YAW] )
            {
                MP.motion_detection.motion_detection_segments.yaw = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.yaw = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_LESSER_THAN):
        {
            if( MP.yaw_filter  <= MP.angle_low_threshold[YAW] )
            {
                MP.motion_detection.motion_detection_segments.yaw = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.yaw = 0;
            }
        }
        break;


        case(MOTION_DETECTION_THRESHOLD_TYPE_GREATER_THAN):
        {
            if( MP.yaw_filter >= MP.angle_low_threshold[YAW] )
            {
                MP.motion_detection.motion_detection_segments.yaw = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.yaw = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_WITHIN_BOUNDS):
        {
            if( ( MP.yaw_filter >= MP.angle_low_threshold[YAW] )
                && ( MP.yaw_filter <= MP.angle_high_threshold[YAW] ) )
            {
                MP.motion_detection.motion_detection_segments.yaw = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.yaw = 0;
            }
        }
        break;

        case(MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_OUTSIDE_BOUNDS):
        {
            if( ( MP.yaw_filter <= MP.angle_low_threshold[YAW] )
                || ( MP.yaw_filter >= MP.angle_high_threshold[YAW] ) )
            {
                MP.motion_detection.motion_detection_segments.yaw = 1;
            }
            else
            {
                MP.motion_detection.motion_detection_segments.yaw = 0;
            }
        }
        break;
    }


    /* Load data into communication stack. */

    MP_ROLL_ACCEL.all  = (int32_t)_IQ12toF( _IQ12mpy( MP.roll_accel, _IQ12(1000) ) );
    MP_ROLL_GYRO.all   = (int32_t)_IQ12toF( _IQ12mpy( MP.roll_gyro, _IQ12(1000) ) );
    MP_ROLL_FILTER.all = (int32_t)_IQ12toF( _IQ12mpy( MP.roll_filter, _IQ12(1000) ) );

    MP_PITCH_ACCEL.all  = (int32_t)_IQ12toF( _IQ12mpy( MP.pitch_accel, _IQ12(1000) ) );
    MP_PITCH_GYRO.all   = (int32_t)_IQ12toF( _IQ12mpy( MP.pitch_gyro, _IQ12(1000) ) );
    MP_PITCH_FILTER.all = (int32_t)_IQ12toF( _IQ12mpy( MP.pitch_filter, _IQ12(1000) ) );

    MP_YAW_ACCEL.all  = (int32_t)_IQ12toF( _IQ12mpy( MP.yaw_accel, _IQ12(1000) ) );
    MP_YAW_GYRO.all   = (int32_t)_IQ12toF( _IQ12mpy( MP.yaw_gyro, _IQ12(1000) ) );
    MP_YAW_FILTER.all = (int32_t)_IQ12toF( _IQ12mpy( MP.yaw_filter, _IQ12(1000) ) );

    MD_OUTPUT.MD_OUTPUT_segments.MD_OUTPUT_ACCEL_X = MP.motion_detection.motion_detection_segments.acc_x;
    MD_OUTPUT.MD_OUTPUT_segments.MD_OUTPUT_ACCEL_Y = MP.motion_detection.motion_detection_segments.acc_y;
    MD_OUTPUT.MD_OUTPUT_segments.MD_OUTPUT_ACCEL_Z = MP.motion_detection.motion_detection_segments.acc_z;
    MD_OUTPUT.MD_OUTPUT_segments.MD_OUTPUT_GYRO_X  = MP.motion_detection.motion_detection_segments.gyr_x;
    MD_OUTPUT.MD_OUTPUT_segments.MD_OUTPUT_GYRO_Y  = MP.motion_detection.motion_detection_segments.gyr_y;
    MD_OUTPUT.MD_OUTPUT_segments.MD_OUTPUT_GYRO_Z  = MP.motion_detection.motion_detection_segments.gyr_z;
    MD_OUTPUT.MD_OUTPUT_segments.MD_OUTPUT_PITCH   = MP.motion_detection.motion_detection_segments.roll;
    MD_OUTPUT.MD_OUTPUT_segments.MD_OUTPUT_YAW     = MP.motion_detection.motion_detection_segments.pitch;
    MD_OUTPUT.MD_OUTPUT_segments.MD_OUTPUT_ROLL    = MP.motion_detection.motion_detection_segments.yaw;
}
