
#include "ADCP.hpp"
#include "cli.hpp"
#include <windows.h>

ADCP* adcp;			//	Session object.

CLI_PROMPT CLI_PROMPTS[PROMPT_COUNT];
using namespace ADCP_CLI;

#define CLI_DEBUG "DEBUG >>  "

/* Entry function. */

void cli_init()
{
  adcp = new ADCP(); 
  adcp->print_API_info();
  cout << "\n";
  
  CLI_PROMPTS[READ_FILE].command = "";
  //CLI_PROMPTS[READ_FILE].function = &(ADCP_CLI::read_file);  
}

void cli_help()
{
	cout << "	Application Debug and Control Protocol 			" << endl;
	cout << endl << endl;
	cout << "Command List" << endl << endl << endl;
	
	cout << setw(10) << "readfile --filename=[symbol_file_name]: \r\n Read parameters & data packets (frames) from .csv file" << endl << endl;
	cout << setw(10) << "delay --time=[delay_time_ms]: \r\n Generate delay in milli-seconds" << endl << endl;
	cout << setw(10) << "print_parameter_list: \r\n Prints all available parameters in loaded symbol file" << endl << endl;
	cout << setw(10) << "print_frame_list: \r\n Prints all available frames/data packets in loaded symbol file" << endl << endl;
	cout << setw(10) << "print_parameter_details --index=[parameter_index]: \r\n Print details of parameter addressed by number" << endl << endl;
	cout << setw(10) << "print_parameter_details --name=[parameter_name]: \r\n Print details of parameter addressed by name" << endl << endl;
	cout << setw(10) << "print_frame_details --index=[frame_index]: \r\n Print details of frame addressed by number" << endl << endl;
	cout << setw(10) << "print_frame_details --name=[frame_name]: \r\n Print details of parameter addressed by name" << endl << endl;
	cout << setw(10) << "list_serial_ports: \r\n Lists all available serial port" << endl << endl;
	cout << setw(10) << "close_node_connection --node_id=[target_node_id] --timeout=[timeout_ms]: \r\n Closes a connection with target node" << endl << endl;
	cout << setw(10) << "open_node_connection --node_name=[target_node_name] --timeout=[timeout_ms]:  \r\n Opens a connection with a target node" << endl << endl;
	cout << setw(10) << "send_dataframe --frame_name=[frame_name]: \r\n Sends target data frame to its owning device" << endl << endl;
	cout << setw(10) << "receive_dataframe --frame_name=[frame_name] --timeout=[timeout_ms]: \r\n Receives target data frame/packet from target node" << endl << endl;
	cout << setw(10) << "open_serial_port --com_port=[com_port] --baud_rate=[baud_rate]: \r\n Opens a serial port session." << endl << endl;
	cout << setw(10) << "close_serial_port" << endl << endl;
	cout << setw(10) << "reset_node --node_id=[node_id] --timeout=[timeout_ms]:  \r\n Resets connection with target device node" << endl << endl;
	cout << setw(10) << "node_queue_status --node_id=[node_id] --timeout=[timeout_ms]:  \r\n  Gets the status of the device/node receiver queue" << endl << endl;
	cout << setw(10) << "node_queue_status --node_name=[node_name] --timeout=[timeout_ms]:   \r\n Gets the status of the device/node receiver queue" << endl << endl;
	cout << setw(10) << "node_queue_element_count --node_id=[node_id] --timeout=[timeout_ms]:   \r\n Gets the number of elements in the node receiver queue" << endl << endl;
	cout << setw(10) << "node_queue_element_count --node_name=[node_name] --timeout=[timeout_ms]:  \r\n  Gets the number of elements in the node receiver queue" << endl << endl;
	cout << setw(10) << "node_network_error_status --node_id=[node_id] --timeout=[timeout_ms]:  \r\n Gets the network error status from the device" << endl << endl;
	cout << setw(10) << "node_network_error_status --node_name=[node_name] --timeout=[timeout_ms]:  \r\n Gets the network error status from the device" << endl << endl;
	cout << setw(10) << "node_application_error_code --node_id=[node_id] --timeout=[timeout_ms]:  \r\n Gets last reported application error code from the device" << endl << endl;
	cout << setw(10) << "node_application_error_code --node_name=[node_name] --timeout=[timeout_ms]:  \r\n Gets last reported application error code from the device" << endl << endl;
	cout << setw(10) << "remote_request --node_id=[node_id] --frames=[frame_names] --timeout=[timeout_ms]:  \r\n Request data packet from target node" << endl << endl;
	cout << setw(10) << "read_parameter	--name=[parameter_name]:  \r\n Reads the target parameter value from its owning data packet" << endl << endl;
	cout << setw(10) << "write_parameter --name=[parameter_name] --value=[parameter_value]:  \r\n Writes the value to its owning data packet" << endl << endl;
	cout << setw(10) << "transmit_parameter --name=[parameter_name] --=value=[parameter_value]:  \r\n Writes value into owning data packet and then transmits it" << endl << endl;
	cout << setw(10) << "receive_parameter --name=[parameter_name]:  \r\n Read its owning data packet from the serial port and then updates the parameter value" << endl << endl;
	cout << endl << endl;
 }

void cli_script(string filename)
{
  string prompt = "";
  string cmd = "";
  vector<string> params;
  vector<string> values;
  
  ifstream file(filename);
  
  /* Command syntax is as follows. 
  *
  *  [command] --[parameter_1]=[value_1] --[parameter_2]=[value_2] ..... --[parameter_n]=[value_n]
  *
  */
  
  while(getline( file, prompt ))
  {
	//cout << ">>  ";  
	  
	//getline( file, prompt );	
		
    //cout << CLI_DEBUG << "Input prompt: " << prompt << endl;
	
	cout << endl;
	cout << ">> " << prompt << endl;
	cout << endl;
	
	cmd = get_cmd(prompt);
	params = get_params(prompt);
	values = get_values(prompt);
	
	cli_interpreter( cmd, params, values );	
  }
	
  cout << endl << "Exiting application....." << endl;		
}	

void cli()
{
  string prompt = "";
  string cmd = "";
  vector<string> params;
  vector<string> values;
  
  /* Command syntax is as follows. 
  *
  *  [command] --[parameter_1]=[value_1] --[parameter_2]=[value_2] ..... --[parameter_n]=[value_n]
  *
  */
  
  do
  {
	cout << ">>  ";  
	  
	getline( std::cin, prompt );	
		
    //cout << CLI_DEBUG << "Input prompt: " << prompt << endl;
	
	cmd = get_cmd(prompt);
	params = get_params(prompt);
	values = get_values(prompt);
	
	cli_interpreter( cmd, params, values );
	
  }while( prompt != "exit" );
	
  cout << endl << "Exiting application....." << endl;	
}

string get_cmd(const string& in)
{
  string cmd = "";	
	
  /* If prompt does not contain any parameters. */	
	
  if( stringUtils::charIndex(in,' ') == -1 )
  {
    cmd = in;
  }
 else  
 {
    cmd = stringUtils::stringBeforeMatch(in," ");
 }	 
 
 //cout << CLI_DEBUG << "Received command: " << cmd << endl;
  
 return(cmd);
}

std::vector<string> get_params(const string& in)
{
  string temp = in;
  int paramCount = 0;
  string paramName;
  vector<string> names;	

  do
  {
    temp = stringUtils::stringAfterMatch(temp,"--");
	
	//cout << "After match[" << paramCount << "]:  " << temp << endl;
	
	paramName = stringUtils::stringBeforeMatch(temp,"=");

	//cout << "Before match[" << paramCount << "]:  " << paramName << endl;
	
	names.push_back(paramName);
	
    //cout << "Parameter [" << paramCount << "]: " << paramName << endl;
	paramCount++;
	
  }while(stringUtils::searchSubstring(temp,"--") != -1);
  
  return(names);
}

std::vector<string> get_values(const string& in)
{
  string temp = in;
  int paramCount = 0;
  string paramName;
  vector<string> names;	

  do
  {
    temp = stringUtils::stringAfterMatch(temp,"=");
	
	//cout << "After match[" << paramCount << "]:  " << temp << endl;
	
	if( stringUtils::searchSubstring(temp," ") != -1 )
	{
	  paramName = stringUtils::stringBeforeMatch(temp," ");
	  
	  //cout << "Before match[" << paramCount << "]:  " << paramName << endl;
	
	  names.push_back(paramName);
	
      //cout << "Parameter [" << paramCount << "]: " << paramName << endl;
	}
	else
	{
	  paramName = stringUtils::stringBeforeMatch(temp,"\n");
	  
	  //cout << "Before match[" << paramCount << "]:  " << paramName << endl;
	
	  names.push_back(paramName);
	
      //cout << "Value [" << paramCount << "]: " << paramName << endl;		
	}
	paramCount++;
	
  }while(stringUtils::searchSubstring(temp,"=") != -1);
  
  return(names);
}

static void error_beep()
{
	Beep(1250,50);	
	Sleep(10);
	Beep(1250/2,50);
	Sleep(10);	
}

string remove_quotes(string s)
{
	s.erase(0,1);
	s.erase(s.length()-1,1);
	return(s);
}

void cli_interpreter(string cmd, std::vector<string> parameter, std::vector<string> value)
{
  if( cmd == "help" )
  {
	cli_help();
  }
  /* Read file command. */		
  /* Command syntax: readfile --filename="parameter_file_name.csv"	*/		
  else if(cmd == "rf" || cmd == "readfile" || cmd == "RF" || cmd == "Readfile")	
  {
	  try
	  {
		if( parameter.at(0) == "filename" )
		{			
			string file_name = value.at(0);

			file_name.erase(file_name.length()-1,1);  
			file_name.erase(0,1);  	
			
			cout << "Reading file: " << file_name << endl;	
			
			adcp->read_file(file_name.c_str());
		}
		else
		{
			cout << "File name not provided !" << endl;
		}
	  }
	  catch(...)
	  {
		  cout << "Invalid file provided / file not found !" << endl;
	  }
  }
  //	Command to provide delay in milli-seconds
  //	SYNTAX: delay --time=[time_value]
  // 	@parameter time_value		:	Delay time in milli-seconds.  
  else if( cmd == "delay" ) 
  {
	try
	{	
		if( parameter.at(0) == "time" )
		{		
			cout << "Waiting for " << std::atoi(value[0].c_str()) << " (ms)" << endl;
			Sleep( std::atoi(value[0].c_str()) );
		}
		else
		{
			cout << "Invalid parameter provided for command: delay" << endl;
			cout << "Syntax of command is: delay --time=[time_delay]" << endl;
		}
	}
	catch(...)
	{
		cout << "No time parameter provided for command: delay" << endl;
	}		
  }
  //	Command to list the available parameters.
  //	SYNTAX: print_parameter_list
  else if((cmd == "print_parameter_list") || (cmd == "ppl") || (cmd == "print_param_list"))
  {
    ADCP_CLI::print_parameter_list();	  
  }
  //	Command to list all the available frames.
  //	SYNTAX: print_frame_list
  else if( ( cmd == "print_frame_list" ) || ( cmd == "pfl" ) )
  {
	ADCP_CLI::print_frame_list();  
  }
  //	Command to print the details of a specific parameter.
  //	SYNTAX:	print_parameter_details	--index=[parameter_index]
  //			pritn_parameter_details --name=[parameter_name]
  //	@parameter parameter_index	:	Index of the parameter.
  //	@parameter parameter_name	:	Name of the parameter.	
  else if( ( cmd == "print_parameter_details" ) || ( cmd == "ppd") )
  {
	try
	{	
		if( parameter[0] == "index" )
		{
			ADCP_CLI::print_parameter_details(stoi(value[0]));
		}
		else if( parameter[0] == "name" )
		{
			ADCP_CLI::print_parameter_details(value[0]);
		}
		else
		{
			cout << "print parameter details (): Invalid argument !!" << endl;
			cout << "SYNTAX: print_parameter_details --index=[parameter_index]" << endl;
			cout << "        print_parameter_details --name=[parameter_name]" << endl;
		}
	}
	catch(...)
	{
		cout << "No parameter index / variable provided !" << endl;
		cout << "SYNTAX: print_parameter_details --index=[parameter_index]" << endl;
		cout << "        print_parameter_details --name=[parameter_name]" << endl;		
	}
  }
  //	Command to print details of a packet/frame.
  //	SYNTAX:	print_frame_details	--index=[frame_index]
  //			print_frame_details --name=[frame_name]
  else if( ( cmd == "print_frame_details" ) || ( cmd == "pfd") )
  {
	if( parameter[0] == "index" )
	{
		ADCP_CLI::print_frame_details(stoi(value[0]));
	}
	else if( parameter[0] == "name" )
	{
		ADCP_CLI::print_frame_details(value[0]);
	}
	else
	{
		cout << "print parameter details (): Invalid argument !!" << endl;
		cout << "SYNTAX: print_frame_details --index=[frame_index]" << endl;
		cout << "        print_frame_details --name=[frame_name]" << endl;
	}
  }
  //	Command to list all available serial ports in the device.
  //	SYNTAX: list_serial_port
  else if( ( cmd == "list_serial_ports" ) || ( cmd == "lsp" ) )
  {
	  ADCP_CLI::list_serial_ports();
  }
  //	Close a connection with a target node.
  //	SYNTAX:	close_node_connection --node_id=[target_node_id]	--timeout=[timeout_ms]
  //			close_node_connection --node_name=[target_node_name]	--timeout=[timeout_ms]
  else if( ( cmd == "cnc" ) || ( cmd == "close_node_connection" ) )
  {
	if( (parameter[0] == "node_id") || (parameter[0] == "id") )
	{		
		try
		{
			if( parameter.at(1) != "timeout" )
			{
				cout << "Timeout not provided !" << endl;
				cout << "SYNTAX: close_node_connection --node_id=[target_node_id]	--timeout=[timeout_ms]" << endl;
				cout << "        close_node_connection -- node_id=[target_node_id]	--timeout=[timeout_ms]" << endl;				
			}
			
			if( adcp->close_node_connection( std::atoi(value.at(0).c_str()), std::atoi(value.at(1).c_str()) ) )
			{
				cout << "Close connection request successful at node " << value.at(0) << endl;
			}
			else
			{
				cout << "Close connection request NOT successful at node " << value.at(0) << endl;
			}
		}
		catch(const out_of_range& e)
		{
			cout << "Timeout not provided !" << endl;
			cout << "SYNTAX: close_node_connection --node_id=[target_node_id]	--timeout=[timeout_ms]" << endl;
			cout << "        close_node_connection -- node_id=[target_node_id]	--timeout=[timeout_ms]" << endl;
		}
	}
	else if( ( parameter[0] == "node_name" ) || ( parameter[0] == "name" ) )
	{
		try
		{
			if( parameter.at(1) != "timeout" )
			{
				cout << "Timeout not provided !" << endl;
				cout << "SYNTAX: close_node_connection --node_id=[target_node_id]	--timeout=[timeout_ms]" << endl;
				cout << "        close_node_connection -- node_id=[target_node_id]	--timeout=[timeout_ms]" << endl;				
			}

			if( adcp->close_node_connection( adcp->get_node_id(value.at(0)) , std::atoi(value.at(1).c_str()) ) )
			{
				cout << "Close connection request successful at node " << value.at(0) << endl;
			}
			else
			{
				cout << "Close connection request NOT successful at node " << value.at(0) << endl;
			}
		}
		catch(const out_of_range& e)
		{
			cout << "Timeout not provided !" << endl;
			cout << "SYNTAX: close_node_connection --node_id=[target_node_id]	--timeout=[timeout_ms]" << endl;
			cout << "        close_node_connection -- node_id=[target_node_id]	--timeout=[timeout_ms]" << endl;
		}
	}
	else
	{
		cout << "Incorrect syntax" << endl;
		cout << "SYNTAX: close_node_connection --node_id=[node_id] --timeout=[timeout_ms]" << endl;
		cout << "        close_node_connection --node_id=[node_id] --timeout-[timeout_ms]" << endl;		
	}
  }
  //	Command to open a connection with target node.
  //	SYNTAX:	open_node_connection --node_id=[node_id] --timeout=[timeout_ms]
  //			open_node_connection --node_name=[node_name] --timeout=[timeout_ms]
  else if( ( cmd == "onc" ) || ( cmd == "open_node_connection" ) )
  {
	if( (parameter[0] == "node_id") || (parameter[0] == "id") )
	{
		try
		{
			if( adcp->open_node_connection( std::atoi(value.at(0).c_str()), std::atoi(value.at(1).c_str()) ) == 0 )
			{
				cout << "Connection NOT opened at node: " << value.at(0) << endl;
			}
			else
			{
				cout << "Connection opened successfully at node: " << value.at(0) << endl;
			}
		}
		catch(const out_of_range& e)
		{
			cout << "Timeout not provided !" << endl;
			cout << "SYNTAX: open_node_connection --node_id=[node_id] --timeout=[timeout_ms]" << endl;
			cout << "        open_node_connection --node_id=[node_id] --timeout-[timeout_ms]" << endl;
		}
	}
	else if( ( parameter[0] == "node_name" ) || ( parameter[0] == "name" ) )
	{
		try
		{
			if( adcp->open_node_connection( adcp->get_node_id(value.at(0)) , std::atoi(value.at(1).c_str()) ) == 0 )
			{
				cout << "Connection NOT opened at node: " << value.at(0) << endl;
			}
			else
			{
				cout << "Connection opened successfully at node: " << value.at(0) << endl;
			}
		}
		catch(const out_of_range& e)
		{
			cout << "Timeout not provided !" << endl;
		}
	}
	else
	{
		cout << "Incorrect syntax !" << endl;
		cout << "SYNTAX: open_node_connection --node_id=[node_id] --timeout=[timeout_ms]" << endl;
		cout << "        open_node_connection --node_id=[node_id] --timeout-[timeout_ms]" << endl;		
	}
  }
  //
  //	Command to send a frame to the device node.
  //	SYNTAX: send_dataframe --frame_name=[frame_name]	
  //
  else if( ( cmd == "send_frame") || ( cmd == "sf" ) || ( cmd == "send_dataframe" ) )
  {
	  if( ( parameter[0] == "frame_name" ) || ( parameter[0] == "fn" ) )
	  {
		cout << "Sending frame to device node " << adcp->get_frame(value[0])->node_ID << ":" << value[0];
        cout << " [" << adcp->get_frame(value[0])->frame_ID << "]" << endl;


		if( adcp->is_frame_valid(value[0]) )
			adcp->send_frame(value[0]);
		else
			cout << "Frame not sent as it is not found in frame list !" << endl;
	  }
	  else
	  {
		 cout << "Incorrect syntax provided !" << endl;
		 cout << "SYNTAX: send_frame --frame_name=[frame_name]" << endl;
	  }
  }
  else if( ( cmd == "osp" ) || ( cmd == "open_serial_port") )
  {
	/* This command is used to open a serial port connection at the target serial port. */
    /* @parameter com_port:  	The serial port at which the connection has to established.*/
	/* @parameter baud_rate:	The baud rate of the serial connection. */
	
	//	Syntax: open_serial_port --com_port=[com_port] --baud_rate=[baud_rate]
	
	if( (parameter[0] == "com_port") || (parameter[0] == "COM_PORT") 
		|| (parameter[1] == "baud_rate") || (parameter[1] == "BAUD_RATE") )
	{
		string com_port_name = value[0];
		
		com_port_name.erase(0,1);
		com_port_name.erase(com_port_name.length()-1,1);
	
		ADCP_CLI::open_serial_port(com_port_name,std::atoi(value[1].c_str()));
	}
	else if( (parameter[0] == "comport") || (parameter[0] == "COMPORT") 
		|| (parameter[1] == "baudrate") || (parameter[1] == "BAUDRATE") )
	{
		string com_port_name = value[0];
		
		com_port_name.erase(0,1);
		com_port_name.erase(com_port_name.length()-1,1);
	
		ADCP_CLI::open_serial_port(com_port_name,std::atoi(value[1].c_str()));
	}
	else if( (parameter[1] == "com_port") || (parameter[1] == "COM_PORT") 
		|| (parameter[0] == "baud_rate") || (parameter[0] == "BAUD_RATE") )
	{
		string com_port_name = value[1];
		
		com_port_name.erase(0,1);
		com_port_name.erase(com_port_name.length()-1,1);
	
		ADCP_CLI::open_serial_port(com_port_name,std::atoi(value[0].c_str()));
	}
	else if( (parameter[1] == "comport") || (parameter[1] == "COMPORT") 
		|| (parameter[0] == "baudrate") || (parameter[0] == "BAUDRATE") )
	{
		string com_port_name = value[1];
		
		com_port_name.erase(0,1);
		com_port_name.erase(com_port_name.length()-1,1);
	
		ADCP_CLI::open_serial_port(com_port_name,std::atoi(value[0].c_str()));
	}
	else
	{
		cout << "Incorrect syntax" << endl;
		cout << "Syntax: open_serial_port --com_port=[com_port] --baud_rate=[baud_rate]" << endl;
	}
  }
  else if( (cmd == "csp") || ( cmd == "close_serial_port") )
  {
	/* This command is used to close an existing serial port connection. */
	
	ADCP_CLI::close_serial_port();
  }
  else if( (cmd == "reset_node") || (cmd =="rn") || ( cmd == "reset_node_connection" ) || ( cmd == "rnc") )
  {
	/* This function is used to rest connection at target node. */
	//
	//	SYNTAX: reset_node --node_id=[node_id] --timeout=[timeout_ms]
	//

	if( (parameter[0] == "node_id") || (parameter[0] == "nodeid") || (parameter[1] == "timeout") )
	{
		ADCP_CLI::reset_node( std::atoi(value[0].c_str()), std::atoi(value[1].c_str()) );
	}
	else if( (parameter[1] == "node_id") || (parameter[1] == "nodeid") || (parameter[0] == "timeout") )
	{
		ADCP_CLI::reset_node( std::atoi(value[1].c_str()), std::atoi(value[0].c_str()) );
	}
	else
	{
		cout << "Incorrect parameters provided !" << endl;
		cout << "SYNTAX: reset_node --node_id=[node_id] --timeout=[timeout_ms]" << endl;
	}
  }
  else if( (cmd == "node_queue_status" ) || ( cmd == "nqs") )
  {
	/* This function is used to get the status of the target node's receiver queue. */
	//	SYNTAX:	node_queue_status --node_id=[node_id] --timeout=[timeout_ms]

	try
	{
		if( ( parameter.at(0) == "node_id") || ( parameter.at(0) == "nodeid")
			|| ( parameter.at(1) == "timeout") || ( parameter.at(1) == "timeout") )
		{
			ADCP_CLI::node_queue_status( std::atoi( value.at(0).c_str() ), std::atoi( value.at(1).c_str() ) );
		}
		else if( ( parameter.at(1) == "node_id") || ( parameter.at(1) == "nodeid")
			|| ( parameter.at(0) == "timeout") || ( parameter.at(0) == "timeout") )
		{
			ADCP_CLI::node_queue_status( std::atoi( value.at(1).c_str() ), std::atoi( value.at(0).c_str() ) );
		}
		else
		{
			cout << "Incorrect parameters provided !" << endl;
			cout << "SYNTAX: node_queue_status --node_id=[node_id] --timeout=[timeout_ms]" << endl;
		}
	}
	catch(...)
	{
		cout << "Incorrect parameters provided !" << endl;
		cout << "SYNTAX: node_queue_status --node_id=[node_id] --timeout=[timeout_ms]" << endl;
	}
  }
  else if( (cmd == "node_queue_element_count" ) || ( cmd == "nqec") )
  {
	/* This function is used to get the target node's receiver queue element count. */
	//	SYNTAX: node_queue_element_count --node_id=[node_id] --timeout=[timeout_ms]

	try
	{
		if( ( parameter.at(0) == "node_id") || ( parameter.at(0) == "nodeid")
			|| ( parameter.at(1) == "timeout") || ( parameter.at(1) == "time_out") )
		{
			ADCP_CLI::node_queue_element_count( std::atoi( value.at(0).c_str() ), std::atoi( value.at(1).c_str() ) );
		}
		else if( ( parameter.at(1) == "node_id") || ( parameter.at(1) == "nodeid")
			|| ( parameter.at(0) == "timeout") || ( parameter.at(0) == "time_out") )
		{
			ADCP_CLI::node_queue_element_count( std::atoi( value.at(1).c_str() ), std::atoi( value.at(0).c_str() ) );
		}
		else
		{
			cout << "Incorrect parameters provided !" << endl;
			cout << "SYNTAX: node_queue_element_count --node_id=[node_id] --timeout=[timeout_ms]" << endl;
		}
	}
	catch(...)
	{
		cout << "Invalid arguments passed !" << endl;
		cout << "SYNTAX: node_queue_element_count --node_id=[node_id] --timeout=[timeout_ms]" << endl;
	}
  }
  else if( (cmd == "node_network_error_status" ) || ( cmd == "nnes") )
  {
	/* This function is used to get the target node's network error status. */
	//	SYNTAX: node_network_error_status --nodeid=[node_id] --timeout=[timeout_ms]

	try
	{
		if( ( parameter.at(0) == "node_id") || ( parameter.at(0) == "nodeid")
			|| ( parameter.at(1) == "timeout") || ( parameter.at(1) == "time_out") )
		{
			ADCP_CLI::node_network_error_status( std::atoi( value.at(0).c_str() ), std::atoi( value.at(1).c_str() ) );
		}
		else if( ( parameter.at(1) == "node_id") || ( parameter.at(1) == "nodeid")
			|| ( parameter.at(0) == "timeout") || ( parameter.at(0) == "time_out") )
		{
			ADCP_CLI::node_network_error_status( std::atoi( value.at(1).c_str() ), std::atoi( value.at(0).c_str() ) );
		}
		else
		{
			cout << "Invalid parameters provided !" << endl;
			cout << "SYNTAX: node_network_error_status --nodeid=[node_id] --timeout=[timeout_ms]" << endl;
		}
	}
	catch(...)
	{
		cout << "Invalid arguments passed !" << endl;
		cout << "SYNTAX: node_network_error_status --nodeid=[node_id] --timeout=[timeout_ms]" << endl;
	}
  }
  else if( (cmd == "node_application_error_code" ) || ( cmd == "naec") )
  {
	/* This function is used to get the target node's application error code. */
	//	SYNTAX:	node_application_error_code	--node_id=[node_id] --timeout=[timeout_ms]

	try
	{
		if( ( parameter.at(0) == "node_id") || ( parameter.at(0) == "nodeid")
			|| ( parameter.at(1) == "timeout") || ( parameter.at(1) == "time_out") )
		{
			ADCP_CLI::node_application_error_code( std::atoi( value.at(0).c_str() ), std::atoi( value.at(1).c_str() ) );
		}
		else if( ( parameter.at(1) == "node_id") || ( parameter.at(1) == "nodeid")
			|| ( parameter.at(0) == "timeout") || ( parameter.at(0) == "time_out") )
		{
			ADCP_CLI::node_application_error_code( std::atoi( value.at(1).c_str() ), std::atoi( value.at(0).c_str() ) );
		}
		else
		{
			cout << "Incorrect syntax." << endl;
			cout << "SYNTAX:	node_application_error_code	--node_id=[node_id] --timeout=[timeout_ms]" << endl;
		}
	}
	catch(...)
	{
		cout << "Invalid arguments passed !" << endl;
		cout << "SYNTAX:	node_application_error_code	--node_id=[node_id] --timeout=[timeout_ms]" << endl;
	}
  }
  //
  //	This command is used to request a target frame from target node.
  //	SYNTAX: remote_request --node_id=[node_id] --frames=[frame_names] --timeout=[timeout_ms]
  //
  else if( ( cmd == "rrqst") || ( cmd == "remote_request" ) )
  {
	try
	{	
		if( (parameter.at(0) == "nodeid") || (parameter.at(0) == "node_id") || 
			(parameter.at(1) == "frames") || (parameter.at(1) == "fn") ||
			(parameter.at(2) == "timeout") )
		{		
			ADCP_CLI::node_remote_request( std::atoi(value.at(0).c_str()),
										   stringUtils::parse_array_string(value.at(1)),
										   std::atoi(value.at(2).c_str()) );
		}
		else
		{
			cout << "Incorrect arguments provided" << endl;
			cout << "SYNTAX: remote_request --node_id=[node_id] --frames=[frame_names] --timeout=[timeout_ms]" << endl;
		}
	}
	catch(...)
	{
		cout << "Incorrect arguments provided" << endl;
		cout << "SYNTAX: remote_request --node_id=[node_id] --frames=[frame_names] --timeout=[timeout_ms]" << endl;
	}
  }
  //
  //	Command to transmit a data frame/packet to target node.
  //	SYNTAX: transmit_dataframe --frame_name=[frame_name]
  //
  else if( ( cmd == "trasmit_dataframe" ) || ( cmd == "txdf" ) )
  {
	if( ( parameter[0] == "frame_name" ) || ( parameter[0] == "fname" ) || ( parameter[0] == "framename" ) )
	{
		value[0] = remove_quotes(value[0]);
		
		if( adcp->is_frame_valid(value[0]) )
		{
			cout << "Sending frame to device node (" << adcp->get_frame(value[0])->node_ID << "): " << adcp->get_frame(value[0])->frame_name << " [" << adcp->get_frame(value[0])->frame_ID << "]" << endl;  
			adcp->send_frame(value[0]);	
		}
		else
		{
			cout << "Frame not found. Transmit aborted." << endl;		
		}
	}
	else
	{
		cout << "Invalid parameters provided !" << endl;
		cout << "SYNTAX: transmit_dataframe --frame_name=[frame_name]" << endl;
	}		
  }
  else if( (cmd == "receive_dataframe") || (cmd == "rxdf") )
  {
	//
	//	Command to receive target dataframe from device.
	//	SYNTAX:	receive_dataframe --frame_name=[frame_name] --timeout=[timeout_ms]
	  
	try
	{
		adcp->serial_port_flush(NULL);
		
		if( ( parameter.at(0) == "fname" ) || ( parameter.at(0) == "framename" ) || ( parameter.at(0) == "frame_name" ) )
		{
			value[0] = remove_quotes(value[0]);
			
			if( adcp->receive_frame( value.at(0), std::atoi( value.at(1).c_str() ) ) )
			{
				cout << "Frame received from node (" << adcp->get_frame(value.at(0))->node_ID << ") : " << adcp->get_frame(value.at(0))->frame_name << endl;  
			}
			else
			{
				cout << "Target frame NOT received within requested timeout interval !" << endl;		
			}
		}
		else
		{
			cout << "Invalid parameters provided" << endl;
			cout << "receive_dataframe --frame_name=[frame_name] --timeout=[timeout_ms]" << endl;			
		}
	}
	catch(...)
	{
		cout << "Invalid parameters provided" << endl;
		cout << "receive_dataframe --frame_name=[frame_name] --timeout=[timeout_ms]" << endl;
	}		
  }
  //
  //  Commands:	Command to write a value to parameter
  //
  //  SYNTAX: write_parameter --name=[parameter_name] --value=[parameter_value]
  //
  //  NOTE: 		This command does not transmit the parameter to the device.
  //				Call the "send_frame" command to send the data packet to the device via serial port.
  //
  else if( ( cmd == "write_parameter" ) || (cmd == "wp") )
  {
	try
	{
		if( ( parameter.at(0) == "name" ) || ( parameter.at(1) == "value" ) ) 
		{
			cout << "Writing parameter value: " << remove_quotes(value.at(0)) << endl;
			adcp->write_parameter( remove_quotes(value.at(0)), std::stod(value.at(1)) );
		}
		else
		{
			cout << "Incorrect parameters provided !" << endl;
			cout << "SYNTAX: write_parameter --name=[parameter_name] --value=[parameter_value]" << endl;
		}
	}
	catch(...)
	{
		cout << "Incorrect parameters provided !" << endl;
		cout << "SYNTAX: write_parameter --name=[parameter_name] --value=[parameter_value]" << endl;
	}		
  }
  //
  //	Command to read a parameter from its respective data packet.
  //	SYNTAX:		read_parameter	--name=[parameter_name]
  //	NOTE: 		Enclose parameter name with quotes.
  //
  //
  else if( ( cmd == "read_parameter" ) || (cmd == "rp") )
  {
	try
	{
		if( ( parameter.at(0) == "name" ) ) 
		{
			double _value = adcp->read_parameter( remove_quotes(value.at(0)) );
			
			cout << "Value of parameter (" << remove_quotes(value.at(0)) << ": " << _value << endl;
		}
		else
		{
			cout << "Invalid syntax" << endl;
			cout << "read_parameter	--name=[parameter_name]" << endl;
		}
	}
	catch(...)
	{
		cout << "Invalid syntax" << endl;
		cout << "read_parameter	--name=[parameter_name]" << endl;
	}		
  }
  //
  //	Command to write a parameter to its data packet and then transmit the data packet via serial port.
  //	SYNTAX: transmit_parameter --name=[parameter_name] --value=[parameter_value]
  //
  else if( ( cmd == "transmit_parameter" ) || (cmd == "txp") )
  {
	try
	{
		if( ( parameter.at(0) == "name" ) || ( parameter.at(1) == "value" ) ) 
		{
			cout << "Transmitting parameter value: " << remove_quotes(value.at(0)) << endl;
			adcp->send_parameter( remove_quotes(value.at(0)), std::stod(value.at(1)) );
		}
		else
		{
			cout << "Invalid syntax" << endl;
			cout << "SYNTAX: transmit_parameter --name=[parameter_name] --value=[parameter_value]" << endl;
		}
	}
	catch(...)
	{
		cout << "Invalid syntax" << endl;
		cout << "SYNTAX: transmit_parameter --name=[parameter_name]" << endl;
	}		
  }
  //
  //	Command to receive the parameter's owning frame and then reading the parameter from its owning data packet.
  //	SYNTAX: receive_parameter --name=[parameter_name]
  //  
  else if( ( cmd == "receive_parameter" ) || (cmd == "rxp") )
  {
	try
	{
		string s;
		
		if( ( parameter.at(0) == "pname" ) || ( parameter.at(1) == "timeout" ) ) 
		{	
			value[0].erase(0,1);
			value[0].erase(value[0].length()-1,1);
;	
			clock_t startTime = clock();
			double _value = adcp->receive_parameter( value.at(0), std::atoi(value.at(1).c_str()) );
			clock_t endTime = clock();
			
			uint32_t timeout_ms = std::atoi(value.at(1).c_str());
			uint32_t timeInterval = ((double)1000.0*(double)(endTime - startTime))/(double)CLOCKS_PER_SEC;
			
			if(timeInterval < timeout_ms)
			{
			  cout << "Reading parameter (" << value.at(0) << "): " << _value << " time = " << timeInterval << " (ms)" << endl;
			}
			else
			{
			  cout << "Timeout event occured !" << endl;	
			}
		}
		else
		{
			cout << "Invalid syntax" << endl;
			cout << "SYNTAX: receive_parameter --name=[parameter_name]" << endl;			
		}
	}
	catch(...)
	{
		cout << "Invalid syntax" << endl;
		cout << "SYNTAX: receive_parameter --name=[parameter_name]" << endl;			
	}		
  }
  else
  {
	cout << ">> INVALID COMMAND !!!!" << endl;
	//error_beep();	
  }

}

/******************	Application specific CLI functions start here.  **********************/

void ADCP_CLI::read_file()
{
  string file_name = (CLI_PROMPTS[READ_FILE].values)[0];

  file_name.erase(file_name.length()-1,1);  
  file_name.erase(0,1);  	
	
  cout << "Reading file: " << file_name << endl;	
	
  adcp->read_file(file_name.c_str());

  (CLI_PROMPTS[READ_FILE].parameters).clear();
  (CLI_PROMPTS[READ_FILE].values).clear();  
}

void ADCP_CLI::print_parameter_list()
{
  adcp->print_parameter_list();	
}

void ADCP_CLI::print_frame_list()
{
  adcp->print_frame_list();		
}

void ADCP_CLI::print_parameter_details(unsigned int index)
{
  adcp->print_parameter_details(index);
}	

void ADCP_CLI::print_parameter_details(string name)
{
  adcp->print_parameter_details(name);	
}

void ADCP_CLI::print_frame_details(unsigned int index)
{
  adcp->print_frame_details(index);
}	

void ADCP_CLI::print_frame_details(string name)
{
  adcp->print_frame_details(name);	
}

void ADCP_CLI::list_serial_ports()
{
  adcp->serial_port_scan();  
  
  cout << endl;
}

void ADCP_CLI::open_connection(unsigned int node_id,uint32_t timeout_ms)
{
	
}

void ADCP_CLI::close_connection(unsigned int node_id,uint32_t timeout_ms)
{
	
}

void ADCP_CLI::open_serial_port(string com_port_name,unsigned int baud_rate)
{
	uint8_t is_valid_baud_rate = 0;
		
	unsigned int _baudrate = 0;

	if( baud_rate == 4800 )
	{
		_baudrate = CBR_4800;
		is_valid_baud_rate = 1;
	}
		
	if( baud_rate == 9600 )
	{
		_baudrate = CBR_9600;
		is_valid_baud_rate = 1;
	}

	if( baud_rate == 19200 )
	{
		_baudrate = CBR_19200;
		is_valid_baud_rate = 1;
	}

	if( baud_rate == 38400 )
	{
		_baudrate = CBR_38400;
		is_valid_baud_rate = 1;
	}

	if( baud_rate == 57600 )
	{
		_baudrate = CBR_57600;
		is_valid_baud_rate = 1;
	}
		
	if( baud_rate == 115200 )
	{
		_baudrate = CBR_115200;
		is_valid_baud_rate = 1;
	}		
		
	if( is_valid_baud_rate)
	{
		cout << "Baud rate supported !" << endl;
			
		adcp->serial_port_init(com_port_name,_baudrate);
			
		if( adcp->serialPort.status == ADCP::OPEN )
		{
			cout << "Serial connection opened successfully @ com port " << com_port_name << " with baud rate of " << _baudrate << " (bps)" << endl;
		}
		else
		{
			cout << "Serial connection at COM port " << com_port_name << " not established !" << endl;
		}
	}
	else
	{
		cout << "Baud rate not supported ! Supported baud rates are 4800, 9600, 19200, 38400, 57600, 115200 bps." << endl;
	}		
}

void ADCP_CLI::close_serial_port()
{
  cout << "Closing serial port connection" << endl;	
  adcp->serial_port_close();  
}

void ADCP_CLI::reset_node(unsigned int node_id,uint32_t timeout_ms)
{
  uint32_t timeout_status = adcp->reset_node(node_id,timeout_ms);

  if(timeout_status == 0)
  {
	cout << "Timeout event occured !" << endl;
  }
  else
  {
	if( adcp->Connection_Management_Frame.SYNC_ACK )
	{
		cout << "Reset node request acknowledged." << endl; 
	}
	else
	{
		cout << "Reset node request NOT acknowledged !" << endl;
	}
  }
}

void ADCP_CLI::node_queue_status(unsigned int node_id,uint32_t timeout_ms)
{
	if( adcp->get_error_and_status(node_id, timeout_ms) )
	{
		if( adcp->Error_Status_Frame.SACK )
		{
			cout << "Error/status frame received from target node." << endl;
			cout << "Queue status: ";
			
			if(adcp->Error_Status_Frame.QSTAT == 0)
			{
				cout << "EMPTY";
			}

			if(adcp->Error_Status_Frame.QSTAT == 1)
			{
				cout << "OK";
			}
			
			if(adcp->Error_Status_Frame.QSTAT == 2)
			{
				cout << "FULL";
			}			
		}
		else
		{
			cout << "Error/status frame not received from target node." << endl;
		}
		
		cout << endl;
	}
	else
	{
		cout << "Timeout event occured !" << endl;
	}
}

void ADCP_CLI::node_queue_element_count(unsigned int node_id,uint32_t timeout_ms)
{
	if( adcp->get_error_and_status(node_id, timeout_ms) )
	{
		if( adcp->Error_Status_Frame.SACK )
		{
			cout << "Error/status frame received from target node." << endl;
			cout << "Queue element count: " << adcp->Error_Status_Frame.QSZ << endl;			
		}
		else
		{
			cout << "Error/status frame not received from target node." << endl;
		}
	}
	else
	{
		cout << "Timeout event occured !" << endl;
	}
}

void ADCP_CLI::node_network_error_status(unsigned int node_id,uint32_t timeout_ms)
{
	if( adcp->get_error_and_status(node_id, timeout_ms) )
	{
		if( adcp->Error_Status_Frame.SACK )
		{
			cout << "Error/status frame received from target node." << endl;
			cout << "Network error status: ";
			
			if(adcp->Error_Status_Frame.NES == 0)
			{
				cout << "NONE";
			}

			if(adcp->Error_Status_Frame.NES == 1)
			{
				cout << "ALREADY OPEN";
			}
			
			if(adcp->Error_Status_Frame.NES == 2)
			{
				cout << "ALREADY CLOSE";
			}

			if(adcp->Error_Status_Frame.NES == 3)
			{
				cout << "INVALID FRAME REQUESTED";
			}
			
			cout << endl;
		}
		else
		{
			cout << "Error/status frame not received from target node." << endl;
		}
		
		cout << endl;
	}
	else
	{
		cout << "Timeout event occured !" << endl;
	}
}

void ADCP_CLI::node_application_error_code(unsigned int node_id, uint32_t timeout_ms)
{
	if( adcp->get_error_and_status(node_id, timeout_ms) )
	{
		if( adcp->Error_Status_Frame.SACK )
		{
			cout << "Error/status frame received from target node." << endl;
			cout << "Application error code: " << adcp->Error_Status_Frame.AERR << endl;			
		}
		else
		{
			cout << "Error/status frame not received from target node." << endl;
		}
	}
	else
	{
		cout << "Timeout event occured !" << endl;
	}	
}


void ADCP_CLI::node_remote_request(unsigned int node_id,vector<string> frame_names,uint32_t timeout_ms)
{
	if( adcp->remote_frame_request(node_id,frame_names,timeout_ms) )
	{
		cout << "Remote request frame received." << endl;
		
		if( adcp->Remote_Request_Frame.RMT_ACK )
		{
			cout << "Remote request acknowledged by target node" << endl;
		}
		else
		{
			cout << "Remote request NOT acknowledged by target node" << endl;
		}
	}
	else
	{
		cout << "Timeout event occured !" << endl;
	}
}


void ADCP_CLI::transmit_dataframe(string s)
{
	
}

void ADCP_CLI::receive_dataframe(string frame_name,uint32_t timeout_ms)
{
	
}

void ADCP_CLI::write_parameter(string frame_name,double value)
{
	
}

void ADCP_CLI::read_parameter(string parameter_name)
{
	
}

void ADCP_CLI::transmit_parameter(string parameter_name,double value)
{
	
}

void ADCP_CLI::receive_parameter(string parameter_name)
{
	
}

void ADCP_CLI::request_parameter(string parameter_name)
{
	
}
