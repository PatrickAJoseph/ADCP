
#include "ADCP_Transmit.h"
#include <msp430.h>

/* Transmit group tick counters. */

uint8_t ADCP_Transmit_Group_1_Tick_Counter = 0;

/* Transmit group transmit frames. */

const ADCP_Frame_Pointer_t ADCP_Transmit_Group_1_Frame_Pointers[] = {
   (void*)&IMU_X_RAW,
   (void*)&IMU_Y_RAW,
   (void*)&IMU_Z_RAW,
   (void*)&IMU_X_PROCESSED,
   (void*)&IMU_Y_PROCESSED,
   (void*)&IMU_Z_PROCESSED,
   (void*)&MP_ROLL_ACCEL,
   (void*)&MP_ROLL_GYRO,
   (void*)&MP_ROLL_FILTER,
   (void*)&MP_PITCH_ACCEL,
   (void*)&MP_PITCH_GYRO,
   (void*)&MP_PITCH_FILTER,
   (void*)&MP_YAW_ACCEL,
   (void*)&MP_YAW_GYRO,
   (void*)&MP_YAW_FILTER,
   (void*)&MD_OUTPUT,
   (void*)&LED_OUTPUT
};

/* Transmit group frame IDs. */

const uint8_t ADCP_Transmit_Group_1_Frame_IDs[] =
{
 IMU_X_RAW_FID,
 IMU_Y_RAW_FID,
 IMU_Z_RAW_FID,
 IMU_X_PROCESSED_FID,
 IMU_Y_PROCESSED_FID,
 IMU_Z_PROCESSED_FID,
 MP_ROLL_ACCEL_FID,
 MP_ROLL_GYRO_FID,
 MP_ROLL_FILTER_FID,
 MP_PITCH_ACCEL_FID,
 MP_PITCH_GYRO_FID,
 MP_PITCH_FILTER_FID,
 MP_YAW_ACCEL_FID,
 MP_YAW_GYRO_FID,
 MP_YAW_FILTER_FID,
 MD_OUTPUT_FID,
 LED_OUTPUT_FID
};

/* Transmit group transmit frame priorities. */

const uint8_t ADCP_Transmit_Group_1_Priorities[ ((ADCP_TRANSMIT_GROUP_1_N_FRAMES) >> 3) + 1 ] = { 0x00 };


void ADCP_Transmit_Encode(void* frame, uint8_t frame_id, uint8_t priority, ADCP_Packet_t* packet)
{
    ADCP_Raw_Packet_t rawPacket;
    ADCP_Processed_Packet_t processedPacket;
    uint32_t data = *(uint32_t*)frame;

    rawPacket.byte[0] = ( ( priority << 7 ) | ADCP_NODE_ID );
    rawPacket.byte[1] = frame_id;
    rawPacket.byte[2] = ( (data & 0xFF000000) >> 24 );
    rawPacket.byte[3] = ( (data & 0x00FF0000) >> 16 );
    rawPacket.byte[4] = ( (data & 0x0000FF00) >> 8 );
    rawPacket.byte[5] = (uint8_t)data;

    ADCP_generate_checksum_raw(&rawPacket);
    ADCP_raw_to_processed(&rawPacket,&processedPacket);

    (*packet) = (ADCP_Packet_t)processedPacket;
}

void ADCP_Transmit_Handler()
{
    P1OUT |= BIT1;

    uint8_t i = 0;
    uint8_t priority = 0;
    uint8_t position = 0;
    ADCP_Packet_t packet;

    /* Transmit group 1 handler. */

    ADCP_Transmit_Group_1_Tick_Counter++;       //  Increment tick counter.

    /* If tick counter for given transmit group has reached interval. */

    if(ADCP_Transmit_Group_1_Tick_Counter >= ADCP_TRANSMIT_GROUP_1_TRANSMIT_INTERVAL_TICK )
    {
        ADCP_Transmit_Group_1_Tick_Counter = 0;     //  Reset counter.

        /* Loop through frames corresponding to current transmit group. */

        for(i = 0 ; i < ADCP_TRANSMIT_GROUP_1_N_FRAMES; i ++)
        {
            priority = ADCP_Transmit_Group_1_Priorities[ i >> 3 ];
            position = (i % 8);
            priority = ((priority >> position) & ( 1 << position ));

            /* Encode frame. */

            ADCP_Transmit_Encode( (void*)ADCP_Transmit_Group_1_Frame_Pointers[i],
                                  ADCP_Transmit_Group_1_Frame_IDs[i],
                                  priority,
                                  &packet
                                 );

            /* Enqueue into transmit frame. */

            ADCP_Transmit_Enqueue(packet);
        }
    }


    if( ADCP_Transmit_Handle )
    {
        ADCP_Flush_Dataframes();
    }

    P1OUT &= ~BIT1;
}


