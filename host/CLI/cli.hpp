
#include "ADCP.hpp"

#define PROMPT_COUNT		3
			

extern ADCP* adcp;

typedef enum
{
  TEST_FUNCTION_A = 0,	
  TEST_FUNCTION_B = 1,
  READ_FILE = 2	
}PROMPT_t;


typedef struct
{
  string command;
  vector<string> parameters;
  vector<string> values;
  void (*function)(void);
}CLI_PROMPT_struct;	

typedef CLI_PROMPT_struct CLI_PROMPT;

extern CLI_PROMPT CLI_PROMPTS[PROMPT_COUNT];

void cli_init();
void cli();
void cli_script(string filename);
string get_cmd(const string& in);
std::vector<string> get_params(const string& in);
std::vector<string> get_values(const string& in);
void cli_interpreter(string cmd, std::vector<string> parameter, std::vector<string> value);

/* Application specific CLI functions. */

namespace ADCP_CLI
{
  void read_file();
  void print_parameter_list();
  void print_frame_list();
  void print_parameter_details(unsigned int index);
  void print_parameter_details(string name);
  void print_frame_details(unsigned int index);
  void print_frame_details(string name);
  void list_serial_ports();
  void open_serial_port(string com_port,unsigned int baudrate);
  void close_serial_port();
  
  /* Connection management functions. */
  
  void open_connection(unsigned int node_id,uint32_t timeout_ms);
  void close_connection(unsigned int node_id,uint32_t timeout_ms);
  void reset_node(unsigned int node_id,uint32_t timeout_ms);
  
  /* Error status and management functions. */
  
  void node_queue_status(unsigned int node_id,uint32_t timeout_ms);
  void node_queue_element_count(unsigned int node_id,uint32_t timeout_ms);
  void node_network_error_status(unsigned int node_id,uint32_t timeout_ms);
  void node_application_error_code(unsigned int node_id, uint32_t timeout_ms);
  
  /* Remote request functions. */
  
  void node_remote_request(unsigned int node_id,vector<string> frame_names,uint32_t timeout_ms);
  
  /* Functions to handle data frames. */
  
  void transmit_dataframe(string s);
  void receive_dataframe(string frame_name,uint32_t timeout_ms);
  void write_parameter(string frame_name,double value);
  void read_parameter(string parameter_name);
  void transmit_parameter(string parameter_name,double value);
  void receive_parameter(string parameter_name);
  void request_parameter(string parameter_name);
}