#include "ADCP.hpp"
#include <bits/stdc++.h>

void ADCP::print_API_info()
{
  std::cout << "Application Debug and Control Protocol (version 1.0)" << endl;		
}


void ADCP::raw_to_processed(RAW_FRAME* raw, PROCESSED_FRAME* processed)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: ADCP::raw_to_processed ....." << endl;
  #endif

  uint16_t carry = 0;

  /* Obtain carry byte by adding each byte of raw frame */	

  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)raw->byte[0] ) >> 8 ) << 0 );
  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)raw->byte[1] ) >> 8 ) << 1 );
  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)raw->byte[2] ) >> 8 ) << 2 );
  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)raw->byte[3] ) >> 8 ) << 3 );
  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)raw->byte[4] ) >> 8 ) << 4 );
  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)raw->byte[5] ) >> 8 ) << 5 );
  carry |= ( ( ( (uint16_t)0x0B + (uint16_t)raw->byte[6] ) >> 8 ) << 6 );
  
  carry += 0x0B;
  
  processed->byte[0] = carry;		//	Load carry byte.
  
  /* Compute other processed bytes */

  for(int i = 1 ; i <= 7 ; i++)
  {	
    /* If byte value is greater than or equal to 0xFF-0x0A = 0xF5, then when 0x0B is added to it,
	   a carry is generated. In that case, first, the raw value must be added with 0x0B, the leftover
	   must be taken ( 0x00 <= leftover <= 0x0A ) and the leftover is added with 0x0B. */

    if(raw->byte[i-1] >= 0xF5)
	{
	  processed->byte[i] = (((uint16_t)raw->byte[i-1] + (uint16_t)0x0B) & (uint16_t)0xFF) + 0x0B;	
	}
	else
	{
	  processed->byte[i] = 	(raw->byte[i-1] + 0x0B);
	}
  }
  
  processed->byte[8] = 0x0A;		//	Termination byte (0x0A).
  
  #ifdef DEBUG

    (this->debugLog) << "Raw packet: " << (uint16_t)raw->byte[0] << " " <<
	(uint16_t)raw->byte[1] << " " << (uint16_t)raw->byte[2] << " " <<
	(uint16_t)raw->byte[3] << " " << (uint16_t)raw->byte[4] << " " <<
	(uint16_t)raw->byte[5] << " " << (uint16_t)raw->byte[6] << endl;
	
	(this->debugLog) << "Processed packet: " << (uint16_t)processed->byte[0] << " " <<
	(uint16_t)processed->byte[1] << " " << (uint16_t)processed->byte[2] << " " <<
	(uint16_t)processed->byte[3] << " " << (uint16_t)processed->byte[4] << " " <<
	(uint16_t)processed->byte[5] << " " << (uint16_t)processed->byte[6] << " " <<
	(uint16_t)processed->byte[7] << " " << (uint16_t)processed->byte[8] << endl;	

	(this->debugLog) << "Exiting function: ADCP::raw_to_processed ....." << endl;

  #endif	
}

void ADCP::processed_to_raw(PROCESSED_FRAME* processed, RAW_FRAME* raw)
{
  /* The most significant byte contains the carry result offset by 0x0B. */	

  #ifdef DEBUG
    (this->debugLog) << "Entering function: ADCP::raw_to_processed ....." << endl;
  #endif
	
  uint8_t carry = (processed->byte[0] - 0x0B);
  
  raw->byte[0] = (processed->byte[1]-0x0B) + ( ( ((carry & ( 1 << 0 )) >> 0) ) * 0xF5);
  raw->byte[1] = (processed->byte[2]-0x0B) + ( ( ((carry & ( 1 << 1 )) >> 1) ) * 0xF5);
  raw->byte[2] = (processed->byte[3]-0x0B) + ( ( ((carry & ( 1 << 2 )) >> 2) ) * 0xF5);
  raw->byte[3] = (processed->byte[4]-0x0B) + ( ( ((carry & ( 1 << 3 )) >> 3) ) * 0xF5);
  raw->byte[4] = (processed->byte[5]-0x0B) + ( ( ((carry & ( 1 << 4 )) >> 4) ) * 0xF5);
  raw->byte[5] = (processed->byte[6]-0x0B) + ( ( ((carry & ( 1 << 5 )) >> 5) ) * 0xF5);
  raw->byte[6] = (processed->byte[7]-0x0B) + ( ( ((carry & ( 1 << 6 )) >> 6) ) * 0xF5);
  
  #ifdef DEBUG
	(this->debugLog) << "Processed packet: " << (uint16_t)processed->byte[0] << " " <<
	(uint16_t)processed->byte[1] << " " << (uint16_t)processed->byte[2] << " " <<
	(uint16_t)processed->byte[3] << " " << (uint16_t)processed->byte[4] << " " <<
	(uint16_t)processed->byte[5] << " " << (uint16_t)processed->byte[6] << " " <<
	(uint16_t)processed->byte[7] << " " << (uint16_t)processed->byte[8] << endl;	

    (this->debugLog) << "Raw packet: " << (uint16_t)raw->byte[0] << " " <<
	(uint16_t)raw->byte[1] << " " << (uint16_t)raw->byte[2] << " " <<
	(uint16_t)raw->byte[3] << " " << (uint16_t)raw->byte[4] << " " <<
	(uint16_t)raw->byte[5] << " " << (uint16_t)raw->byte[6] << endl;	

    (this->debugLog) << "Exiting function: ADCP::raw_to_processed ....." << endl;

  #endif	  
}

void ADCP::generate_checksum(RAW_FRAME* raw)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function ADCP::generate_checksum (raw frame) ....." << endl;
  #endif    
	
  /* The checksum is the last byte of the raw frame. Any data loaded into the
     last (7th) byte will over-written by the checksum. */

    uint16_t checksum = 0;

	checksum += raw->byte[0]; 
	checksum += raw->byte[1];
	checksum += raw->byte[2]; 
	checksum += raw->byte[3];
	checksum += raw->byte[4]; 
	checksum += raw->byte[5];

    checksum = (255 - (checksum%255));

    raw->byte[6] = checksum;
	
  #ifdef DEBUG
    (this->debugLog) << "Checksum of raw packet/frame [" << (uint16_t)raw->byte[0] << " " << (uint16_t)raw->byte[1]
	<< " " << (uint16_t)raw->byte[2] << " " << (uint16_t)raw->byte[3] << " " << (uint16_t)raw->byte[4] << " "
	<< (uint16_t)raw->byte[5] << "] is " << (uint16_t)checksum << endl; 

    (this->debugLog) << "Exiting function ADCP::generate_checksum (raw frame) ....." << endl;
  #endif  
}


void ADCP::generate_checksum(PROCESSED_FRAME* processed)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function ADCP::generate_checksum (processed frame) ....." << endl;
  #endif    
	
  /* The checksum is the last byte of the raw frame. Any data loaded into the
     last (7th) byte will over-written by the checksum. */

    RAW_FRAME raw = {0};
	
	this->processed_to_raw(processed,&raw);

    uint16_t checksum = 0;

	checksum += raw.byte[0]; 
	checksum += raw.byte[1];
	checksum += raw.byte[2]; 
	checksum += raw.byte[3];
	checksum += raw.byte[4]; 
	checksum += raw.byte[5];

    checksum = (255 - (checksum%255));

    raw.byte[6] = checksum;
	
  #ifdef DEBUG
    (this->debugLog) << "Checksum of raw packet/frame [" << (uint16_t)raw.byte[0] << " " << (uint16_t)raw.byte[1]
	<< " " << (uint16_t)raw.byte[2] << " " << (uint16_t)raw.byte[3] << " " << (uint16_t)raw.byte[4] << " "
	<< (uint16_t)raw.byte[5] << "] is " << (uint16_t)checksum << endl; 
	
    (this->debugLog) << "Exiting function ADCP::generate_checksum (processed frame) ....." << endl;	
  #endif  
}


ADCP::CHECKSUM_STATUS_t ADCP::verify_checksum(RAW_FRAME* raw)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function: ADCP::verify_checksum (raw frame) ....." << endl;	
  #endif  
	
  uint16_t checksum = 0;

  checksum += (uint16_t)raw->byte[0];  
  checksum += (uint16_t)raw->byte[1];
  checksum += (uint16_t)raw->byte[2];
  checksum += (uint16_t)raw->byte[3];
  checksum += (uint16_t)raw->byte[4];
  checksum += (uint16_t)raw->byte[5];
  checksum += (uint16_t)raw->byte[6]; 

  ADCP::CHECKSUM_STATUS_t status;	

  if((checksum%255)==0)
  {
    status = ADCP::CHECKSUM_PASSED;    	  
  }
  else
  {
    status = ADCP::CHECKSUM_FAILED;	  
  }
  
  #ifdef DEBUG
	(this->debugLog) << "Checksum of raw frame/packet: " << checksum << endl;
    (this->debugLog) << "Checksum status of raw frame: " << status << endl;

    (this->debugLog) << "Exiting function: ADCP::verify_checksum (raw frame) ....." << endl;	
  #endif
  
  return(status);
}


ADCP::CHECKSUM_STATUS_t ADCP::verify_checksum(PROCESSED_FRAME* processed)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function: ADCP::verify_checksum (processed frame) ....." << endl;	
  #endif  
	
  uint16_t checksum = 0;

  RAW_FRAME raw = {0};

  this->processed_to_raw(processed,&raw);

  checksum += (uint16_t)raw.byte[0];  
  checksum += (uint16_t)raw.byte[1];
  checksum += (uint16_t)raw.byte[2];
  checksum += (uint16_t)raw.byte[3];
  checksum += (uint16_t)raw.byte[4];
  checksum += (uint16_t)raw.byte[5];
  checksum += (uint16_t)raw.byte[6];

  ADCP::CHECKSUM_STATUS_t status;	

  if((checksum%255)==0)
  {
    status = ADCP::CHECKSUM_PASSED;    	  
  }
  else
  {
    status = ADCP::CHECKSUM_FAILED;	  
  }
  
  #ifdef DEBUG
    (this->debugLog) << "Checksum of processed frame: " << checksum << endl;
    (this->debugLog) << "Checksum status of processed frame: " << status << endl;
    (this->debugLog) << "Exiting function: ADCP::verify_checksum (processed frame) ....." << endl;	
  #endif
  
  return(status);
}

void ADCP::put_DF(void* pData,uint8_t NID,uint8_t FID,uint8_t PRI,RAW_FRAME* pRaw)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function: put_DF (raw frame) ....." << endl;
  #endif  
		
  uint32_t data = *((uint32_t*)pData);	

  /*  Byte 0 is the frame descriptor.
   *  Bit 7 	--> Priority of frame/packet.
   *  Bit 6-4 	-->	Frame type. For data frame/packet, frame type is 0.
   *  Bit 3-0   --> Device node ID.   
  */	

  pRaw->byte[0] = ( PRI << 7 ) | (0x00 << 4) | ( NID & 0x0F );

  /* Byte 1 contains the frame ID. */
  
  pRaw->byte[1] = FID;

  /* Bytes 2 - 5 contain the frame data. */
  
  pRaw->byte[2] = ((data & 0xFF000000) >> 24);
  pRaw->byte[3] = ((data & 0x00FF0000) >> 16);
  pRaw->byte[4] = ((data & 0x0000FF00) >> 8);
  pRaw->byte[5] = (uint8_t)data;
  
  /* Byte 6 contains checksum of bytes 0 to 5. */
  
  this->generate_checksum(pRaw);  

  #ifdef DEBUG
    (this->debugLog) << "Frame data value: " << data << endl;
    (this->debugLog) << "Created data frame/packet: [ " << (uint16_t)pRaw->byte[0] << ","
    << (uint16_t)pRaw->byte[1] << "," << (uint16_t)pRaw->byte[2] << "," << (uint16_t)pRaw->byte[3] << 
	"," << (uint16_t)pRaw->byte[4] << "," << (uint16_t)pRaw->byte[5] << "," << (uint16_t)pRaw->byte[6] << " ] " << endl;	

    (this->debugLog) << "Exiting function: put_DF (raw frame) ....." << endl;
  #endif	
}

void ADCP::put_DF(void* pData,uint8_t NID,uint8_t FID,uint8_t PRI,PROCESSED_FRAME* pProcessed)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function: put_DF (processed frame) ....." << endl;
  #endif  
	
  uint32_t data = (*(uint32_t*)pData);  

  /*  The first byte is the frame descriptor.
   *  Bit 7 	--> Priority of frame/packet.
   *  Bit 6-4 	-->	Frame type. For data frame/packet, frame type is 0.
   *  Bit 3-0   --> Device node ID.   
  */	
  
  RAW_FRAME raw = {0};
	
  raw.byte[0] = ( PRI << 7 ) | (0x00 << 4) | ( NID & 0x0F );

  /* Byte 1 contains the frame ID. */

  raw.byte[1] = FID;
  
  /* Bytes 2 - 5 contain the frame data. */  
  
  raw.byte[2] = ((data & 0xFF000000) >> 24);
  raw.byte[3] = ((data & 0x00FF0000) >> 16);
  raw.byte[4] = ((data & 0x0000FF00) >> 8);
  raw.byte[5] = (uint8_t)data;
  
  /* Byte 6 contains checksum of bytes 0 to 5. */
  
  this->generate_checksum(&raw);

  this->raw_to_processed(&raw,pProcessed);  

  #ifdef DEBUG
    (this->debugLog) << "Frame data value: " << data << endl;
    (this->debugLog) << "Created processed data frame/packet: [ " << (uint16_t)pProcessed->byte[0] << ","
    << (uint16_t)pProcessed->byte[1] << "," << (uint16_t)pProcessed->byte[2] << "," << (uint16_t)pProcessed->byte[3] << 
	"," << (uint16_t)pProcessed->byte[4] << "," << (uint16_t)pProcessed->byte[5] << "," << (uint16_t)pProcessed->byte[6] << " ] " << endl;	

    (this->debugLog) << "Exiting function: put_DF (processed frame) ....." << endl;
  #endif	
}


void ADCP::get_DF(void* pData,uint8_t* pNID,uint8_t* pFID,uint8_t* pPRI,RAW_FRAME* pRaw)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function: get_DF (raw) ....." << endl;
  #endif	

  /*  The first byte is the frame descriptor.
   *  Bit 7 	--> Priority of frame/packet.
   *  Bit 6-4 	-->	Frame type. For data frame/packet, frame type is 0.
   *  Bit 3-0   --> Device node ID.   
  */	

  *pPRI = ((pRaw->byte[0] & ( 1 << 7 )) >> 7);
  *pNID = ( pRaw->byte[0] & 0x0F );
  
  /* Byte 1 contains the frame ID. */
  
  *pFID = pRaw->byte[1];

  /* Bytes 2 - 5 contain the frame data. */  

  uint32_t data = 0;

  data |= (  ((uint32_t)pRaw->byte[2]) << 24 );
  data |= (  ((uint32_t)pRaw->byte[3]) << 16 );
  data |= (  ((uint32_t)pRaw->byte[4]) << 8 );
  data |= (  ((uint32_t)pRaw->byte[5]) << 0 );
  
  (*(uint32_t*)pData) = data;

  #ifdef DEBUG
    (this->debugLog) << "Frame priority:" << ( (*pPRI == 1) ? ("HIGH") : ("LOW") ) << endl;
	(this->debugLog) << "Node ID:" << unsigned(*pNID) << endl;
    (this->debugLog) << "Frame ID:" << unsigned(*pFID) << endl;
    (this->debugLog) << "Frame Data (decimal): " << data << endl;
    (this->debugLog) << "Frame Data (hexa-decimal): " << std::hex << "0x" << data << std::dec << endl;  	
    (this->debugLog) << "Exiting function: get_DF (raw) ....." << endl;
  #endif	  	
}


void ADCP::get_DF(void* pData,uint8_t* pNID,uint8_t* pFID,uint8_t* pPRI,PROCESSED_FRAME* pProcessed)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function: get_DF (processed) ....." << endl;
  #endif	

  /*  The first byte is the frame descriptor.
   *  Bit 7 	--> Priority of frame/packet.
   *  Bit 6-4 	-->	Frame type. For data frame/packet, frame type is 0.
   *  Bit 3-0   --> Device node ID.   
  */	

  RAW_FRAME raw = {0};

  this->processed_to_raw(pProcessed,&raw);

  *pPRI = ((raw.byte[0] & ( 1 << 7 )) >> 7);
  *pNID = ( raw.byte[0] & 0x0F );
  
  /* Byte 1 contains the frame ID. */
  
  *pFID = raw.byte[1];

  /* Bytes 2 - 5 contain the frame data. */  

  uint32_t data = 0;

  data |= (  ((uint32_t)raw.byte[2]) << 24 );
  data |= (  ((uint32_t)raw.byte[3]) << 16 );
  data |= (  ((uint32_t)raw.byte[4]) << 8 );
  data |= (  ((uint32_t)raw.byte[5]) << 0 );
  
  (*(uint32_t*)pData) = data;

  #ifdef DEBUG
    (this->debugLog) << "Frame priority:" << ( (*pPRI == 1) ? ("HIGH") : ("LOW") ) << endl;
	(this->debugLog) << "Node ID:" << unsigned(*pNID) << endl;
    (this->debugLog) << "Frame ID:" << unsigned(*pFID) << endl;
    (this->debugLog) << "Frame Data (decimal): " << data << endl;
    (this->debugLog) << "Frame Data (hexa-decimal): " << std::hex << "0x" << data << std::dec << endl;  	
    (this->debugLog) << "Exiting function: get_DF (processed) ....." << endl;
  #endif	  	
}


int ADCP::txq_push(PROCESSED_FRAME packet)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: txq_push ....." << endl;
  #endif

  RAW_FRAME raw;
  
  this->processed_to_raw(&packet,&raw);
  	
	
  if( raw.byte[0] >> 7 )		/* If priority of the data packet is high. */
  {	  
    ADCP::txq.push_back(packet);    	
  }
  else
  {
	ADCP::txq.push_front(packet);	
  }

  #ifdef DEBUG
    (this->debugLog) << "Pushing packet into queue: " << endl;
	(this->debugLog) << (uint16_t)packet.byte[0] << " " << (uint16_t)packet.byte[1] << " " << (uint16_t)packet.byte[2] << " " << (uint16_t)packet.byte[3] << " ";
	(this->debugLog) << (uint16_t)packet.byte[4] << " " << (uint16_t)packet.byte[5] << " " << (uint16_t)packet.byte[6] << " " << (uint16_t)packet.byte[7] << " ";
	(this->debugLog) << (uint16_t)packet.byte[8] << endl;
	(this->debugLog) << "Exiting function: txq_push ....." << endl;
  #endif
  
  return(0);
}

static ADCP::PROCESSED_FRAME at(std::list<ADCP::PROCESSED_FRAME> q,int index)
{
  std::list<ADCP::PROCESSED_FRAME>::iterator e = q.begin();

  std::advance(e,index);

  return(*e);
}

void ADCP::txq_print()
{
  cout << endl << endl << "Transmit queue: " << endl << endl;	
	
  int i = 0;	

  RAW_FRAME raw;
  PROCESSED_FRAME processed;
	
  for( int j = 0 ; j < (int)(ADCP::txq.size()) ; j++ )
  {
    cout << "Frame " << j << " : ";

    processed = at(ADCP::txq,j);

    this->processed_to_raw( &processed, &raw );

    for(int k = 0 ; k < 9 ; k++)
    {
      cout << (uint16_t)(processed.byte[k]) << " ";
    }

    cout << "  (processed)  ";

    for(int k = 0 ; k < 7 ; k++)
    {
      cout << (uint16_t)(raw.byte[k]) << " ";
    }
	
	cout << "  (raw)  ";

    cout << endl;	
  }
	
  cout << endl; 
}

void ADCP::read_file(const char* file_name)
{	
  #ifdef DEBUG
    (this->debugLog) << "Entering function: read_file(" << file_name << ")" << endl;
  #endif  

  /* Load parameter CSV file. 
  *
  *  This file must contain records in the following order:
  *
  *  Column[0]: 	Parameter name.
  *  Column[1]:		Device node name.	 
  *  Column[2]:		Frame name.
  *  Column[3]:		Frame number/frame ID.
  *  Column[4]:		Data type.		
  *	 Column[5]:		Start bit.		
  *	 Column[6]:		Bit length. 	
  *  Column[7]:		Factor.
  *  Column[8]:		Offset.
  *	 Column[9]:		Offset/enum values.
  *  Column[10]:	Minimum.
  *  Column[11]:	Maximum.  
  *
  */
  
  ifstream _file(file_name);
  
  string line;					//	String to hold a record.
  vector<string> record;		//	Vector to hold the entry of current record.		
  
  int count = 0;  				//	Counter to keep track of current record number.
  
  while( getline(_file,line) )
  {
	/* Read new record from file. */  
	  
	record = stringUtils::csv_strings(line);

    #ifdef DEBUG
	  (this->debugLog) << "Reading line from parameter file: " << count << endl;	
	  
	  for(int i = 0 ; i < 12 ; i++)
	  {
	    (this->debugLog) << "Column [" << i << "] :" << record.at(i) << endl; 	  
	  }
	#endif

	// Ignore the first line as it as the CSV header.

	if( (count != 0) )
	{
	  ADCP::PARAMETER_RECORD parameter;

	  #ifdef DEBUG
		(this->debugLog) << "Parameter name:  " << record[0] << endl;
      #endif	  
	
      ADCP::NODE node;
	  node.name = record[1];
	  node.id = std::atoi(record[2].c_str());
	  this->nodes.push_back(node);
	
	  // Organize parameter name in the format:
	  // DEVICE_NODE_NAME::FRAME_NAME::PARAMETER_NAME
	
	  parameter.parameter_name = (record[1] + "::" + record[3] + "::" + record[0]);	
	  
	  // Node name and node ID.
	  
	  parameter.node_name = record[1];
	  parameter.node_ID   = std::atoi(record[2].c_str());
	  
	  // Frame name and frame ID.
	  
	  parameter.frame_name = (record[1] + "::" + record[3]);
	  parameter.frame_ID   = std::atoi(record[4].c_str());
	  
	  // Data type.

	  if( (record[5] == "bool") || (record[5] == "Bool") || (record[5] == "BOOL") )
	  {
	    parameter.type = BOOL;	  
	  }
	  	  
	  if( (record[5] == "uint") || (record[5] == "unsigned") || (record[5] == "UNSIGNED") 
		  || (record[5] == "Unsigned") || (record[5] == "Uint") )
	  {
	    parameter.type = UNSIGNED;	  
	  }

	  if( (record[5] == "int") || (record[5] == "signed") || (record[5] == "SIGNED") 
          || (record[5] == "Signed") || (record[5] == "Int") )
	  {
	    parameter.type = SIGNED;	  
	  }

	  if( (record[5] == "FLOAT32") || (record[5] == "float") || (record[5] == "Float") )
	  {
	    parameter.type = FLOAT32;	  
	  }

	  if( (record[5] == "ENUM") || (record[5] == "Enum") || (record[5] == "enum") )
	  {
	    parameter.type = ENUM;	  
	  }

	  // Start bit and bit length.	

	  parameter.start_bit  = std::atoi(record[6].c_str());
	  parameter.bit_length = std::atoi(record[7].c_str());
	  
	  // Factor, offset, maximum & minimum.
	  	  
	  if( parameter.type == ADCP::Parameter_Type_t::ENUM )
	  {
		stringstream s(record[9]);

        string tempString;

        while( getline(s,tempString,':') )
		{
          (parameter.enum_values).push_back(tempString);
		}			
	  }
	  else
	  {
		parameter.factor  = std::atof(record[8].c_str());
	    parameter.offset  = std::atof(record[9].c_str());
	  }
	  
	  parameter.minimum = std::atof(record[10].c_str());
	  parameter.maximum = std::atof(record[11].c_str());
	  
	  parameter.value = 0;
	  
	  // Load parameter into the parameter list.

	  this->parameters.push_back(parameter);
	}

	count++;	
  }  
  
  /* Collect all frame names into a single vector. */
  
  std::vector<string> frameNames;
  
  for(auto e = (this->parameters).begin() ; e != (this->parameters).end() ; e++ )
  {
    frameNames.push_back((*e).frame_name);	  
  }
  
  /* Find unique frame names from existing frame name vector. */
  
  vector<string>::iterator itUnique;
  
  itUnique = std::unique( frameNames.begin() , frameNames.begin() + frameNames.size() );
  frameNames.resize( std::distance( frameNames.begin(),itUnique ) );
  
  #ifdef DEBUG
	this->debugLog << "Unique frame names: " << endl;
	
	for(int i = 0 ; i < frameNames.size() ; i ++ )
    {
      this->debugLog << frameNames[i] << endl;
    }		
  #endif
  
  // Make structures for each unique frame.
  
  ADCP::FRAME_RECORD tempFrameRecord;
  
  for( int i = 0 ; i < frameNames.size() ; i++ )
  {
	tempFrameRecord.priority = 0;		//	Set initial frame priority to LOW.
    
	// Iterate through each parameter to search for the name of the frame.
	
    for( int j = 0 ; j < (this->parameters).size() ; j ++ )
	{
      if( (this->parameters[j]).frame_name == frameNames[i] )
	  {
		tempFrameRecord.node_name = (this->parameters[j]).node_name;
        tempFrameRecord.node_ID  = (this->parameters[j]).node_ID;
		tempFrameRecord.frame_name = frameNames[i];
		tempFrameRecord.frame_ID = (this->parameters[j]).frame_ID;
		tempFrameRecord.rFrame = {0};
		tempFrameRecord.pFrame = {0};
		
		(this->frames).push_back(tempFrameRecord);
	    
		break;	  
	  }
	}		
  }
  
  // Load pointers to parameter records into the frame record.
  
  for( auto iFrame = (this->frames).begin() ; iFrame != (this->frames).end() ; iFrame++ )
  {
	for( auto iParameter = (this->parameters).begin() ; iParameter != (this->parameters).end() ; iParameter++ )
	{
      if( (*iParameter).frame_name == (*iFrame).frame_name )
	  {
		((*iFrame).parameters).push_back(&(*iParameter));  
	  }
	}		
  }
  
  /* List unique node names. */
  
  vector<ADCP::NODE> nodeList;
  ADCP::NODE tempNode;
  
  for(int i = 0 ; i < this->nodes.size() ; i++ )
  {
	int j = 0;  
	  
	for(j = 0 ; j < i ; j++)
	{
	  if( this->nodes[i].name == this->nodes[j].name )
        break;		  
	}
	
	if( i == j )
	{
      tempNode.name = this->nodes[i].name;
	  tempNode.id   = this->nodes[i].id;
	  nodeList.push_back(tempNode);
	}
  }

  this->nodes.clear();

  for(int i = 0 ; i < nodeList.size() ; i++ )
	this->nodes.push_back(nodeList[i]);	
  
  #ifdef DEBUG
    (this->debugLog) << "Exiting function: read_file(" << file_name << ")" << endl;
  #endif    
}


// Function to get data type in string format.

static string dtype_to_string(ADCP::Parameter_Type_t t)
{
  string st;
  
  switch(t)
  {
    case(ADCP::Parameter_Type_t::BOOL):{ st = "BOOL"; }; break;	  
    case(ADCP::Parameter_Type_t::UNSIGNED):{ st = "UNSIGNED"; }; break; 	  
    case(ADCP::Parameter_Type_t::SIGNED):{ st = "SIGNED"; }; break;
    case(ADCP::Parameter_Type_t::FLOAT32):{ st = "FLOAT32"; }; break; 	  
    case(ADCP::Parameter_Type_t::ENUM):{ st = "ENUM"; }; break; 	  	  
  }
 
  return(st);  
}

// Function to print parameter details.

void ADCP::print_parameter_details(unsigned int index)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function: " << "print_parameter_details(" << index << ")" << endl;
  #endif  
	
  ADCP::PARAMETER_RECORD p = this->parameters[index];	
	
  cout << endl << ">>>>> PARAMETER DETAILS <<<<<" << endl << endl;	
  cout << "Parameter name: " << p.parameter_name << endl;
  cout << "Owning device: " << p.node_name << " [" << (uint16_t)p.node_ID << "]" << endl;
  cout << "Owning frame: " << p.frame_name << " [" << (uint16_t)p.frame_ID << "]" << endl;
  cout << "Data type: " << dtype_to_string(p.type) << endl;
  cout << "Start bit: " << (uint16_t)p.start_bit << endl;
  cout << "Bit Length: " << (uint16_t)p.bit_length << endl;
  
  if(p.type != ADCP::Parameter_Type_t::ENUM)
  {
    cout << "Factor: " << p.factor << endl;
    cout << "Offset: " << p.offset << endl;
  }
  else
  {
	cout << "Enum values:   ";
    
    for(int i = 0 ; i < ((p.enum_values).size()-1) ; i++ )
    {
      cout << p.enum_values[i] << ",";
    }

    cout << p.enum_values[(p.enum_values).size() - 1] << endl;	
  }
  
  cout << "Minimum: " << p.minimum << endl;
  cout << "Maximum: " << p.maximum << endl;  
  
  switch(p.type)
  {
	case(ADCP::Parameter_Type_t::BOOL):
    {
      if( p.value > 0 )
	  {
		cout << "Value: " << "TRUE" << endl;   
	  }
	  else
	  {
	    cout << "Value: " << "FALSE" << endl;	  
	  }
    }
    break;

    case(ADCP::Parameter_Type_t::UNSIGNED):
    {
      cout << "Value: " << (uint32_t)p.value << endl;
    }	
    break;	

    case(ADCP::Parameter_Type_t::SIGNED):
    {
      cout << "Value: " << (int32_t)p.value << endl;
    }
	break;
	
	case(ADCP::Parameter_Type_t::FLOAT32):
	{
	  cout << "Value: " << p.value << endl;	
	}
	break;
	
	case(ADCP::Parameter_Type_t::ENUM):
	{
	  cout << "Value: " << (p.enum_values).at((uint16_t)p.value) << endl;	
	}
	break;
  }

  #ifdef DEBUG
    (this->debugLog) << "Exiting function: " << "print_parameter_details(" << index << ")" << endl;
  #endif  
  
  //cout << (*(this->parameters[index])).node_name << endl;  
}

// Function to print parameter details.

void ADCP::print_parameter_details(string name)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: " << "print_parameter_details(" << name << ")" << endl;
  #endif
	
  int index = 0;

  for( auto iParameter = (this->parameters).begin() ; iParameter != (this->parameters).end() ; iParameter++ )
  {  
    #ifdef DEBUG
	  (this->debugLog) << "Current parameter name: " << (*iParameter).parameter_name << "\t Target parameter name: " << name << endl;
	#endif

	if( (*iParameter).parameter_name == name )
    {
	  #ifdef DEBUG
		(this->debugLog) << "Parameter match found !" << endl;
      #endif	  
		
      ADCP::print_parameter_details(index);
	  break;
    }		
	  
    index++;    
  }	  

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: " << "print_parameter_details(" << name << ")" << endl;
  #endif
}

// Function to print parameter list which has been loaded from the parameter CSV file.

void ADCP::print_parameter_list()
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function: print_parameter_list()" << endl;
  #endif  
	
  for(int i = 0 ; i < (this->parameters).size() ; i ++ )
  {
	cout << endl << endl << "----------> Parameter number: " << i+1 << " <----------" << endl << endl;  
	  
    ADCP::print_parameter_details(i);
  }

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: print_parameter_list()" << endl;
  #endif  
}

//	Function to print details about a frame.

void ADCP::print_frame_details(unsigned int index)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function print_frame_details(" << index << ")" << endl;
  #endif  
	
  ADCP::FRAME_RECORD frame = (this->frames).at(index);
  ADCP::PARAMETER_RECORD* parameter;

  cout << endl << endl << ">>>>> FRAME DETAILS <<<<<" << endl << endl;

  cout << "Frame: " << frame.frame_name << " [" << (uint16_t)frame.frame_ID << "]" << endl;  
  cout << "Owning node: " << frame.node_name << " [" << (uint16_t)frame.node_ID << "]" << endl; 
  cout << "Number of parameters owned by frame:" << (frame.parameters).size() << endl;
  
  cout << endl << "Parameters under this frame:" << endl << endl;
  
  for(int i = 0 ; i < (frame.parameters).size() ; i++)
  {
	parameter = (frame.parameters).at(i);
	
    cout << "Parameter [" << i << "] : " << parameter->parameter_name << "\t -----> \t";	
	
	switch(parameter->type)
	{
	  case(ADCP::Parameter_Type_t::BOOL):
      {
		(parameter->value > 0) ? (cout << "TRUE" ) : (cout << "FALSE" );  
      }
      break;

	  case(ADCP::Parameter_Type_t::UNSIGNED):
      {
		cout << (uint32_t)parameter->value;  
      }
      break;

	  case(ADCP::Parameter_Type_t::SIGNED):
      {
		cout << (int32_t)parameter->value;  
      }
      break;

	  case(ADCP::Parameter_Type_t::FLOAT32):
	  {
		cout << parameter->value;  
	  }
	  break;
	  
	  case(ADCP::Parameter_Type_t::ENUM):
	  {
		cout << (parameter->enum_values).at((uint16_t)parameter->value);  
	  }
	  break;
	}
	
	cout << endl;
  }

  #ifdef DEBUG
    (this->debugLog) << "Exiting function print_frame_details(" << index << ")" << endl;
  #endif  
}

void ADCP::print_frame_details(string name)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: " << "print_frame_details(" << name << ")" << endl;
  #endif
	
  int index = 0;

  for( auto iFrame = (this->frames).begin() ; iFrame != (this->frames).end() ; iFrame++ )
  {  
    #ifdef DEBUG
	  (this->debugLog) << "Current frame name: " << (*iFrame).frame_name << "\t Target frame name: " << name << endl;
	#endif

	if( (*iFrame).frame_name == name )
    {
	  #ifdef DEBUG
		(this->debugLog) << "Frame match found !" << endl;
      #endif	  
		
      ADCP::print_frame_details(index);
	  break;
    }		
	  
    index++;    
  }	  

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: " << "print_frame_details(" << name << ")" << endl;
  #endif
}

//	Function to print details of all available frames in the parameter file.

void ADCP::print_frame_list()
{
  cout << endl << endl << "Framelist: " << endl << endl;	
	
  for(auto iFrame = (this->frames).begin() ; iFrame != (this->frames).end() ; iFrame++ )
  {
    cout << (*iFrame).frame_name << " [ node_ID: " << (uint16_t)(*iFrame).node_ID << ", parameter_ID: " << (uint16_t)(*iFrame).frame_ID <<" ]" << endl;
  }	  
  
  cout << endl << endl;
}

void ADCP::print_node_list()
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: print_node_list()" << endl;
  #endif

  cout << "Device nodes:" << endl << endl;		

  for( auto e = this->nodes.begin() ; e != this->nodes.end() ; e++ )
  {
	cout << (*e).name << "  [" << (*e).id << "]" << endl;
  }	  

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: print_node_list()" << endl;
  #endif
}

// Function to get the instance of a parameter record addressed via its index. 

ADCP::PARAMETER_RECORD* ADCP::get_parameter(unsigned int index)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: get_parameter(" << index << ")" << endl; 
  #endif

  ADCP::PARAMETER_RECORD* p;

  if( index < (this->parameters).size() )
  {
    p = &(this->parameters)[index];	
  }
  else
  {
	#ifdef DEBUG	
	  (this->debugLog) << "Index out of bounds !" << endl;	
	#endif 	
	  
	p = &(this->parameters)[(this->parameters).size()-1];  
  }
  

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: get_parameter(" << index << ")" << endl; 
  #endif  
  
  return(p);
}

// Function to get the instance of a parameter record addressed via its name. 

ADCP::PARAMETER_RECORD* ADCP::get_parameter(string parameter_name)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: get_parameter(" << parameter_name << ")" << endl; 
  #endif 	

  int index = 0;
  ADCP::PARAMETER_RECORD* p;

  for(auto iParameter = (this->parameters).begin() ; iParameter != (this->parameters).end() ; iParameter++)
  {
	if( (*iParameter).parameter_name == parameter_name )
	{
      p = ADCP::get_parameter(index);
	  break;
	}

    index++;	
  }

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: get_parameter(" << parameter_name << ")" << endl; 
  #endif 

  return(p);  
}

// Function to get the instance of a frame record addressed via its index. 

ADCP::FRAME_RECORD* ADCP::get_frame(unsigned int index)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: get_frame(" << index << ")" << endl; 
  #endif

  ADCP::FRAME_RECORD* p;

  if( index < (this->frames).size() )
  {
    p = &(this->frames)[index];	
  }
  else
  {
	#ifdef DEBUG	
	  (this->debugLog) << "Index out of bounds !" << endl;	
	#endif 	
	  
	p = &(this->frames)[(this->frames).size()-1];  
  }
  

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: get_frame(" << index << ")" << endl; 
  #endif  
  
  return(p);
}

// Function to get the instance of a frame record addressed via its name. 

ADCP::FRAME_RECORD* ADCP::get_frame(string frame_name)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: get_frame(" << frame_name << ")" << endl; 
  #endif 	

  int index = 0;
  ADCP::FRAME_RECORD* p;

  for(auto iFrame = (this->frames).begin() ; iFrame != (this->frames).end() ; iFrame++)
  {
	if( (*iFrame).frame_name == frame_name )
	{
      p = ADCP::get_frame(index);
	  break;
	}

    index++;	
  }

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: get_frame(" << frame_name << ")" << endl; 
  #endif 

  return(p);  
}

unsigned int ADCP::get_node_id(string node_name)
{
	#ifdef DEBUG
		(this->debugLog) << "Entering function: get_node_id(" << node_name << ")" << endl;
	#endif

	unsigned int node_id = 0;

	for(int i = 0 ; i < this->nodes.size() ; i++ )
	{
		if( node_name == nodes[i].name )
		{
			node_id = nodes[i].id;
		}
	}

	#ifdef DEBUG
		(this->debugLog) << "Exiting function: get_node_id(" << node_name << ")" << endl;
	#endif
	
	return(node_id);
}


//	Structure for IEEE754 32-bit notation to floating point conversion.

union IEEE754_struct
{
  float _float;
  uint32_t _int;  
}IEEE754;

//	Function to write a parameter to its owning frame.

void ADCP::write_parameter(unsigned int index,double value)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function: write_parameter(" << index << ", " << value << ")" << endl;
  #endif   

  ADCP::PARAMETER_RECORD* parameter = &((this->parameters)[index]);
  ADCP::FRAME_RECORD* frame = get_frame(parameter->frame_name);

  double frameValue = 0;		//	Double frame value.
  uint32_t iFrameValue = 0;		//	Integer frame value.
  
  /* Limit value of the parameter between mentioned min and max values. */
  
  if( value > parameter->maximum )
  {
	value = parameter->maximum;  
  }
  if( value < parameter->minimum )
  {
	value = parameter->minimum;  
  }
  
  uint32_t frameData = frame->data;
  
  parameter->value = value;
  
  /* When loading data into the frame, the following conversion has to be done:
  *
  *   frame_value = (actual_value - offset) / factor;
  *
  */
  
  frameValue = ( (parameter->value) - (parameter->offset) ) / (parameter->factor);

  //  Generate mask for the value: Mask = (2^(bit_length)) - 1.

 uint32_t mask = (( (uint32_t)1 << (uint16_t)(parameter->bit_length) ) - 1);
  
  // Clear bits associated with current parameter.
   
  if(parameter->bit_length == 32)
  {
	frameData = 0;  
  }
  else
  {
	frameData &= ~( mask << (parameter->start_bit) );   
  }
  
  // Compute the binary value of the frame value.
  
  switch(parameter->type)
  {
	case(ADCP::Parameter_Type_t::BOOL):
    {
      if(frameValue == 1.0f)
	  {
		iFrameValue = (uint32_t)1;  
	  }
	  else
	  {
		iFrameValue = (uint32_t)0;  
	  } 
    }
    break;

    case(ADCP::Parameter_Type_t::UNSIGNED):
    {
	  if(parameter->bit_length != 32)	
        iFrameValue = ( ((uint32_t)frameValue) & mask );
	  else
        iFrameValue = ((uint32_t)frameValue & (uint32_t)0xFFFFFFFF);		  
    }
    break;

	case(ADCP::Parameter_Type_t::SIGNED):
    {
	  uint8_t _sign = (uint8_t)(frameValue < 0);
	  
	  if(parameter->bit_length != 32)
	  {
	  
	    if( frameValue < 0 )
	    {
		  frameValue = -frameValue;  
	    }
	  
	    if(_sign)
	    {
		  iFrameValue = (  ((uint32_t)1 << (uint16_t)parameter->bit_length) - ( (uint32_t)frameValue & mask ) ) & mask;   
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

	case(ADCP::Parameter_Type_t::FLOAT32):
    {
	  IEEE754._float = (float)frameValue;
	  iFrameValue = IEEE754._int;
    }
    break;	
  }

  /* Merge bits to existing frame data. */

  frameData |= (iFrameValue << (parameter->start_bit));
  frame->data = frameData;
  
  /* Update raw & processed frames. */
  
  put_DF(&frameData,frame->node_ID,frame->frame_ID,frame->priority,&(frame->rFrame));
  put_DF(&frameData,frame->node_ID,frame->frame_ID,frame->priority,&(frame->pFrame));
  generate_checksum(&(frame->rFrame));
  generate_checksum(&(frame->pFrame));

  #ifdef DEBUG
    (this->debugLog) << "Exiting function: write_parameter(" << index << ", " << value << ")" << endl;
  #endif
}


void ADCP::write_parameter(string name,double value)
{
  unsigned int index = 0;	
	
  for( auto iParameter = (this->parameters).begin() ; iParameter != (this->parameters).end() ; iParameter++ )
  {
    if( (this->parameters[index]).parameter_name == name )
	{
	  ADCP::write_parameter(index,value);	
	}		
    index++;	  
  }	  
}

void ADCP::write_parameter(unsigned int index, string enum_value)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function: write_parameter(" << index << "," << enum_value << ")" << endl;
  #endif

  ADCP::PARAMETER_RECORD* parameter = &((this->parameters)[index]);
  ADCP::FRAME_RECORD* frame = get_frame(parameter->frame_name);
  
  uint32_t  value = 0;
  uint8_t count = 0;
  uint8_t found = 0;    
	  
  /* Search for enum value. */

  for( auto e = parameter->enum_values.begin() ; e != parameter->enum_values.end() ; e++ )
  {
	if( *e == enum_value )
	{
		value = count;
		found = 1;				
	}
	
	count++;
  }	  

  /* If value is not found. do no processing. */
	  
  if(found != 1)
	return;	
	  
  uint32_t frameData = frame->data;
  
  parameter->value = (double)value;
  
  //  Generate mask for the value: Mask = (2^(bit_length)) - 1.

 uint32_t mask = (( (uint32_t)1 << (uint16_t)(parameter->bit_length) ) - 1);
  
  // Clear bits associated with current parameter.
   
  if(parameter->bit_length == 32)
  {
	frameData = 0;  
  }
  else
  {
	frameData &= ~( mask << (parameter->start_bit) );   
  }
  
  uint32_t iFrameValue = value;
  
  /* Merge bits to existing frame data. */

  frameData |= (iFrameValue << (parameter->start_bit));
  frame->data = frameData;
  
  /* Update raw & processed frames. */
  
  put_DF(&frameData,frame->node_ID,frame->frame_ID,frame->priority,&(frame->rFrame));
  put_DF(&frameData,frame->node_ID,frame->frame_ID,frame->priority,&(frame->pFrame));

  #ifdef DEBUG
    (this->debugLog) << "Exiting function: write_parameter(" << index << "," << enum_value << ")" << endl;
  #endif
}	


void ADCP::write_parameter(string name,string enum_value)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: write_parameter(" << name << "," << enum_value << ")" << endl;
  #endif	
	
  unsigned int index = 0;	
	
  for( auto iParameter = (this->parameters).begin() ; iParameter != (this->parameters).end() ; iParameter++ )
  {
    if( (this->parameters[index]).parameter_name == name )
	{
	  ADCP::write_parameter(index,enum_value);	
	}		
    index++;	  
  }	  
  
  #ifdef DEBUG
	(this->debugLog) << "Exiting function: write_parameter(" << name << "," << enum_value << ")" << endl;
  #endif	  
}


//	Function to read a parameter from its owning frame.

double ADCP::read_parameter(unsigned int index)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function read_parameter(" << index << ")" << endl;
  #endif 	

  ADCP::PARAMETER_RECORD* parameter = &((this->parameters)[index]);
  ADCP::FRAME_RECORD* frame = get_frame(parameter->frame_name);

  uint32_t frameData = frame->data;
  uint32_t mask = (  ((uint32_t)1 << (uint16_t)parameter->bit_length) - 1  );
  uint32_t iFrameValue;
  double actualValue;

  if( parameter->bit_length != 32 )
  {
	iFrameValue = (mask & (frameData >> (uint16_t)parameter->start_bit));  
  }
  else
  {
	iFrameValue = (uint32_t)(frameData & 0xFFFFFFFF);  
  }

  switch(parameter->type)
  {
	case(ADCP::Parameter_Type_t::BOOL):
    {
      actualValue = (double)((uint16_t)(iFrameValue > 0));
    }
    break;

    case(ADCP::Parameter_Type_t::UNSIGNED):
    {
	  actualValue = ((float)parameter->offset + ((float)iFrameValue * (float)parameter->factor)); 
    }
    break;

    case(ADCP::Parameter_Type_t::ENUM):
    {
	  actualValue = ((float)parameter->offset + ((float)iFrameValue * (float)parameter->factor)); 
    }
    break;

    case(ADCP::Parameter_Type_t::SIGNED):
    {
      uint32_t maxAbsValue = (( (uint32_t)1UL << (parameter->bit_length - 1) ) - 1);

      if( iFrameValue > maxAbsValue ) // When value of number > 2*(N_bits-1) value is -ve.
      {
        if( parameter->bit_length < 32 )
        {
          actualValue = ( parameter->offset - parameter->factor * (float)( ( 1UL << parameter->bit_length ) - iFrameValue ) );
        }
        else
        {
          actualValue = ( parameter->factor * (float)( (int32_t)iFrameValue ) + parameter->offset );
        }
      }
      else /* If number is positive. */
      {
        actualValue = ( parameter->factor * (float)iFrameValue + parameter->offset );
      }      
    }
    break;	
	
	case(ADCP::Parameter_Type_t::FLOAT32):
	{
	  IEEE754._int = iFrameValue;
	  actualValue = ( (parameter->factor * IEEE754._float) + parameter->offset ); 			
	}
	break;
  }

  parameter->value = actualValue;

  #ifdef DEBUG
    (this->debugLog) << "Value of parameter " << parameter->parameter_name << " ------>\t" << actualValue << endl;
  #endif 

  #ifdef DEBUG
	(this->debugLog) << "Exiting function read_parameter(" << index << ")" << endl;
  #endif 		

  return(actualValue);
}

double ADCP::read_parameter(string name)
{
  unsigned int index = 0;	
  double value = 0;	
	
  for( auto iParameter = (this->parameters).begin() ; iParameter != (this->parameters).end() ; iParameter++ )
  {
    if( (this->parameters[index]).parameter_name == name )
	{
	  value = ADCP::read_parameter(index);	
	}		
    index++;	  
  }

  return(value);  
}

string ADCP::read_parameter_enum(unsigned int index)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: read_parameter_enum(" << index << ")" << endl; 
  #endif	

  read_parameter(index);

  ADCP::PARAMETER_RECORD p = this->parameters.at(index);

  string s = p.enum_values.at((unsigned int)p.value);  

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: read_parameter_enum(" << index << ")" << endl; 
  #endif
  
  return(s);
}

string ADCP::read_parameter_enum(string name)
{
  unsigned int index = 0;	
  string value = "";	
	
  for( auto iParameter = (this->parameters).begin() ; iParameter != (this->parameters).end() ; iParameter++ )
  {
    if( (this->parameters[index]).parameter_name == name )
	{
	  value = ADCP::read_parameter_enum(index);	
	}		
    index++;	  
  }

  return(value);  
}

void ADCP::serial_port_init(string com_port, unsigned int baud_rate)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function serial_port_init(" << com_port << "," << baud_rate << ")" << endl;
  #endif  
	
  string comPortName = ("\\\\.\\" + com_port);

  //this->serialPort = {0};
  this->serialPort.portname = com_port;
  this->serialPort.RXB_bytes = 0;
  this->serialPort.TXB_bytes = 0;
  this->serialPort.status = CLOSED;  

  (this->serialHandle) =  CreateFile(comPortName.c_str(),			//	Name of file.
						GENERIC_READ | GENERIC_WRITE,		//	Enable both read and write.
						0,									//	sharing mode: None.
						0,									//	Default security descriptor.
						OPEN_EXISTING,						//	Open existing COM Port.
						FILE_ATTRIBUTE_NORMAL,				//	
						0);         	

  if( this->serialHandle == INVALID_HANDLE_VALUE )
  {
    if( GetLastError() == ERROR_FILE_NOT_FOUND )
	{
	  #ifdef DEBUG
		(this->debugLog) << "COM Port: " << com_port << " not found" << endl; 
      #endif	  
		
	  //cout << "COM Port not found !" << endl;	
	}
  }
  else
  {
    #ifdef DEBUG
		(this->debugLog) << "Connection open @ " << com_port << endl;
	#endif	
  }	  
  
  // Serial port parameters.
  
  this->serialDCB = {0};
  this->serialDCB.DCBlength = sizeof(this->serialDCB);
  
  if(GetCommState(this->serialHandle, &this->serialDCB) )
  {
    #ifdef DEBUG
		(this->debugLog) << com_port << " available for configuration." << endl;
    #endif		
  }
  else
  {
	#ifdef DEBUG
		(this->debugLog) << com_port << " not available for configuration." << endl;
    #endif	
  }
  
  this->serialDCB.BaudRate = baud_rate;
  this->serialDCB.ByteSize = 8;
  this->serialDCB.StopBits = ONESTOPBIT;
  this->serialDCB.Parity = NOPARITY;
  
  if(SetCommState(this->serialHandle,&this->serialDCB) )
  {
	#ifdef DEBUG
		(this->debugLog) << "Communication parameters set successfully for " << com_port << endl;
    #endif	  
	
	PurgeComm(this->serialHandle,PURGE_RXCLEAR);
	PurgeComm(this->serialHandle,PURGE_TXCLEAR);
	SetCommMask(this->serialHandle,EV_RXCHAR);
    (this->serialPort).status = OPEN;

  }
  else
  {
	#ifdef DEBUG
		(this->debugLog) << "Communication parameters not set for " << com_port << endl;
    #endif	  

    (this->serialPort).status = CLOSED;	
  }
  
  COMMTIMEOUTS timeout = {0};
  timeout.ReadIntervalTimeout = 60;
  timeout.ReadTotalTimeoutConstant = 60;
  timeout.ReadTotalTimeoutMultiplier = 15;
  timeout.WriteTotalTimeoutConstant = 60;
  timeout.WriteTotalTimeoutMultiplier = 8;
  
  if (SetCommTimeouts(this->serialHandle, &timeout)) 
  {
    #ifdef DEBUG
		(this->debugLog) << "Communication timeouts set successfully for " << com_port << endl;
	#endif  
  }
  else  
  {
    #ifdef DEBUG
		(this->debugLog) << "Communication timeouts NOT set successfully for " << com_port << endl;
	#endif  
  }  	
  
  #ifdef DEBUG
	(this->debugLog) << "Exiting function serial_port_init(" << com_port << "," << baud_rate << ")" << endl;
  #endif    
}

unsigned int ADCP::serial_port_flush(uint8_t* rx_bytes)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: serial_flush()" << endl; 
  #endif	

  serial_port_status();

  unsigned int bytes_in_rx_buffer = this->serialPort.RXB_bytes;

  uint8_t* tempRxBuffer = (uint8_t*)malloc(sizeof(uint8_t) * bytes_in_rx_buffer);

  if( bytes_in_rx_buffer < 1024 )
  { 
	if( rx_bytes != NULL )
		read_bytes(rx_bytes,bytes_in_rx_buffer);
	else
		read_bytes(tempRxBuffer,bytes_in_rx_buffer);	
  }
  
  free(tempRxBuffer);

  #ifdef DEBUG	
	(this->debugLog) << "Number of bytes flushed: " << std::dec << bytes_in_rx_buffer << endl;
  #endif

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: serial_flush()" << endl; 
  #endif
}


vector<string> ADCP::serial_port_scan()
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: serial_port_scan()" << endl;
  #endif
  
  cout << "\n\nScanning serial ports: \n\n";
  
  string com_port_name = "";
  
  HANDLE 	    tempSerialHandle;
  DCB    		tempSerialDCB;
  COMMTIMEOUTS  tempSerialTimeout;
  
  vector<string> comPorts;
  
  for(unsigned int com_index = 0 ; com_index <= 254 ; com_index++)
  {
	/* Form COM Port Name. */
	
	com_port_name = ("\\\\.\\COM" + to_string(com_index));

	/* Get handle name. */

    tempSerialHandle = CreateFile(com_port_name.c_str(),	//	Name of file.
						GENERIC_READ | GENERIC_WRITE,		//	Enable both read and write.
						0,									//	sharing mode: None.
						0,									//	Default security descriptor.
						OPEN_EXISTING,						//	Open existing COM Port.
						FILE_ATTRIBUTE_NORMAL,				//	
						0);         	
	
	if( ( tempSerialHandle == INVALID_HANDLE_VALUE ) || ( GetLastError() == ERROR_FILE_NOT_FOUND ) )
	{
	  /* COM Port handle invalid / COM port access is denied. */
	  continue;		
	}
	else
	{
	  /* COM Port Handle is valid. Configure DCB (Device Control Block). */
	  
	  tempSerialDCB = {0};
	  tempSerialDCB.DCBlength = sizeof(tempSerialDCB);
	  	  
	  if( !GetCommState(tempSerialHandle,&tempSerialDCB) )
	  {
		/* If COM port is not configurable. */
		continue;
	  }
	  else
	  {
		/* COM Port is configurable. */
		cout << "COM" << com_index << " available !" << endl;

		comPorts.push_back("COM" + com_index);

		#ifdef DEBUG
		  (this->debugLog) << "COM" << com_index << " available !" << endl;
		#endif
	  }
	}
	
	CloseHandle(tempSerialHandle);
  }
  
  #ifdef DEBUG
	(this->debugLog) << "Exiting function: serial_port_scan()" << endl;
  #endif
  
  return(comPorts);
}


void ADCP::serial_port_close()
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: serial_port_close()" << endl;
  #endif  
  
  CloseHandle(this->serialHandle);
  
  #ifdef DEBUG
	(this->debugLog) << "Exiting function: serial_port_close()" << endl;
  #endif
}

ADCP::Serial_Port_Status_t ADCP::serial_port_status()
{	
  #ifdef DEBUG
	//(this->debugLog) << "Entering function: serial_port_status()" << endl;
  #endif   
  
  DWORD tempError;
  
  //(this->debugLog) << "Error Status: " << GetLastError() << endl;;
  ClearCommError( this->serialHandle, &tempError, &this->serialStatus );
  
  if( tempError == CE_BREAK )
	  this->serialPort.status = BREAK;
  if( tempError == CE_FRAME )
	  this->serialPort.status = FRAME_ERROR;
  if( tempError == CE_IOE)
	  this->serialPort.status = IO_ERROR;
  if( tempError == CE_OVERRUN )
	  this->serialPort.status = OVERRUN;		
  if( tempError == CE_RXOVER )
	  this->serialPort.status = RX_OVERFLOW;
  if( tempError == CE_RXPARITY )
	  this->serialPort.status = RX_PARITY_DETECT;
  
  this->serialPort.RXB_bytes = this->serialStatus.cbInQue;
  this->serialPort.TXB_bytes = this->serialStatus.cbOutQue;
  
  #ifdef DEBUG
  
  /*
  
    (this->debugLog) << "Serial Port Status: ";
  
	switch(this->serialPort.status)
	{
		case(BREAK):{ (this->debugLog) << "BREAK" << endl; }; break;
		case(FRAME_ERROR):{ (this->debugLog) << "FRAME_ERROR" << endl; }; break;
		case(IO_ERROR):{ (this->debugLog) << "IO_ERROR" << endl; }; break;
		case(OVERRUN):{ (this->debugLog) << "OVERRUN" << endl; }; break;
		case(RX_OVERFLOW):{ (this->debugLog) << "RX_OVERFLOW" << endl; }; break;
		case(RX_PARITY_DETECT):{ (this->debugLog) << "RX_PARITY_DETECT" << endl; }; break;
		default:
		  (this->debugLog) << "Unknown (" << tempError << ")" << endl;
	}
  
    (this->debugLog) << "Number of bytes in transmit buffer: " << this->serialPort.TXB_bytes << endl;
	(this->debugLog) << "Number of bytes in receive buffer: " << this->serialPort.RXB_bytes << endl;
  
	(this->debugLog) << "Exiting function: serial_port_status()" << endl;

  */

  #endif
}

void ADCP::send_packet(PROCESSED_FRAME packet)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: send_packet" << endl;
  #endif	

  char buffer[10];
  buffer[0] = packet.byte[0];
  buffer[1] = packet.byte[1];
  buffer[2] = packet.byte[2];
  buffer[3] = packet.byte[3];
  buffer[4] = packet.byte[4];
  buffer[5] = packet.byte[5];
  buffer[6] = packet.byte[6];
  buffer[7] = packet.byte[7];
  buffer[8] = packet.byte[8];

  DWORD dwRead = 0;
  
  #ifdef DEBUG
	(this->debugLog) << "Writing packet via serial port:" << endl;

    (this->debugLog) << "HEX:\t";
	
	for(int i = 0 ; i < 9 ; i++)
	{
	  (this->debugLog) << std::hex << "0x" << (uint16_t)packet.byte[i] << "  ";	
	}
	
	(this->debugLog) << endl;
	
	(this->debugLog) << "DEC: \t";

	for(int i = 0 ; i < 9 ; i++)
	{
	  (this->debugLog) << std::dec << "0d" << (uint16_t)packet.byte[i] << "  ";	
	}
	
	(this->debugLog) << endl;

	
  #endif 	
  
  if( !WriteFile(this->serialHandle, buffer, 9, &dwRead, NULL) )
  {
	#ifdef DEBUG  
		(this->debugLog) << "Packet not sent successfully ..." << endl;
    #endif	
  }
  else
  {
    #ifdef DEBUG
		(this->debugLog) << "Packet send successfully !" << endl;
	#endif	
  }
  	
  #ifdef DEBUG
	(this->debugLog) << "Exiting function: send_packet" << endl;
  #endif		
}



void ADCP::receive_packet(PROCESSED_FRAME* packet)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: receive_packet" << endl;
  #endif	

  char buffer[10];

  DWORD dwRead = 0;
  
  /* Synchronization for packet. */
  
  uint8_t temp_rx_byte = 0;
  uint8_t byte_count = 0;
  
  vector<uint8_t> tempBuffer;
    
  DWORD eventMask;
  
  do
  {
	//while( !WaitCommEvent(this->serialHandle, &eventMask, &o) );	
	
	/* Wait until serial buffer is filled. */
	
	do{
		serial_port_status();
	}while(this->serialPort.RXB_bytes == 0);
	
	
	ReadFile(this->serialHandle,&temp_rx_byte,1,NULL,NULL);				//	Read single byte from serial port.
	serial_port_status();												//	Get status of transmit and receive queues.
	byte_count++;														//	Increment byte counter.
    tempBuffer.push_back(temp_rx_byte);									//	Store received byte in byte vector.
  }while( !(temp_rx_byte == 0x0A |										//	Wait until termination character is received 
		 ( this->serialPort.RXB_bytes == 0 ) | 							//  or all bytes in serial port have not been read
		 ( byte_count > 1000 )) ); 										//	or maximum numbe of byte reads have expired.
    
  if(byte_count != 9)
  {
	/* Invalid packet received !. Discard. */

    #ifdef DEBUG
	  (this->debugLog) << "Number of bytes before termination character: " << (unsigned int)byte_count << endl;	
	#endif 

	return;	
  }
  else
  {
	/* Transfer all bytes into buffer. */  
	
    #ifdef DEBUG
	  (this->debugLog) << "Complete packet received !" << endl;
    #endif	  
	
	for(int i = 0 ; i < 9 ; i++)
		buffer[i] = tempBuffer[i];	
  }
  
  /*
  
  if( !ReadFile(this->serialHandle, buffer, 9, &dwRead, NULL) )
  {
	#ifdef DEBUG  
		(this->debugLog) << "Packet not received successfully ..." << endl;
    #endif	
  }
  else
  {
    #ifdef DEBUG
		(this->debugLog) << "Packet received successfully !" << endl;
	#endif	
  }
  
  */

  for(int i = 0 ; i < 9 ; i++)
  {
	packet->byte[i] = buffer[i];  
  }

  #ifdef DEBUG
	(this->debugLog) << "Received packet via serial port:" << endl;

    (this->debugLog) << "HEX:\t";
	
	for(int i = 0 ; i < 9 ; i++)
	{
	  (this->debugLog) << std::hex << "0x" << (uint16_t)packet->byte[i] << "  ";	
	}
	
	(this->debugLog) << endl;
	
	(this->debugLog) << "DEC: \t";

	for(int i = 0 ; i < 9 ; i++)
	{
	  (this->debugLog) << std::dec << "0d" << (uint16_t)packet->byte[i] << "  ";	
	}
	
	(this->debugLog) << endl;	
  #endif 	

  	
  #ifdef DEBUG
	(this->debugLog) << "Exiting function: receive_packet" << endl;
  #endif		
}


void ADCP::receive_packet(PROCESSED_FRAME* packet,uint32_t timeout_ms)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: receive_packet" << endl;
  #endif	

  char buffer[10];

  DWORD dwRead = 0;
  
  /* Synchronization for packet. */
  
  uint8_t temp_rx_byte = 0;
  uint8_t byte_count = 0;
  
  vector<uint8_t> tempBuffer;
    
  DWORD eventMask;
  
  clock_t startTime = clock();
  uint32_t timeInterval = 0;
  
  do
  {
	//while( !WaitCommEvent(this->serialHandle, &eventMask, &o) );	
	
	/* Wait until serial buffer is filled. */
	
	do{
		serial_port_status();
		
		timeInterval = (uint32_t)((double)(1000 * (double)( clock() - startTime )) / (double)CLOCKS_PER_SEC);
		
	}while( (this->serialPort.RXB_bytes == 0) && (timeInterval < timeout_ms) );
	
	
	ReadFile(this->serialHandle,&temp_rx_byte,1,NULL,NULL);				//	Read single byte from serial port.
	serial_port_status();												//	Get status of transmit and receive queues.
	byte_count++;														//	Increment byte counter.
    tempBuffer.push_back(temp_rx_byte);									//	Store received byte in byte vector.
    timeInterval = (uint32_t)((double)(1000 * (double)( clock() - startTime )) / (double)CLOCKS_PER_SEC);
  }while( !(temp_rx_byte == 0x0A |										//	Wait until termination character is received 
		 ( this->serialPort.RXB_bytes == 0 ) | 							//  or all bytes in serial port have not been read
		 ( byte_count > 1000 ) |
		 ( timeInterval > timeout_ms )) ); 										//	or maximum numbe of byte reads have expired.
    
  if(byte_count != 9)
  {
	/* Invalid packet received !. Discard. */

    #ifdef DEBUG
	  (this->debugLog) << "Number of bytes before termination character: " << (unsigned int)byte_count << endl;	
	#endif 

	return;	
  }
  else
  {
	/* Transfer all bytes into buffer. */  
	
    #ifdef DEBUG
	  (this->debugLog) << "Complete packet received !" << endl;
    #endif	  
	
	for(int i = 0 ; i < 9 ; i++)
		buffer[i] = tempBuffer[i];	
  }
  
  /*
  
  if( !ReadFile(this->serialHandle, buffer, 9, &dwRead, NULL) )
  {
	#ifdef DEBUG  
		(this->debugLog) << "Packet not received successfully ..." << endl;
    #endif	
  }
  else
  {
    #ifdef DEBUG
		(this->debugLog) << "Packet received successfully !" << endl;
	#endif	
  }
  
  */

  for(int i = 0 ; i < 9 ; i++)
  {
	packet->byte[i] = buffer[i];  
  }

  #ifdef DEBUG
  
    if( timeInterval >= timeout_ms )
	{
	  (this->debugLog) << "Timeout event occured while receiving packet" << endl;		
	}
  
	(this->debugLog) << "Received packet via serial port:" << endl;

    (this->debugLog) << "HEX:\t";
	
	for(int i = 0 ; i < 9 ; i++)
	{
	  (this->debugLog) << std::hex << "0x" << (uint16_t)packet->byte[i] << "  ";	
	}
	
	(this->debugLog) << endl;
	
	(this->debugLog) << "DEC: \t";

	for(int i = 0 ; i < 9 ; i++)
	{
	  (this->debugLog) << std::dec << "0d" << (uint16_t)packet->byte[i] << "  ";	
	}
	
	(this->debugLog) << endl;	
  #endif 	

  	
  #ifdef DEBUG
	(this->debugLog) << "Exiting function: receive_packet" << endl;
  #endif		
}



unsigned int ADCP::poll_byte(uint8_t byte)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function poll_byte(" << (uint16_t)byte << ")" << endl; 
  #endif	

  unsigned int count = 0;

  uint8_t buffer[2];

  DWORD dwRead;	

  do
  {
    if(ReadFile(this->serialHandle, buffer, 10, &dwRead, NULL))
	{
	    count++;	
		
		#ifdef DEBUG
		    (this->debugLog) << "Number of bytes read: " << dwRead << endl;
			(this->debugLog) << "Current byte read: " << std::hex << "0x" << (uint16_t)buffer[0] << endl;
			(this->debugLog) << "Byte count: " << std::dec << (unsigned int)count << endl; 		
		#endif		
	}
	else
	{
		#ifdef DEBUG
			(this->debugLog) << "Byte not read successfully ....." << endl;
		#endif	
	}
	
	if( buffer[0] == byte )
	{
		#ifdef DEBUG
			(this->debugLog) << "Target byte found after reading " << count << " bytes" << endl;
		#endif		
	}
	
  }while( buffer[0] != byte );	  


  #ifdef DEBUG
	(this->debugLog) << "Exiting function poll_byte(" << (uint16_t)byte << ")" << endl; 
  #endif		
}


unsigned int ADCP::read_bytes(uint8_t* pBuffer, unsigned int n_bytes)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: read_bytes" << endl;
  #endif	

  uint8_t* _buffer = (uint8_t*)malloc( sizeof(uint8_t) * (n_bytes+1) );  

  DWORD dwRead;

  if(ReadFile(this->serialHandle, _buffer, n_bytes, &dwRead, NULL))
  {
    (this->debugLog) << "Bytes read successfully !" << endl;
  }	  
  else
  {
	(this->debugLog) << "Requested number of bytes not read...." << endl;  
  }

  for( int i = 0 ; i < n_bytes ; i++ )
  {
	pBuffer[i] = _buffer[i];  
  }

  #ifdef DEBUG

	for(int i = 0 ; i < n_bytes ; i++)
	{
		(this->debugLog) << "Received byte [" << i << "]: " << std::hex << "0x" << (uint16_t)pBuffer[i] << endl; 	
	}

  #endif		

  free(_buffer);

  #ifdef DEBUG
	(this->debugLog) << "Number of bytes received: " << dwRead << endl;
  #endif	

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: read_bytes" << endl;
  #endif	
}
	
unsigned int ADCP::write_bytes(uint8_t* pBuffer, unsigned int n_bytes)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: write_bytes" << endl;
  #endif	

  uint8_t* _buffer = (uint8_t*)malloc( sizeof(uint8_t) * (n_bytes+1) );  

  DWORD dwRead;

  for( int i = 0 ; i < n_bytes ; i++ )
  {
	_buffer[i] = pBuffer[i];  
  }

  if(WriteFile(this->serialHandle, _buffer, n_bytes, &dwRead, NULL))
  {
    (this->debugLog) << "Bytes written successfully !" << endl;
  }	  
  else
  {
	(this->debugLog) << "Requested number of bytes not written...." << endl;  
  }

  #ifdef DEBUG

	for(int i = 0 ; i < n_bytes ; i++)
	{
		(this->debugLog) << "Written byte [" << i << "]: " << std::hex << "0x" << (uint16_t)pBuffer[i] << endl; 	
	}

  #endif		

  free(_buffer);

  #ifdef DEBUG
	(this->debugLog) << "Number of bytes written: " << dwRead << endl;
  #endif	

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: read_bytes" << endl;
  #endif		
  
  return((unsigned int)dwRead);
}


void ADCP::send_parameter(unsigned int parameter_index, double value)
{
  #ifdef DEBUG	
	(this->debugLog) << "Entering function: send_parameter(" << parameter_index << "," << value << ")" << endl;
  #endif	
	
  write_parameter(parameter_index,value);
  
  /* Get the frame name associated with the parameter name. */
  /* Get the processed packet related to the frame and send it via serial port. */
  
  send_frame(this->parameters[parameter_index].frame_name);	

  #ifdef DEBUG	
	(this->debugLog) << "Exiting function: send_parameter(" << parameter_index << "," << value << ")" << endl;
  #endif
}

void ADCP::repeated_send_parameter(string parameter_name, double value, unsigned int count, unsigned int interval_ms)
{
	#ifdef DEBUG
		(this->debugLog) << "Entering function: repeated_send_parameter(" << parameter_name << "," << value << "," << count << "," << interval_ms << ")" << endl;
	#endif	

	unsigned int counter = 0;
	
	for( counter = 0 ; counter < count ; counter++ )
	{
		#ifdef DEBUG
			(this->debugLog) << "Transmitting parameter: " << parameter_name << " ( count: " << count << " )" << endl;
		#endif
		
		send_parameter(parameter_name,value);
		Sleep(interval_ms);		
	}
	
	#ifdef DEBUG
		(this->debugLog) << "Exiting function: repeated_send_parameter(" << parameter_name << "," << value << "," << count << "," << interval_ms << ")" << endl;
	#endif	
}

void ADCP::send_parameter(string parameter_name, double value)
{
  #ifdef DEBUG	
	(this->debugLog) << "Entering function: send_parameter(" << parameter_name << "," << value << ")" << endl;
  #endif	
	
  write_parameter(parameter_name,value);
  
  /* Get the frame name associated with the parameter name. */
  /* Get the processed packet related to the frame and send it via serial port. */
  
  send_frame(get_parameter(parameter_name)->frame_name);	

  #ifdef DEBUG	
	(this->debugLog) << "Exiting function: send_parameter(" << parameter_name << "," << value << ")" << endl;
  #endif
}

void ADCP::send_frame(unsigned int frame_index)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: send_frame(" << frame_index << ")" << endl;
  #endif   

  raw_to_processed(&(get_frame(frame_index)->rFrame),&(get_frame(frame_index)->pFrame));

  #ifdef DEBUG
	(this->debugLog) << "Sending frame: " << get_frame(frame_index)->frame_name << endl;
	
	for(int i = 0 ; i < 9 ; i++ )
		(this->debugLog) << "0d" << get_frame(frame_index)->pFrame.byte[i] << " ";
	
	(this->debugLog) << endl;
  #endif
	
  generate_checksum(&(get_frame(frame_index)->rFrame));
  generate_checksum(&(get_frame(frame_index)->pFrame));  
	
  send_packet(get_frame(frame_index)->pFrame);	


  for(int i = 0 ; i < 9 ; i++ )
  {
	this->transmitLog << (unsigned int)(get_frame(frame_index)->pFrame.byte[i]) << ",";
  }
  
  this->transmitLog << (( (double)clock() - (double)this->logStartTime ) * 1000 ) / (double)CLOCKS_PER_SEC << endl;

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: send_frame(" << frame_index << ")" << endl;
  #endif
}

void ADCP::send_frame(string frame_name)
{ 
  #ifdef DEBUG
	(this->debugLog) << "Entering function: send_frame(" << frame_name << ")" << endl;
  #endif

  raw_to_processed(&(get_frame(frame_name)->rFrame),&(get_frame(frame_name)->pFrame));

  #ifdef DEBUG
	(this->debugLog) << "Sending frame: " << get_frame(frame_name)->frame_name << endl;
	
	for(int i = 0 ; i < 9 ; i++ )
		(this->debugLog) << "0d" << get_frame(frame_name)->pFrame.byte[i] << " ";
	
	(this->debugLog) << endl;
  #endif
	
  generate_checksum( &(get_frame(frame_name)->rFrame) );
  generate_checksum( &(get_frame(frame_name)->pFrame) );  
	
  send_packet(get_frame(frame_name)->pFrame);	

  for(int i = 0 ; i < 9 ; i++ )
  {
	this->transmitLog << (unsigned int)(get_frame(frame_name)->pFrame.byte[i]) << ",";
  }
  
  this->transmitLog << (( (double)clock() - (double)this->logStartTime ) * 1000 ) / (double)CLOCKS_PER_SEC << endl;

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: send_frame(" << frame_name << ")" << endl;
  #endif
}

uint8_t ADCP::is_frame_valid(unsigned int node_id, unsigned int frame_id)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: is_frame_valid(" << node_id << "," << frame_id << ")" << endl;
  #endif

  uint8_t current_node_id = 0;
  uint8_t current_frame_id = 0;

  for( int i = 0 ; i < this->frames.size() ; i++ )
  {
	current_node_id  = this->frames[i].node_ID;
    current_frame_id = this->frames[i].frame_ID;

    if( ( current_node_id == node_id ) && ( current_frame_id == frame_id ) )
	{
		return(1);	
	}		
  }

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: is_frame_valid(" << node_id << "," << frame_id << ")" << endl;
  #endif
  
  return(0);
}

uint8_t ADCP::is_frame_valid(string frame_name)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: is_frame_valid(" << frame_name << ")" << endl;
  #endif  

  for(int i = 0 ; i < this->frames.size() ; i++)
  {
	if( frame_name == this->frames[i].frame_name )
	{
		return(1);
	}		
  }

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: is_frame_valid(" << frame_name << ")" << endl;
  #endif  
  
  return(0);
}


void ADCP::write_frame_to_receive_log_file(ADCP::PROCESSED_FRAME frame)
{
	for(int i = 0 ; i < 9 ; i++ )
	{
		this->receiveLog << (unsigned int)frame.byte[i] << ",";
	}
  
	this->receiveLog << (( (double)clock() - (double)this->logStartTime ) * 1000 ) / (double)CLOCKS_PER_SEC << endl;	
}

void ADCP::write_frame_to_transmit_log_file(ADCP::PROCESSED_FRAME frame)
{
	for(int i = 0 ; i < 9 ; i++ )
	{
		this->transmitLog << (unsigned int)frame.byte[i] << ",";
	}
  
	this->transmitLog << (( (double)clock() - (double)this->logStartTime ) * 1000 ) / (double)CLOCKS_PER_SEC << endl;	
}

uint8_t ADCP::receive_frame(unsigned int index,uint32_t timeout_ms)
{
  #ifdef DEBUG	
	(this->debugLog) << "Entering function: receive_frame(" << index << ")" << endl;
  #endif
  
  uint32_t tempFrameData = 0;
  uint8_t  tempNodeID = 0;
  uint8_t  tempFrameID = 0;
  uint8_t  tempPriority = 0;
  uint32_t time_interval_ms = 0;
  clock_t   startTime = clock();
  ADCP::PROCESSED_FRAME tempProcessedFrame = {0};  
  ADCP::RAW_FRAME tempRawFrame = {0};
  ADCP::CONTROL_FRAME tempControlFrame = {0};
  
  do
  {
	/* Read a packet. */  
	  
	receive_packet(&tempProcessedFrame,timeout_ms);

	if( verify_checksum(&tempProcessedFrame) )
    {
		processed_to_raw(&tempProcessedFrame,&tempRawFrame); 

		/* If BIT7 and BIT6 of BYTE0 of raw frame is non-zero, then received frame
		   is a control frame and not a data frame. */

		if((tempRawFrame.byte[0] >> 4) & 0x07)
		{
			#ifdef DEBUG
				(this->debugLog) << "Control frame detected !" << endl;
			#endif
			
			//for(int i = 0 ; i < 9 ; i++ )
			//	tempControlFrame.byte[i] = tempProcessedFrame.byte[i];
			//this->controlFrames.push_back(tempControlFrame);
			
			update_control_frame(tempRawFrame);
			
			continue;
		}

		#ifdef DEBUG
		
		  (this->debugLog) << "Received packet: ";
			
		  for(int i = 0 ; i < 9 ; i++)
			(this->debugLog) << std::hex << "0x" << (unsigned int)tempProcessedFrame.byte[i] << " ";

		(this->debugLog) << " " <<std::dec << endl;
		
		#endif

		for(int i = 0 ; i < 9 ; i++ )
		{
			this->receiveLog << (unsigned int)tempProcessedFrame.byte[i] << ",";
		}
	  
		this->receiveLog << (( (double)clock() - (double)this->logStartTime ) * 1000 ) / (double)CLOCKS_PER_SEC << endl;


		/* Get details regarding received packet. */

		get_DF((void*)&tempFrameData,&tempNodeID,&tempFrameID,&tempPriority,&tempProcessedFrame);

		#ifdef DEBUG
		
		  (this->debugLog) << "Node ID: " << tempNodeID << ", Frame ID: " << tempFrameID << endl;		
		  
		#endif
		
		if( is_frame_valid(tempNodeID, tempFrameID) )
		{

			for(int i = 0 ; this->frames.size() ; i++)
			{
				if( (this->frames[i].node_ID == tempNodeID) && (this->frames[i].frame_ID == tempFrameID) )
				{
					this->frames[i].data = tempFrameData;
					break;
				}
			}
		}
	}
	
  }while( (tempFrameID != get_frame(index)->frame_ID) && (time_interval_ms =  (( (clock()-startTime) * 1000 ) / CLOCKS_PER_SEC) ) <= timeout_ms );
  
  /* If timeout event has not occured, target frame was received successfully. */
  
  if(time_interval_ms < timeout_ms)
  {
	get_frame(index)->data = tempFrameData;  
  }
  
  #ifdef DEBUG
	if(time_interval_ms >= timeout_ms )
	{
		(this->debugLog) << "Timeout occured when reading frame: " << get_frame(index)->frame_name << " [";
		(this->debugLog) << get_frame(index)->frame_ID << "]" << endl;
	}
	else
	{
		(this->debugLog) << "Target frame received within timeout interval: " << get_frame(index)->frame_name << " [";
		(this->debugLog) << get_frame(index)->frame_ID << "]\t" << time_interval_ms << " (ms)" << endl;		
	}
  #endif
  
  #ifdef DEBUG
	(this->debugLog) << "Exiting function: receive_frame(" << index << ")" << endl;
  #endif
  
  return((uint8_t)(time_interval_ms < timeout_ms));
}

uint8_t ADCP::receive_frame(string name,uint32_t timeout_ms)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function receive_frame(" << name << "," << timeout_ms << ")" << endl;
  #endif	

  int index = -1;

  for(int i = 0 ; i < this->frames.size() ; i++ )
  {
	if( this->frames[i].frame_name == name )
		index = i;
  }
  
  uint8_t result = 0;
  
  if(index != -1)
	result = receive_frame(index,timeout_ms);	

  #ifdef DEBUG
	(this->debugLog) << "Exiting function receive_frame(" << name << "," << timeout_ms << ")" << endl;
  #endif

  return(result);  
}

double ADCP::receive_parameter(unsigned int index,uint32_t timeout_ms)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: receive_parameter(" << index << "," << timeout_ms << ")" << endl;
  #endif  
 
  /* Receive owning frame of parameter. */

  uint8_t timeout_result = receive_frame(this->parameters[index].frame_name,timeout_ms);  
  
  /* The previous call will load data into the associated frame struct. */
  /* Calling the next function will read parameter from the updated frame struct. */
  
  double value = this->parameters[index].value;
  
  if(timeout_result)
	value = read_parameter(index);  

  #ifdef DEBUG
	(this->debugLog) << "No timeout occured: " << ( timeout_result ? "PASSSED" : "FAILED" ) << endl;
	(this->debugLog) << "Received value: " << value << endl;
  #endif

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: receive_parameter(" << index << "," << timeout_ms << ")" << endl;
  #endif
  
  return(value);
}


double ADCP::receive_parameter(string name,uint32_t timeout_ms)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: receive_parameter(" << name << "," << timeout_ms << ")" << endl;
  #endif  
 
  double value = 0;
 
   int index = -1;
   
   for(int i = 0 ; i < this->parameters.size() ; i++ )
   {
      if( this->parameters[i].parameter_name == name )
		index = i;	
   }	   
 
 if( index != -1 )
 {
   value = receive_parameter(index,timeout_ms);
 }
 else
 {
	#ifdef DEBUG
		(this->debugLog) << "Target parameter not found !" << endl;
	#endif	
 }
 
  #ifdef DEBUG
	(this->debugLog) << "Exiting function: receive_parameter(" << name << "," << timeout_ms << ")" << endl;
  #endif
  
  return(value);
}

void ADCP::send_control_frame(unsigned int node_id, Frame_Type_t control_frame)
{
	#ifdef DEBUG
		(this->debugLog) << "Entering function: send_control_frame(" << node_id << "," << control_frame << ")" << endl;
	#endif
	
	uint8_t byte[7] = {0,0,0,0,0,0,0};
	
	ADCP::RAW_FRAME       rawFrame;
	ADCP::PROCESSED_FRAME processedFrame;
	
	switch(control_frame)
	{
		case(ADCP::Frame_Type_t::CONNECTION_MANAGEMENT):
		{
			/* Frame header. */
			
			byte[0]	= ( ( 1 << 7 ) | ( 1 << 4 ) | (uint8_t)node_id );
			
			/* Connection Control byte. */
			
			byte[1] = ( ( (this->Connection_Management_Frame.SYNC_RQST)<< 5 )|
					  ( (this->Connection_Management_Frame.SYNC_ACK)   << 4 ) |
					  ( (this->Connection_Management_Frame.OC_RQST)    << 3 ) |
                      ( (this->Connection_Management_Frame.OC_ACK)     << 2 ) |
					  ( (this->Connection_Management_Frame.CC_RQST)    << 1 ) |
					  ( (this->Connection_Management_Frame.CC_ACK)     << 0 ) );

			#ifdef DEBUG
				(this->debugLog) << "Connection_Management.SYNC_RQST = " << this->Connection_Management_Frame.SYNC_RQST << endl;
				(this->debugLog) << "Connection_Management.SYNC_ACK = " << this->Connection_Management_Frame.SYNC_ACK << endl;
				(this->debugLog) << "Connection_Management.OC_RQST = " << this->Connection_Management_Frame.OC_RQST << endl;
				(this->debugLog) << "Connection_Management.OC_ACK = " << this->Connection_Management_Frame.OC_ACK << endl;
				(this->debugLog) << "Connection_Management.CC_RQST = " << this->Connection_Management_Frame.CC_RQST << endl;
				(this->debugLog) << "Connection_Management.CC_ACK = " << this->Connection_Management_Frame.CC_ACK << endl;
			#endif	
		}
		break;
		
		case(ADCP::Frame_Type_t::REMOTE_REQUEST):
		{
			/* Frame header. */
			
			byte[0] = ( ( 1 << 7 ) | ( 2 << 4 ) | (uint8_t)node_id );
			
			byte[1] = ( ( this->Remote_Request_Frame.RMT_REQ << 7 ) |
					    ( this->Remote_Request_Frame.RMT_ACK << 6 ) |
						( this->Remote_Request_Frame.N_FID << 0 ) );
			byte[2] = this->Remote_Request_Frame.FID_0;
			byte[3] = this->Remote_Request_Frame.FID_1;
			byte[4] = this->Remote_Request_Frame.FID_2;
			byte[5] = this->Remote_Request_Frame.FID_3;
			
			#ifdef DEBUG
				(this->debugLog) << "Remote_Request.RMT_REQ = " << this->Remote_Request_Frame.RMT_REQ << endl;
				(this->debugLog) << "Remote_Request.RMT_ACK = " << this->Remote_Request_Frame.RMT_ACK << endl;
				(this->debugLog) << "Remote_Request.N_FID = " << this->Remote_Request_Frame.N_FID << endl;
				(this->debugLog) << "Remote_Request.FID_0 = " << this->Remote_Request_Frame.FID_0 << endl;
				(this->debugLog) << "Remote_Request.FID_1 = " << this->Remote_Request_Frame.FID_1 << endl;
				(this->debugLog) << "Remote_Request.FID_2 = " << this->Remote_Request_Frame.FID_2 << endl;
				(this->debugLog) << "Remote_Request.FID_3 = " << this->Remote_Request_Frame.FID_3 << endl;
			#endif
		}
		break;
		
		case(ADCP::Frame_Type_t::ERROR_STATUS):
		{
			byte[0] = ( ( 1 << 7 ) | ( 3 << 4 ) | (uint8_t)node_id );
			
			byte[1] = ( ( this->Error_Status_Frame.SREQ << 3 ) |
						( this->Error_Status_Frame.SACK << 2 ) |
						( this->Error_Status_Frame.QSTAT << 0 ) );

			byte[2] = this->Error_Status_Frame.QSZ;
			byte[3] = this->Error_Status_Frame.NES;
			byte[4] = (this->Error_Status_Frame.AERR >> 8);
			byte[5] = (this->Error_Status_Frame.AERR & 0xFF);

			#ifdef DEBUG
				(this->debugLog) << "Error_Status.SREQ = " << this->Error_Status_Frame.SREQ << endl;
				(this->debugLog) << "Error_Status.SACK = " << this->Error_Status_Frame.SACK << endl;
				(this->debugLog) << "Error_Status.QSTAT = " << this->Error_Status_Frame.QSTAT << endl;
				(this->debugLog) << "Error_Status.QSZ = " << this->Error_Status_Frame.QSZ << endl;
				(this->debugLog) << "Error_Status.NES = " << this->Error_Status_Frame.NES << endl;
				(this->debugLog) << "Error_Status.AES = " << this->Error_Status_Frame.AERR << endl;	
			#endif	
		}
		break;
	}		

    for(int i = 0 ; i < 7 ; i++)
	{
	  rawFrame.byte[i] = byte[i];
	}
	
	generate_checksum(&rawFrame);
	raw_to_processed(&rawFrame,&processedFrame);
	send_packet(processedFrame);

	#ifdef DEBUG
		(this->debugLog) << "Exiting function: send_control_frame(" << node_id << "," << control_frame << ")" << endl;
	#endif
}


void ADCP::send_control_frame(string node_name, Frame_Type_t control_frame)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function: send_control_frame(" << node_name << "," << control_frame << ")" << endl;	
  #endif

  unsigned int index = -1;
  
  for(int i = 0 ; i < this->nodes.size() ; i++)
  {
	if( this->nodes[i].name == node_name )
	{
		index = i;
	}
  }
  
  if(index == -1)
  {
	#ifdef DEBUG
		(this->debugLog) << "Node not found !" << endl;
	#endif	
  }
  else
  {
	send_control_frame(index,control_frame);  
  }

  #ifdef DEBUG
    (this->debugLog) << "Exiting function: send_control_frame(" << node_name << "," << control_frame << ")" << endl;	
  #endif
}

void ADCP::update_control_frame(ADCP::RAW_FRAME rawFrame)
{
  ADCP::Frame_Type_t control_frame_type = static_cast<ADCP::Frame_Type_t>((rawFrame.byte[0] >> 4)& 0x07 );
  
	switch( control_frame_type )
	{
	  case(ADCP::Frame_Type_t::CONNECTION_MANAGEMENT):
	  {
		this->Connection_Management_Frame.CC_ACK    = (( rawFrame.byte[1] & ( 1 << 0 ) ) >> 0);
		this->Connection_Management_Frame.CC_RQST   = (( rawFrame.byte[1] & ( 1 << 1 ) ) >> 1);
		this->Connection_Management_Frame.OC_ACK    = (( rawFrame.byte[1] & ( 1 << 2 ) ) >> 2);
		this->Connection_Management_Frame.OC_RQST   = (( rawFrame.byte[1] & ( 1 << 3 ) ) >> 3);
		this->Connection_Management_Frame.SYNC_ACK  = (( rawFrame.byte[1] & ( 1 << 4 ) ) >> 4);		
		this->Connection_Management_Frame.SYNC_RQST = (( rawFrame.byte[1] & ( 1 << 5 ) ) >> 5);			
		
		#ifdef DEBUG
			(this->debugLog) << "Received control frame: Connection Management." << endl;
			(this->debugLog) << "Connection_Management_Frame.CC_ACK: " << this->Connection_Management_Frame.CC_ACK << endl;
			(this->debugLog) << "Connection_Management_Frame.CC_RQST: " << this->Connection_Management_Frame.CC_RQST << endl;
			(this->debugLog) << "Connection_Management_Frame.OC_ACK: " << this->Connection_Management_Frame.OC_ACK << endl;
			(this->debugLog) << "Connection_Management_Frame.OC_RQST: " << this->Connection_Management_Frame.OC_RQST << endl;
			(this->debugLog) << "Connection_Management_Frame.SYNC_ACK: " << this->Connection_Management_Frame.SYNC_ACK << endl;
			(this->debugLog) << "Connection_Management_Frame.SYNC_RQST: " << this->Connection_Management_Frame.SYNC_RQST << endl;
		#endif
	  }
	  break;
	
	  case(ADCP::Frame_Type_t::REMOTE_REQUEST):
	  {
		this->Remote_Request_Frame.RMT_REQ = (( rawFrame.byte[1] & ( 1 << 7 ) ) >> 7);
		this->Remote_Request_Frame.RMT_ACK = (( rawFrame.byte[1] & ( 1 << 6 ) ) >> 6);
		this->Remote_Request_Frame.N_FID   = (( rawFrame.byte[1] & ( 0x3F << 0 ) ) >> 0);		
		this->Remote_Request_Frame.FID_0   = rawFrame.byte[2];
		this->Remote_Request_Frame.FID_1   = rawFrame.byte[3];
		this->Remote_Request_Frame.FID_2   = rawFrame.byte[4];
		this->Remote_Request_Frame.FID_3   = rawFrame.byte[5];
		
		#ifdef DEBUG
			(this->debugLog) << "Received control frame: Remote Request." << endl;
			(this->debugLog) << "Remote_Request_Frame.RMT_REQ: " << this->Remote_Request_Frame.RMT_REQ << endl;
			(this->debugLog) << "Remote_Request_Frame.RMT_ACK: " << this->Remote_Request_Frame.RMT_ACK << endl;
			(this->debugLog) << "Remote_Request_Frame.N_FID: " << this->Remote_Request_Frame.N_FID << endl;
			(this->debugLog) << "Remote_Request_Frame.FID_0: " << this->Remote_Request_Frame.FID_0 << endl;
			(this->debugLog) << "Remote_Request_Frame.FID_1: " << this->Remote_Request_Frame.FID_1 << endl;
			(this->debugLog) << "Remote_Request_Frame.FID_2: " << this->Remote_Request_Frame.FID_2 << endl;
			(this->debugLog) << "Remote_Request_Frame.FID_3: " << this->Remote_Request_Frame.FID_3 << endl;
		#endif
	  }
	  break;
	
	  case(ADCP::Frame_Type_t::ERROR_STATUS):
      {
		this->Error_Status_Frame.SREQ   = (( rawFrame.byte[1] & ( 1 << 3 ) ) >> 3);
		this->Error_Status_Frame.SACK   = (( rawFrame.byte[1] & ( 1 << 2 ) ) >> 2);
		this->Error_Status_Frame.QSTAT  = (( rawFrame.byte[1] & ( 0x03 << 0 ) ) >> 0);
		this->Error_Status_Frame.QSZ 	= rawFrame.byte[2];
		this->Error_Status_Frame.NES 	= rawFrame.byte[3];
		this->Error_Status_Frame.AERR 	= (( (uint32_t)rawFrame.byte[4] << 8 ) | (uint32_t)rawFrame.byte[5]);
		
		#ifdef DEBUG
			(this->debugLog) << "Received control frame: Error/Status frame." << endl;
			(this->debugLog) << "Error_Status_Frame.SREQ: "  << this->Error_Status_Frame.SREQ << endl;
			(this->debugLog) << "Error_Status_Frame.SACK: "  << this->Error_Status_Frame.SACK << endl;
			(this->debugLog) << "Error_Status_Frame.QSTAT: " << this->Error_Status_Frame.QSTAT << endl;
			(this->debugLog) << "Error_Status_Frame.QSZ: " << this->Error_Status_Frame.QSZ << endl;
			(this->debugLog) << "Error_Status_Frame.NES: " << this->Error_Status_Frame.NES << endl;
			(this->debugLog) << "Error_Status_Frame.AERR: " << this->Error_Status_Frame.AERR << endl;
		#endif
      }
      break;	  
	}  
}

uint8_t ADCP::read_control_frame(unsigned int node_id, ADCP::Frame_Type_t target_control_frame, uint32_t timeout_ms)
{
  #ifdef DEBUG	
	(this->debugLog) << "Entering function: read_control_frame(" << node_id << "," << target_control_frame << "," << timeout_ms << ")" << endl; 	
  #endif

  ADCP::RAW_FRAME 		rawFrame;	
  ADCP::PROCESSED_FRAME processedFrame;	
  Frame_Type_t control_frame_type = INVALID;
  clock_t startTime = clock();
  uint32_t timeInterval = 0;

  do
  {
	receive_packet(&processedFrame,timeout_ms);  
	processed_to_raw(&processedFrame, &rawFrame);

	control_frame_type = static_cast<ADCP::Frame_Type_t>((rawFrame.byte[0] >> 4)& 0x07 );

    if( control_frame_type != target_control_frame )
	{
		update_control_frame(rawFrame);
	}
	
	if( (unsigned int)control_frame_type == 0 )
	{
	  uint32_t tempFrameData;
	  uint8_t tempNodeID;
      uint8_t tempFrameID;
      uint8_t tempPriority;
      ADCP::PROCESSED_FRAME tempProcessedFrame;	  
		
	  get_DF((void*)&tempFrameData,&tempNodeID,&tempFrameID,&tempPriority,&tempProcessedFrame);

	  if( is_frame_valid(tempNodeID, tempFrameID) )
	  {

		for(int i = 0 ; this->frames.size() ; i++)
		{
		  if( (this->frames[i].node_ID == tempNodeID) && (this->frames[i].frame_ID == tempFrameID) )
		  {
		    this->frames[i].data = tempFrameData;
			break;
		  }
		}
	  }
	  
	}
	
	timeInterval = (uint32_t)(((double)(clock() - startTime) * 1000)/(double)CLOCKS_PER_SEC);
	
  }while( ( control_frame_type != target_control_frame ) && (timeInterval < timeout_ms) );

  /* Select control frame read from device. */

  if( control_frame_type == target_control_frame )
  {
	#ifdef DEBUG
	  (this->debugLog) << "Target control frame received within " << timeInterval << " (ms)." << endl;	
	#endif	
	  
	switch( control_frame_type )
	{
	  case(ADCP::Frame_Type_t::CONNECTION_MANAGEMENT):
	  {
		this->Connection_Management_Frame.CC_ACK    = (( rawFrame.byte[1] & ( 1 << 0 ) ) >> 0);
		this->Connection_Management_Frame.CC_RQST   = (( rawFrame.byte[1] & ( 1 << 1 ) ) >> 1);
		this->Connection_Management_Frame.OC_ACK    = (( rawFrame.byte[1] & ( 1 << 2 ) ) >> 2);
		this->Connection_Management_Frame.OC_RQST   = (( rawFrame.byte[1] & ( 1 << 3 ) ) >> 3);
		this->Connection_Management_Frame.SYNC_ACK  = (( rawFrame.byte[1] & ( 1 << 4 ) ) >> 4);		
		this->Connection_Management_Frame.SYNC_RQST = (( rawFrame.byte[1] & ( 1 << 5 ) ) >> 5);			
		
		#ifdef DEBUG
			(this->debugLog) << "Received control frame: Connection Management." << endl;
			(this->debugLog) << "Connection_Management_Frame.CC_ACK: " << this->Connection_Management_Frame.CC_ACK << endl;
			(this->debugLog) << "Connection_Management_Frame.CC_RQST: " << this->Connection_Management_Frame.CC_RQST << endl;
			(this->debugLog) << "Connection_Management_Frame.OC_ACK: " << this->Connection_Management_Frame.OC_ACK << endl;
			(this->debugLog) << "Connection_Management_Frame.OC_RQST: " << this->Connection_Management_Frame.OC_RQST << endl;
			(this->debugLog) << "Connection_Management_Frame.SYNC_ACK: " << this->Connection_Management_Frame.SYNC_ACK << endl;
			(this->debugLog) << "Connection_Management_Frame.SYNC_RQST: " << this->Connection_Management_Frame.SYNC_RQST << endl;
		#endif
	  }
	  break;
	
	  case(ADCP::Frame_Type_t::REMOTE_REQUEST):
	  {
		this->Remote_Request_Frame.RMT_REQ = (( rawFrame.byte[1] & ( 1 << 7 ) ) >> 7);
		this->Remote_Request_Frame.RMT_ACK = (( rawFrame.byte[1] & ( 1 << 6 ) ) >> 6);
		this->Remote_Request_Frame.N_FID   = (( rawFrame.byte[1] & ( 0x3F << 0 ) ) >> 0);		
		this->Remote_Request_Frame.FID_0   = rawFrame.byte[2];
		this->Remote_Request_Frame.FID_1   = rawFrame.byte[3];
		this->Remote_Request_Frame.FID_2   = rawFrame.byte[4];
		this->Remote_Request_Frame.FID_3   = rawFrame.byte[5];
		
		#ifdef DEBUG
			(this->debugLog) << "Received control frame: Remote Request." << endl;
			(this->debugLog) << "Remote_Request_Frame.RMT_REQ: " << this->Remote_Request_Frame.RMT_REQ << endl;
			(this->debugLog) << "Remote_Request_Frame.RMT_ACK: " << this->Remote_Request_Frame.RMT_ACK << endl;
			(this->debugLog) << "Remote_Request_Frame.N_FID: " << this->Remote_Request_Frame.N_FID << endl;
			(this->debugLog) << "Remote_Request_Frame.FID_0: " << this->Remote_Request_Frame.FID_0 << endl;
			(this->debugLog) << "Remote_Request_Frame.FID_1: " << this->Remote_Request_Frame.FID_1 << endl;
			(this->debugLog) << "Remote_Request_Frame.FID_2: " << this->Remote_Request_Frame.FID_2 << endl;
			(this->debugLog) << "Remote_Request_Frame.FID_3: " << this->Remote_Request_Frame.FID_3 << endl;
		#endif
	  }
	  break;
	
	  case(ADCP::Frame_Type_t::ERROR_STATUS):
      {
		this->Error_Status_Frame.SREQ   = (( rawFrame.byte[1] & ( 1 << 3 ) ) >> 3);
		this->Error_Status_Frame.SACK   = (( rawFrame.byte[1] & ( 1 << 2 ) ) >> 2);
		this->Error_Status_Frame.QSTAT  = (( rawFrame.byte[1] & ( 0x03 << 0 ) ) >> 0);
		this->Error_Status_Frame.QSZ 	= rawFrame.byte[2];
		this->Error_Status_Frame.NES 	= rawFrame.byte[3];
		this->Error_Status_Frame.AERR 	= (( (uint32_t)rawFrame.byte[4] << 8 ) | (uint32_t)rawFrame.byte[5]);
		
		#ifdef DEBUG
			(this->debugLog) << "Received control frame: Error/Status frame." << endl;
			(this->debugLog) << "Error_Status_Frame.SREQ: "  << this->Error_Status_Frame.SREQ << endl;
			(this->debugLog) << "Error_Status_Frame.SACK: "  << this->Error_Status_Frame.SACK << endl;
			(this->debugLog) << "Error_Status_Frame.QSTAT: " << this->Error_Status_Frame.QSTAT << endl;
			(this->debugLog) << "Error_Status_Frame.QSZ: " << this->Error_Status_Frame.QSZ << endl;
			(this->debugLog) << "Error_Status_Frame.NES: " << this->Error_Status_Frame.NES << endl;
			(this->debugLog) << "Error_Status_Frame.AERR: " << this->Error_Status_Frame.AERR << endl;
		#endif
      }
      break;	  
	}
  }
  else
  {
	#ifdef DEBUG
	  (this->debugLog) << "Target control frame not received !" << endl;	
	#endif	
  }
  
  if( timeInterval >= timeout_ms )
  {
	#ifdef DEBUG
	  (this->debugLog) << "Timeout event occured while trying to fetch target control frame." << endl;
	#endif	
  }

  #ifdef DEBUG	
	(this->debugLog) << "Exiting function: read_control_frame(" << node_id << "," << target_control_frame << "," << timeout_ms << ")" << endl; 	
  #endif
  
  return(timeInterval >= timeout_ms);
}


uint8_t ADCP::read_control_frame(string node_name, Frame_Type_t control_frame, uint32_t timeout_ms)
{
  #ifdef DEBUG
    (this->debugLog) << "Entering function: read_control_frame(" << node_name << "," << control_frame << "," << timeout_ms <<  ")" << endl;	
  #endif

  unsigned int index = -1;
  
  for(int i = 0 ; i < this->nodes.size() ; i++)
  {
	if( this->nodes[i].name == node_name )
	{
		index = i;
	}
  }

  uint8_t timeout_status = 1;
  
  if(index == -1)
  {
	#ifdef DEBUG
		(this->debugLog) << "Node not found !" << endl;
	#endif	
  }
  else
  {
	timeout_status = read_control_frame(index,control_frame,timeout_ms);  
  }

  #ifdef DEBUG
    (this->debugLog) << "Exiting function: read_control_frame(" << node_name << "," << control_frame << "," << timeout_ms << ")" << endl;	
  #endif

  return(timeout_status);
}

void ADCP::generate_frame_files(unsigned int node_index)
{
	ofstream source_file;
	ofstream header_file;
	source_file.open("ADCP_frames.c",ios::out);
	header_file.open("ADCP_frames.h",ios::out);
	
	vector<ADCP::FRAME_RECORD> nodeFrames;
	vector<ADCP::PARAMETER_RECORD> nodeParameters;
	
	/* Extract parameters and frames associated with target node. */
	
	for(int i = 0 ; i < this->frames.size() ; i++ )
	{
		if( this->frames[i].node_name == this->nodes[node_index].name )
		{
			nodeFrames.push_back(this->frames[i]);
		}
	}

	for(int i = 0 ; i < this->parameters.size() ; i++ )
	{
		if( this->parameters[i].node_name == this->nodes[node_index].name )
		{
			nodeParameters.push_back(this->parameters[i]);
		}
	}

	/* Generate header file first. */	

    header_file << endl;
	header_file << "#ifndef ADCP_FRAMES_H" << endl;
	header_file << "#define ADCP_FRAMES_H" << endl;
	header_file << endl;
	header_file << "#include <stdint.h>" << endl << endl;;
	header_file << "/* Frame unions */" << endl;
	header_file << endl;
	
	string tempFrameName;
	string tempParameterName;
		
	for(int i = 0 ; i < nodeFrames.size() ; i++)
	{
		header_file << "\n\n";
		
		header_file << "/* Frame name: " << nodeFrames[i].frame_name << " , Frame ID: " << (unsigned int)nodeFrames[i].frame_ID << " */" << endl << endl; 

		for(int j = 0 ; j < nodeParameters.size() ; j++ )
		{
			tempFrameName     = stringUtils::stringAfterMatch(nodeParameters[j].frame_name,"::");
			tempParameterName = stringUtils::stringAfterMatch(stringUtils::stringAfterMatch(nodeParameters[j].parameter_name,"::"),"::");
			
			if( nodeParameters[j].frame_name == nodeFrames[i].frame_name )
			{
				header_file << "\n#ifndef " << tempFrameName << "_FID \t" << endl; 
				header_file << "#define " << tempFrameName << "_FID \t" << (unsigned int)nodeFrames[i].frame_ID << endl; 
				header_file << "#endif" << endl << endl;
				
				if( nodeParameters[j].type == ADCP::Parameter_Type_t::UNSIGNED )					
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_TYPE \t" << "ADCP_PARAMETER_TYPE_UNSIGNED" << endl;

				if( nodeParameters[j].type == ADCP::Parameter_Type_t::SIGNED )					
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_TYPE \t" << "ADCP_PARAMETER_TYPE_SIGNED" << endl;

				if( nodeParameters[j].type == ADCP::Parameter_Type_t::FLOAT32 )					
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_TYPE \t" << "ADCP_PARAMETER_TYPE_FLOAT32" << endl;

				if( nodeParameters[j].type == ADCP::Parameter_Type_t::ENUM )					
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_TYPE \t" << "ADCP_PARAMETER_TYPE_UNSIGNED" << endl;

				if( nodeParameters[j].type == ADCP::Parameter_Type_t::BOOL )					
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_TYPE \t" << "ADCP_PARAMETER_TYPE_UNSIGNED" << endl;
				
				header_file << "#define " << tempFrameName << "_" << tempParameterName << "_START_BIT \t" << (unsigned int)nodeParameters[j].start_bit << endl;
				header_file << "#define " << tempFrameName << "_" << tempParameterName << "_BIT_LENGTH \t" << (unsigned int)nodeParameters[j].bit_length << endl;
				
				float tempFloat;	
					
				if( modf(nodeParameters[j].factor,&tempFloat) != 0.0f )
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_FACTOR \t" << (float)nodeParameters[j].factor << "f" << endl;
				else
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_FACTOR \t" << (float)nodeParameters[j].factor << ".0f" << endl;
				
				if( modf(nodeParameters[j].offset,&tempFloat) != 0.0f )	
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_OFFSET \t" << (float)nodeParameters[j].offset << "f" << endl;
				else
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_OFFSET \t" << (float)nodeParameters[j].offset << ".0f" << endl;
					
				if( modf(nodeParameters[j].minimum,&tempFloat) != 0.0f )				
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_MINIMUM \t" << (float)nodeParameters[j].minimum << "f" << endl;
				else
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_MINIMUM \t" << (float)nodeParameters[j].minimum << ".0f" << endl;
				
				if( modf(nodeParameters[j].maximum,&tempFloat) != 0.0f )
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_MAXIMUM \t" << (float)nodeParameters[j].maximum << "f" << endl;
				else
					header_file << "#define " << tempFrameName << "_" << tempParameterName << "_MAXIMUM \t" << (float)nodeParameters[j].maximum << ".0f" << endl;
			
				header_file << endl;			
			}
		}
		
		header_file << endl << endl;
		
		header_file << "union " << stringUtils::stringAfterMatch(nodeFrames[i].frame_name,"::") << "_union\n{" << endl;

        header_file << "\tuint32_t all;" << endl << endl;
	
		header_file << "\tstruct " << stringUtils::stringAfterMatch(nodeFrames[i].frame_name,"::") << "_segments" << "\n\t{";

		for(int j = 0 ; j < nodeParameters.size() ; j++ )
		{
			if( nodeParameters[j].frame_name == nodeFrames[i].frame_name )
			{
				header_file << "\n\t\t uint32_t " << stringUtils::stringAfterMatch(stringUtils::stringAfterMatch(nodeParameters[j].parameter_name,"::"),"::") << ":" << (unsigned int)nodeParameters[j].bit_length << ";";
			}
		}
		
		header_file << "\n\t}" << stringUtils::stringAfterMatch(nodeFrames[i].frame_name,"::") << "_segments;" << endl << endl;

		header_file << "}" << stringUtils::stringAfterMatch(nodeFrames[i].frame_name,"::") << "_union;" << endl;
		
		header_file << "\nextern union " << stringUtils::stringAfterMatch(nodeFrames[i].frame_name,"::") << "_union " << stringUtils::stringAfterMatch(nodeFrames[i].frame_name,"::") << ";" <<endl;
	}

	

    header_file << endl;
	header_file << "#endif /* ADCP_FRAMES_H */" << endl;


	source_file << endl;
	source_file << "#include \"ADCP_frames.h\"" << endl;
	source_file << endl;

	for(int i = 0 ; i < nodeFrames.size() ; i++)
	{
		source_file << "\n\n";
		
		source_file << "/* Frame name: " << nodeFrames[i].frame_name << " , Frame ID: " << (unsigned int)nodeFrames[i].frame_ID << " */" << endl << endl; 
		
		source_file << endl << endl;
		
		source_file << "union " << stringUtils::stringAfterMatch(nodeFrames[i].frame_name,"::") << "_union " << stringUtils::stringAfterMatch(nodeFrames[i].frame_name,"::") << ";" << endl;
	}

			
	source_file.close();
	header_file.close();
}



uint8_t ADCP::open_node_connection(unsigned int node_id, uint32_t timeout_ms)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: open_node_connection()" << endl;
  #endif  

  this->Connection_Management_Frame.CC_RQST = 0;
  this->Connection_Management_Frame.CC_ACK  = 0;  
  this->Connection_Management_Frame.OC_ACK = 0;	
  this->Connection_Management_Frame.OC_RQST = 1;

  send_control_frame(node_id, ADCP::Frame_Type_t::CONNECTION_MANAGEMENT);
   
  ADCP::Network_Error_Status_t status;
  
  /* If there is an existing connection, the device node does not send connection management frame.
     It only sends an error/status frame. Hence, poll for error/status frame first followed by the
	 connection management frame. */

  #ifdef DEBUG
	(this->debugLog) << "Requesting connection to node " << node_id << endl;
  #endif

  uint8_t timeout_status = 0;
  timeout_status = read_control_frame(node_id, ADCP::Frame_Type_t::CONNECTION_MANAGEMENT,timeout_ms);
  
  if(timeout_status == 1)
  {
	/* If a timeout event has occured, then and error frame is expected from the host. */

	this->network_status = status = (ADCP::Network_Error_Status_t)this->Error_Status_Frame.NES;
	
	#ifdef DEBUG
	  (this->debugLog) << "Request not acknowledged !" << endl;

	  if( status == ADCP::Network_Error_Status_t::ALREADY_OPEN )
	  {
		(this->debugLog) << "Connection is already open at node: " << node_id << endl;
	  }
	  else
	  {
		(this->debugLog) << "Invalid error code received !" << endl;  
	  }
	#endif
  }
  else
  {
	/* Set network error status to none. */

	this->Error_Status_Frame.NES = 0;
	this->network_status = status = ADCP::Network_Error_Status_t::NONE;

	#ifdef DEBUG
	  if( this->Connection_Management_Frame.OC_ACK )
	  {		  
		(this->debugLog) << "Request acknowledged !" << endl;
	  }
	  else
	  {
		(this->debugLog) << "Request not acknowledged !" << endl;
	  }		  
	#endif
  }
  
  #ifdef DEBUG
	(this->debugLog) << "Exiting function: open_node_connection()" << endl;
  #endif

  return(this->Connection_Management_Frame.OC_ACK);
}


uint8_t ADCP::open_node_connection(unsigned int node_id,uint32_t timeout_ms,uint32_t tries)
{
	#ifdef DEBUG
		(this->debugLog) << "Entering function: open_node_connection(" << node_id << "," << timeout_ms << "," << tries << ")" << endl;
 	#endif

	uint32_t count = 0;
	uint8_t result = 0;
	
	for( count = 0 ; count < tries ; count++ )
	{
		if( result = open_node_connection(node_id,timeout_ms) )
		{
			break;
		}
	}

	#ifdef DEBUG
		if(result)
		{
			(this->debugLog) << "Opened connection successfully ! (Number of tries: " << count << ")" << endl;
		}
		else
		{
			(this->debugLog) << "Open connection not successful." << endl;
		}
	#endif

	#ifdef DEBUG
		(this->debugLog) << "Exiting function: open_node_connection(" << node_id << "," << timeout_ms << "," << tries << ")" << endl;
 	#endif
	
	return(result);
}


uint8_t ADCP::close_node_connection(unsigned int node_id, uint32_t timeout_ms)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: close_node_connection()" << endl;
  #endif 


  this->Connection_Management_Frame.OC_RQST = 0;
  this->Connection_Management_Frame.OC_ACK  = 0;  
  this->Connection_Management_Frame.CC_RQST = 1;
  this->Connection_Management_Frame.CC_ACK = 0;

  send_control_frame(node_id, ADCP::Frame_Type_t::CONNECTION_MANAGEMENT);
   
  ADCP::Network_Error_Status_t status;
  
  /* If there is an existing connection, the device node does not send connection management frame.
     It only sends an error/status frame. Hence, poll for error/status frame first followed by the
	 connection management frame. */

  #ifdef DEBUG
	(this->debugLog) << "Requesting connection to node " << node_id << endl;
  #endif

  uint8_t timeout_status = 0;
  timeout_status = read_control_frame(node_id, ADCP::Frame_Type_t::CONNECTION_MANAGEMENT,timeout_ms);
  
  if(timeout_status == 1)
  {
	/* If a timeout event has occured, then and error frame is expected from the host. */

	this->network_status = status = (ADCP::Network_Error_Status_t)this->Error_Status_Frame.NES;
	
	#ifdef DEBUG
	  (this->debugLog) << "Request not acknowledged !" << endl;

	  if( status == ADCP::Network_Error_Status_t::ALREADY_OPEN )
	  {
		(this->debugLog) << "Connection is already open at node: " << node_id << endl;
	  }
	  else
	  {
		(this->debugLog) << "Invalid error code received !" << endl;  
	  }
	#endif
  }
  else
  {
	/* Set network error status to none. */

	this->Error_Status_Frame.NES = 0;
	this->network_status = status = ADCP::Network_Error_Status_t::NONE;

	#ifdef DEBUG
	  if( this->Connection_Management_Frame.CC_ACK )
	  {		  
     	  (this->debugLog) << "Request acknowledged !" << endl;
	  }		  
	  else if( this->Connection_Management_Frame.CC_ACK == 0 )
	  {		  
     	  (this->debugLog) << "Request not acknowledged !" << endl;
	  }
	  
	#endif
  }
    
  #ifdef DEBUG
	(this->debugLog) << "Exiting function: close_node_connection()" << endl;
  #endif

  return(this->Connection_Management_Frame.CC_ACK);
}

uint8_t ADCP::close_node_connection(unsigned int node_id,uint32_t timeout_ms,uint32_t tries)
{
	#ifdef DEBUG
		(this->debugLog) << "Entering function: close_node_connection(" << node_id << "," << timeout_ms << "," << tries << ")" << endl;
	#endif	

	uint32_t count = 0;
	uint8_t result = 0;
	
	
	for( count = 0 ; count < tries ; count++ )
	{
		if( result = close_node_connection(node_id,timeout_ms) )
		{
			break;
		}
	}

	#ifdef DEBUG
		if( result )
		{
			(this->debugLog) << "Connection closed successfully ! (Number of tries: " << count << ")" << endl;
		}
		else
		{
			(this->debugLog) << "Connection not closed successfully" << endl;
		}
	#endif

	#ifdef DEBUG
		(this->debugLog) << "Exiting function: close_node_connection(" << node_id << "," << timeout_ms << "," << tries << ")" << endl;
	#endif
	
	return(result);
}

uint8_t ADCP::remote_frame_request(unsigned int node_id,vector<string> frames, uint32_t timeout_ms)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: remote_frame_request()" << endl;
  #endif
  
  vector<unsigned int> frame_IDs;
  vector<string> frame_names;
  unsigned int count = 0;
  
  for(int i = 0 ; i < frames.size() ; i++)
  {
	if( get_frame( frames[i] )->node_ID == node_id )
	{
		frame_IDs.push_back( get_frame( frames[i] )->frame_ID );
		frame_names.push_back( get_frame( frames[i] )->frame_name );
		count++;
		
		#ifdef DEBUG
			(this->debugLog) << "Remote request frame found: " << get_frame( frames[i] )->frame_name << " [" << (unsigned int)get_frame( frames[i] )->frame_ID << "]" <<endl;
		#endif
	}
  }
  
  #ifdef DEBUG
	(this->debugLog) << "Number of frames to be transmitted: " << count << endl;
  #endif
  
  #ifdef DEBUG
	(this->debugLog) << "Frame IDs to be requested from target node:\t" << endl;
	
	for( int i = 0 ; i < frame_IDs.size() ; i++)
	{
		(this->debugLog) << frame_IDs[i] << " ";
	}
	
	(this->debugLog) << endl;
  #endif
  
  if( frame_IDs.size() > 4 )
  {
	#ifdef DEBUG
		(this->debugLog) << "Number of frames requested > 4. Discarding frames after 4th frame." << endl; 
    #endif	
	  
	frame_IDs.resize(4);
  }
  
  if( frame_IDs.size() != 0 )
  {
	this->Remote_Request_Frame.N_FID = frame_IDs.size();

	if( frame_IDs.size() == 1 )
	{
		this->Remote_Request_Frame.FID_0 = frame_IDs.at(0);
		this->Remote_Request_Frame.FID_1 = 0;
		this->Remote_Request_Frame.FID_2 = 0;
		this->Remote_Request_Frame.FID_3 = 0;
	}
	
	if( frame_IDs.size() == 2 )
	{
		this->Remote_Request_Frame.FID_0 = frame_IDs.at(0);
		this->Remote_Request_Frame.FID_1 = frame_IDs.at(1);
		this->Remote_Request_Frame.FID_2 = 0;
		this->Remote_Request_Frame.FID_3 = 0;
	}
	
	if( frame_IDs.size() == 3 )
	{
		this->Remote_Request_Frame.FID_0 = frame_IDs.at(0);
		this->Remote_Request_Frame.FID_1 = frame_IDs.at(1);
		this->Remote_Request_Frame.FID_2 = frame_IDs.at(2);
		this->Remote_Request_Frame.FID_3 = 0;
	}

	if( frame_IDs.size() == 4 )
	{
		this->Remote_Request_Frame.FID_0 = frame_IDs.at(0);
		this->Remote_Request_Frame.FID_1 = frame_IDs.at(1);
		this->Remote_Request_Frame.FID_2 = frame_IDs.at(2);
		this->Remote_Request_Frame.FID_3 = frame_IDs.at(3);
	}
	
	this->Remote_Request_Frame.RMT_ACK = 0;
	this->Remote_Request_Frame.RMT_REQ = 1;
	
	send_control_frame(node_id,ADCP::Frame_Type_t::REMOTE_REQUEST);
	
	uint8_t timeout_status = 0;
	timeout_status = read_control_frame(node_id,ADCP::Frame_Type_t::REMOTE_REQUEST,timeout_ms);
	
    if( !timeout_status )
	{
		if( this->Remote_Request_Frame.RMT_ACK )
		{
			//this->Remote_Request_Frame.RMT_ACK = 0;
			this->Remote_Request_Frame.RMT_REQ = 0;
			
							
							
			for(int i = 0 ; i < frame_IDs.size() ; i++)
			{
				cout << frame_names[i] << endl;
				
				timeout_status = receive_frame( frame_names[i], timeout_ms );
				
				#ifdef DEBUG
					if(timeout_status == 1)
						(this->debugLog) << "Remote Request Frame received from target node !" << endl;
					else
						(this->debugLog) << "Remote Request Frame NOT received from target node !" << endl;
				#endif
				
				if( timeout_status == 0 )
				{
					return(0);		
				}
			}
		}
	}
	else
	{

		return(0);
	}		
  }
  
  frame_IDs.clear();
  
  #ifdef DEBUG
	(this->debugLog) << "Exiting function: remote_frame_request()" << endl;
  #endif
  
  return(1);
}

uint8_t ADCP::get_error_and_status(unsigned int node_id,uint32_t timeout_ms)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: get_error_and_status()" << endl;
  #endif  
	
  uint8_t timeout_status = 0;

  this->Error_Status_Frame.SACK = 0;
  this->Error_Status_Frame.SREQ = 1;
  
  send_control_frame(node_id,ADCP::Frame_Type_t::ERROR_STATUS);
  timeout_status = read_control_frame(node_id,ADCP::Frame_Type_t::ERROR_STATUS,timeout_ms);	
  
  if(timeout_status)
  {
	this->Error_Status_Frame.SACK = 0;  
  }
  
  #ifdef DEBUG
  
	if(timeout_status)
	{
		(this->debugLog) << "Remote request timeout event occured !" << endl;
	}
	else
	{
		(this->debugLog) << "Network status: ";
		
		switch((ADCP::Network_Error_Status_t)this->Error_Status_Frame.NES)
		{
			case(ADCP::Network_Error_Status_t::NONE):
			{
				(this->debugLog) << "None." << endl;
			}
			break;
			
			case(ADCP::Network_Error_Status_t::ALREADY_OPEN):
			{
				(this->debugLog) << "Already open." << endl;
			}
			break;

			case(ADCP::Network_Error_Status_t::ALREADY_CLOSED):
			{
				(this->debugLog) << "Already closed." << endl;
			}
			break;
			
			case(ADCP::Network_Error_Status_t::INVALID_FRAME_REQUESTED):
			{
				(this->debugLog) << "Invalid frame requested." << endl;
			}
			break;
		}
		
		(this->debugLog) << "Node receive queue element count: " << (this->Error_Status_Frame.QSZ) << endl;
		(this->debugLog) << "Device node queue status: ";
		
		if( this->Error_Status_Frame.QSTAT == 0 )
		{
			(this->debugLog) << "QUEUE_EMPTY." << endl;
		}

		if( this->Error_Status_Frame.QSTAT == 1 )
		{
			(this->debugLog) << "QUEUE_OK." << endl;
		}
		
		if( this->Error_Status_Frame.QSTAT == 2 )
		{
			(this->debugLog) << "QUEUE_FULL." << endl;
		}
		
		(this->debugLog) << "Application error code: " << this->Error_Status_Frame.AERR << endl;
	}
  
	(this->debugLog) << "Exiting function: get_error_and_status()" << endl;
  #endif  
   
  return((timeout_status == 0) && (this->Error_Status_Frame.SACK == 1));
}
	
ADCP::Network_Error_Status_t ADCP::get_node_network_error_status(uint8_t node_id,uint32_t timeout_ms)
{
	#ifdef DEBUG
		(this->debugLog) << "Entering function: get_node_network_error_status(" << node_id << ")" << endl;
	#endif
	
	uint8_t timeout_status = get_error_and_status(node_id,timeout_ms);
	ADCP::Network_Error_Status_t error_status;

	if( timeout_status == 0 )
	{
		error_status = ADCP::Network_Error_Status_t::TIMEOUT;
	}
	else
	{
		error_status = (ADCP::Network_Error_Status_t)this->Error_Status_Frame.NES;
	}

	#ifdef DEBUG
		(this->debugLog) << "Network status: ";

		if(error_status == ADCP::Network_Error_Status_t::NONE)
		{
			(this->debugLog) << "NONE" << endl;
		}
		if(error_status == ADCP::Network_Error_Status_t::ALREADY_OPEN)
		{
			(this->debugLog) << "ALREADY OPEN" << endl;
		}
		if(error_status == ADCP::Network_Error_Status_t::ALREADY_CLOSED)
		{
			(this->debugLog) << "ALREADY CLOSED" << endl;
		}
		if(error_status == ADCP::Network_Error_Status_t::TIMEOUT)
		{
			(this->debugLog) << "TIMEOUT" << endl;
		}
	#endif

	#ifdef DEBUG
		(this->debugLog) << "Exiting function: get_node_network_error_status(" << node_id << ")" << endl;
	#endif
	
	return(error_status);
}

uint8_t ADCP::get_node_receive_queue_count(uint8_t node_id,uint32_t timeout_ms)	
{
	#ifdef DEBUG
		(this->debugLog) << "Entering function: get_node_receive_queue_count(" << node_id << "," << timeout_ms << ")" << endl;
	#endif

	uint8_t timeout_status = get_error_and_status(node_id,timeout_ms);		

	#ifdef DEBUG
		if( timeout_status == 0 )
			(this->debugLog) << "Number of elements in target node receiver queue: " << this->Error_Status_Frame.QSZ << endl;
		else
			(this->debugLog) << "Timeout event occured !" << endl;
	#endif

	#ifdef DEBUG
		(this->debugLog) << "Exiting function: get_node_receive_queue_count(" << node_id << "," << timeout_ms << ")" << endl;
	#endif
	
	return(this->Error_Status_Frame.QSZ);
}

uint16_t ADCP::get_node_application_error_code(uint8_t node_id,uint32_t timeout_ms)
{
	#ifdef DEBUG
		(this->debugLog) << "Entering function: get_node_application_error_code(" << node_id << "," << timeout_ms << ")" << endl; 
	#endif

	uint8_t timeout_status = get_error_and_status(node_id,timeout_ms);
	
	#ifdef DEBUG
		(this->debugLog) << "Application error code: " << this->Error_Status_Frame.AERR << endl;
	#endif

	#ifdef DEBUG
		(this->debugLog) << "Exiting function: get_node_application_error_code(" << node_id << "," << timeout_ms << ")" << endl; 
	#endif
	
	return(this->Error_Status_Frame.AERR);
}

uint8_t ADCP::reset_node(uint8_t node_id, uint32_t timeout_ms)
{
	#ifdef DEBUG
		(this->debugLog) << "Entering function: reset_mode(" << node_id << "," << timeout_ms << ")" << endl;
	#endif

	this->Connection_Management_Frame.SYNC_RQST = 1;
	this->Connection_Management_Frame.SYNC_ACK  = 0;
	
	send_control_frame(node_id,ADCP::Frame_Type_t::CONNECTION_MANAGEMENT);
	uint8_t timeout_status = read_control_frame(node_id,ADCP::Frame_Type_t::CONNECTION_MANAGEMENT,timeout_ms);

	if(timeout_status)
	{
		this->Connection_Management_Frame.SYNC_RQST = 0;
		this->Connection_Management_Frame.SYNC_ACK  = 0;
		
		#ifdef DEBUG
			(this->debugLog) << "Timeout event occured." << endl;
		#endif
	}
	else
	{
		if( this->Connection_Management_Frame.SYNC_ACK )
		{
			this->Connection_Management_Frame.SYNC_RQST = 0;
			
			#ifdef DEBUG
				(this->debugLog) << "Acknowledgement received from target node" << endl;	
			#endif
		}
		else
		{
			this->Connection_Management_Frame.SYNC_RQST = 0;

			#ifdef DEBUG
				(this->debugLog) << "Acknowledgement NOT received from target node" << endl;	
			#endif			
		}
	}

	#ifdef DEBUG
		(this->debugLog) << "Exiting function: reset_mode(" << node_id << "," << timeout_ms << ")" << endl;
	#endif
	
	return( ( timeout_status == 0 ) && ( this->Connection_Management_Frame.SYNC_ACK ) );
}

uint8_t ADCP::reset_node(uint8_t node_id, uint32_t timeout_ms,uint32_t tries)
{
	#ifdef DEBUG
		(this->debugLog) << "Entering function: reset_node(" << node_id << "," << timeout_ms << "," << tries << ")" << endl;
	#endif

	uint32_t count = 0;
	uint8_t result = 0;
	
	for( count = 0 ; count < tries ; count++ )
	{
		if( result = reset_node(node_id,timeout_ms) )
		{
			break;
		}
	}
	
	#ifdef DEBUG
		if( result )
		{
			(this->debugLog) << "Node reset successful ! (Number of tries: " << result << ")" << endl;
		}
		else
		{
			(this->debugLog) << "Node reset not successful" << endl;
		}
	#endif

	#ifdef DEBUG
		(this->debugLog) << "Exiting function: reset_node(" << node_id << "," << timeout_ms << "," << tries << ")" << endl;
	#endif
}


ADCP::PARAMETER_SAMPLES ADCP::monitor_parameter(string parameter_name, uint32_t monitoring_time_ms)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: monitor_parameter(" << parameter_name << "," << monitoring_time_ms << ")" << endl;
  #endif

  clock_t start_time = clock();
  
  double current_time = 0;
  double interval_ms = 0;
  
  ADCP::PARAMETER_SAMPLES parameter_samples;
  double value;
  double percentage = 0;
  uint32_t last_receive_time = 0;
  
  string s;
  char buffer[256];
  const unsigned int n_positions = 50;

  unsigned int count = 0;

  
  serial_port_flush(NULL);

  parameter_samples.name = parameter_name;	
  
  do
  {
	value = receive_parameter(parameter_name,monitoring_time_ms);

	
	current_time = ( 1000 * ( ( (double)(clock() - start_time) ) / (double)CLOCKS_PER_SEC ) );
	interval_ms  = (current_time - last_receive_time);
    last_receive_time = current_time;

    parameter_samples.values.push_back(value);
	parameter_samples.times_ms.push_back(current_time);

	
    //cout << current_time << endl;

	percentage = (double)current_time / (double)monitoring_time_ms;	

	if( count != 0 )
	{
		cout << "\r";
		cout << "                                                 ";
		cout << "                                                 ";
		cout << "                                                 ";
		cout << "\r";
	}

	cout << "[";
	
	for(int i = 0 ; i < n_positions ; i++)
	{
		if( i < (int)(n_positions * percentage) )
		{
			cout << "*";
		}
		else
		{
			cout << " ";
		}
	}
	
	cout << "]";
	cout << " time = " << current_time << " (ms)  " << " receive interval: " << interval_ms << " (ms)" << " value = " << value; 
	cout << "\r";
		
	count++;	
		
  }while( current_time < (double)monitoring_time_ms );	  

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: monitor_parameter(" << parameter_name << "," << monitoring_time_ms << ")" << endl;
  #endif
  
  return(parameter_samples);
}

vector<ADCP::PARAMETER_SAMPLES> ADCP::monitor_parameters(vector<string> parameter_names, uint32_t monitoring_time_ms)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: monitor_parameters()" << endl;
  #endif

  serial_port_flush(NULL);
  
  uint32_t startTime = (uint32_t)clock();
  uint32_t currentTime = 0;
  uint32_t monTime = (uint32_t)(( (double)1000 * (double)monitoring_time_ms ) / (double)CLOCKS_PER_SEC); 

  ADCP::RAW_FRAME 		rawPacket;
  ADCP::PROCESSED_FRAME	processedPacket;
  
  vector<ADCP::PARAMETER_SAMPLES> samples( parameter_names.size() );

  for(int i = 0 ; i < samples.size() ; i++)
  {
	samples[i].name = parameter_names[i];  
  }


  while(currentTime < monTime)
  {
	receive_packet(&processedPacket);
	processed_to_raw(&processedPacket,&rawPacket);

	currentTime = ((uint32_t)clock() - startTime);

	
	/* If control frame is received, update control frame. */
	
	if( (rawPacket.byte[0] >> 4) && 0x07 )		
	{
		update_control_frame(rawPacket);
	}
	else	/* If data packet is received. */
	{
		unsigned int tempNodeID = (rawPacket.byte[0] & 0x07);
		unsigned int tempFrameID = rawPacket.byte[1];
		
		if( is_frame_valid(tempNodeID,tempFrameID) )
		{
			ADCP::PROCESSED_FRAME processedFrame;

			raw_to_processed(&rawPacket,&processedFrame);
			
			write_frame_to_receive_log_file(processedFrame);	
		}
		
		uint32_t tempFrameData = 0;
		
		tempFrameData |= ( (uint32_t)(rawPacket.byte[2]) << 24 );
		tempFrameData |= ( (uint32_t)(rawPacket.byte[3]) << 16 );
		tempFrameData |= ( (uint32_t)(rawPacket.byte[4]) << 8 );
		tempFrameData |= ( (uint32_t)(rawPacket.byte[5]) << 0 );
		
		/* Update received data packet's structure. */
		
		if( is_frame_valid(tempNodeID, tempFrameID) )
		{
			for(int i = 0 ; this->frames.size() ; i++)
			{
				if( (this->frames[i].node_ID == tempNodeID) && (this->frames[i].frame_ID == tempFrameID) )
				{
					this->frames[i].data = tempFrameData;
					break;
				}
			}
		}	

		/* Loop through parameters to be monitored. */

		cout << "\r";
		cout << "                                                                          ";
		cout << "                                                                          ";
		cout << "\r";

		for(int i = 0 ; i < parameter_names.size() ; i++)
		{
			if( get_parameter(parameter_names[i])->frame_ID == tempFrameID )
			{
				double _value;
				samples[i].values.push_back( _value = read_parameter(parameter_names[i]) );
				samples[i].times_ms.push_back( currentTime );
	
	
				cout << parameter_names[i] << " : " << _value << " || ";	
			}
		}
		
	}
  }	  

  #ifdef DEBUG
	(this->debugLog) << "Exiting function: monitor_parameters()" << endl;
  #endif
  
  return(samples);
}

uint32_t ADCP::get_cycle_time(string frame_name,uint32_t timeout_ms)
{
  #ifdef DEBUG
	(this->debugLog) << "Entering function: get_cycle_time(" << frame_name <<  ")" << endl;
  #endif

  serial_port_flush(NULL);
  serial_port_flush(NULL);
  
  clock_t start_time = 0;
  clock_t interval = 0;
  uint32_t interval_ms = 0;

  for(int i = 0 ; i < 10 ; i++)
  {
	  //serial_port_flush(NULL);
	  start_time = clock();    
	  receive_frame(frame_name,timeout_ms);
	  interval = clock() - start_time;
	  interval_ms += (uint32_t)( ((double)interval * (double)1000)/(double)CLOCKS_PER_SEC );  
  }
  
  interval_ms /= 10;

  #ifdef DEBUG
    (this->debugLog) << "Cycle time of frame " << frame_name << ": " << interval_ms << " (ms)" << endl;
	(this->debugLog) << "Exiting function: get_cycle_time(" << frame_name <<  ")" << endl;
  #endif
  
  return(interval_ms);
}	
