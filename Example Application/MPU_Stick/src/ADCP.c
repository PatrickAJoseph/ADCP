
#include "ADCP.h"

#include <stdio.h>

ADCP_Packet_t ADCP_Transmit_Queue[ADCP_TX_QUEUE_SIZE];       //  Transmit Queue.
ADCP_Packet_t ADCP_Receive_Queue[ADCP_RX_QUEUE_SIZE];        //  Receive Queue.
int8_t ADCP_Transmit_Queue_Front = -1;                       //  Transmit Queue front pointer.
int8_t ADCP_Transmit_Queue_Back = -1;                        //  Transmit Queue back pointer.
int8_t ADCP_Receive_Queue_Front = -1;                        //  Receive Queue front pointer.
int8_t ADCP_Receive_Queue_Back = -1;                         //  Receive Queue back pointer.
uint8_t ADCP_Transmit_Queue_Element_Count = 0;                //  Number of elements in transmit queue.
uint8_t ADCP_Receive_Queue_Element_Count = 0;				 //  Number of elements in receive queue.
uint8_t ADCP_Transmit_Queue_Status = ADCP_QUEUE_EMPTY;
uint8_t ADCP_Receive_Queue_Status = ADCP_QUEUE_EMPTY;

uint8_t ADCP_connection_status = 0;

uint16_t ADCP_RSR[16] = {0};

union _ADCP_flags ADCP_flags = {.all = 0};

ADCP_Packet_t backup_packet = {0};

/*
 * @brief:                      Function to convert a raw data packet into processed format.
 * @parameter rawPacket:        Pointer to raw packet structure.
 * @parameter processedPacket:  Pointer to processed packet structure.
 *
 */


/*
 * @brief:                      Function to initialize the protocol state machines.
 * @parameter  :                None
 * @return     :                None
 *
 * */

void ADCP_Init()
{
  /* Initialize error and status frames. */

  ADCP_Error_Status_Frame.AERR_HIGH = 0;
  ADCP_Error_Status_Frame.AERR_LOW  = 0;
  ADCP_Error_Status_Frame.NES       = ADCP_NETWORK_ERROR_NONE;
  ADCP_Error_Status_Frame.QSTAT     = ADCP_DEVICE_NODE_RECEIVE_QUEUE_EMPTY;
  ADCP_Error_Status_Frame.QSZ       = 0;
  ADCP_Error_Status_Frame.SACK      = 0;
  ADCP_Error_Status_Frame.SREQ      = 0;

  /* Initialize connection management frame. */

  ADCP_Connection_Management_Frame.CC_ACK       =   0;
  ADCP_Connection_Management_Frame.CC_RQST      =   0;
  ADCP_Connection_Management_Frame.OC_ACK       =   0;
  ADCP_Connection_Management_Frame.OC_RQST      =   0;
  ADCP_Connection_Management_Frame.SYNC_ACK     =   0;
  ADCP_Connection_Management_Frame.SYNC_RQST    =   0;

  /* Initialize remote request frame structure. */

  ADCP_Remote_Request_Frame.FID_0               =   0;
  ADCP_Remote_Request_Frame.FID_1               =   0;
  ADCP_Remote_Request_Frame.FID_2               =   0;
  ADCP_Remote_Request_Frame.FID_3               =   0;
  ADCP_Remote_Request_Frame.N_FID               =   0;
  ADCP_Remote_Request_Frame.RMT_ACK             =   0;
  ADCP_Remote_Request_Frame.RMT_ACK             =   0;

  ADCP_Receive_Queue_Status = ADCP_QUEUE_EMPTY;
  ADCP_Transmit_Queue_Status = ADCP_QUEUE_EMPTY;

  ADCP_Receive_Queue_Element_Count = 0;
  ADCP_Transmit_Queue_Element_Count = 0;
  //ADCP_Receive_Queue_Front = -1;
  //ADCP_Receive_Queue_Back  = -1;
  //ADCP_Transmit_Queue_Front = -1;
  //ADCP_Transmit_Queue_Back = -1;
}


void ADCP_raw_to_processed(ADCP_Raw_Packet_t* rawPacket, ADCP_Processed_Packet_t* processedPacket)
{
  uint16_t carry = 0;

  /* Obtain carry byte by adding each byte of raw frame */	

  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)rawPacket->byte[0] ) >> 8 ) << 0 );
  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)rawPacket->byte[1] ) >> 8 ) << 1 );
  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)rawPacket->byte[2] ) >> 8 ) << 2 );
  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)rawPacket->byte[3] ) >> 8 ) << 3 );
  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)rawPacket->byte[4] ) >> 8 ) << 4 );
  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)rawPacket->byte[5] ) >> 8 ) << 5 );
  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)rawPacket->byte[6] ) >> 8 ) << 6 );
  
  carry += 0x0B;
  
  processedPacket->byte[0] = carry;		//	Load carry byte.
  
  /* Compute other processed bytes */

  int i = 0;

  for(i = 1 ; i <= 7 ; i++)
  {	
    /* If byte value is greater than or equal to 0xFF-0x0A = 0xF5, then when 0x0B is added to it,
	   a carry is generated. In that case, first, the raw value must be added with 0x0B, the leftover
	   must be taken ( 0x00 <= leftover <= 0x0A ) and the leftover is added with 0x0B. */

    if(rawPacket->byte[i-1] >= 0xF5)
	{
	  processedPacket->byte[i] = (((uint16_t)rawPacket->byte[i-1] + (uint16_t)0x0B) & (uint16_t)0xFF) + 0x0B;	
	}
	else
	{
	  processedPacket->byte[i] = 	(rawPacket->byte[i-1] + 0x0B);
	}
  }
  
  processedPacket->byte[8] = 0x0A;		//	Termination byte (0x0A).
}

/*
 * @brief:                      Function to convert a processed packet into raw format.
 * @parameter rawPacket:        Pointer to raw packet structure.
 * @parameter processedPacket:  Pointer to processed packet structure.
 *
 */

void ADCP_processed_to_raw(ADCP_Processed_Packet_t* processedPacket, ADCP_Raw_Packet_t* rawPacket)
{
  uint8_t carry = (processedPacket->byte[0] - 0x0B);
  
  rawPacket->byte[0] = (processedPacket->byte[1]-0x0B) + ( ( ((carry & ( 1 << 0 )) >> 0) ) * 0xF5);
  rawPacket->byte[1] = (processedPacket->byte[2]-0x0B) + ( ( ((carry & ( 1 << 1 )) >> 1) ) * 0xF5);
  rawPacket->byte[2] = (processedPacket->byte[3]-0x0B) + ( ( ((carry & ( 1 << 2 )) >> 2) ) * 0xF5);
  rawPacket->byte[3] = (processedPacket->byte[4]-0x0B) + ( ( ((carry & ( 1 << 3 )) >> 3) ) * 0xF5);
  rawPacket->byte[4] = (processedPacket->byte[5]-0x0B) + ( ( ((carry & ( 1 << 4 )) >> 4) ) * 0xF5);
  rawPacket->byte[5] = (processedPacket->byte[6]-0x0B) + ( ( ((carry & ( 1 << 5 )) >> 5) ) * 0xF5);
  rawPacket->byte[6] = (processedPacket->byte[7]-0x0B) + ( ( ((carry & ( 1 << 6 )) >> 6) ) * 0xF5);
}

/*
 * @brief:                      Function to push packet into transmit queue.
 * @parameter packet:           (Processed) packet structure to be pushed into the transmit queue.
 * @return status:              Status of the transmit queue.
 *
 */

uint8_t ADCP_Transmit_Enqueue(ADCP_Packet_t packet)
{
  uint8_t priority = ADCP_packet_priority(&packet);	
	
  if( ( ADCP_Transmit_Queue_Front == -1 ) && ( ADCP_Transmit_Queue_Back == -1 ) )
  {
	ADCP_Transmit_Queue_Front = 0;
	ADCP_Transmit_Queue_Back = 0;
	ADCP_Transmit_Queue_Element_Count++;
  }
  else
  {
	if(ADCP_Transmit_Queue_Element_Count >= ADCP_TX_QUEUE_SIZE)
	{
		ADCP_Transmit_Queue_Element_Count = ADCP_TX_QUEUE_SIZE;
		return(ADCP_Transmit_Queue_Status = ADCP_QUEUE_FULL);  
	}	  
	  
	ADCP_Transmit_Queue_Element_Count++;
	
	if(!priority)
	{
		ADCP_Transmit_Queue_Back++;
	}
	else
	{
		//printf("\nPriority of packet is high !\n");
		ADCP_Transmit_Queue_Front--;
	}
	
	if(ADCP_Transmit_Queue_Back == ADCP_TX_QUEUE_SIZE)
	{
		ADCP_Transmit_Queue_Back = 0;
	}
	
	if(ADCP_Transmit_Queue_Front < 0)
	{
		ADCP_Transmit_Queue_Front = (ADCP_TX_QUEUE_SIZE - 1);
	}
  }
  
  if( !priority )
  {
	ADCP_Transmit_Queue[ADCP_Transmit_Queue_Back] = packet;
  }
  else  
  {
	ADCP_Transmit_Queue[ADCP_Transmit_Queue_Front] = packet;
  }
  
  return(ADCP_Transmit_Queue_Status = ADCP_QUEUE_OK);
}

/*
 * @brief:                      Function to pull packet from transmit queue.
 * @parameter packet:           (Processed) packet structure to be pulled from the transmit queue.
 * @return status:              Status of the transmit queue.
 *
 */

uint8_t ADCP_Transmit_Dequeue(ADCP_Packet_t* packet)
{
  if( ( ADCP_Transmit_Queue_Front == -1 ) && ( ADCP_Transmit_Queue_Back == -1 ) )
  {
	return(ADCP_Transmit_Queue_Status = ADCP_QUEUE_EMPTY);  
  }
  else if( ADCP_Transmit_Queue_Front == ADCP_Transmit_Queue_Back )
  {
    (*packet) = ADCP_Transmit_Queue[ADCP_Transmit_Queue_Front];

	ADCP_Transmit_Queue_Front = -1;
	ADCP_Transmit_Queue_Back = -1;
  }
  else if( ADCP_Transmit_Queue_Front != ADCP_Transmit_Queue_Back )
  {
    (*packet) = ADCP_Transmit_Queue[ADCP_Transmit_Queue_Front];
	  
	ADCP_Transmit_Queue_Front++;

	if( ADCP_Transmit_Queue_Front >= ADCP_TX_QUEUE_SIZE )
	{
		ADCP_Transmit_Queue_Front = 0;
	}
  }
  
  ADCP_Transmit_Queue_Element_Count--;
  
  return(ADCP_Transmit_Queue_Status = ADCP_QUEUE_OK);
}

/*
 * @brief:                      Function to flush contents of the transmit queue to bus.
 * @parameter:                  None.
 * @return:                     None.
 */

void ADCP_Transmit_Flush()
{
	if( *ADCP_Transmit_Handle )
	{
		
	}
}


/*
 * @brief:                      Function to push packet into receive queue.
 * @parameter packet:           (Processed) packet structure to be pushed into the receive queue.
 * @return status:              Status of the receive queue.
 *
 */

uint8_t ADCP_Receive_Enqueue(ADCP_Packet_t packet)
{
  ADCP_Packet_Info_t packetInfo;
  ADCP_packet_info(&packet,&packetInfo);
  
  uint8_t priority = packetInfo.priority;
  uint8_t node_id  = packetInfo.node_id;
    
  
  if(node_id == ADCP_NODE_ID)
  {
	  if( ( ADCP_Receive_Queue_Front == -1 ) && ( ADCP_Receive_Queue_Back == -1 ) )
	  {
		ADCP_Receive_Queue_Front = 0;
		ADCP_Receive_Queue_Back = 0;
		ADCP_Receive_Queue_Element_Count++;
		ADCP_Receive_Queue[ADCP_Receive_Queue_Front] = packet;
		ADCP_Receive_Queue_Status = ADCP_QUEUE_OK;
	  }
	  else
	  {
		if(ADCP_Receive_Queue_Element_Count >= ADCP_TX_QUEUE_SIZE)
		{
			ADCP_Receive_Queue_Element_Count = ADCP_TX_QUEUE_SIZE;
			ADCP_Receive_Queue_Status = ADCP_QUEUE_FULL;
		}	  
		else
		{
            ADCP_Receive_Queue_Element_Count++;

            if(!priority)
            {
                ADCP_Receive_Queue_Back++;
            }
            else
            {
                ADCP_Receive_Queue_Front--;
            }

            if(ADCP_Receive_Queue_Back == ADCP_TX_QUEUE_SIZE)
            {
                ADCP_Receive_Queue_Back = 0;
            }

            if(ADCP_Receive_Queue_Front < 0)
            {
                ADCP_Receive_Queue_Front = (ADCP_TX_QUEUE_SIZE - 1);
            }

            if( !priority )
            {
              ADCP_Receive_Queue[ADCP_Receive_Queue_Back] = packet;
            }
            else
            {
              ADCP_Receive_Queue[ADCP_Receive_Queue_Front] = packet;
            }

            if(ADCP_Receive_Queue_Element_Count >= ADCP_RX_QUEUE_SIZE)
            {
              ADCP_Receive_Queue_Element_Count = ADCP_RX_QUEUE_SIZE;
              ADCP_Receive_Queue_Status = ADCP_QUEUE_FULL;
            }
            else
            {
              ADCP_Receive_Queue_Status = ADCP_QUEUE_OK;
            }
		}
	  }
  }

  return(ADCP_Receive_Queue_Status);
}

/*
 * @brief:                      Function to pull packet from receive queue.
 * @parameter packet:           (Processed) packet structure to be pulled from the receive queue.
 * @return status:              Status of the receive queue.
 *
 */

uint8_t ADCP_Receive_Dequeue(ADCP_Packet_t* packet)
{
  if( ( ADCP_Receive_Queue_Front == -1 ) && ( ADCP_Receive_Queue_Back == -1 ) )
  {
	ADCP_Receive_Queue_Status = ADCP_QUEUE_EMPTY;
  }
  else if( ADCP_Receive_Queue_Front == ADCP_Receive_Queue_Back )
  {
	packet->byte[0] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[0];
	packet->byte[1] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[1];
	packet->byte[2] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[2];
	packet->byte[3] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[3];
	packet->byte[4] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[4];
	packet->byte[5] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[5];
	packet->byte[6] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[6];
	packet->byte[7] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[7];
	packet->byte[8] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[8];

	ADCP_Receive_Queue_Front = -1;
	ADCP_Receive_Queue_Back = -1;
	ADCP_Receive_Queue_Status = ADCP_QUEUE_EMPTY;
	ADCP_Receive_Queue_Element_Count--;
  }
  else if( ADCP_Receive_Queue_Front != ADCP_Receive_Queue_Back )
  {
	//(*packet) = ADCP_Receive_Queue[ADCP_Receive_Queue_Front];

      packet->byte[0] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[0];
      packet->byte[1] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[1];
      packet->byte[2] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[2];
      packet->byte[3] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[3];
      packet->byte[4] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[4];
      packet->byte[5] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[5];
      packet->byte[6] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[6];
      packet->byte[7] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[7];
      packet->byte[8] = ADCP_Receive_Queue[ADCP_Receive_Queue_Front].byte[8];

	ADCP_Receive_Queue_Front++;

	if( ADCP_Receive_Queue_Front >= ADCP_TX_QUEUE_SIZE )
	{
		ADCP_Receive_Queue_Front = 0;
	}

	ADCP_Receive_Queue_Element_Count--;
	ADCP_Receive_Queue_Status = ADCP_QUEUE_OK;
  }
  
  return(ADCP_Receive_Queue_Status);
}

/*
 * @brief:                      Function to flush contents of the receive queue to bus.
 * @parameter:                  None.
 * @return:                     None.
 */

void ADCP_Receive_Flush()
{

}

/*
 * @brief:                      Function to calculate checksum of given raw packet.
 * @parameter rawPacket:        Pointer to raw packet.
 * @return:                     Checksum value of raw packet.
 *
 * @details: CAUTION: This function automatically updates the checksum bytes. Use this function
 *                    to generate checksum of packets to be transmitted to host.
 * */

uint8_t ADCP_generate_checksum_raw(ADCP_Raw_Packet_t* rawPacket)
{
	uint16_t checksum = 0;

	checksum += (uint16_t)rawPacket->byte[0];
	checksum += (uint16_t)rawPacket->byte[1];
	checksum += (uint16_t)rawPacket->byte[2];
	checksum += (uint16_t)rawPacket->byte[3];
	checksum += (uint16_t)rawPacket->byte[4];
	checksum += (uint16_t)rawPacket->byte[5];
	
	checksum = (255 - (checksum%255));
	
	rawPacket->byte[6] = (uint8_t)checksum;
	
	return((uint8_t)checksum);
}

/*
 * @brief:                      Function to calculate checksum of given raw packet.
 * @parameter processedPacket:  Pointer to processed packet.
 * @return:                     Checksum value of processed packet.
 *
 * @details: CAUTION: This function automatically updates the checksum bytes. Use this function
 *                    to generate checksum of packets to be transmitted to host.
 * */

uint8_t ADCP_generate_checksum_processed(ADCP_Processed_Packet_t* processedPacket)
{
	ADCP_Raw_Packet_t rawPacket;
    ADCP_processed_to_raw(processedPacket,&rawPacket);
	uint8_t checksum = ADCP_generate_checksum_raw(&rawPacket);
	ADCP_raw_to_processed(&rawPacket,processedPacket);
	return(checksum);
}

/*
 * @brief:                      Function to verify checksum of given raw packet.
 * @parameter rawPacket:        Pointer to raw packet.
 * @return:                     Checksum status of raw packet.
 *
 * */

uint8_t ADCP_verify_checksum_raw(ADCP_Raw_Packet_t* rawPacket)
{
  uint16_t checksum = 0;

  checksum += (uint16_t)rawPacket->byte[0];  
  checksum += (uint16_t)rawPacket->byte[1];
  checksum += (uint16_t)rawPacket->byte[2];
  checksum += (uint16_t)rawPacket->byte[3];
  checksum += (uint16_t)rawPacket->byte[4];
  checksum += (uint16_t)rawPacket->byte[5];
  checksum += (uint16_t)rawPacket->byte[6];

  uint8_t checksumStatus = 0;

  if((checksum%255)==0)
  {
    checksumStatus = ADCP_CHECKSUM_PASSED;    	  
  }
  else
  {
    checksumStatus = ADCP_CHECKSUM_FAILED;	  
  }
  
  return(checksumStatus);
}

/*
 * @brief:                      Function to verify checksum of given processed packet.
 * @parameter processedPacket:  Pointer to processed packet.
 * @return:                     Checksum status of processed packet.
 *
 * */

uint8_t ADCP_verify_checksum_processed(ADCP_Processed_Packet_t* processedPacket)
{
  ADCP_Raw_Packet_t rawPacket;
  ADCP_processed_to_raw(processedPacket,&rawPacket);  
  return(ADCP_verify_checksum_raw(&rawPacket));	
}

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

void ADCP_write_parameter(ADCP_Parameter_t parameter_details, float parameter_value, void* frame)
{
  float frameValue = 0;		//	Float frame value.
  uint32_t iFrameValue = 0;		//	Integer frame value.
  
  /* Limit value of the parameter between mentioned min and max values. */
  
  if( parameter_value > parameter_details.maximum )
  {
	parameter_value = parameter_details.maximum;  
  }
  if( parameter_value < parameter_details.minimum )
  {
	parameter_value = parameter_details.minimum;  
  }
  
  uint32_t frameData = *((uint32_t*)frame);
    
  /* When loading data into the frame, the following conversion has to be done:
  *
  *   frame_value = (actual_value - offset) / factor;
  *
  */
  
  frameValue = ( (parameter_value) - (parameter_details.offset) ) / (parameter_details.factor);

  //  Generate mask for the value: Mask = (2^(bit_length)) - 1.

 uint32_t mask = (( (uint32_t)1 << (uint16_t)(parameter_details.bit_length) ) - 1);
  
  // Clear bits associated with current parameter.
   
  if(parameter_details.bit_length == 32)
  {
	frameData = 0;  
  }
  else
  {
	frameData &= ~( mask << (parameter_details.start_bit) );   
  }
  
  // Compute the binary value of the frame value.
  
  switch(parameter_details.type)
  {
    case(ADCP_PARAMETER_TYPE_UNSIGNED):
    {
	  if(parameter_details.bit_length != 32)	
        iFrameValue = ( ((uint32_t)frameValue) & mask );
	  else
        iFrameValue = ((uint32_t)frameValue & (uint32_t)0xFFFFFFFF);		  
    }
    break;

	case(ADCP_PARAMETER_TYPE_SIGNED):
    {
	  uint8_t _sign = (uint8_t)(frameValue < 0);
	  
	  if(parameter_details.bit_length != 32)
	  {
	  
	    if( frameValue < 0 )
	    {
		  frameValue = -frameValue;  
	    }
	  
	    if(_sign)
	    {
		  iFrameValue = (  ((uint32_t)1 << (uint16_t)parameter_details.bit_length) - ( (uint32_t)frameValue & mask ) ) & mask;   
	    }
	    else
	    {
	 	  iFrameValue = ( (uint32_t)frameValue & mask );  
	    }
	  }
	  else
	  {
        iFrameValue = (uint32_t)frameValue;
	  }		  
	}
    break;

	case(ADCP_PARAMETER_TYPE_FLOAT32):
    {
	  IEEE754._float = (float)frameValue;
	  iFrameValue = IEEE754._int;
    }
    break;	
  }

  /* Merge bits to existing frame data. */

  frameData |= (iFrameValue << (parameter_details.start_bit));
  *((uint32_t*)frame) = frameData;
}


/*
*  @brief:							Function to read parameter from strcture containing frame data.
*  @parameter parameter_details:	A constant structure containing details about parameter to be read from the frame strcture.
*  @parameter parameter_value:		Point to parameter to be read from the target frame strcture. Value is float by default.
*  @parameter frame_structure:		Pointer to structure/union containing frame data.	
*/

void ADCP_read_parameter(ADCP_Parameter_t parameter_details, float* parameter_value, void* frame)
{
  uint32_t frameData = *((uint32_t*)frame);
  uint32_t mask;
  
  if( parameter_details.bit_length != 32 )
	mask = (  ((uint32_t)1 << (uint16_t)parameter_details.bit_length) - 1  );
  else
	mask = 0xFFFFFFFF;  
  
  uint32_t iFrameValue;
  float actualValue;

  if( parameter_details.bit_length != 32 )
  {
	iFrameValue = (mask & (frameData >> (uint16_t)parameter_details.start_bit));  
  }
  else
  {
	iFrameValue = (uint32_t)(frameData & 0xFFFFFFFF);  
  }

  switch(parameter_details.type)
  {
    case(ADCP_PARAMETER_TYPE_UNSIGNED):
    {
	  actualValue = ((float)parameter_details.offset + ((float)iFrameValue * (float)parameter_details.factor)); 
    }
    break;

    case(ADCP_PARAMETER_TYPE_SIGNED):
    {
      uint32_t maxAbsValue = (( (uint32_t)1UL << (parameter_details.bit_length - 1) ) - 1);

      if( iFrameValue > maxAbsValue ) // When value of number > 2*(N_bits-1) value is -ve.
      {
        if( parameter_details.bit_length < 32 )
        {
          actualValue = ( parameter_details.offset - parameter_details.factor * (float)( ( 1UL << parameter_details.bit_length ) - iFrameValue ) );
        }
        else
        {
          actualValue = ( parameter_details.factor * (float)( (int32_t)iFrameValue ) + parameter_details.offset );
        }
      }
      else /* If number is positive. */
      {
        actualValue = ( parameter_details.factor * (float)iFrameValue + parameter_details.offset );
      }      
    }
    break;	
	
	case(ADCP_PARAMETER_TYPE_FLOAT32):
	{
	  IEEE754._int = iFrameValue;
	  actualValue = ( (parameter_details.factor * IEEE754._float) + parameter_details.offset ); 			
	}
	break;
  }


   if(parameter_value != NULL)
   {
	 *parameter_value = actualValue;	
   }
}

/*
*	@brief:							Function to write data bytes into packet.
*   @parameter frame:				Pointer to frame structure.
*   @return:						None.
*/

void ADCP_write_data_to_packet(void* frame, unsigned int node_id, unsigned int frame_ID, unsigned int priority, ADCP_Packet_t* packet)
{
  ADCP_Raw_Packet_t rawPacket;

  ADCP_processed_to_raw((ADCP_Processed_Packet_t*)packet,&rawPacket);
  
  rawPacket.byte[0] = (node_id&0x0F) | (priority << 7);
  rawPacket.byte[1] = frame_ID;
  rawPacket.byte[2] = ( ((*(uint32_t*)frame) & 0xFF000000) >> 24); 
  rawPacket.byte[3] = ( ((*(uint32_t*)frame) & 0x00FF0000) >> 16); 
  rawPacket.byte[4] = ( ((*(uint32_t*)frame) & 0x0000FF00) >> 8); 
  rawPacket.byte[5] = ( ((*(uint32_t*)frame) & 0x000000FF) >> 0); 
  
  ADCP_generate_checksum_raw(&rawPacket);
  ADCP_raw_to_processed(&rawPacket,(ADCP_Processed_Packet_t*)packet);
}

/*
*	@brief:							Function to read data bytes from packet.
*   @parameter frame:				Pointer to frame structure.
*   @return:						None.
*/

void ADCP_read_data_from_packet(void* frame, unsigned int* node_id, unsigned int* frame_ID, unsigned int* priority, ADCP_Packet_t* packet)
{
  ADCP_Raw_Packet_t rawPacket;

  ADCP_processed_to_raw((ADCP_Processed_Packet_t*)packet,&rawPacket);

  uint32_t data = 0;
  
  *priority = rawPacket.byte[0] >> 7;
  *node_id = (rawPacket.byte[0] & 0x0F);
  *frame_ID = rawPacket.byte[1];
    
  data |= ((uint32_t)rawPacket.byte[2] << 24);
  data |= ((uint32_t)rawPacket.byte[3] << 16);
  data |= ((uint32_t)rawPacket.byte[4] << 8);
  data |= ((uint32_t)rawPacket.byte[5] << 0);
  
  *((uint32_t*)frame) = data;  
}


/*
*	@brief:							Function to get type of packet.
*   @parameter packet:				Pointer to packet.
*	@return:						Enum value corresponding to packet type.
*/

uint8_t ADCP_packet_type(ADCP_Packet_t* packet)
{
  uint8_t carry = (packet->byte[0] - 0x0B);

  uint8_t byte = (packet->byte[1]-0x0B) + ( ( ((carry & ( 1 << 0 )) >> 0) ) * 0xF5);
  
  uint8_t packet_type = ((byte & 0x70) >> 4);
  
  if( packet_type > ADCP_FRAME_TYPE_ERROR_AND_STATUS )
  {
	packet_type = ADCP_FRAME_TYPE_INVALID;  
  }
  
  return( packet_type );
}

/*
*	@brief:							Function to get priority of packet.
*   @parameter packet:				Pointer to packet.
*	@return:						Packet priority.
*
*   @details:						This function returns 0 if priority of frame is low, else
*									returns high indicating high priority.	 
*/

uint8_t ADCP_packet_priority(ADCP_Packet_t* packet)
{
  uint8_t carry = (packet->byte[0] - 0x0B);

  uint8_t byte = (packet->byte[1]-0x0B) + ( ( ((carry & ( 1 << 0 )) >> 0) ) * 0xF5);
      
  return((byte & 0x80) >> 7);	
}

/*
*   @brief:                         Function to get frame id of packet.
*   @parameter packet:              Pointer to packet.
*   @return:                        Frame ID.
*
*   @details:                       Returns frame ID of packet between 0 to 255.
*
*/

uint8_t ADCP_packet_frame_id(ADCP_Packet_t* packet)
{
    uint8_t carry = (packet->byte[0] - 0x0B);

    uint8_t byte = (packet->byte[2]-0x0B) + ( ( ((carry & ( 1 << 0 )) >> 0) ) * 0xF5);

    return(byte);
}

/*
 * @brief:                          Function to send a control frame to the host.
 * @return:                         None.
 * @parameter:                      None.
 *
 * @details:                        NOTE: The relevant control frame's structures must be updated before calling
 *                                        this function.
 * */

void ADCP_send_control_frame(uint8_t node_id, uint8_t control_frame_type)
{
  ADCP_Raw_Packet_t rawPacket;
  ADCP_Processed_Packet_t processedPacket;
  uint8_t is_valid = 0;

  switch(control_frame_type)
  {
    case(ADCP_CONNECTION_MANAGEMENT):
    {
        rawPacket.byte[0] = ( ( 1 << 7 ) | ( 1 << 4 ) | ( node_id & 0x0F ) );
        rawPacket.byte[1] = (  ( (uint8_t)ADCP_Connection_Management_Frame.SYNC_RQST << 5 ) |
                               ( (uint8_t)ADCP_Connection_Management_Frame.SYNC_ACK  << 4 )  |
                               ( (uint8_t)ADCP_Connection_Management_Frame.OC_RQST   << 3 )   |
                               ( (uint8_t)ADCP_Connection_Management_Frame.OC_ACK    << 2 )   |
                               ( (uint8_t)ADCP_Connection_Management_Frame.CC_RQST   << 1 )   |
                               ( (uint8_t)ADCP_Connection_Management_Frame.CC_ACK    << 0 )   );
        rawPacket.byte[2] = 0;
        rawPacket.byte[3] = 0;
        rawPacket.byte[4] = 0;
        rawPacket.byte[5] = 0;
        rawPacket.byte[6] = 0;

        ADCP_generate_checksum_raw(&rawPacket);
        ADCP_raw_to_processed(&rawPacket,&processedPacket);

        is_valid = 1;
    }
    break;

    case(ADCP_REMOTE_REQUEST):
    {
        rawPacket.byte[0] = ( ( 1 << 7 ) | ( 2 << 4 ) | ( node_id & 0x0F ) );
        rawPacket.byte[1] = ( ( (uint8_t)ADCP_Remote_Request_Frame.RMT_REQ << 7 ) |
                              ( (uint8_t)ADCP_Remote_Request_Frame.RMT_ACK << 6 ) |
                              ( (uint8_t)ADCP_Remote_Request_Frame.N_FID << 0 )  );
        rawPacket.byte[2] = ADCP_Remote_Request_Frame.FID_0;
        rawPacket.byte[3] = ADCP_Remote_Request_Frame.FID_1;
        rawPacket.byte[4] = ADCP_Remote_Request_Frame.FID_2;
        rawPacket.byte[5] = ADCP_Remote_Request_Frame.FID_3;

        ADCP_generate_checksum_raw(&rawPacket);
        ADCP_raw_to_processed(&rawPacket,&processedPacket);

        is_valid = 1;
    }
    break;

    case(ADCP_ERROR_AND_STATUS):
    {
        rawPacket.byte[0] = ( ( 1 << 7 ) | ( 3 << 4 ) | ( node_id & 0x0F ) );
        rawPacket.byte[1] = (  ( (uint8_t)ADCP_Error_Status_Frame.SREQ << 3 ) |
                               ( (uint8_t)ADCP_Error_Status_Frame.SACK << 2 ) |
                               ( (uint8_t)ADCP_Error_Status_Frame.QSTAT << 0 ) );
        rawPacket.byte[2] = ADCP_Error_Status_Frame.QSZ;
        rawPacket.byte[3] = ADCP_Error_Status_Frame.NES;
        rawPacket.byte[4] = ADCP_Error_Status_Frame.AERR_HIGH;
        rawPacket.byte[5] = ADCP_Error_Status_Frame.AERR_LOW;

        ADCP_generate_checksum_raw(&rawPacket);
        ADCP_raw_to_processed(&rawPacket,&processedPacket);

        is_valid = 1;
    }
    break;

    default:
    {
        is_valid = 0;
    }
  }

  if( ( ADCP_Transmit_Handle != NULL ) && ( is_valid ) )
  {
      uint8_t tempTransmitBuffer[9];

      tempTransmitBuffer[0] = processedPacket.byte[0];
      tempTransmitBuffer[1] = processedPacket.byte[1];
      tempTransmitBuffer[2] = processedPacket.byte[2];
      tempTransmitBuffer[3] = processedPacket.byte[3];
      tempTransmitBuffer[4] = processedPacket.byte[4];
      tempTransmitBuffer[5] = processedPacket.byte[5];
      tempTransmitBuffer[6] = processedPacket.byte[6];
      tempTransmitBuffer[7] = processedPacket.byte[7];
      tempTransmitBuffer[8] = processedPacket.byte[8];

      ADCP_Transmit_Handle(tempTransmitBuffer,9);
  }
}

/*
 * @brief:                          Function to send a control frame to the host.
 * @return:                         None.
 * @parameter:                      node_id: Pointer to node id.
 *                                  control_frame_type: Pointer to control_frame_type.
 * @details:                        NOTE: The relevant control frame's structures must be updated before calling
 *                                        this function.
 * */

void ADCP_read_control_frame(ADCP_Packet_t* packet)
{
    ADCP_Raw_Packet_t rawPacket;

    uint8_t packet_type = ADCP_packet_type(packet);

    switch( packet_type )
    {
        case(ADCP_CONNECTION_MANAGEMENT):
        {
            ADCP_processed_to_raw((ADCP_Processed_Packet_t*)packet, &rawPacket);

            ADCP_Connection_Management_Frame.SYNC_RQST = (( rawPacket.byte[1] & ( 1 << 5 ) ) >> 5);
            //ADCP_Connection_Management_Frame.SYNC_ACK  = (( rawPacket.byte[1] & ( 1 << 4 ) ) >> 4);
            ADCP_Connection_Management_Frame.OC_RQST   = (( rawPacket.byte[1] & ( 1 << 3 ) ) >> 3);
            //ADCP_Connection_Management_Frame.OC_ACK    = (( rawPacket.byte[1] & ( 1 << 2 ) ) >> 2);
            ADCP_Connection_Management_Frame.CC_RQST   = (( rawPacket.byte[1] & ( 1 << 1 ) ) >> 1);
            //ADCP_Connection_Management_Frame.CC_ACK    = (( rawPacket.byte[1] & ( 1 << 0 ) ) >> 0);
        }
        break;

        case(ADCP_REMOTE_REQUEST):
        {
            ADCP_processed_to_raw((ADCP_Processed_Packet_t*)packet, &rawPacket);

            ADCP_Remote_Request_Frame.RMT_REQ           = ( ( rawPacket.byte[1] & ( 1 << 7 ) ) >> 7 );
            //ADCP_Remote_Request_Frame.RMT_ACK           = ( ( rawPacket.byte[1] & ( 1 << 6 ) ) >> 6 );
            ADCP_Remote_Request_Frame.N_FID             = ( ( rawPacket.byte[1] & ( 0x0F << 0 ) ) >> 0 );
            ADCP_Remote_Request_Frame.FID_0             = rawPacket.byte[2];
            ADCP_Remote_Request_Frame.FID_1             = rawPacket.byte[3];
            ADCP_Remote_Request_Frame.FID_2             = rawPacket.byte[4];
            ADCP_Remote_Request_Frame.FID_3             = rawPacket.byte[5];
        }
        break;

        case(ADCP_ERROR_AND_STATUS):
        {
            ADCP_processed_to_raw((ADCP_Processed_Packet_t*)packet, &rawPacket);

            ADCP_Error_Status_Frame.SREQ                =   ( ( rawPacket.byte[1] & ( 1 << 3 ) ) >> 3 );
            //ADCP_Error_Status_Frame.SACK                =   ( ( rawPacket.byte[1] & ( 1 << 2 ) ) >> 2 );
            //ADCP_Error_Status_Frame.QSTAT               =   ( ( rawPacket.byte[1] & ( 3 << 0 ) ) >> 0 );
            //ADCP_Error_Status_Frame.QSZ                 =   rawPacket.byte[2];
            //ADCP_Error_Status_Frame.NES                 =   rawPacket.byte[3];
            //ADCP_Error_Status_Frame.AERR_HIGH           =   rawPacket.byte[4];
            //ADCP_Error_Status_Frame.AERR_LOW            =   rawPacket.byte[5];
        }
        break;
    }
}


/*
*	@brief:							Function to get node ID of packet.
*   @parameter packet:				Pointer to packet.
*	@return:						Packet node ID.
*
*   @details:						This function gets the node ID which has sent the packet.
* 
*/

uint8_t ADCP_packet_node_id(ADCP_Packet_t* packet)
{
  uint8_t carry = (packet->byte[0] - 0x0B);

  uint8_t byte = (packet->byte[1]-0x0B) + ( ( ((carry & ( 1 << 0 )) >> 0) ) * 0xF5);
      
  return(byte & 0x0F);		
}


/*
*
*	@brief:							Function information about protocol packet.
*	@parameter packet:				Pointer to protocol packet.
*
*/

void ADCP_packet_info(ADCP_Packet_t* packet, ADCP_Packet_Info_t* packetInfo)
{
  uint8_t carry = (packet->byte[0] - 0x0B);
  uint8_t byte = ((packet->byte[1]-0x0B) + ( carry & 0x1 ) * 0xF5);
    	
  packetInfo->node_id    = ( byte & 0x0F );
  packetInfo->frame_type = ( (byte >> 4) & 0x07 );
  packetInfo->priority   = ( byte >> 7 );   
}

/*
 * @brief:                          Function to handle packets received from the host.
 * @parameter:                      None.
 * @return:                         None.
 *
 * */

void ADCP_Receive_Handler()
{
    if( ADCP_Receive_Queue_Status != ADCP_QUEUE_EMPTY )
    {
        ADCP_Packet_t packet;
        ADCP_Packet_Info_t packetInfo;

        if(ADCP_flags.flags.backup_packet_loaded)
        {
            packet = backup_packet;
            ADCP_flags.flags.backup_packet_loaded = 0;
        }
        else
        {
            ADCP_Receive_Dequeue(&packet);
        }

        ADCP_packet_info( &packet, &packetInfo );

        if( ADCP_verify_checksum_processed( (ADCP_Packet_t*)&packet ) )
        {
            switch(packetInfo.frame_type)
            {
                 /* Received frame type is a data/message packet. */

                case(ADCP_FRAME_TYPE_DATA):
                {
                    uint8_t frame_id = ADCP_packet_frame_id(&packet);

                    int i = 0;

                    /* For all possible receive frames... */

                    for( i = 0 ; i < ADCP_N_FRAMES ; i++ )
                    {
                        /* If frame ID of the received data packet matches with target frame ID. */

                        if( ADCP_Frame_Handler_Array[i].frame_id == frame_id )
                        {
                            ADCP_Packet_t rawPacket;
                            ADCP_processed_to_raw( (ADCP_Processed_Packet_t*)&packet, (ADCP_Raw_Packet_t*)&rawPacket );
                            uint32_t data = 0;

                            data |= ( (uint32_t)rawPacket.byte[2] << 24 );
                            data |= ( (uint32_t)rawPacket.byte[3] << 16 );
                            data |= ( (uint32_t)rawPacket.byte[4] << 8  );
                            data |= ( (uint32_t)rawPacket.byte[5] << 0  );

                            *((uint32_t*)ADCP_Frame_Handler_Array[i].prtFrame) = data;

                            /* If function handle is valid. */

                            if( ADCP_Frame_Handler_Array[i].function )
                            {
                                (*(ADCP_Frame_Handler_Array[i].function))();
                            }

                            /* Update receive status flag. */

                            ADCP_RSR_write(frame_id,1);
                        }
                    }
                }
                break;

                /* Received frame type is a connection management control packet. */

                case(ADCP_FRAME_TYPE_CONNECTION_MANAGEMENT):
                {
                    ADCP_CM_Handler(&packet);
                }
                break;

                /* Received frame type is a remote request control packet. */

                case(ADCP_FRAME_TYPE_REMOTE_REQUEST):
                {
                    ADCP_RRQST_Handler(&packet);
                }
                break;

                /* Received frame type is a error/status control packet. */

                case(ADCP_FRAME_TYPE_ERROR_AND_STATUS):
                {
                    ADCP_ERRSTAT_Handler(&packet);
                }
                break;

                /* Default handler. */

                default:
                {
                    //asm("NOP");
                }
            };
        }
    }
}

/*
 * @brief:                          Function to handle connection management packets.
 * @parameter:                      Pointer to protocol packet.
 *
 */

void ADCP_CM_Handler(ADCP_Packet_t* packet)
{
  ADCP_read_control_frame(packet);

  /* If a open-connection request is received and transmission is not active,
   * set OC_ACK bit in the connection management control register. */

  if( (ADCP_Connection_Management_Frame.OC_RQST & (ADCP_connection_status == 0) ) )
  {
    ADCP_Connection_Management_Frame.OC_ACK = 1;
    ADCP_Connection_Management_Frame.OC_RQST = 0;

    ADCP_send_control_frame(ADCP_NODE_ID,ADCP_CONNECTION_MANAGEMENT);

    ADCP_Connection_Management_Frame.OC_ACK = 0;
    ADCP_Connection_Management_Frame.OC_RQST = 0;
	ADCP_Error_Status_Frame.NES = ADCP_NETWORK_ERROR_NONE;

    ADCP_connection_status = 1;
  }
  else if( (ADCP_Connection_Management_Frame.OC_RQST & (ADCP_connection_status == 1) ) )
  {
    //    If a connection is already open and host requests connection.

    ADCP_Error_Status_Frame.NES = ADCP_NETWORK_ERROR_CONNECTION_ALREADY_OPEN;

    ADCP_send_control_frame(ADCP_NODE_ID,ADCP_ERROR_AND_STATUS);
  }
  else if( ( ADCP_connection_status == 1 ) &&
           ( ADCP_Connection_Management_Frame.CC_RQST ) )
  {
	  //	If connection is open and a close connection request is made.
	  
      ADCP_Connection_Management_Frame.CC_ACK = 1;
      ADCP_Connection_Management_Frame.CC_RQST = 0;

      ADCP_send_control_frame(ADCP_NODE_ID,ADCP_CONNECTION_MANAGEMENT);

      ADCP_Connection_Management_Frame.CC_ACK = 0;
      ADCP_Connection_Management_Frame.CC_RQST = 0;

      ADCP_connection_status = 0;
	  ADCP_Error_Status_Frame.NES = ADCP_NETWORK_ERROR_NONE;
  }
  else if( ( ADCP_connection_status == 0 ) &&
           ( ADCP_Connection_Management_Frame.CC_RQST ) )
  {
	  //	If connection is already closed and a close connection request is made.
	  
      ADCP_Error_Status_Frame.NES = ADCP_NETWORK_ERROR_CONNECTION_ALREADY_CLOSED;
      ADCP_send_control_frame(ADCP_NODE_ID,ADCP_ERROR_AND_STATUS);
  }

  /* If a synchronization/reset request is received from host. */

  if( ADCP_Connection_Management_Frame.SYNC_RQST )
  {
      ADCP_Packet_t packet;

      /* Discard contents of transmit buffer. */

      while(ADCP_Transmit_Queue_Element_Count)
      {
          ADCP_Transmit_Dequeue(&packet);
      }
      ADCP_Transmit_Queue_Status = ADCP_QUEUE_EMPTY;

      /* Discard contents of receive buffer. */

      while(ADCP_Receive_Queue_Element_Count)
      {
          ADCP_Receive_Dequeue(&packet);
      }
      ADCP_Receive_Queue_Status = ADCP_QUEUE_EMPTY;

      /* Clear error/status. */

      ADCP_Error_Status_Frame.AERR_HIGH = 0;
      ADCP_Error_Status_Frame.AERR_LOW  = 0;
      ADCP_Error_Status_Frame.NES       = 0;
      ADCP_Error_Status_Frame.QSZ       = 0;
      ADCP_Error_Status_Frame.QSTAT     = ADCP_QUEUE_EMPTY;
      ADCP_Error_Status_Frame.SACK      = 0;
      ADCP_Error_Status_Frame.SREQ      = 0;

      /* Close connection with host. */

      ADCP_connection_status = 0;

      /* Send back acknowledgement to host. */

      ADCP_Connection_Management_Frame.SYNC_RQST = 0;
      ADCP_Connection_Management_Frame.SYNC_ACK  = 1;

      ADCP_send_control_frame(ADCP_NODE_ID,ADCP_CONNECTION_MANAGEMENT);

      ADCP_Connection_Management_Frame.SYNC_RQST = 0;
      ADCP_Connection_Management_Frame.SYNC_ACK  = 0;
  }
}

/*
 * @brief:                          Function to handle remote request packets.
 * @parameter:                      Pointer to protocol packet.
 *
 */

void ADCP_RRQST_Handler(ADCP_Packet_t* packet)
{
  ADCP_read_control_frame(packet);

  if( ADCP_Remote_Request_Frame.RMT_REQ )
  {
      ADCP_Remote_Request_Frame.RMT_ACK = 1;
      ADCP_Remote_Request_Frame.RMT_REQ = 0;

      ADCP_send_control_frame(ADCP_NODE_ID,ADCP_REMOTE_REQUEST);

      int i = 0;
      int j = 0;

      uint8_t requestedFIDs[4];
      ADCP_Packet_t packet;

      requestedFIDs[0] = ADCP_Remote_Request_Frame.FID_0;
      requestedFIDs[1] = ADCP_Remote_Request_Frame.FID_1;
      requestedFIDs[2] = ADCP_Remote_Request_Frame.FID_2;
      requestedFIDs[3] = ADCP_Remote_Request_Frame.FID_3;

	  uint8_t found = 0;

      /* Loop through requested frame IDs. */

      for( i = 0 ; i < ADCP_Remote_Request_Frame.N_FID ; i++ )
      {
		found = 0;  
		  
        /* Loop through all available frames. */

        for(j = 0 ; j < ADCP_N_FRAMES ; j++)
        {
           /* If current frame's FID matches with current requested FID, send frame. */

           if( ADCP_Frame_Handler_Array[j].frame_id == requestedFIDs[i] )
           {
             ADCP_write_data_to_packet( (void*)(ADCP_Frame_Handler_Array[j].prtFrame),
                                        ADCP_NODE_ID,
                                        ADCP_Frame_Handler_Array[j].frame_id,
                                        1,
                                        &packet);

             ADCP_Transmit_Handle( (uint8_t*)&packet, 9 );
			 
			 found = 1;
			 break;
           }
        }
		
		/* If target frame is not found at all, send requested frame not found error message. */
		
		if( found == 0 )
		{
			ADCP_Error_Status_Frame.NES = ADCP_NETWORK_ERROR_INVALID_FRAME_REQUEST;
			
			ADCP_send_control_frame(ADCP_NODE_ID,ADCP_ERROR_AND_STATUS);
			
			break;
		}
      }

  }
}

/*
 * @brief:                          Function to handle error/status packets.
 * @parameter:                      Pointer to protocol packet.
 *
 */

void ADCP_ERRSTAT_Handler(ADCP_Packet_t* packet)
{
    ADCP_read_control_frame(packet);

    if( ADCP_Error_Status_Frame.SREQ )
    {
      ADCP_Error_Status_Frame.SACK = 1;
      ADCP_Error_Status_Frame.SREQ = 0;
      ADCP_Error_Status_Frame.QSZ = ADCP_Receive_Queue_Element_Count;

      if(ADCP_Receive_Queue_Element_Count == 0)
      {
          ADCP_Receive_Queue_Status = ADCP_DEVICE_NODE_RECEIVE_QUEUE_EMPTY;
      }
      else if(ADCP_Receive_Queue_Element_Count == ADCP_RX_QUEUE_SIZE)
      {
          ADCP_Receive_Queue_Status = ADCP_DEVICE_NODE_RECEIVE_QUEUE_FULL;
      }
      else
      {
          ADCP_Receive_Queue_Status = ADCP_DEVICE_NODE_RECEIVE_QUEUE_NOT_EMPTY;
      }

      ADCP_Error_Status_Frame.QSTAT = ADCP_Receive_Queue_Status;

      ADCP_send_control_frame(ADCP_NODE_ID,ADCP_ERROR_AND_STATUS);
    }
}

void ADCP_breakpoint()
{

}

/*
 * @brief:                          Function to transmit a frame.
 * @parameter frame:                Pointer to frame to be transmitted.
 * @parameter priority:             Priority of frame.
 * @return:                         None.
 *
 * */

void ADCP_Transmit_Dataframe(void* frame, uint8_t frame_id, uint8_t priority)
{
  if( ADCP_connection_status )
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

      if(ADCP_Transmit_Handle != NULL)
      {
          ADCP_Transmit_Handle((uint8_t*)&processedPacket.byte[0],9);
      }
  }
}

/*
 * @brief:                          Function to enqueue a frame into the transmit queue.
 * @parameter frame:                Pointer to frame to be transmitted.
 * @parameter priority:             Priority of frame.
 * @return:                         None.
 *
 * */

void ADCP_Enqueue_Dataframe(void* frame, uint8_t frame_id, uint8_t priority)
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

  ADCP_Transmit_Enqueue((ADCP_Packet_t)processedPacket);
}

/*
 * @brief:                          Function to flush the data frames from transmit queue.
 * @parameter:                      None
 * @return:                         None.
 *
 * */

void ADCP_Flush_Dataframes()
{
  if( ADCP_connection_status )
  {
      ADCP_Packet_t packet[ADCP_TX_QUEUE_SIZE];
      uint8_t count = 0;

      while(ADCP_Transmit_Queue_Element_Count)
      {
          ADCP_Transmit_Dequeue(&packet[count]);
          count++;
      }

      if( ADCP_Transmit_Handle != NULL )
      {
          ADCP_Transmit_Handle( (uint8_t*)packet, 9 * count );
      }
  }
}


/*
 * @brief:                          Function to set value of receive status register.
 * @parameter frame_ID:             Frame ID whose receive status value has to be changed.
 * @parameter value:                New value of the receive status.
 *
 * */

void ADCP_RSR_write(uint8_t frame_ID, uint8_t value)
{
  uint8_t position = (frame_ID % 16);

  ADCP_RSR[frame_ID >> 4] &= ~( (uint16_t)1 << position );
  ADCP_RSR[frame_ID >> 4] |= ( (uint16_t)value << position );
}

/*
 * @brief:                          Function to get value of receive status register.
 * @parameter frame_ID:             Frame ID whose receive status value has to be changed.
 * @return:                         Receive status of the target frame ID.
 * */

uint8_t ADCP_RSR_read(uint8_t frame_ID)
{
    uint8_t position = (frame_ID % 16);
    return( (ADCP_RSR[frame_ID >> 4] & ( 1 << position )) >> position );
}
