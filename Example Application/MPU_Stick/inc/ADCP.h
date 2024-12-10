
#ifndef ADCP_H_
#define ADCP_H_

#include <stdint.h>
#include <stdlib.h>

#define ADCP_RX_QUEUE_SIZE        					20UL
#define ADCP_TX_QUEUE_SIZE        				    20UL

/* Queue status. */

#define ADCP_QUEUE_EMPTY       						0
#define ADCP_QUEUE_FULL        						2
#define ADCP_QUEUE_OK          						1
#define ADCP_CHECKSUM_PASSED   						1
#define ADCP_CHECKSUM_FAILED   						0

/* Protocol packet parameter types. */

#define ADCP_PARAMETER_TYPE_UNSIGNED				0
#define ADCP_PARAMETER_TYPE_SIGNED					1
#define ADCP_PARAMETER_TYPE_FLOAT32					2

/* Protocol frame types. */

#define ADCP_FRAME_TYPE_DATA						(uint8_t)0
#define ADCP_FRAME_TYPE_CONNECTION_MANAGEMENT		(uint8_t)1
#define ADCP_FRAME_TYPE_REMOTE_REQUEST				(uint8_t)2
#define ADCP_FRAME_TYPE_ERROR_AND_STATUS			(uint8_t)3
#define ADCP_FRAME_TYPE_INVALID						(uint8_t)255

/* Control frame types. */

#define ADCP_CONNECTION_MANAGEMENT    				ADCP_FRAME_TYPE_CONNECTION_MANAGEMENT
#define ADCP_REMOTE_REQUEST           				ADCP_FRAME_TYPE_REMOTE_REQUEST
#define ADCP_ERROR_AND_STATUS         				ADCP_FRAME_TYPE_ERROR_AND_STATUS

/* Network error status codes. */

#define ADCP_NETWORK_ERROR_NONE                               (uint8_t)0
#define ADCP_NETWORK_ERROR_CONNECTION_ALREADY_OPEN            (uint8_t)1
#define ADCP_NETWORK_ERROR_CONNECTION_ALREADY_CLOSED          (uint8_t)2
#define ADCP_NETWORK_ERROR_INVALID_FRAME_REQUEST			  (uint8_t)3

/* Device node receive queue status. */

#define ADCP_DEVICE_NODE_RECEIVE_QUEUE_EMPTY                  (uint8_t)0
#define ADCP_DEVICE_NODE_RECEIVE_QUEUE_NOT_EMPTY              (uint8_t)1
#define ADCP_DEVICE_NODE_RECEIVE_QUEUE_FULL                   (uint8_t)2

/* General parameters. */

#define ADCP_NODE_ID								(uint8_t)0
#define ADCP_N_FRAMES							    (uint8_t)49
#define ADCP_N_TRANSMIT_FRAMES                      (uint8_t)17

typedef struct
{
  void (*function)();
  uint8_t frame_id;
  void*   prtFrame;
}ADCP_Frame_Handler_t;


extern const ADCP_Frame_Handler_t ADCP_Frame_Handler_Array[ADCP_N_FRAMES];

/* Processed packet structure. */

typedef struct
{
    uint8_t byte[9];
}ADCP_Processed_Packet_t;

/* Raw Packet structure. */

typedef struct
{
  uint8_t byte[7];
}ADCP_Raw_Packet_t;

/* Protocol Packet structure. */

typedef ADCP_Processed_Packet_t ADCP_Packet_t;

/* Parameter strcture. */

typedef struct
{
	uint8_t type;		/* Data type of parameter. */
	float   minimum;	/* Minimum value of parameter. */
	float   maximum;	/* Maximum value of parameter. */
	float   factor;		/* Factor. */
	float   offset;		/* Offset. */
	uint8_t start_bit;	/* Start bit. */
	uint8_t bit_length;	/* Bit length. */
}ADCP_Parameter_t;

//	Structure for IEEE754 32-bit notation to floating point conversion.

union IEEE754_struct
{
  float _float;
  uint32_t _int;  
}IEEE754;

extern ADCP_Packet_t ADCP_Transmit_Queue[ADCP_TX_QUEUE_SIZE];       //  Transmit Queue.
extern ADCP_Packet_t ADCP_Receive_Queue[ADCP_RX_QUEUE_SIZE];        //  Receive Queue.
extern int8_t ADCP_Transmit_Queue_Front;                            //  Transmit Queue front pointer.
extern int8_t ADCP_Transmit_Queue_Back;                             //  Transmit Queue back pointer.
extern int8_t ADCP_Receive_Queue_Front;                             //  Receive Queue front pointer.
extern int8_t ADCP_Receive_Queue_Back;                              //  Receive Queue back pointer.
extern uint8_t ADCP_Transmit_Queue_Element_Count;                   //  Receive Queue front pointer.
extern uint8_t ADCP_Receive_Queue_Element_Count;
extern uint8_t ADCP_Transmit_Queue_Status;
extern uint8_t ADCP_Receive_Queue_Status;

/* Connection management frame structure. */

struct _Connection_Management_Frame
{
  uint8_t CC_ACK:1;            //  Close connection acknowledgement.
  uint8_t CC_RQST:1;           //  Close connection request.
  uint8_t OC_ACK:1;            //  Open connection acknowledgement.
  uint8_t OC_RQST:1;           //  Open connection request.
  uint8_t SYNC_ACK:1;          //  Synchronization acknowledgement.
  uint8_t SYNC_RQST:1;         //  Synchronization request.
}ADCP_Connection_Management_Frame;


/* Remote request frame structure. */

struct _Remote_Request_Frame
{
  uint8_t FID_3: 8;            //  Request frame ID 3.
  uint8_t FID_2: 8;            //  Request frame ID 2.
  uint8_t FID_1: 8;            //  Request frame ID 1.
  uint8_t FID_0: 8;            //  Request frame ID 0.
  uint8_t N_FID: 6;            //  Number of requested frames.
  uint8_t RMT_ACK: 1;          //  Remote frame acknowledgement.
  uint8_t RMT_REQ: 1;          //  Remote frame request.
}ADCP_Remote_Request_Frame;

/* Error/status request frame structure. */

struct _Error_Status_Frame
{
  uint8_t AERR_HIGH:8;         //  Application error code.
  uint8_t AERR_LOW:8;          //  Application error code.
  uint8_t NES:8;               //  Network error/status.
  uint8_t QSZ:8;               //  Device receiver queue size.
  uint8_t QSTAT:2;             //  Device receiver queue status.
  uint8_t SACK:1;              //  Status acknowledgement.
  uint8_t SREQ:1;              //  Status request.
}ADCP_Error_Status_Frame;

extern uint8_t ADCP_connection_status;

/* UART Transmit message handler. The first argument is a buffer containing bytes
 * to be transmitted and the second argument is the number of bytes to be transmitted. */

void (*ADCP_Transmit_Handle)(uint8_t*, uint8_t);

/* Structure to contain information about data packet. */

typedef struct _ADCP_Packet_Info
{
  uint8_t priority;
  uint8_t frame_type;
  uint8_t node_id;  
}ADCP_Packet_Info_t;

/* Receive status registers. */

extern uint16_t ADCP_RSR[16];

/* ADCP flags. */

extern union _ADCP_flags
{
    uint8_t all;

    struct _flags
    {
      uint8_t backup_packet_loaded: 1;
    }flags;

}ADCP_flags;

/* Backup packet. */

extern ADCP_Packet_t backup_packet;

/* Frame pointer typedef. */

typedef void* ADCP_Frame_Pointer_t;

/* Transmit Group structure. */

/*
 * @brief:                      Function to initialize the protocol state machines.
 * @parameter  :                None
 * @return     :                None
 *
 * */

void ADCP_Init();

/*
 * @brief:                      Function to convert a raw data packet into processed format.
 * @parameter rawPacket:        Pointer to raw packet structure.
 * @parameter processedPacket:  Pointer to processed packet structure.
 *
 */

void ADCP_raw_to_processed(ADCP_Raw_Packet_t* rawPacket, ADCP_Processed_Packet_t* processedPacket);

/*
 * @brief:                      Function to convert a processed packet into raw format.
 * @parameter rawPacket:        Pointer to raw packet structure.
 * @parameter processedPacket:  Pointer to processed packet structure.
 *
 */

void ADCP_processed_to_raw(ADCP_Processed_Packet_t* processedPacket, ADCP_Raw_Packet_t* rawPacket);

/*
 * @brief:                      Function to push packet into transmit queue.
 * @parameter packet:           (Processed) packet structure to be pushed into the transmit queue.
 * @return status:              Status of the transmit queue.
 *
 */

uint8_t ADCP_Transmit_Enqueue(ADCP_Packet_t packet);

/*
 * @brief:                      Function to pull packet from transmit queue.
 * @parameter packet:           (Processed) packet structure to be pulled from the transmit queue.
 * @return status:              Status of the transmit queue.
 *
 */

uint8_t ADCP_Transmit_Dequeue(ADCP_Packet_t* packet);

/*
 * @brief:                      Function to flush contents of the transmit queue to bus.
 * @parameter:                  None.
 * @return:                     None.
 */

void ADCP_Transmit_Flush();

/*
 * @brief:                      Function to push packet into receive queue.
 * @parameter packet:           (Processed) packet structure to be pushed into the receive queue.
 * @return status:              Status of the receive queue.
 *
 */

uint8_t ADCP_Receive_Enqueue(ADCP_Packet_t packet);

/*
 * @brief:                      Function to pull packet from receive queue.
 * @parameter packet:           (Processed) packet structure to be pulled from the receive queue.
 * @return status:              Status of the receive queue.
 *
 */

uint8_t ADCP_Receive_Dequeue(ADCP_Packet_t* packet);

/*
 * @brief:                      Function to flush contents of the receive queue to bus.
 * @parameter:                  None.
 * @return:                     None.
 */

void ADCP_Receive_Flush();

/*
 * @brief:                      Function to calculate checksum of given raw packet.
 * @parameter rawPacket:        Pointer to raw packet.
 * @return:                     Checksum value of raw packet.
 *
 * @details: CAUTION: This function automatically updates the checksum bytes. Use this function
 *                    to generate checksum of packets to be transmitted to host.
 * */

uint8_t ADCP_generate_checksum_raw(ADCP_Raw_Packet_t* rawPacket);

/*
 * @brief:                      Function to calculate checksum of given raw packet.
 * @parameter processedPacket:  Pointer to processed packet.
 * @return:                     Checksum value of processed packet.
 *
 * @details: CAUTION: This function automatically updates the checksum bytes. Use this function
 *                    to generate checksum of packets to be transmitted to host.
 * */

uint8_t ADCP_generate_checksum_processed(ADCP_Processed_Packet_t* processedPacket);

/*
 * @brief:                      Function to verify checksum of given raw packet.
 * @parameter rawPacket:        Pointer to raw packet.
 * @return:                     Checksum status of raw packet.
 *
 * */

uint8_t ADCP_verify_checksum_raw(ADCP_Raw_Packet_t* rawPacket);

/*
 * @brief:                      Function to verify checksum of given processed packet.
 * @parameter processedPacket:  Pointer to processed packet.
 * @return:                     Checksum status of processed packet.
 *
 * */

uint8_t ADCP_verify_checksum_processed(ADCP_Processed_Packet_t* processedPacket);

/*
 * @brief:                      Function to verify checksum of given processed packet.
 * @parameter packet:  Pointer to packet.
 * @return:                     Checksum status of processed packet.
 *
 * */

uint8_t ADCP_verify_checksum(ADCP_Packet_t* packet);


/*
*  @brief:							Function to write parameter into strcture containing frame data.
*  @parameter parameter_details:	A constant structure containing details about parameter to be written into the frame strcture.
*  @parameter parameter_value:		Value of parameter to be written into the target frame strcture. Value is float by default.
*  @parameter frame_structure:		Pointer to structure/union containing frame data.	
*/



void ADCP_write_parameter(ADCP_Parameter_t parameter_details, float parameter_value, void* frame_structure);

/*
*  @brief:							Function to read parameter from strcture containing frame data.
*  @parameter parameter_details:	A constant structure containing details about parameter to be read from the frame strcture.
*  @parameter parameter_value:		Point to parameter to be read from the target frame strcture. Value is float by default.
*  @parameter frame_structure:		Pointer to structure/union containing frame data.	
*/

void ADCP_read_parameter(ADCP_Parameter_t parameter_details, float* parameter_value, void* frame_structure);

/*
*	@brief:							Function to write data bytes into packet.
*   @parameter frame:				Pointer to frame structure.
*   @return:						None.
*/

void ADCP_write_data_to_packet(void* frame, unsigned int node_id, unsigned int frame_id, unsigned int priority, ADCP_Packet_t* packet);

/*
*	@brief:							Function to read data bytes from packet.
*   @parameter frame:				Pointer to frame structure.
*   @return:						None.
*/

void ADCP_read_data_from_packet(void* frame, unsigned int* node_id, unsigned int* frame_id, unsigned int* priority, ADCP_Packet_t* packet);

/*
*	@brief:							Function to get type of packet.
*   @parameter packet:				Pointer to packet.
*	@return:						Enum value corresponding to packet type.
*/

uint8_t ADCP_packet_type(ADCP_Packet_t* packet);

/*
*	@brief:							Function to get priority of packet.
*   @parameter packet:				Pointer to packet.
*	@return:						Packet priority.
*
*   @details:						This function returns 0 if priority of frame is low, else
*									returns high indicating high priority.	 
*/

uint8_t ADCP_packet_priority(ADCP_Packet_t* packet);

/*
*   @brief:                         Function to get frame id of packet.
*   @parameter packet:              Pointer to packet.
*   @return:                        Frame ID.
*
*   @details:                       Returns frame ID of packet between 0 to 255.
*
*/

uint8_t ADCP_packet_frame_id(ADCP_Packet_t* packet);

/*
*	@brief:							Function to get node ID of packet.
*   @parameter packet:				Pointer to packet.
*	@return:						Packet node ID.
*
*   @details:						This function gets the node ID which has sent the packet.
* 
*/

uint8_t ADCP_packet_node_id(ADCP_Packet_t* packet);

/*
*
*	@brief:							Function information about protocol packet.
*	@parameter packet:				Pointer to protocol packet.
*
*/

void ADCP_packet_info(ADCP_Packet_t* packet, ADCP_Packet_Info_t* packetInfo);

/*
 * @brief:                          Function to send a control frame to the host.
 * @return:                         None.
 * @parameter:                      None.
 *
 * @details:                        NOTE: The relevant control frame's structures must be updated before calling
 *                                        this function.
 * */

void ADCP_send_control_frame(uint8_t node_id, uint8_t control_frame_type);

/*
 * @brief:                          Function to send a control frame to the host.
 * @return:                         None.
 * @parameter:                      packet: Pointer to protocol packet.
 * @details:                        NOTE: The relevant control frame's structures must be updated before calling
 *                                        this function.
 * */

void ADCP_read_control_frame(ADCP_Packet_t* packet);

/*
 * @brief:                          Function to handle packets received from the host.
 * @parameter:                      None.
 * @return:                         None.
 *
 * */

void ADCP_Receive_Handler();

/*
 * @brief:                          Function to handle connection management packets.
 * @parameter:                      Pointer to protocol packet.
 *
 */

void ADCP_CM_Handler(ADCP_Packet_t* packet);

/*
 * @brief:                          Function to handle remote request packets.
 * @parameter:                      Pointer to protocol packet.
 *
 */

void ADCP_RRQST_Handler(ADCP_Packet_t* packet);

/*
 * @brief:                          Function to handle error/status packets.
 * @parameter:                      Pointer to protocol packet.
 *
 */

void ADCP_ERRSTAT_Handler(ADCP_Packet_t* packet);

/*
 * @brief:                          Function to transmit a frame.
 * @parameter frame:                Pointer to frame to be transmitted.
 * @parameter priority:             Priority of frame.
 * @return:                         None.
 *
 * */

void ADCP_Transmit_Dataframe(void* frame,uint8_t frame_id, uint8_t priority);

/*
 * @brief:                          Function to enqueue a frame into the transmit queue.
 * @parameter frame:                Pointer to frame to be transmitted.
 * @parameter priority:             Priority of frame.
 * @return:                         None.
 *
 * */

void ADCP_Enqueue_Dataframe(void* frame, uint8_t frame_id, uint8_t priority);

/*
 * @brief:                          Function to flush the data frames from transmit queue.
 * @parameter:                      None
 * @return:                         None.
 *
 * */

void ADCP_Flush_Dataframes();

/*
 * @brief:                          Function to set value of receive status register.
 * @parameter frame_ID:             Frame ID whose receive status value has to be changed.
 * @parameter value:                New value of the receive status.
 *
 * */

void ADCP_RSR_write(uint8_t frame_ID, uint8_t value);

/*
 * @brief:                          Function to get value of receive status register.
 * @parameter frame_ID:             Frame ID whose receive status value has to be changed.
 * @return:                         Receive status of the target frame ID.
 * */

uint8_t ADCP_RSR_read(uint8_t frame_ID);


void ADCP_breakpoint();

#endif /* ADCP_H_ */
