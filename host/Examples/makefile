

COMPILER = gcc
DEBUG_FLAGS = -DDEBUG
CFLAGS = -Wall
MAIN_FILE = test.cpp
DEPENDENCIES = ADCP.cpp

#auto_detect:
#	g++ auto_detect.cpp ../stringUtils.cpp ../stringUtils.hpp ../ADCP.cpp ../ADCP.hpp -o auto_detect.exe
	
#open_close_node_connection:
#	g++ open_close_node_connection.cpp ../stringUtils.cpp ../stringUtils.hpp ../ADCP.cpp ../ADCP.hpp -o open_close_node_connection.exe

#connection_management:
#	g++ connection_management.cpp ../stringUtils.cpp ../stringUtils.hpp ../ADCP.cpp ../ADCP.hpp -o connection_management.exe

#remote_data_packet_request:
#	g++ remote_data_packet_request.cpp ../stringUtils.cpp ../stringUtils.hpp ../ADCP.cpp ../ADCP.hpp -o remote_data_packet_request.exe

#error_and_status_packet:
#	g++ error_and_status_packet.cpp ../stringUtils.cpp ../stringUtils.hpp ../ADCP.cpp ../ADCP.hpp -o error_and_status_packet.exe

read_parameter:
	g++ -pthread read_parameter.cpp ../ADCP.hpp ../ADCP.cpp ../stringUtils.cpp ../stringUtils.hpp -DDEBUG -DRX_LOG_ENABLE -o read_parameter.exe

write_parameter:
	g++ write_parameter.cpp ../ADCP.hpp ../ADCP.cpp ../stringUtils.cpp ../stringUtils.hpp -DDEBUG -DTX_LOG_ENABLE -DRX_LOG_ENABLE -o write_parameter.exe
	
ADCP_file_generator:
	g++ ADCP_file_generator.cpp ../stringUtils.cpp ../stringUtils.hpp ../ADCP.cpp ../ADCP.hpp -o ADCP_file_generator.exe
	
