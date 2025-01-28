#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

# include <stdint.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct	s_info
{
  char		*dev;
  char		*ip_src;
  char		*ip_target;
  char		*mac_src;
  char		*mac_target;
  char		gateway_ip[16];
  char		gateway_mac[18];
  //pcap_t	*handle;
}		t_info;


// Utils.c
void  ft_check_errors(int argc);
void  ft_usage(void);


#endif
