/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irifarac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:18:40 by irifarac          #+#    #+#             */
/*   Updated: 2025/02/10 21:49:04 by israel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

# include <stdint.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

# include <signal.h>

# include <arpa/inet.h>
# include <net/ethernet.h>
# include <net/if.h>
# include <net/if_arp.h>
# include <netpacket/packet.h>
# include <netinet/ip.h>
# include <netinet/tcp.h>
# include <netinet/udp.h>
# include <netinet/ip_icmp.h>
# include <netinet/if_ether.h>

# include <sys/socket.h>
# include <sys/types.h>
# include <ifaddrs.h>

# include <pcap.h>

# define MAC_ADDR_LEN 6

# define TC_NRM "\x1B[0m"
# define TC_RED "\x1B[31m"
# define TC_GRN "\x1B[32m"
# define TC_YEL "\x1B[33m"
# define TC_BLU "\x1B[34m"

extern volatile sig_atomic_t  stop;

typedef struct	s_info
{
  char		dev[IFNAMSIZ];
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
void  ft_init(t_info *info, char **argv);
void  ft_check_syntax(t_info *info);

// Signals
void	ft_set_signals(void);

// ARP Spoofing
void	ft_set_arp_spoof(t_info info);
void	ft_set_device(t_info *info, struct sockaddr_ll *dev);
void	ft_send_arp(t_info info, int sock, struct ether_arp *recv_arp, struct ether_header *recv_eth);
void	ft_recv(int sock, char *recv_buffer, size_t buf_size);
#endif
