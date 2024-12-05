

COMPILER = gcc
DEBUG_FLAGS = -DDEBUG
CFLAGS = -Wall
MAIN_FILE = test.cpp
DEPENDENCIES = ADCP.cpp


cli:
	g++ cli_test.cpp cli.cpp cli.hpp stringUtils.cpp stringUtils.hpp ADCP.cpp ADCP.hpp -o cli.exe

cli_debug:
	g++ cli_test.cpp cli.cpp cli.hpp stringUtils.cpp stringUtils.hpp ADCP.cpp ADCP.hpp -DDEBUG -o cli.exe

cli_script:
	g++ cli_script.cpp cli.cpp cli.hpp stringUtils.cpp stringUtils.hpp ADCP.cpp ADCP.hpp -DDEBUG -o cli_script.exe

cli_string_util_debug:
	g++ cli_test.cpp cli.cpp cli.hpp stringUtils.cpp stringUtils.hpp ADCP.cpp ADCP.hpp -DDEBUG -DDEBUG_STDOUT -o cli.exe

test:
	g++ test.cpp stringUtils.cpp stringUtils.hpp ADCP.cpp ADCP.hpp -o test.exe
	
test_debug:
	g++ test.cpp stringUtils.cpp stringUtils.hpp ADCP.cpp ADCP.hpp -o test.exe -DDEBUG	
	
file_generator:
	g++ ADCP_file_generator.cpp stringUtils.cpp stringUtils.hpp ADCP.cpp ADCP.hpp -o file_generator.exe	-DDEBUG