
#ifndef ADCP_TRANSMIT_H_
#define ADCP_TRANSMIT_H_

#include "ADCP.h"
#include "ADCP_frames.h"

#define ADCP_N_TRANSMIT_GROUPS              1

/* Number of frames per transmit groups. */

#define ADCP_TRANSMIT_GROUP_1_N_FRAMES      17

/* Transmit group transmit tick interval. */

#define ADCP_TRANSMIT_GROUP_1_TRANSMIT_INTERVAL_TICK    1

/* Transmit group tick counters. */

extern uint8_t ADCP_Transmit_Group_1_Tick_Counter;

/* Transmit group transmit frames. */

extern const ADCP_Frame_Pointer_t ADCP_Transmit_Group_1_Frame_Pointers[];

/* Transmit group frame IDs. */

extern const uint8_t ADCP_Transmit_Group_1_Frame_IDs[];

/* Transmit group transmit frame priorities. */

extern const uint8_t ADCP_Transmit_Group_1_Priorities[ ((ADCP_TRANSMIT_GROUP_1_N_FRAMES) >> 3) + 1 ];


/* ADCP Transmit Handler. */

void ADCP_Transmit_Handler();

/* ADCP Transmit Encoder. */

void ADCP_Transmit_Encode(void* pFrame, uint8_t frame_ID, uint8_t priority, ADCP_Packet_t* packet);

#endif /* ADCP_TRANSMIT_H_ */
