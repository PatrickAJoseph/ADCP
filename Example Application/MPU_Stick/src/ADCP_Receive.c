
#include "ADCP_Receive.h"


/* This is the callback map for handling events on receiving parameters from host. */

const ADCP_Frame_Handler_t ADCP_Frame_Handler_Array[ADCP_N_FRAMES] =
{
  {
   .frame_id = IMU_X_RAW_FID,
   .prtFrame = (void*)&IMU_X_RAW,
   .function = (void*)0
  },
  {
    .frame_id = IMU_Y_RAW_FID,
    .prtFrame = (void*)&IMU_Y_RAW,
    .function = (void*)0
  },
  {
     .frame_id = IMU_Z_RAW_FID,
     .prtFrame = (void*)&IMU_Z_RAW,
     .function = (void*)0
  },
  {
    .frame_id = IMU_X_PROCESSED_FID,
    .prtFrame = (void*)&IMU_X_PROCESSED,
    .function = (void*)0
  },
  {
    .frame_id = IMU_Y_PROCESSED_FID,
    .prtFrame = (void*)&IMU_Y_PROCESSED,
    .function = (void*)0
  },
  {
     .frame_id = IMU_Z_PROCESSED_FID,
     .prtFrame = (void*)&IMU_Z_PROCESSED,
     .function = (void*)0
   },


  {.frame_id = IMU_CONFIG_REG_A_FID,
   .prtFrame = (void*)&IMU_CONFIG_REG_A,
   .function = &IMU_CONFIG_REG_A_callback },
  {.frame_id = IMU_CONFIG_REG_B_FID,
   .prtFrame = (void*)&IMU_CONFIG_REG_B,
   .function = &IMU_CONFIG_REG_B_callback },
  {.frame_id = IMU_CONFIG_WRITE_ENABLE_FID,
   .prtFrame = (void*)&IMU_CONFIG_WRITE_ENABLE,
   .function = &IMU_CONFIG_WRITE_ENABLE_callback },
  {.frame_id = MD_THRESHOLD_TYPE_FID,
   .prtFrame = (void*)&MD_THRESHOLD_TYPE,
   .function = &MD_THRESHOLD_TYPE_callback },

   {
      .frame_id = MP_ROLL_ACCEL_FID,
      .prtFrame = (void*)&MP_ROLL_ACCEL,
      .function = (void*)0
    },
   {
       .frame_id = MP_ROLL_GYRO_FID,
       .prtFrame = (void*)&MP_ROLL_GYRO,
       .function = (void*)0
   },
   {
        .frame_id = MP_ROLL_FILTER_FID,
        .prtFrame = (void*)&MP_ROLL_FILTER,
        .function = (void*)0
   },


   {
      .frame_id = MP_PITCH_ACCEL_FID,
      .prtFrame = (void*)&MP_PITCH_ACCEL,
      .function = (void*)0
    },
   {
       .frame_id = MP_PITCH_GYRO_FID,
       .prtFrame = (void*)&MP_PITCH_GYRO,
       .function = (void*)0
   },
   {
        .frame_id = MP_PITCH_FILTER_FID,
        .prtFrame = (void*)&MP_PITCH_FILTER,
        .function = (void*)0
   },


   {
      .frame_id = MP_YAW_ACCEL_FID,
      .prtFrame = (void*)&MP_YAW_ACCEL,
      .function = (void*)0
    },
   {
       .frame_id = MP_YAW_GYRO_FID,
       .prtFrame = (void*)&MP_YAW_GYRO,
       .function = (void*)0
   },
   {
        .frame_id = MP_YAW_FILTER_FID,
        .prtFrame = (void*)&MP_YAW_FILTER,
        .function = (void*)0
   },

   {
        .frame_id = MD_OUTPUT_FID,
        .prtFrame = (void*)&MD_OUTPUT,
        .function = (void*)0
   },
   {
        .frame_id = LED_OUTPUT_FID,
        .prtFrame = (void*)&LED_OUTPUT,
        .function = (void*)0
   },
   {
        .frame_id = WRITE_PARAMETER_FID,
        .prtFrame = (void*)&WRITE_PARAMETER,
        .function = (void*)0
   },
   {
        .frame_id = UPDATE_NODE_DATA_FID,
        .prtFrame = (void*)&UPDATE_NODE_DATA,
        .function = (void*)0
   },

  {.frame_id = MD_ACCEL_X_THRESHOLD_LOW_FID,
   .prtFrame = (void*)&MD_ACCEL_X_THRESHOLD_LOW,
   .function = &MD_ACCEL_X_THRESHOLD_LOW_callback },
  {.frame_id = MD_ACCEL_X_THRESHOLD_HIGH_FID,
   .prtFrame = (void*)&MD_ACCEL_X_THRESHOLD_HIGH,
   .function = &MD_ACCEL_X_THRESHOLD_HIGH_callback },
  {.frame_id = MD_ACCEL_Y_THRESHOLD_LOW_FID,
   .prtFrame = (void*)&MD_ACCEL_Y_THRESHOLD_LOW,
   .function = &MD_ACCEL_Y_THRESHOLD_LOW_callback },
  {.frame_id = MD_ACCEL_Y_THRESHOLD_HIGH_FID,
   .prtFrame = (void*)&MD_ACCEL_Y_THRESHOLD_HIGH,
   .function = &MD_ACCEL_Y_THRESHOLD_HIGH_callback },
  {.frame_id = MD_ACCEL_Z_THRESHOLD_LOW_FID,
   .prtFrame = (void*)&MD_ACCEL_Z_THRESHOLD_LOW,
   .function = &MD_ACCEL_Z_THRESHOLD_LOW_callback },
  {.frame_id = MD_ACCEL_Z_THRESHOLD_HIGH_FID,
   .prtFrame = (void*)&MD_ACCEL_Z_THRESHOLD_HIGH,
   .function = &MD_ACCEL_Z_THRESHOLD_HIGH_callback },


   {.frame_id = MD_GYRO_X_THRESHOLD_LOW_FID,
    .prtFrame = (void*)&MD_GYRO_X_THRESHOLD_LOW,
    .function = &MD_GYRO_X_THRESHOLD_LOW_callback },
   {.frame_id = MD_GYRO_X_THRESHOLD_HIGH_FID,
    .prtFrame = (void*)&MD_GYRO_X_THRESHOLD_HIGH,
    .function = &MD_GYRO_X_THRESHOLD_HIGH_callback },
   {.frame_id = MD_GYRO_Y_THRESHOLD_LOW_FID,
    .prtFrame = (void*)&MD_GYRO_Y_THRESHOLD_LOW,
    .function = &MD_GYRO_Y_THRESHOLD_LOW_callback },
   {.frame_id = MD_GYRO_Y_THRESHOLD_HIGH_FID,
    .prtFrame = (void*)&MD_GYRO_Y_THRESHOLD_HIGH,
    .function = &MD_GYRO_Y_THRESHOLD_HIGH_callback },
   {.frame_id = MD_GYRO_Z_THRESHOLD_LOW_FID,
    .prtFrame = (void*)&MD_GYRO_Z_THRESHOLD_LOW,
    .function = &MD_GYRO_Z_THRESHOLD_LOW_callback },
   {.frame_id = MD_GYRO_Z_THRESHOLD_HIGH_FID,
    .prtFrame = (void*)&MD_GYRO_Z_THRESHOLD_HIGH,
    .function = &MD_GYRO_Z_THRESHOLD_HIGH_callback },


  {.frame_id = MD_ROLL_THRESHOLD_LOW_FID,
   .prtFrame = (void*)&MD_ROLL_THRESHOLD_LOW,
   .function = &MD_ROLL_THRESHOLD_LOW_callback },
  {.frame_id = MD_ROLL_THRESHOLD_HIGH_FID,
   .prtFrame = (void*)&MD_ROLL_THRESHOLD_HIGH,
   .function = &MD_ROLL_THRESHOLD_HIGH_callback },
  {.frame_id = MD_PITCH_THRESHOLD_LOW_FID,
   .prtFrame = (void*)&MD_PITCH_THRESHOLD_LOW,
   .function = &MD_PITCH_THRESHOLD_LOW_callback },
  {.frame_id = MD_PITCH_THRESHOLD_HIGH_FID,
   .prtFrame = (void*)&MD_PITCH_THRESHOLD_HIGH,
   .function = &MD_PITCH_THRESHOLD_HIGH_callback },
  {.frame_id = MD_YAW_THRESHOLD_LOW_FID,
   .prtFrame = (void*)&MD_YAW_THRESHOLD_LOW,
   .function = &MD_YAW_THRESHOLD_LOW_callback },
  {.frame_id = MD_YAW_THRESHOLD_HIGH_FID,
   .prtFrame = (void*)&MD_YAW_THRESHOLD_HIGH,
   .function = &MD_YAW_THRESHOLD_HIGH_callback },

  { .frame_id = MP_ROLL_FILTER_CONSTANT_FID,
    .prtFrame = (void*)&MP_ROLL_FILTER_CONSTANT,
    .function = &MP_ROLL_FILTER_CONSTANT_callback,
  },
  { .frame_id = MP_PITCH_FILTER_CONSTANT_FID,
    .prtFrame = (void*)&MP_PITCH_FILTER_CONSTANT,
    .function = &MP_PITCH_FILTER_CONSTANT_callback,
  },
  { .frame_id = MP_YAW_FILTER_CONSTANT_FID,
    .prtFrame = (void*)&MP_YAW_FILTER_CONSTANT,
    .function = &MP_YAW_FILTER_CONSTANT_callback,
  },
  { .frame_id = MD_WRITE_CONFIG_FID,
    .prtFrame = (void*)&MD_WRITE_CONFIG,
    .function = &MD_WRITE_CONFIG_callback,
  },
  { .frame_id = LED_MODE_CONFIG_FID,
    .prtFrame = (void*)&LED_MODE_CONFIG,
    .function = &LED_MODE_CONFIG_callback,
  },
  { .frame_id = LED_UC_WRITE_FID,
    .prtFrame = (void*)&LED_UC_WRITE,
    .function = &LED_UC_WRITE_callback,
  },
  { .frame_id = CONTROL_FID,
    .prtFrame = (void*)&CONTROL,
    .function = &CONTROL_callback,
  }
};


static void write_parameter_acknowledgement(uint8_t parameter_index)
{
    uint8_t previous_connection_status = ADCP_connection_status;

    WRITE_PARAMETER.WRITE_PARAMETER_segments.WRITE_PARAMETER_INDEX = parameter_index;
    WRITE_PARAMETER.WRITE_PARAMETER_segments.WRITE_PARAMETER_ACK = 1;

    ADCP_connection_status = 1;

    ADCP_Transmit_Dataframe( (void*)&WRITE_PARAMETER, WRITE_PARAMETER_FID, 1 );

    ADCP_connection_status = previous_connection_status;

    WRITE_PARAMETER.WRITE_PARAMETER_segments.WRITE_PARAMETER_INDEX = 65535;
    WRITE_PARAMETER.WRITE_PARAMETER_segments.WRITE_PARAMETER_ACK = 0;
}

void IMU_CONFIG_REG_A_callback()
{
    /* Update sampling rate. */

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_SAMPLING_RATE_WE)
    {
        IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_SAMPLING_RATE_WE = 0;

        MPU9250.sampling_rate = IMU_CONFIG_REG_A.IMU_CONFIG_REG_A_segments.IMU_CONFIG_SAMPLING_RATE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_SAMPLING_RATE);

        MPU9250_configure();
    }

    /* Update calibration interval. */

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_CALIB_INTERVAL_WE)
    {
        IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_CALIB_INTERVAL_WE = 0;

        MPU9250.calibration_interval_ms = IMU_CONFIG_REG_A.IMU_CONFIG_REG_A_segments.IMU_CONFIG_CALIB_INTERVAL;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_CALIB_INTERVAL);
    }

    /* Update number of calibration points. */

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_CALIB_POINTS_WE)
    {
        IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_CALIB_POINTS_WE = 0;

        MPU9250.calibration_point_count = IMU_CONFIG_REG_A.IMU_CONFIG_REG_A_segments.IMU_CONFIG_CALIB_POINTS;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_CALIB_POINTS);
    }
}

void IMU_CONFIG_REG_B_callback()
{
    /* Update gyroscope filter bandwidth. */

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_GYRO_BW_WE)
    {
        IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_GYRO_BW_WE = 0;

        MPU9250.configuration_A.configuration_A_segments.gyroscope_filter_bandwidth =
        IMU_CONFIG_REG_B.IMU_CONFIG_REG_B_segments.IMU_CONFIG_GYRO_BW;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_GYRO_BW);

        MPU9250_configure();
    }

    /* Update accelerometer filter bandwidth. */

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_ACCEL_BW_WE)
    {
        IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_ACCEL_BW_WE = 0;

        MPU9250.configuration_A.configuration_A_segments.accelerometer_filter_bandwidth =
        IMU_CONFIG_REG_B.IMU_CONFIG_REG_B_segments.IMU_CONFIG_ACCEL_BW;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_ACCEL_BW);

        MPU9250_configure();
    }

    /* Update gyroscope full-scale value. */

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_GYRO_FS_WE)
    {
        IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_GYRO_FS_WE = 0;

        MPU9250.configuration_A.configuration_A_segments.gyroscope_full_scale =
        IMU_CONFIG_REG_B.IMU_CONFIG_REG_B_segments.IMU_CONFIG_GYRO_FS;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_GYRO_FS);

        MPU9250_configure();
    }

    /* Update accelerometer full-scale value. */

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_ACCEL_FS_WE)
    {
        IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_ACCEL_FS_WE = 0;

        MPU9250.configuration_A.configuration_A_segments.accelerometer_full_scale =
        IMU_CONFIG_REG_B.IMU_CONFIG_REG_B_segments.IMU_CONFIG_ACCEL_FS;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_ACCEL_FS);

        MPU9250_configure();
    }
}

void IMU_CONFIG_WRITE_ENABLE_callback()
{
    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_ACCEL_BW_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_ACCEL_BW_WE);
    }

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_ACCEL_FS_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_ACCEL_FS_WE);
    }

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_CALIB_INTERVAL_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_CALIB_INTERVAL_WE);
    }

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_CALIB_POINTS_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_CALIB_POINTS_WE);
    }

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_GYRO_BW_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_GYRO_BW_WE);
    }

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_GYRO_FS_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_GYRO_FS_WE);
    }

    if(IMU_CONFIG_WRITE_ENABLE.IMU_CONFIG_WRITE_ENABLE_segments.IMU_CONFIG_SAMPLING_RATE_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CONFIG_SAMPLING_RATE_WE);
    }
}

void MD_THRESHOLD_TYPE_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_X_THRESHOLD_TYPE_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_X_THRESHOLD_TYPE_WE = 0;

        MP.motion_detection.motion_detection_segments.acc_x_threshold_type =
        MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_ACCEL_X_THRESHOLD_TYPE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_X_THRESHOLD_TYPE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Y_THRESHOLD_TYPE_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Y_THRESHOLD_TYPE_WE = 0;

        MP.motion_detection.motion_detection_segments.acc_y_threshold_type =
        MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_ACCEL_Y_THRESHOLD_TYPE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_Y_THRESHOLD_TYPE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Z_THRESHOLD_TYPE_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Z_THRESHOLD_TYPE_WE = 0;

        MP.motion_detection.motion_detection_segments.acc_z_threshold_type =
        MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_ACCEL_Z_THRESHOLD_TYPE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_Z_THRESHOLD_TYPE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_X_THRESHOLD_TYPE_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_X_THRESHOLD_TYPE_WE = 0;

        MP.motion_detection.motion_detection_segments.gyr_x_threshold_type =
        MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_GYRO_X_THRESHOLD_TYPE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_X_THRESHOLD_TYPE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Y_THRESHOLD_TYPE_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Y_THRESHOLD_TYPE_WE = 0;

        MP.motion_detection.motion_detection_segments.gyr_y_threshold_type =
        MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_GYRO_Y_THRESHOLD_TYPE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_Y_THRESHOLD_TYPE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Z_THRESHOLD_TYPE_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Z_THRESHOLD_TYPE_WE = 0;

        MP.motion_detection.motion_detection_segments.gyr_z_threshold_type =
        MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_GYRO_Z_THRESHOLD_TYPE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_Z_THRESHOLD_TYPE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ROLL_THRESHOLD_TYPE_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ROLL_THRESHOLD_TYPE_WE = 0;

        MP.motion_detection.motion_detection_segments.roll_threshold_type =
        MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_ROLL_THRESHOLD_TYPE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ROLL_THRESHOLD_TYPE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_PITCH_THRESHOLD_TYPE_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_PITCH_THRESHOLD_TYPE_WE = 0;

        MP.motion_detection.motion_detection_segments.pitch_threshold_type =
        MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_PITCH_THRESHOLD_TYPE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_PITCH_THRESHOLD_TYPE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_YAW_THRESHOLD_TYPE_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_YAW_THRESHOLD_TYPE_WE = 0;

        MP.motion_detection.motion_detection_segments.yaw_threshold_type =
        MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_YAW_THRESHOLD_TYPE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_YAW_THRESHOLD_TYPE);
    }
}

void MD_ACCEL_X_THRESHOLD_LOW_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_X_THRESHOLD_LOW_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_X_THRESHOLD_LOW_WE = 0;

        IEEE754._int = MD_ACCEL_X_THRESHOLD_LOW.MD_ACCEL_X_THRESHOLD_LOW_segments.MD_ACCEL_X_THRESHOLD_LOW;

        MP.accelerometer_low_threshold[X_AXIS] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_X_THRESHOLD_LOW);
    }
}

void MD_ACCEL_X_THRESHOLD_HIGH_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_X_THRESHOLD_HIGH_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_X_THRESHOLD_HIGH_WE = 0;

        IEEE754._int = MD_ACCEL_X_THRESHOLD_HIGH.MD_ACCEL_X_THRESHOLD_HIGH_segments.MD_ACCEL_X_THRESHOLD_HIGH;

        /* Accelerometer threshold values in the firmware are mentioned in g * 1000.0f */

        MP.accelerometer_high_threshold[X_AXIS] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_X_THRESHOLD_HIGH);
    }
}

void MD_ACCEL_Y_THRESHOLD_LOW_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Y_THRESHOLD_LOW_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Y_THRESHOLD_LOW_WE = 0;

        IEEE754._int = MD_ACCEL_Y_THRESHOLD_LOW.MD_ACCEL_Y_THRESHOLD_LOW_segments.MD_ACCEL_Y_THRESHOLD_LOW;

        /* Accelerometer threshold values in the firmware are mentioned in g * 1000.0f */

        MP.accelerometer_low_threshold[Y_AXIS] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_Y_THRESHOLD_LOW);
    }
}

void MD_ACCEL_Y_THRESHOLD_HIGH_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Y_THRESHOLD_HIGH_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Y_THRESHOLD_HIGH_WE = 0;

        IEEE754._int = MD_ACCEL_Y_THRESHOLD_HIGH.MD_ACCEL_Y_THRESHOLD_HIGH_segments.MD_ACCEL_Y_THRESHOLD_HIGH;

        /* Accelerometer threshold values in the firmware are mentioned in g * 1000.0f */

        MP.accelerometer_low_threshold[Y_AXIS] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_Y_THRESHOLD_HIGH);
    }
}

void MD_ACCEL_Z_THRESHOLD_LOW_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Z_THRESHOLD_LOW_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Z_THRESHOLD_LOW_WE = 0;

        IEEE754._int = MD_ACCEL_Z_THRESHOLD_LOW.MD_ACCEL_Z_THRESHOLD_LOW_segments.MD_ACCEL_Z_THRESHOLD_LOW;

        /* Accelerometer threshold values in the firmware are mentioned in g * 1000.0f */

        MP.accelerometer_low_threshold[Z_AXIS] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_Z_THRESHOLD_LOW);
    }
}

void MD_ACCEL_Z_THRESHOLD_HIGH_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Z_THRESHOLD_HIGH_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Z_THRESHOLD_HIGH_WE = 0;

        IEEE754._int = MD_ACCEL_Z_THRESHOLD_HIGH.MD_ACCEL_Z_THRESHOLD_HIGH_segments.MD_ACCEL_Z_THRESHOLD_HIGH;

        /* Accelerometer threshold values in the firmware are mentioned in g * 1000.0f */

        MP.accelerometer_low_threshold[Z_AXIS] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_Z_THRESHOLD_HIGH);
    }
}

void MD_GYRO_X_THRESHOLD_LOW_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_X_THRESHOLD_LOW_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_X_THRESHOLD_LOW_WE = 0;

        IEEE754._int = MD_GYRO_X_THRESHOLD_LOW.MD_GYRO_X_THRESHOLD_LOW_segments.MD_GYRO_X_THRESHOLD_LOW;

        /* Gyroscope threshold values in the firmware are mentioned in DPS * 10.0f */

        MP.gyroscope_low_threshold[X_AXIS] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_X_THRESHOLD_LOW);
    }
}

void MD_GYRO_X_THRESHOLD_HIGH_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_X_THRESHOLD_HIGH_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_X_THRESHOLD_HIGH_WE = 0;

        IEEE754._int = MD_GYRO_X_THRESHOLD_HIGH.MD_GYRO_X_THRESHOLD_HIGH_segments.MD_GYRO_X_THRESHOLD_HIGH;

        /* Gyroscope threshold values in the firmware are mentioned in DPS * 10.0f */

        MP.gyroscope_high_threshold[X_AXIS] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_X_THRESHOLD_HIGH);
    }
}

void MD_GYRO_Y_THRESHOLD_LOW_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Y_THRESHOLD_LOW_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Y_THRESHOLD_LOW_WE = 0;

        IEEE754._int = MD_GYRO_Y_THRESHOLD_LOW.MD_GYRO_Y_THRESHOLD_LOW_segments.MD_GYRO_Y_THRESHOLD_LOW;

        /* Gyroscope threshold values in the firmware are mentioned in DPS * 10.0f */

        MP.gyroscope_low_threshold[Y_AXIS] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_Y_THRESHOLD_LOW);
    }
}

void MD_GYRO_Y_THRESHOLD_HIGH_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Y_THRESHOLD_HIGH_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Y_THRESHOLD_HIGH_WE = 0;

        IEEE754._int = MD_GYRO_Y_THRESHOLD_HIGH.MD_GYRO_Y_THRESHOLD_HIGH_segments.MD_GYRO_Y_THRESHOLD_HIGH;

        /* Gyroscope threshold values in the firmware are mentioned in DPS * 10.0f */

        MP.gyroscope_high_threshold[Y_AXIS] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_Y_THRESHOLD_HIGH);
    }
}

void MD_GYRO_Z_THRESHOLD_LOW_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Z_THRESHOLD_LOW_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Z_THRESHOLD_LOW_WE = 0;

        IEEE754._int = MD_GYRO_Z_THRESHOLD_LOW.MD_GYRO_Z_THRESHOLD_LOW_segments.MD_GYRO_Z_THRESHOLD_LOW;

        /* Gyroscope threshold values in the firmware are mentioned in DPS * 10.0f */

        MP.gyroscope_low_threshold[Z_AXIS] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_Z_THRESHOLD_LOW);
    }
}

void MD_GYRO_Z_THRESHOLD_HIGH_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Z_THRESHOLD_HIGH_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Z_THRESHOLD_HIGH_WE = 0;

        IEEE754._int = MD_GYRO_Z_THRESHOLD_HIGH.MD_GYRO_Z_THRESHOLD_HIGH_segments.MD_GYRO_Z_THRESHOLD_HIGH;

        /* Gyroscope threshold values in the firmware are mentioned in DPS * 10.0f */

        MP.gyroscope_high_threshold[Z_AXIS] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_Z_THRESHOLD_HIGH);
    }
}

void MD_ROLL_THRESHOLD_LOW_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ROLL_THRESHOLD_LOW_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ROLL_THRESHOLD_LOW_WE = 0;

        IEEE754._int = MD_ROLL_THRESHOLD_LOW.MD_ROLL_THRESHOLD_LOW_segments.MD_ROLL_THRESHOLD_LOW;

        /* Gyroscope threshold values in the firmware are mentioned in degrees */

        MP.angle_low_threshold[ROLL] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ROLL_THRESHOLD_LOW);
    }
}

void MD_ROLL_THRESHOLD_HIGH_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ROLL_THRESHOLD_HIGH_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ROLL_THRESHOLD_HIGH_WE = 0;

        IEEE754._int = MD_ROLL_THRESHOLD_HIGH.MD_ROLL_THRESHOLD_HIGH_segments.MD_ROLL_THRESHOLD_HIGH;

        /* Gyroscope threshold values in the firmware are mentioned in degrees */

        MP.angle_high_threshold[ROLL] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ROLL_THRESHOLD_HIGH);
    }
}

void MD_PITCH_THRESHOLD_LOW_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_PITCH_THRESHOLD_LOW_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_PITCH_THRESHOLD_LOW_WE = 0;

        IEEE754._int = MD_PITCH_THRESHOLD_LOW.MD_PITCH_THRESHOLD_LOW_segments.MD_PITCH_THRESHOLD_LOW;

        /* Gyroscope threshold values in the firmware are mentioned in degrees */

        MP.angle_low_threshold[PITCH] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_PITCH_THRESHOLD_LOW);
    }
}

void MD_PITCH_THRESHOLD_HIGH_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_PITCH_THRESHOLD_HIGH_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_PITCH_THRESHOLD_HIGH_WE = 0;

        IEEE754._int = MD_PITCH_THRESHOLD_HIGH.MD_PITCH_THRESHOLD_HIGH_segments.MD_PITCH_THRESHOLD_HIGH;

        /* Gyroscope threshold values in the firmware are mentioned in degrees */

        MP.angle_high_threshold[PITCH] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_PITCH_THRESHOLD_HIGH);
    }
}

void MD_YAW_THRESHOLD_LOW_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_YAW_THRESHOLD_LOW_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_YAW_THRESHOLD_LOW_WE = 0;

        IEEE754._int = MD_YAW_THRESHOLD_LOW.MD_YAW_THRESHOLD_LOW_segments.MD_YAW_THRESHOLD_LOW;

        /* Gyroscope threshold values in the firmware are mentioned in degrees */

        MP.angle_low_threshold[YAW] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_YAW_THRESHOLD_LOW);
    }
}

void MD_YAW_THRESHOLD_HIGH_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_YAW_THRESHOLD_HIGH_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_YAW_THRESHOLD_HIGH_WE = 0;

        IEEE754._int = MD_YAW_THRESHOLD_HIGH.MD_YAW_THRESHOLD_HIGH_segments.MD_YAW_THRESHOLD_HIGH;

        /* Gyroscope threshold values in the firmware are mentioned in degrees */

        MP.angle_high_threshold[YAW] = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_YAW_THRESHOLD_HIGH);
    }
}

void MP_ROLL_FILTER_CONSTANT_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MP_ROLL_FILTER_CONSTANT_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MP_ROLL_FILTER_CONSTANT_WE = 0;

        IEEE754._int = MP_ROLL_FILTER_CONSTANT.all;

        /* Complementary filter constants take values from 0 to 1. */

        MP.roll_filter_constant = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MP_ROLL_FILTER_CONSTANT);
    }
}

void MP_PITCH_FILTER_CONSTANT_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MP_PITCH_FILTER_CONSTANT_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MP_PITCH_FILTER_CONSTANT_WE = 0;

        IEEE754._int = MP_PITCH_FILTER_CONSTANT.all;

        /* Complementary filter constants take values from 0 to 1. */

        MP.pitch_filter_constant = _IQ12(IEEE754._float);

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MP_PITCH_FILTER_CONSTANT);
    }
}

void MP_YAW_FILTER_CONSTANT_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MP_YAW_FILTER_CONSTANT_WE)
    {
        MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MP_YAW_FILTER_CONSTANT_WE = 0;

        IEEE754._int = MP_YAW_FILTER_CONSTANT.all;
    }
}

void MD_WRITE_CONFIG_callback()
{
    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_X_THRESHOLD_HIGH_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_X_THRESHOLD_HIGH_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_X_THRESHOLD_LOW_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_X_THRESHOLD_LOW_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_X_THRESHOLD_TYPE_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_X_THRESHOLD_TYPE_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Y_THRESHOLD_HIGH_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_Y_THRESHOLD_HIGH_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Y_THRESHOLD_LOW_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_Y_THRESHOLD_LOW_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Y_THRESHOLD_TYPE_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_Y_THRESHOLD_TYPE_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Z_THRESHOLD_HIGH_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_Z_THRESHOLD_HIGH_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Z_THRESHOLD_LOW_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_Z_THRESHOLD_LOW_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ACCEL_Z_THRESHOLD_TYPE_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ACCEL_Z_THRESHOLD_TYPE_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_X_THRESHOLD_HIGH_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_X_THRESHOLD_HIGH_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_X_THRESHOLD_LOW_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_X_THRESHOLD_LOW_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_X_THRESHOLD_TYPE_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_X_THRESHOLD_TYPE_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Y_THRESHOLD_HIGH_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_Y_THRESHOLD_HIGH_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Y_THRESHOLD_LOW_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_Y_THRESHOLD_LOW_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Y_THRESHOLD_TYPE_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_Y_THRESHOLD_TYPE_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Z_THRESHOLD_HIGH_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_Z_THRESHOLD_HIGH_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Z_THRESHOLD_LOW_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_Z_THRESHOLD_LOW_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_GYRO_Z_THRESHOLD_TYPE_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_GYRO_Z_THRESHOLD_TYPE_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_PITCH_THRESHOLD_HIGH_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_PITCH_THRESHOLD_HIGH_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_PITCH_THRESHOLD_LOW_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_PITCH_THRESHOLD_LOW_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_PITCH_THRESHOLD_TYPE_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_PITCH_THRESHOLD_TYPE_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ROLL_THRESHOLD_HIGH_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ROLL_THRESHOLD_HIGH_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ROLL_THRESHOLD_LOW_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ROLL_THRESHOLD_LOW_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_ROLL_THRESHOLD_TYPE_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_ROLL_THRESHOLD_TYPE_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_YAW_THRESHOLD_HIGH_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_YAW_THRESHOLD_HIGH_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_YAW_THRESHOLD_LOW_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_YAW_THRESHOLD_LOW_WE);
    }

    if(MD_WRITE_CONFIG.MD_WRITE_CONFIG_segments.MD_YAW_THRESHOLD_TYPE_WE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MD_YAW_THRESHOLD_TYPE_WE);
    }
}

void LED_MODE_CONFIG_callback()
{
    if(LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED1_MODE_WE)
    {
        LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED1_MODE_WE = 0;

        LED1_config.LED_config_segments.LED_mode = LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED1_MODE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__LED1_MODE);

        LED_update();
    }

    if(LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED2_MODE_WE)
    {
        LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED2_MODE_WE = 0;

        LED2_config.LED_config_segments.LED_mode = LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED2_MODE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__LED2_MODE);

        LED_update();
    }

    if(LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED3_MODE_WE)
    {
        LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED3_MODE_WE = 0;

        LED3_config.LED_config_segments.LED_mode = LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED3_MODE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__LED3_MODE);

        LED_update();
    }
}

void LED_UC_WRITE_callback()
{
    if(LED_UC_WRITE.LED_UC_WRITE_segments.LED_UC_DUTY_WE)
    {
        LED_UC_WRITE.LED_UC_WRITE_segments.LED_UC_DUTY_WE = 0;

        LED1_config.LED_config_segments.LED_duty = LED_UC_WRITE.LED_UC_WRITE_segments.LED1_UC_DUTY;
        LED2_config.LED_config_segments.LED_duty = LED_UC_WRITE.LED_UC_WRITE_segments.LED2_UC_DUTY;
        LED3_config.LED_config_segments.LED_duty = LED_UC_WRITE.LED_UC_WRITE_segments.LED3_UC_DUTY;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__LED1_UC_DUTY);
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__LED2_UC_DUTY);
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__LED3_UC_DUTY);

        LED_update();
    }

    if(LED_UC_WRITE.LED_UC_WRITE_segments.LED_UC_STATE_WE)
    {
        LED_UC_WRITE.LED_UC_WRITE_segments.LED_UC_STATE_WE = 0;

        LED1_config.LED_config_segments.LED_state = LED_UC_WRITE.LED_UC_WRITE_segments.LED1_UC_STATE;
        LED2_config.LED_config_segments.LED_state = LED_UC_WRITE.LED_UC_WRITE_segments.LED2_UC_STATE;
        LED3_config.LED_config_segments.LED_state = LED_UC_WRITE.LED_UC_WRITE_segments.LED3_UC_STATE;

        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__LED1_UC_STATE);
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__LED2_UC_STATE);
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__LED3_UC_STATE);

        LED_update();
    }
}

void CONTROL_callback()
{
    if(CONTROL.CONTROL_segments.CONFIG_SAVE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__CONFIG_SAVE);

        __bic_SR_register(GIE);

        P2IE &= ~BIT0;
        TB0CCTL0 &= ~CCIE;
        P1OUT &= ~BIT0;

        uint16_t counter = 0;

        for( counter = 0 ; counter < 1000 ; counter++ )
        {
            __delay_cycles(24000);      //  1 milli-second delay
        }

        CONTROL.CONTROL_segments.CONFIG_SAVE = 0;
        config_write();

        for( counter = 0 ; counter < 1000 ; counter++ )
        {
            __delay_cycles(24000);      //  1 milli-second delay
        }

        P2IE |= BIT0;
        TB0CCTL0 |= CCIE;

        for( counter = 0 ; counter < 100 ; counter++ )
        {
            __delay_cycles(24000);      //  1 milli-second delay
        }

        __bis_SR_register(GIE);

    }

    if(CONTROL.CONTROL_segments.IMU_CALIBRATE)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__IMU_CALIBRATE);

        __bic_SR_register(GIE);

        P2IE &= ~BIT0;
        TB0CCTL0 &= ~CCIE;
        P1OUT &= ~BIT0;
        __delay_cycles(80000);

        CONTROL.CONTROL_segments.IMU_CALIBRATE = 0;
        MPU9250_calibrate();

        __delay_cycles(80000);
        P2IE |= BIT0;
        TB0CCTL0 |= CCIE;
        __delay_cycles(80000);


        __bis_SR_register(GIE);
    }

    if(CONTROL.CONTROL_segments.MP_RESET)
    {
        write_parameter_acknowledgement(WRITE_PARAMETER_INDEX__MP_RESET);

        CONTROL.CONTROL_segments.MP_RESET = 0;

        //MPU9250_calibrate();
        MP_Init();
    }
}



void UPDATE_NODE_DATA_callback()
{

    switch( UPDATE_NODE_DATA.UPDATE_NODE_DATA_segments.frame_id )
    {
        case(IMU_CONFIG_REG_A_FID):
        {
            IMU_CONFIG_REG_A.IMU_CONFIG_REG_A_segments.IMU_CONFIG_SAMPLING_RATE     =   MPU9250.sampling_rate;
            IMU_CONFIG_REG_A.IMU_CONFIG_REG_A_segments.IMU_CONFIG_CALIB_INTERVAL    =   MPU9250.calibration_interval_ms;
            IMU_CONFIG_REG_A.IMU_CONFIG_REG_A_segments.IMU_CONFIG_CALIB_POINTS      =   MPU9250.calibration_point_count;
        }
        break;

        case(IMU_CONFIG_REG_B_FID):
        {
            IMU_CONFIG_REG_B.IMU_CONFIG_REG_B_segments.IMU_CONFIG_ACCEL_FS =
            MPU9250.configuration_A.configuration_A_segments.accelerometer_full_scale;
            IMU_CONFIG_REG_B.IMU_CONFIG_REG_B_segments.IMU_CONFIG_ACCEL_BW =
            MPU9250.configuration_A.configuration_A_segments.accelerometer_filter_bandwidth;
            IMU_CONFIG_REG_B.IMU_CONFIG_REG_B_segments.IMU_CONFIG_GYRO_FS =
            MPU9250.configuration_A.configuration_A_segments.gyroscope_full_scale;
            IMU_CONFIG_REG_B.IMU_CONFIG_REG_B_segments.IMU_CONFIG_GYRO_BW =
            MPU9250.configuration_A.configuration_A_segments.gyroscope_filter_bandwidth;
        }
        break;

        case(MD_THRESHOLD_TYPE_FID):
        {
            MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_ACCEL_X_THRESHOLD_TYPE = MP.motion_detection.motion_detection_segments.acc_x_threshold_type;
            MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_ACCEL_Y_THRESHOLD_TYPE = MP.motion_detection.motion_detection_segments.acc_y_threshold_type;
            MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_ACCEL_Z_THRESHOLD_TYPE = MP.motion_detection.motion_detection_segments.acc_z_threshold_type;
            MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_GYRO_X_THRESHOLD_TYPE = MP.motion_detection.motion_detection_segments.gyr_x_threshold_type;
            MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_GYRO_Y_THRESHOLD_TYPE = MP.motion_detection.motion_detection_segments.gyr_y_threshold_type;
            MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_GYRO_Z_THRESHOLD_TYPE = MP.motion_detection.motion_detection_segments.gyr_z_threshold_type;
            MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_ROLL_THRESHOLD_TYPE   = MP.motion_detection.motion_detection_segments.roll_threshold_type;
            MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_PITCH_THRESHOLD_TYPE   = MP.motion_detection.motion_detection_segments.pitch_threshold_type;
            MD_THRESHOLD_TYPE.MD_THRESHOLD_TYPE_segments.MD_YAW_THRESHOLD_TYPE   = MP.motion_detection.motion_detection_segments.yaw_threshold_type;
        }
        break;

        case(MD_ACCEL_X_THRESHOLD_LOW_FID):
        {
            IEEE754._float = _IQ12toF(MP.accelerometer_low_threshold[X_AXIS]);
            MD_ACCEL_X_THRESHOLD_LOW.all = IEEE754._int;
        }
        break;

        case(MD_ACCEL_X_THRESHOLD_HIGH_FID):
        {
            IEEE754._float = _IQ12toF(MP.accelerometer_high_threshold[X_AXIS]);
            MD_ACCEL_X_THRESHOLD_HIGH.all = IEEE754._int;
        }
        break;

        case(MD_ACCEL_Y_THRESHOLD_LOW_FID):
        {
            IEEE754._float = _IQ12toF(MP.accelerometer_low_threshold[Y_AXIS]);
            MD_ACCEL_Y_THRESHOLD_LOW.all = IEEE754._int;
        }
        break;

        case(MD_ACCEL_Y_THRESHOLD_HIGH_FID):
        {
            IEEE754._float = _IQ12toF(MP.accelerometer_high_threshold[Y_AXIS]);
            MD_ACCEL_Y_THRESHOLD_HIGH.all = IEEE754._int;
        }
        break;

        case(MD_ACCEL_Z_THRESHOLD_LOW_FID):
        {
            IEEE754._float = _IQ12toF(MP.accelerometer_low_threshold[Z_AXIS]);
            MD_ACCEL_Z_THRESHOLD_LOW.all = IEEE754._int;
        }
        break;

        case(MD_ACCEL_Z_THRESHOLD_HIGH_FID):
        {
            IEEE754._float = _IQ12toF(MP.accelerometer_high_threshold[Z_AXIS]);
            MD_ACCEL_Z_THRESHOLD_HIGH.all = IEEE754._int;
        }
        break;

        case(MD_GYRO_X_THRESHOLD_LOW_FID):
        {
            IEEE754._float = _IQ12toF(MP.gyroscope_low_threshold[X_AXIS]);
            MD_GYRO_X_THRESHOLD_LOW.all = IEEE754._int;
        }
        break;

        case(MD_GYRO_X_THRESHOLD_HIGH_FID):
        {
            IEEE754._float = _IQ12toF(MP.gyroscope_high_threshold[X_AXIS]);
            MD_GYRO_X_THRESHOLD_HIGH.all = IEEE754._int;
        }
        break;

        case(MD_GYRO_Y_THRESHOLD_LOW_FID):
        {
            IEEE754._float = _IQ12toF(MP.gyroscope_low_threshold[Y_AXIS]);
            MD_GYRO_Y_THRESHOLD_LOW.all = IEEE754._int;
        }
        break;

        case(MD_GYRO_Y_THRESHOLD_HIGH_FID):
        {
            IEEE754._float = _IQ12toF(MP.gyroscope_high_threshold[Y_AXIS]);
            MD_GYRO_Y_THRESHOLD_HIGH.all = IEEE754._int;
        }
        break;


        case(MD_GYRO_Z_THRESHOLD_LOW_FID):
        {
            IEEE754._float = _IQ12toF(MP.gyroscope_low_threshold[Z_AXIS]);
            MD_GYRO_Z_THRESHOLD_LOW.all = IEEE754._int;
        }
        break;

        case(MD_GYRO_Z_THRESHOLD_HIGH_FID):
        {
            IEEE754._float = _IQ12toF(MP.gyroscope_high_threshold[Z_AXIS]);
            MD_GYRO_Z_THRESHOLD_HIGH.all = IEEE754._int;
        }
        break;

        case(MD_ROLL_THRESHOLD_LOW_FID):
        {
            IEEE754._float = _IQ12toF(MP.angle_low_threshold[ROLL]);
            MD_ROLL_THRESHOLD_LOW.all = IEEE754._int;
        }
        break;

        case(MD_ROLL_THRESHOLD_HIGH_FID):
        {
            IEEE754._float = _IQ12toF(MP.angle_high_threshold[ROLL]);
            MD_ROLL_THRESHOLD_HIGH.all = IEEE754._int;
        }
        break;

        case(MD_PITCH_THRESHOLD_LOW_FID):
        {
            IEEE754._float = _IQ12toF(MP.angle_low_threshold[PITCH]);
            MD_PITCH_THRESHOLD_LOW.all = IEEE754._int;
        }
        break;

        case(MD_PITCH_THRESHOLD_HIGH_FID):
        {
            IEEE754._float = _IQ12toF(MP.angle_high_threshold[PITCH]);
            MD_PITCH_THRESHOLD_HIGH.all = IEEE754._int;
        }
        break;

        case(MD_YAW_THRESHOLD_LOW_FID):
        {
            IEEE754._float = _IQ12toF(MP.angle_low_threshold[YAW]);
            MD_YAW_THRESHOLD_LOW.all = IEEE754._int;
        }
        break;

        case(MD_YAW_THRESHOLD_HIGH_FID):
        {
            IEEE754._float = _IQ12toF(MP.angle_high_threshold[YAW]);
            MD_YAW_THRESHOLD_HIGH.all = IEEE754._int;
        }
        break;

        case(MP_ROLL_FILTER_CONSTANT_FID):
        {
            IEEE754._float = _IQ12toF(MP.roll_filter_constant);
            MP_ROLL_FILTER_CONSTANT.all = IEEE754._int;
        }
        break;

        case(MP_PITCH_FILTER_CONSTANT_FID):
        {
            IEEE754._float = _IQ12toF(MP.pitch_filter_constant);
            MP_PITCH_FILTER_CONSTANT.all = IEEE754._int;
        }
        break;

        case(LED_MODE_CONFIG_FID):
        {
            LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED1_MODE = LED1_config.LED_config_segments.LED_mode;
            LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED2_MODE = LED1_config.LED_config_segments.LED_mode;
            LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED3_MODE = LED1_config.LED_config_segments.LED_mode;
        }
        break;

        default:
        {

        }
    }
}

