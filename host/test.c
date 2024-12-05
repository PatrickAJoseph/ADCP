
#include <stdio.h>
#include <stdint.h>


struct s
{
  uint32_t entry_1;
  
  uint8_t entries[5];
}s;


int main()
{	
	printf("%d", sizeof(struct s) );	
	
	return(0);
}