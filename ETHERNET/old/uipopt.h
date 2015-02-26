/*opyright (c) 2001-2003, Adam Dunkels.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions
  * are met:
  * 1. Redistributions of source code must retain the above copyright
  *    notice, this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright
  *    notice, this list of conditions and the following disclaimer in the
  *    documentation and/or other materials provided with the distribution.
  * 3. The name of the author may not be used to endorse or promote
  *    products derived from this software without specific prior
  *    written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
  * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
  * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  * This file is part of the uIP TCP/IP stack.
  *
  * $Id: uipopt.h,v 1.4 2006/06/12 08:00:31 adam Exp $
  *
  */
 
 #ifndef __UIPOPT_H__
 #define __UIPOPT_H__
 
 #ifndef UIP_LITTLE_ENDIAN
 #define UIP_LITTLE_ENDIAN  3412
 #endif /* UIP_LITTLE_ENDIAN */
 #ifndef UIP_BIG_ENDIAN
 #define UIP_BIG_ENDIAN     1234
 #endif /* UIP_BIG_ENDIAN */
 
 #include "uip-conf.h"
 
 /*------------------------------------------------------------------------------*/
 
 #define UIP_FIXEDADDR    0
 
 #ifdef UIP_CONF_PINGADDRCONF
 #define UIP_PINGADDRCONF UIP_CONF_PINGADDRCONF
 #else /* UIP_CONF_PINGADDRCONF */
 #define UIP_PINGADDRCONF 0
 #endif /* UIP_CONF_PINGADDRCONF */
 
 
 #define UIP_FIXEDETHADDR 0
 
 /*------------------------------------------------------------------------------*/
 #define UIP_TTL         64
 
 #define UIP_REASSEMBLY 0
 
 #define UIP_REASS_MAXAGE 40
 
 /*------------------------------------------------------------------------------*/
 #ifdef UIP_CONF_UDP
 #define UIP_UDP UIP_CONF_UDP
 #else /* UIP_CONF_UDP */
 #define UIP_UDP           0
 #endif /* UIP_CONF_UDP */
 
 #ifdef UIP_CONF_UDP_CHECKSUMS
 #define UIP_UDP_CHECKSUMS UIP_CONF_UDP_CHECKSUMS
 #else
 #define UIP_UDP_CHECKSUMS 0
 #endif
 
 #ifdef UIP_CONF_UDP_CONNS
 #define UIP_UDP_CONNS UIP_CONF_UDP_CONNS
 #else /* UIP_CONF_UDP_CONNS */
 #define UIP_UDP_CONNS    10
 #endif /* UIP_CONF_UDP_CONNS */
 
 /*------------------------------------------------------------------------------*/
 #define UIP_ACTIVE_OPEN 1
 
 #ifndef UIP_CONF_MAX_CONNECTIONS
 #define UIP_CONNS       10
 #else /* UIP_CONF_MAX_CONNECTIONS */
 #define UIP_CONNS UIP_CONF_MAX_CONNECTIONS
 #endif /* UIP_CONF_MAX_CONNECTIONS */
 
 
 #ifndef UIP_CONF_MAX_LISTENPORTS
 #define UIP_LISTENPORTS 20
 #else /* UIP_CONF_MAX_LISTENPORTS */
 #define UIP_LISTENPORTS UIP_CONF_MAX_LISTENPORTS
 #endif /* UIP_CONF_MAX_LISTENPORTS */
 
 #define UIP_URGDATA      0
 
 #define UIP_RTO         3
 
 #define UIP_MAXRTX      8
 
 #define UIP_MAXSYNRTX      5
 
 #define UIP_TCP_MSS     (UIP_BUFSIZE - UIP_LLH_LEN - UIP_TCPIP_HLEN)
 
 #ifndef UIP_CONF_RECEIVE_WINDOW
 #define UIP_RECEIVE_WINDOW UIP_TCP_MSS
 #else
 #define UIP_RECEIVE_WINDOW UIP_CONF_RECEIVE_WINDOW
 #endif
 
 #define UIP_TIME_WAIT_TIMEOUT 120
 
 
 /*------------------------------------------------------------------------------*/
 #ifdef UIP_CONF_ARPTAB_SIZE
 #define UIP_ARPTAB_SIZE UIP_CONF_ARPTAB_SIZE
 #else
 #define UIP_ARPTAB_SIZE 8
 #endif
 
 #define UIP_ARP_MAXAGE 120
 
 /*------------------------------------------------------------------------------*/
 
 #ifndef UIP_CONF_BUFFER_SIZE
 #define UIP_BUFSIZE     1500
 #else /* UIP_CONF_BUFFER_SIZE */
 #define UIP_BUFSIZE UIP_CONF_BUFFER_SIZE
 #endif /* UIP_CONF_BUFFER_SIZE */
 
 
 #ifndef UIP_CONF_STATISTICS
 #define UIP_STATISTICS  0
 #else /* UIP_CONF_STATISTICS */
 #define UIP_STATISTICS UIP_CONF_STATISTICS
 #endif /* UIP_CONF_STATISTICS */
 
 #ifndef UIP_CONF_LOGGING
 #define UIP_LOGGING     0
 #else /* UIP_CONF_LOGGING */
 #define UIP_LOGGING     UIP_CONF_LOGGING
 #endif /* UIP_CONF_LOGGING */
 
 #ifndef UIP_CONF_BROADCAST
 #define UIP_BROADCAST 0
 #else /* UIP_CONF_BROADCAST */
 #define UIP_BROADCAST UIP_CONF_BROADCAST
 #endif /* UIP_CONF_BROADCAST */
 
 void uip_log(char *msg);
 
 #ifdef UIP_CONF_LLH_LEN
 #define UIP_LLH_LEN UIP_CONF_LLH_LEN
 #else /* UIP_CONF_LLH_LEN */
 #define UIP_LLH_LEN     14
 #endif /* UIP_CONF_LLH_LEN */
 
 /*------------------------------------------------------------------------------*/
 #ifdef UIP_CONF_BYTE_ORDER
 #define UIP_BYTE_ORDER     UIP_CONF_BYTE_ORDER
 #else /* UIP_CONF_BYTE_ORDER */
 #define UIP_BYTE_ORDER     UIP_LITTLE_ENDIAN
 #endif /* UIP_CONF_BYTE_ORDER */
 
 /*------------------------------------------------------------------------------*/
 
 #endif /* __UIPOPT_H__ */
