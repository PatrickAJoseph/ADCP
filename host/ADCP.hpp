
#ifndef _ADCP_H
#define _ADCP_H

#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <list>
#include <ctime>

#include "windows.h"
#include "stringUtils.hpp"

#define MAX_RX_DELAY		0xFFFFFFFFUL

using namespace std;


class ADCP
{
  public: 		/* Typedefs go here. */
  	
    HANDLE serialHandle;
	COMSTAT serialStatus;
	DCB serialDCB;
	
	typedef struct _DATA_FRAME
	{
	  uint8_t priority;
	  uint8_t ID;
	  uint32_t data;
	  uint8_t checksum;  
	}DATA_FRAME;

	typedef struct _PACKET
	{
	  uint8_t byte[9];	
	}PACKET;

	typedef struct _FRAME
	{
	  uint8_t byte[9];	
	}FRAME;

	typedef struct
	{
		uint8_t byte[9];
	}CONTROL_FRAME;		

	typedef struct _PROCESSED_FRAME
	{
	  uint8_t byte[9];	
	}PROCESSED_FRAME;

	typedef struct _RAW_FRAME
	{
		uint8_t byte[7];	
	}RAW_FRAME;
	  	  
	typedef enum Frame_Type_t
	{
	  DATA = 0,
	  CONNECTION_MANAGEMENT = 1,
	  REMOTE_REQUEST = 2,
	  ERROR_STATUS = 3,
	  INVALID = 255	  
	}Frame_Type_t;
		
	/* Connection management frame strcuture. */
	struct _Connection_Management_Frame
	{	
	  uint32_t CC_ACK:1;			//	Close connection acknowledgement.
	  uint32_t CC_RQST:1;			//	Close connection request.
	  uint32_t OC_ACK:1;			//	Open connection acknowledgement.
	  uint32_t OC_RQST:1;			//	Open connection request.
	  uint32_t SYNC_ACK:1;			//	Synchronization acknowledgement.
	  uint32_t SYNC_RQST:1;			//	Synchronization request.
	}Connection_Management_Frame;


	/* Remote request frame structure. */
	
	struct _Remote_Request_Frame
	{
	  uint32_t FID_3: 8;			//	Request frame ID 3.
	  uint32_t FID_2: 8;			//	Request frame ID 2.
	  uint32_t FID_1: 8;			//	Request frame ID 1.
	  uint32_t FID_0: 8;			//	Request frame ID 0.
	  uint32_t N_FID: 6;			//	Number of requested frames.
	  uint32_t RMT_ACK: 1;			//	Remote frame acknowledgement.
	  uint32_t RMT_REQ: 1;			//	Remote frame request.
	}Remote_Request_Frame;

	/* Error/status request frame structure. */
	
	struct _Error_Status_Frame
	{
	  uint32_t AERR:16;				//	Application error code.
	  uint32_t NES:8;				//	Network error/status.
	  uint32_t QSZ:8;				//	Device receiver queue size.
	  uint32_t QSTAT:2;				//	Device receiver queue status.
	  uint32_t SACK:1;				//	Status acknowledgement.
	  uint32_t SREQ:1;				//	Status request.
	}Error_Status_Frame;

	typedef enum _CHECKSUM_STATUS_t
	{
	  CHECKSUM_PASSED = 1,
      CHECKSUM_FAILED = 0	  
	}CHECKSUM_STATUS_t;
	
	
	typedef enum _Parameter_Type_t
	{
	  BOOL,
      UNSIGNED,
	  SIGNED,
	  FLOAT32,
	  ENUM
	}Parameter_Type_t;
	
	typedef struct _PARAMETER
	{
	  string parameter_name;
      string node_name;
      uint8_t node_ID;
      string frame_name;
      uint8_t frame_ID;
	  Parameter_Type_t type;
	  uint8_t start_bit;
	  uint8_t bit_length;
	  double factor;
	  double offset;
	  double maximum;
	  double minimum;
	  double value;
	  vector<string> enum_values;
	  string comments;
	}PARAMETER_RECORD;
	
	typedef struct FRAME_RECORD
	{
	  uint8_t priority;	
	  string node_name;
      uint8_t node_ID;
	  string frame_name;
	  uint8_t frame_ID;
	  uint32_t data;
	  vector<PARAMETER_RECORD*> parameters;
	  RAW_FRAME rFrame;
	  PROCESSED_FRAME pFrame; 
	}FRAME_RECORD;

    typedef enum
	{
		CLOSED = 0,
		OPEN,
		DISCONNECTED,
		BREAK,
		FRAME_ERROR,
		IO_ERROR,
		OVERRUN,
		RX_OVERFLOW,
		RX_PARITY_DETECT
	}Serial_Port_Status_t;

	typedef enum
	{
		NONE = 0,
		ALREADY_OPEN   = 1,
		ALREADY_CLOSED = 2,
		INVALID_FRAME_REQUESTED = 3,
		TIMEOUT = 255	
	}Network_Error_Status_t;

    struct serialPort
	{
	    string portname;
	    unsigned int TXB_bytes;
	    unsigned int RXB_bytes;
	    unsigned int TXB_size;
	    unsigned int RXB_size;
	    Serial_Port_Status_t status;
	}serialPort;		
	
	typedef struct
	{
		unsigned int id;
		string name;
	}NODE;
	
	typedef struct
	{
		string name;
		vector<double> values;
		vector<uint32_t> times_ms;
	}PARAMETER_SAMPLES;

  public:
    int test_var;
	ofstream debugLog;
	ofstream transmitLog;
	ofstream receiveLog;
	clock_t logStartTime;
	Network_Error_Status_t network_status;

  public:
	uint8_t pTempRaw[7];						//	Temporary raw message frame/packet.
	uint8_t pTempProcessed[9];					//	Temporary processed message frame/packet.	
    DATA_FRAME* pTempDF;						//	Temporary data frame.	
	std::list<PROCESSED_FRAME> txq; 			//	Transmit queue containing processed frames/packets.
	std::list<PROCESSED_FRAME> rxq;				//	Receive queue containing processed frames/packets.
	std::vector<PARAMETER_RECORD> parameters;	//	Vector of parameters.
	std::vector<FRAME_RECORD> frames;			//	Vector of data frames.
	std::vector<CONTROL_FRAME> controlFrames;	//	Vector of control frames.
	std::vector<NODE> nodes;					//	List of node names.
	
  public:
  
    /* Constructor. */
  
    ADCP()
	{
      #ifdef DEBUG
		debugLog.open("ADCP_Debug_Log.txt",ios::out);
      #endif
	  
		transmitLog.open("ADCP_Transmit_Log.csv",ios::out);
		receiveLog.open("ADCP_Receive_Log.csv",ios::out);
		
		logStartTime = clock();
		
		this->Connection_Management_Frame = {0};
		this->Remote_Request_Frame        = {0};
		this->Error_Status_Frame		  = {0};
	}
	
	/* Destructor. */
	
	~ADCP()
	{
	  #ifdef DEBUG
        debugLog.close();
      #endif	  

		transmitLog.close();
		receiveLog.close();
	}
  
    /*
	*  @brief: Prints API version information.
	*/
	
	void print_API_info();
    
    /*
	*  @brief: Function to convert a raw-frame to a processed frame.
	*
	*  @parameter raw:			Pointer to a 7-byte array consisting of raw packet bytes.
	*  @parameter processed:  	Pointer to a 9-byte array consisting of processed packet bytes.  
	*
	*  @details: This function converts a raw frame consisting of 7 bytes to
	*            a processed frame of 9 bytes. This function must be called 
	*            before loading the packet into the transmit buffer. 
	*/
   
    void raw_to_processed(RAW_FRAME* raw, PROCESSED_FRAME* processed);		
	
    /*
	*  @brief: Function to convert a processed-frame to a data-frame.
	*
	*  @details: This function converts a processed frame consisting of 9 bytes to
	*            a raw frame of 7 bytes. This function must be called 
	*            before loading the packet into the transmit buffer. 
	*/	
	
    void processed_to_raw(PROCESSED_FRAME* processed, RAW_FRAME* raw);  		
	
	/*
	*  @brief: Generate checksum for a raw frame/packet.
	* 
	*/
	
	void generate_checksum(RAW_FRAME* raw);
	void generate_checksum(PROCESSED_FRAME* processed);
	
	/*
	*  @brief: Verify checksum of a received raw/processed frame/packet.
	* 
	*/
	
	CHECKSUM_STATUS_t verify_checksum(RAW_FRAME* raw);
	CHECKSUM_STATUS_t verify_checksum(PROCESSED_FRAME* processed);
	
	/*
	*  @brief: Function to print a raw/processed packet on debugger screen.
	*
	*/
	
	//void print_raw_frame(RAW_FRAME* raw);
	//void print_processed_frame(PROCESSED_FRAME* processed);
	
	/*
	*  @brief: Function to generate a raw/processed data frame from given data.
	*
	*  @parameter pData:	Pointer to data. The size of the data container must be 32-bits.
	*  @parameter NID:		ID of device node to which the data frame has to be sent to.
	*  @parameter FID:		Frame ID of the data frame. Frame IDs can take values from 0 to 255.
	*  @parameter PRI:		Priority of the data frame. Priority of 0 pushes data frame to the back
	                        of the device nodes receiver queue.
	*/

    void put_DF(void* pData,uint8_t NID,uint8_t FID,uint8_t PRI,RAW_FRAME* pRaw);	
    void put_DF(void* pData,uint8_t NID,uint8_t FID,uint8_t PRI,PROCESSED_FRAME* pProcessed);
	
	/*
	*  @brief: Function to retrive data frame information from a raw/processed packet/frame.
	*
	*  @parameter pData:	Pointer to data. The size of the data container must be 32-bits.
	*  @parameter pNID:		Pointer to ID of node from which the data packet was received from.
	*  @parameter pFID:		Pointer to Frame ID of the data frame. Frame IDs can take values from 0 to 255.
	*  @parameter pPRI:		Pointer to Priority of the data frame. Priority of 0 pushes data frame to the back
	                        of the device nodes receiver queue.
	*/	

    void get_DF(void* pData,uint8_t* pNID,uint8_t* pFID,uint8_t* pPRI,RAW_FRAME* pRaw);	
    void get_DF(void* pData,uint8_t* pNID,uint8_t* pFID,uint8_t* pPRI,PROCESSED_FRAME* pProcessed);
	
	/*
	*
	*  @brief: Function to get type of the received message packet.
	* 
	*  @parameter processed:	Pointer to a raw/processed frame/packet.
	*
	*  @return packet_type:	Type of the received packet. Refer to ADCP::Frame_Type_t.
	*
	*  @details: This function takes in the received message packet and returns the type of
	*            the message packet/frame.
	*/

    public : Frame_Type_t get_frame_type(RAW_FRAME* raw)
	{
      #ifdef DEBUG
	    (this->debugLog) << "Entering function: get_frame_type (raw)....." << endl;
	  #endif
	
	  Frame_Type_t frameType = (Frame_Type_t)((raw->byte[0] >> 4) & 0x07 );

      #ifdef DEBUG
	    (this->debugLog) << "Frame Type: " << frameType << endl;
	    (this->debugLog) << "Entering function: get_frame_type (processed)....." << endl;
	  #endif	  
	  
	  return(frameType);
	}
	
    public : Frame_Type_t get_frame_type(PROCESSED_FRAME* processed)
	{
      #ifdef DEBUG
	    (this->debugLog) << "Entering function: get_frame_type (raw)....." << endl;
	  #endif

      RAW_FRAME raw;
	  
	  this->processed_to_raw(processed,&raw);
	
	  Frame_Type_t frameType = (Frame_Type_t)((raw.byte[0] >> 4) & 0x07 );

      #ifdef DEBUG
		(this->debugLog) << "Frame Type: " << frameType << endl;
	    (this->debugLog) << "Entering function: get_frame_type (processed)....." << endl;
	  #endif	  
	  
	  return(frameType);		
	}
	
	/*
	*  @brief: Function to a processed packet into the transmit queue.
	*  
	*  @parameter: Processed frame to be pushed into the transmit queue.
	*
	*  @return: Transmit queue status. Returns 1 if queue is full, -1 is queue is empty
	*           or 0 if element can be loaded into queue.
	*/
	
	int txq_push(PROCESSED_FRAME packet);
	
	/*
	*  @brief: Function to print the contents of a transmit queue.
	*  
	*/
	
	void txq_print();
	
	/*
	*  @brief: Function to read parameters from CSV file.
	*/
	
	void read_file(const char* file_name);
	
	/*
	*
	*  @brief:	Function to print details about a particular parameter.
	*
	*/
	
	void print_parameter_details(unsigned int index);
	void print_parameter_details(string name);
	
	/*
	*
	* @brief: Function to print the parameter list.
	*
	*/
	
	void print_parameter_list();
	
	/*
	*  @brief:	Function to print node list.
	*
	*/
	
	void print_node_list();
	
	/*
	*  Function to get parameter instance.
	*
	*/
	
	ADCP::PARAMETER_RECORD get_parameter_instance(unsigned int index);
	
	/*
	*  @brief Function to print details of a particular frame.
	*
	*/
	
	void print_frame_details(unsigned int index);
	void print_frame_details(string name);
	
	/*
	*
	*  @brief: Function to print the frame list.
	*
	*/
	
	void print_frame_list();
	
	/*
	*  @brief: Function to get an instance of a parameter. 
	*
	*/
	
	ADCP::PARAMETER_RECORD* get_parameter(unsigned int index);
	ADCP::PARAMETER_RECORD* get_parameter(string parameter_name);
	
	/*
	*  @brief: Function to get instance of a frame.
	*
	*/
	
	ADCP::FRAME_RECORD* get_frame(unsigned int index);
	ADCP::FRAME_RECORD* get_frame(string frame_name); 
	
	/*
	*	@brief:					Function to get the node ID value.
	*	@parameter node_name:	Name of target node.
	*	@return node_id:		ID of target node.
	*/
	
	unsigned int get_node_id(string node_name);
	
	/*
	*  @brief: 			Function to write value to a parameter.
	*  @details:		When this function is called, the function not only updates the value of	
	*					the parameter in the parameter struct but also updates the frame.
	*
	*/
	
	void write_parameter(unsigned int index,double value);
	void write_parameter(string name,double value);
	
	/* 
	*
	*  @brief:					Function to write an enum parameter.
	*  @parameter name:			Name/index of parameter.
	*  @parameter enum_value:	String representing enum value.
	*
	*/

	void write_parameter(unsigned int index, string enum_value);	
	void write_parameter(string name, string enum_value);
	
	/*
	*	
	*  @brief:					Function to read enum value of a parameter.
	*  @parameter index/name:	Index / name of parameter.
	*  @return:					String enum. 
	*
	*/
	
	string read_parameter_enum(unsigned int index);
	string read_parameter_enum(string name);
	
	/*
	*  @brief: 			Function to read value of a parameter.
	*  @details:		When this function is called, it fetches the value of the parameter
	*                   from its associated frame.
	*
	*/
	
	double read_parameter(unsigned int index);
	double read_parameter(string name);
	
	/* @brief: 					Function to open a serial port session. 
	*
	*  @parameter com_port:		Name of serial port to which connection has to be established.
	*  @parameter baud_rate:	Baud rate for serial communication
    *							Allowed values are from CBR_110 to CBR_115200.	
	*
	*/
	
	void serial_port_init(string com_port, unsigned int baud_rate);
	
	/*
	*  @brief:					Scans all CDC serial ports on the host device.
	*  @return:					Name of all valid COM Ports configured as USB CDC.
	*
	*/
	
	vector<string> serial_port_scan();
	
	/*
	*  @brief: 					Function to get status of serial port.
	*  @return:				    Serial port structure.	 
	*/
	
	
	Serial_Port_Status_t serial_port_status();
	
	/*
	*  @brief:					Function to flush serial buffers.
	*  @parameter:				None.
    *  @return:					Number of bytes present in serial buffer before flushing buffer.
	*/
	
	unsigned int serial_port_flush(uint8_t* rx_bytes);
	
	/*
	*    @brief:				Function to close serial port connection.
	*/
	
	void serial_port_close();
	
	/*
	*  @brief:					Function to send a message packet to serial port. 
	*  @parameter packet:		structure of packet containing bytes to be sent via serial port.
	*
	*/	
	
	void send_packet(PROCESSED_FRAME packet);
	
	/*
	*  @brief:					Function to receive a message packet from serial port.
	*  @parameter packet:		Pointer to structure 	
	*
	*/
	
	void receive_packet(PROCESSED_FRAME* packet);

	/*
	*
	*  @brief:					Function to receive a message packet from serial port.
	*  @parameter packet:		Pointer to structure 	
	*  @parameter timeout_ms:	Timeout in milli-seconds.
	*
	*/
	
	void receive_packet(PROCESSED_FRAME* packet,uint32_t timeout_ms);
	
	/*
	*	@brief:					Function to poll the serial stream until a target byte is received.
	*	@parameter pollByte:	The byte to wait for.
	*   @return:				Number of bytes read before encountering target byte.
	*/
	
	unsigned int poll_byte(uint8_t byte);
	
	/*
	*  @brief:					Function to read specified number of bytes via serial port.
	* 
	*  @parameter pBuffer:		Pointer to buffer to store received data.
	*  @parameter nBytes:		Number of bytes to receive.
    *	
	*  @return:					Number of bytes read from the serial port.
	* 
	*/
	
	unsigned int read_bytes(uint8_t* pBuffer, unsigned int n_bytes);

	/*
	*  @brief:					Function to write specified number of bytes via serial port.
	* 
	*  @parameter pBuffer:		Pointer to buffer containing bytes to be written via serial port.
	*  @parameter nBytes:		Number of bytes to transmit.
    *	 
	*/
	
	unsigned int write_bytes(uint8_t* pBuffer, unsigned int n_bytes);
	
	/*
	*  @brief:							Function to write a parameter to serial port.
	*  @parameter parameter_name:		Name of parameter to be written.
	*  @parameter value:				Value to be loaded into the parameter.			 	 
	*
	*  @details:						This function updates the value of the parameter in the frame
	*									and then transmits the frame associated with the parameter
	*									via the serial port.
	*/
	
	void send_parameter(string parameter_name, double value);
	void send_parameter(unsigned int index, double value);
	
	/*
	* 	@brief:							Function to repeately send a parameter via serial port.
	*   @parameter parameter_name:		Name of parameter to be transmitted to device.
	*	@parameter value:				Value of parameter to be sent.
	*   @parameter count:				Number of times the parameter has to be transmitted.
    *   @parameter interval_ms:			Interval between transmit events (in milli-seconds).	
	*
	*/
	
	void repeated_send_parameter(string parameter_name, double value, unsigned int count, unsigned int interval_ms);
	
	/* 
	*
	*  @brief: 										Function to send frame to device. 
	*  @parameter frame_index/frame_name:			Index/name of the frame.		
	*
	*/
	
	void send_frame(unsigned int frame_index);
	void send_frame(string frame_name);
	
	/*
	*  @brief:							Function to receive target data/message frame from serial port.
	*  @parameter index/name:			Index or name of target frame.
	*
	*/
	
	uint8_t receive_frame(unsigned int index,uint32_t timeout_ms);
	uint8_t receive_frame(string name,uint32_t timeout_ms);

	/*
	*  @brief:							Function to receive a parameter from serial port.
	*  @parameter index/name:			Name/index of parameter							 
	*  @parameter timeout_ms:			Timeout in milli-seconds.
	*
	*/

	double receive_parameter(unsigned int index,uint32_t timeout_ms);
	double receive_parameter(string name,uint32_t timeout_ms);
	
	/* Control frame handling functions. */
	
	/*
	*  @brief:						Function to send a control frame to targer device node.
	*  @parameter node_id:			ID of target device node.
	*  @parameter control_frame:	Type of control frame.
	*/
	
	void send_control_frame(unsigned int node_id, Frame_Type_t control_frame);
	void send_control_frame(string node_name, Frame_Type_t control_frame);	
	
	/*
	*	@brief:						Function to read a control frame from target device node.
	*	@parameter node_id:			ID of the target device node.
	*   @parameter control_frame:	Type of control frame to be received from the target device node.
	*   @parameter timeout_ms:		Timeout in milli-seconds.
	*/
	
	uint8_t read_control_frame(unsigned int node_id, Frame_Type_t control_frame, uint32_t timeout_ms);
	uint8_t read_control_frame(string node_name, Frame_Type_t control_frame, uint32_t timeout_ms);
	
	/*
	*	@brief:						Function to update a control frame.
	*   @parameter rawFrame:		Structure containing raw packet.
	*
	*/
	
	void update_control_frame(RAW_FRAME rawFrame);
	
	/*
    * 
	*  @brief:						Function to check if frame is valid for target node. 
	*  @parameter node_id:			ID of target node.
	*  @parameter frame_id:			ID of data frame/packet from target node.	
	*  @return frame_available:		Returns 1 if frame is available, else returns zero.
	*
	*/
	
	uint8_t is_frame_valid(unsigned int node_id, unsigned int frame_id);
	
	/*
	*  @brief:						Function to check if frame is valid for target node.
	*  @parameter frame_name:		Frame name.	
	*  @return frame_available:		Returns 1 if frame is availabe, else returns zero.
	*/
	
	uint8_t is_frame_valid(string frame_name);
	
	/*
	*	@brief:						Function to generate files to parse frames for device.
    *   @parameter node_index:		Node index of the device node for which the files have to be generated.	
	*   @detail:					This function generates two files "ADCP_frames.c" and "ADCP_frames.h"
	*								containing unions to store frame data.
	*/
	
	void generate_frame_files(unsigned int node_index);
	
	/*
	*	@brief:						Reads a packet from serial port and updates the relevant packet.
	*								The packet can be of any type.
	*	@parameter timeout_ms:		Timeout of read operatin in milli-seconds.
	*
	*/
	
	void read_and_update_frame(uint32_t timeout_ms);
	
	/*
	*  @brief:						Function to open connection with target device node.
	*  @parameter node_id:			Index of the target device node.
	*  @parameter timeout_ms:		Timeout of open connection request.		
	*  @return status:				Status of opening connection.											
	*/
	
	uint8_t open_node_connection(unsigned int node_id,uint32_t timeout_ms);
	uint8_t open_node_connection(unsigned int node_id,uint32_t timeout_ms,uint32_t tries);

	/*
	*  @brief:						Function to close connection with target device node.
	*  @parameter node_id:			Index of the target device node.
	*  @parameter timeout_ms:		Timeout of open connection request.		
	*  @return status:				Status of opening connection.											
	*/
	
	uint8_t close_node_connection(unsigned int node_id,uint32_t timeout_ms);
	uint8_t close_node_connection(unsigned int node_id,uint32_t timeout_ms,uint32_t tries);
	
	/*
	*	@brief:						Function to request frames from target device node.
	*	@parameter node_id:			Index of target device node ID.
	*	@parameter timeout_ms:		Timeout in milli-seconds.
	*   @return:					Status of transfer.
	*/
	
	uint8_t remote_frame_request(unsigned int node_id,vector<string> frames, uint32_t timeout_ms);
	uint8_t remote_frame_request(unsigned int node_id,vector<string> frames, uint32_t interval, uint32_t tries);
	
	/*
	*	@brief:						Function to get error and status of target device.
	*	@parameter node_id:			Node ID.
	*   @parameter timeout_ms:		Timeout in milli-seconds.
	*   @Status of timeout;
	*
	*/
	
	uint8_t get_error_and_status(unsigned int node_id,uint32_t timeout_ms);
	
	/*
	*   @brief:						Function to get network error status for target device node.
	*   @parameter node_id:			ID of the target device node in the network.
	*   @parameter timeout_ms:		Timeout in milli-seconds.
	*   @return:					Network error status (Refer to ADCP::Network_Error_Status_t for more details)  
	*
	*/
	
	Network_Error_Status_t get_node_network_error_status(uint8_t node_id,uint32_t timeout_ms);
	Network_Error_Status_t get_node_network_error_status(uint8_t node_id,uint32_t interval,uint32_t tries);	
	
	/*
	*	@brief:								Function to get size of target node receive buffer.
	*   @parameter node_id:					Node ID of the target device node.
	*	@parameter timeout_ms:				Timeout of request in milli-seconds.
	*   @return queue_element_count:		Number of elements in receiver node queue.
	*
	*/
	
	uint8_t get_node_receive_queue_count(uint8_t node_id,uint32_t timeout_ms);	
	
	/*
	*  @brief:								Function to get application error code from target device node.
	*  @parameter node_id:					Target device node.
	*  @parameter timeout_ms:				Timeout in milli-seconds.
    *  @return:								Application error which is a 16-bit unsigned integer.					
	*/
	
	uint16_t get_node_application_error_code(uint8_t node_id,uint32_t timeout_ms);
	
	/*
	*  @brief:								Function to reset/synchronize the target device node.
	*  @parameter node_id:					Target device node.
	*  @parameter timeout_ms:				Timeout of request in milli-seconds.
	*  @return timeout_status:				Returns 1 if timeout status has not and acknowledgement has been received.
	*										Else returns 0.
	*/
	
	uint8_t reset_node(uint8_t node_id, uint32_t timeout_ms);
	uint8_t reset_node(uint8_t node_id, uint32_t timeout_ms,uint32_t tries);
		
	/*
	*  @brief:								Function to monitor a parameter in live mode.
	*  @parameter parameter_name:			Name of parameter to be monitored.
	*  @parameter mon_time_ms:				Time for which the parameter has to be monitored.
	*  @return:								Vector containing sampled parameter values.
	*/
	
	PARAMETER_SAMPLES monitor_parameter(string parameter_name, uint32_t monitoring_time_ms);
	
	/*
	*  @brief:								Function to monitor multiple parameters in live mode.
	*  @parameter parameter_names:			A vector of parameter names to be monitored.
	*  @parameter mon_time_ms:				Monitoring time in milli-seconds.
    *  @return:								Vector containing vector of samples.	
	*/
	
	vector<PARAMETER_SAMPLES> monitor_parameters(vector<string> parameter_names, uint32_t monitoring_time_ms);
	
	/*
	*  @brief:								Function to write a processed frame to receive log file.
    *  @parameter frame:					Processed frame.
    *  @return		   :					None.	
	*
	*/
	
	void write_frame_to_receive_log_file(PROCESSED_FRAME frame);

	/*
	*  @brief:								Function to write a processed frame to transmit log file.
    *  @parameter frame:					Processed frame.
    *  @return		   :					None.	
	*
	*/

	void write_frame_to_transmit_log_file(PROCESSED_FRAME frame);
	
	/*
	*	@brief:								Function to get cycle time of target frame.
	*   @parameter frame_name:				Name of target frame.
    *	@parameter timeout_ms:				Timeout in milli-seconds.
	*   @return cycle_time_ms:				Cycle time in milli-seconds.
	*
	*/
	
	uint32_t get_cycle_time(string frame_name,uint32_t timeout_ms); 
};



#endif /* _ADCP_H */
