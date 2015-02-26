  /*
  * Copyright (c) 2001-2003, Adam Dunkels.
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
  * $Id: uip.h,v 1.40 2006/06/08 07:12:07 adam Exp $
  *
  */
 
 #ifndef __UIP_H__
 #define __UIP_H__
 
 #include "uipopt.h"
 
 typedef u16_t uip_ip4addr_t[2];
 typedef u16_t uip_ip6addr_t[8];
 #if UIP_CONF_IPV6
 typedef uip_ip6addr_t uip_ipaddr_t;
 #else /* UIP_CONF_IPV6 */
 typedef uip_ip4addr_t uip_ipaddr_t;
 #endif /* UIP_CONF_IPV6 */
 
 /*---------------------------------------------------------------------------*/
 /* First, the functions that should be called from the
  * system. Initialization, the periodic timer and incoming packets are
  * handled by the following three functions.
  */
 
 #define uip_sethostaddr(addr) uip_ipaddr_copy(uip_hostaddr, (addr))
 
 #define uip_gethostaddr(addr) uip_ipaddr_copy((addr), uip_hostaddr)
 
 #define uip_setdraddr(addr) uip_ipaddr_copy(uip_draddr, (addr))
 
 #define uip_setnetmask(addr) uip_ipaddr_copy(uip_netmask, (addr))
 
 
 #define uip_getdraddr(addr) uip_ipaddr_copy((addr), uip_draddr)
 
 #define uip_getnetmask(addr) uip_ipaddr_copy((addr), uip_netmask)
 
 void uip_init(void);
 
 void uip_setipid(u16_t id);
 
 #define uip_input()        uip_process(UIP_DATA)
 
 #define uip_periodic(conn) do { uip_conn = &uip_conns[conn]; \
                                 uip_process(UIP_TIMER); } while (0)
 
 #define uip_conn_active(conn) (uip_conns[conn].tcpstateflags != UIP_CLOSED)
 
 #define uip_periodic_conn(conn) do { uip_conn = conn; \
                                      uip_process(UIP_TIMER); } while (0)
 
 #define uip_poll_conn(conn) do { uip_conn = conn; \
                                  uip_process(UIP_POLL_REQUEST); } while (0)
 
 
 #if UIP_UDP
 
 #define uip_udp_periodic(conn) do { uip_udp_conn = &uip_udp_conns[conn]; \
                                 uip_process(UIP_UDP_TIMER); } while (0)
 
 #define uip_udp_periodic_conn(conn) do { uip_udp_conn = conn; \
                                          uip_process(UIP_UDP_TIMER); } while (0)
 
 
 #endif /* UIP_UDP */
 
 #ifndef UIP_CONF_EXTERNAL_BUFFER
         extern u8_t uip_buf[UIP_BUFSIZE+2];
 #else
         extern unsigned char *uip_buf;
 #endif
 
 /*---------------------------------------------------------------------------*/
 /* Functions that are used by the uIP application program. Opening and
  * closing connections, sending and receiving data, etc. is all
  * handled by the functions below.
 */
 void uip_listen(u16_t port);
 
 void uip_unlisten(u16_t port);
 
 struct uip_conn *uip_connect(uip_ipaddr_t *ripaddr, u16_t port);
 
 
 
 #define uip_outstanding(conn) ((conn)->len)
 
 void uip_send(const void *data, int len);
 
 /*void uip_datalen(void);*/
 #define uip_datalen()       uip_len
 
 #define uip_urgdatalen()    uip_urglen
 
 #define uip_close()         (uip_flags = UIP_CLOSE)
 
 #define uip_abort()         (uip_flags = UIP_ABORT)
 
 #define uip_stop()          (uip_conn->tcpstateflags |= UIP_STOPPED)
 
 #define uip_stopped(conn)   ((conn)->tcpstateflags & UIP_STOPPED)
 
 #define uip_restart()         do { uip_flags |= UIP_NEWDATA; \
                                    uip_conn->tcpstateflags &= ~UIP_STOPPED; \
                               } while(0)
 
 
 /* uIP tests that can be made to determine in what state the current
    connection is, and what the application function should do. */
 
 #define uip_udpconnection() (uip_conn == NULL)
 
 #define uip_newdata()   (uip_flags & UIP_NEWDATA)
 
 #define uip_acked()   (uip_flags & UIP_ACKDATA)
 
 #define uip_connected() (uip_flags & UIP_CONNECTED)
 
 #define uip_closed()    (uip_flags & UIP_CLOSE)
 
 #define uip_aborted()    (uip_flags & UIP_ABORT)
 
 #define uip_timedout()    (uip_flags & UIP_TIMEDOUT)
 
 #define uip_rexmit()     (uip_flags & UIP_REXMIT)
 
 #define uip_poll()       (uip_flags & UIP_POLL)
 
 #define uip_initialmss()             (uip_conn->initialmss)
 
 #define uip_mss()             (uip_conn->mss)
 
 struct uip_udp_conn *uip_udp_new(uip_ipaddr_t *ripaddr, u16_t rport);
 
 #define uip_udp_remove(conn) (conn)->lport = 0
 
 #define uip_udp_bind(conn, port) (conn)->lport = port
 
 #define uip_udp_send(len) uip_send((char *)uip_appdata, len)
 
 /* uIP convenience and converting functions. */
 
 #define uip_ipaddr(addr, addr0,addr1,addr2,addr3) do { \
                      ((u16_t *)(addr))[0] = HTONS(((addr0) << 8) | (addr1)); \
                      ((u16_t *)(addr))[1] = HTONS(((addr2) << 8) | (addr3)); \
                   } while(0)
 
 #define uip_ip6addr(addr, addr0,addr1,addr2,addr3,addr4,addr5,addr6,addr7) do { \
                      ((u16_t *)(addr))[0] = HTONS((addr0)); \
                      ((u16_t *)(addr))[1] = HTONS((addr1)); \
                      ((u16_t *)(addr))[2] = HTONS((addr2)); \
                      ((u16_t *)(addr))[3] = HTONS((addr3)); \
                      ((u16_t *)(addr))[4] = HTONS((addr4)); \
                      ((u16_t *)(addr))[5] = HTONS((addr5)); \
                      ((u16_t *)(addr))[6] = HTONS((addr6)); \
                      ((u16_t *)(addr))[7] = HTONS((addr7)); \
                   } while(0)
 
 #if !UIP_CONF_IPV6
 #define uip_ipaddr_copy(dest, src) do { \
                      ((u16_t *)dest)[0] = ((u16_t *)src)[0]; \
                      ((u16_t *)dest)[1] = ((u16_t *)src)[1]; \
                   } while(0)
 #else /* !UIP_CONF_IPV6 */
 #define uip_ipaddr_copy(dest, src) memcpy(dest, src, sizeof(uip_ip6addr_t))
 #endif /* !UIP_CONF_IPV6 */
 
 #if !UIP_CONF_IPV6
 #define uip_ipaddr_cmp(addr1, addr2) (((u16_t *)addr1)[0] == ((u16_t *)addr2)[0] && \
                                       ((u16_t *)addr1)[1] == ((u16_t *)addr2)[1])
 #else /* !UIP_CONF_IPV6 */
 #define uip_ipaddr_cmp(addr1, addr2) (memcmp(addr1, addr2, sizeof(uip_ip6addr_t)) == 0)
 #endif /* !UIP_CONF_IPV6 */
 
 #define uip_ipaddr_maskcmp(addr1, addr2, mask) \
                           (((((u16_t *)addr1)[0] & ((u16_t *)mask)[0]) == \
                             (((u16_t *)addr2)[0] & ((u16_t *)mask)[0])) && \
                            ((((u16_t *)addr1)[1] & ((u16_t *)mask)[1]) == \
                             (((u16_t *)addr2)[1] & ((u16_t *)mask)[1])))
 
 
 #define uip_ipaddr_mask(dest, src, mask) do { \
                      ((u16_t *)dest)[0] = ((u16_t *)src)[0] & ((u16_t *)mask)[0]; \
                      ((u16_t *)dest)[1] = ((u16_t *)src)[1] & ((u16_t *)mask)[1]; \
                   } while(0)
 
 #define uip_ipaddr1(addr) (htons(((u16_t *)(addr))[0]) >> 8)
 
 #define uip_ipaddr2(addr) (htons(((u16_t *)(addr))[0]) & 0xff)
 
 #define uip_ipaddr3(addr) (htons(((u16_t *)(addr))[1]) >> 8)
 
 #define uip_ipaddr4(addr) (htons(((u16_t *)(addr))[1]) & 0xff)
 
 #ifndef HTONS
 #   if UIP_BYTE_ORDER == UIP_BIG_ENDIAN
 #      define HTONS(n) (n)
 #   else /* UIP_BYTE_ORDER == UIP_BIG_ENDIAN */
 #      define HTONS(n) (u16_t)((((u16_t) (n)) << 8) | (((u16_t) (n)) >> 8))
 #   endif /* UIP_BYTE_ORDER == UIP_BIG_ENDIAN */
 #else
 #error "HTONS already defined!"
 #endif /* HTONS */
 
 #ifndef htons
 u16_t htons(u16_t val);
 #endif /* htons */
 #ifndef ntohs
 #define ntohs htons
 #endif
 
 extern void *uip_appdata;
 
 #if UIP_URGDATA > 0
 /* u8_t *uip_urgdata:
  *
  * This pointer points to any urgent data that has been received. Only
  * present if compiled with support for urgent data (UIP_URGDATA).
  */
 extern void *uip_urgdata;
 #endif /* UIP_URGDATA > 0 */
 
 
 extern u16_t uip_len;
 
 #if UIP_URGDATA > 0
 extern u16_t uip_urglen, uip_surglen;
 #endif /* UIP_URGDATA > 0 */
 
 
 struct uip_conn {
   uip_ipaddr_t ripaddr;   
   u16_t lport;        
   u16_t rport;        
   u8_t rcv_nxt[4];    
   u8_t snd_nxt[4];    
   u16_t len;          
   u16_t mss;          
   u16_t initialmss;   
   u8_t sa;            
   u8_t sv;            
   u8_t rto;           
   u8_t tcpstateflags; 
   u8_t timer;         
   u8_t nrtx;          
   uip_tcp_appstate_t appstate;
 };
 
 
 extern struct uip_conn *uip_conn;
 /* The array containing all uIP connections. */
 extern struct uip_conn uip_conns[UIP_CONNS];
 extern u8_t uip_acc32[4];
 
 #if UIP_UDP
 
 struct uip_udp_conn {
   uip_ipaddr_t ripaddr;   
   u16_t lport;        
   u16_t rport;        
   u8_t  ttl;          
   uip_udp_appstate_t appstate;
 };
 
 extern struct uip_udp_conn *uip_udp_conn;
 extern struct uip_udp_conn uip_udp_conns[UIP_UDP_CONNS];
 #endif /* UIP_UDP */
 
 struct uip_stats {
   struct {
     uip_stats_t drop;     
     uip_stats_t recv;     
     uip_stats_t sent;     
     uip_stats_t vhlerr;   
     uip_stats_t hblenerr; 
     uip_stats_t lblenerr; 
     uip_stats_t fragerr;  
     uip_stats_t chkerr;   
     uip_stats_t protoerr; 
   } ip;                   
   struct {
     uip_stats_t drop;     
     uip_stats_t recv;     
     uip_stats_t sent;     
     uip_stats_t typeerr;  
   } icmp;                 
   struct {
     uip_stats_t drop;     
     uip_stats_t recv;     
     uip_stats_t sent;     
     uip_stats_t chkerr;   
     uip_stats_t ackerr;   
     uip_stats_t rst;      
     uip_stats_t rexmit;   
     uip_stats_t syndrop;  
     uip_stats_t synrst;   
   } tcp;                  
 #if UIP_UDP
   struct {
     uip_stats_t drop;     
     uip_stats_t recv;     
     uip_stats_t sent;     
     uip_stats_t chkerr;   
   } udp;                  
 #endif /* UIP_UDP */
 };
 
 extern struct uip_stats uip_stat;
 
 
 /*---------------------------------------------------------------------------*/
 /* All the stuff below this point is internal to uIP and should not be
  * used directly by an application or by a device driver.
  */
 /*---------------------------------------------------------------------------*/
 /* u8_t uip_flags:
  *
  * When the application is called, uip_flags will contain the flags
  * that are defined in this file. Please read below for more
  * infomation.
  */
 extern u8_t uip_flags;
 
 /* The following flags may be set in the global variable uip_flags
    before calling the application callback. The UIP_ACKDATA,
    UIP_NEWDATA, and UIP_CLOSE flags may both be set at the same time,
    whereas the others are mutualy exclusive. Note that these flags
    should *NOT* be accessed directly, but only through the uIP
    functions/macros. */
 
 #define UIP_ACKDATA   1     /* Signifies that the outstanding data was
                                acked and the application should send
                                out new data instead of retransmitting
                                the last data. */
 #define UIP_NEWDATA   2     /* Flags the fact that the peer has sent
                                us new data. */
 #define UIP_REXMIT    4     /* Tells the application to retransmit the
                                data that was last sent. */
 #define UIP_POLL      8     /* Used for polling the application, to
                                check if the application has data that
                                it wants to send. */
 #define UIP_CLOSE     16    /* The remote host has closed the
                                connection, thus the connection has
                                gone away. Or the application signals
                                that it wants to close the
                                connection. */
 #define UIP_ABORT     32    /* The remote host has aborted the
                                connection, thus the connection has
                                gone away. Or the application signals
                                that it wants to abort the
                                connection. */
 #define UIP_CONNECTED 64    /* We have got a connection from a remote
                                host and have set up a new connection
                                for it, or an active connection has
                                been successfully established. */
 
 #define UIP_TIMEDOUT  128   /* The connection has been aborted due to
                                too many retransmissions. */
 
 /* uip_process(flag):
  *
  * The actual uIP function which does all the work.
  */
 void uip_process(u8_t flag);
 
 /* The following flags are passed as an argument to the uip_process()
    function. They are used to distinguish between the two cases where
    uip_process() is called. It can be called either because we have
    incoming data that should be processed, or because the periodic
    timer has fired. These values are never used directly, but only in
    the macrose defined in this file. */
 
 #define UIP_DATA          1     /* Tells uIP that there is incoming
                                    data in the uip_buf buffer. The
                                    length of the data is stored in the
                                    global variable uip_len. */
 #define UIP_TIMER         2     /* Tells uIP that the periodic timer
                                    has fired. */
 #define UIP_POLL_REQUEST  3     /* Tells uIP that a connection should
                                    be polled. */
 #define UIP_UDP_SEND_CONN 4     /* Tells uIP that a UDP datagram
                                    should be constructed in the
                                    uip_buf buffer. */
 #if UIP_UDP
 #define UIP_UDP_TIMER     5
 #endif /* UIP_UDP */
 
 /* The TCP states used in the uip_conn->tcpstateflags. */
 #define UIP_CLOSED      0
 #define UIP_SYN_RCVD    1
 #define UIP_SYN_SENT    2
 #define UIP_ESTABLISHED 3
 #define UIP_FIN_WAIT_1  4
 #define UIP_FIN_WAIT_2  5
 #define UIP_CLOSING     6
 #define UIP_TIME_WAIT   7
 #define UIP_LAST_ACK    8
 #define UIP_TS_MASK     15
 
 #define UIP_STOPPED      16
 
 /* The TCP and IP headers. */
 
 #ifdef __ICCARM__
         #pragma pack(1)
 #endif
 
 
 #if defined ( __CC_ARM   )
 __packed
 #elif defined   (  __GNUC__  )
 __attribute__ ((__packed__))
 #endif
 struct uip_tcpip_hdr {
 #if UIP_CONF_IPV6
   /* IPv6 header. */
   u8_t vtc,
     tcflow;
   u16_t flow;
   u8_t len[2];
   u8_t proto, ttl;
   uip_ip6addr_t srcipaddr, destipaddr;
 #else /* UIP_CONF_IPV6 */
   /* IPv4 header. */
   u8_t vhl,
     tos,
     len[2],
     ipid[2],
     ipoffset[2],
     ttl,
     proto;
   u16_t ipchksum;
   u16_t srcipaddr[2],
     destipaddr[2];
 #endif /* UIP_CONF_IPV6 */
 
   /* TCP header. */
   u16_t srcport,
     destport;
   u8_t seqno[4],
     ackno[4],
     tcpoffset,
     flags,
     wnd[2];
   u16_t tcpchksum;
   u8_t urgp[2];
   u8_t optdata[4];
 } /*PACK_STRUCT_END*/;
 
 #ifdef __ICCARM__
         #pragma pack()
 #endif
 
 /* The ICMP and IP headers. */
 #ifdef __ICCARM__
         #pragma pack(1)
 #endif
 
 
 #if defined ( __CC_ARM   )
 __packed
 #elif defined   (  __GNUC__  )
 __attribute__ ((__packed__))
 #endif
 struct uip_icmpip_hdr {
 #if UIP_CONF_IPV6
   /* IPv6 header. */
   u8_t vtc,
     tcf;
   u16_t flow;
   u8_t len[2];
   u8_t proto, ttl;
   uip_ip6addr_t srcipaddr, destipaddr;
 #else /* UIP_CONF_IPV6 */
   /* IPv4 header. */
   u8_t vhl,
     tos,
     len[2],
     ipid[2],
     ipoffset[2],
     ttl,
     proto;
   u16_t ipchksum;
   u16_t srcipaddr[2],
     destipaddr[2];
 #endif /* UIP_CONF_IPV6 */
 
   /* ICMP (echo) header. */
   u8_t type, icode;
   u16_t icmpchksum;
 #if !UIP_CONF_IPV6
   u16_t id, seqno;
 #else /* !UIP_CONF_IPV6 */
   u8_t flags, reserved1, reserved2, reserved3;
   u8_t icmp6data[16];
   u8_t options[1];
 #endif /* !UIP_CONF_IPV6 */
 } /*PACK_STRUCT_END*/;
 
 #ifdef __ICCARM__
         #pragma pack()
 #endif
 
 
 /* The UDP and IP headers. */
 #ifdef __ICCARM__
         #pragma pack(1)
 #endif
 
 
 #if defined ( __CC_ARM   )
 __packed
 #elif defined   (  __GNUC__  )
 __attribute__ ((__packed__))
 #endif
 struct uip_udpip_hdr {
 #if UIP_CONF_IPV6
   /* IPv6 header. */
   u8_t vtc,
     tcf;
   u16_t flow;
   u8_t len[2];
   u8_t proto, ttl;
   uip_ip6addr_t srcipaddr, destipaddr;
 #else /* UIP_CONF_IPV6 */
   /* IP header. */
   u8_t vhl,
     tos,
     len[2],
     ipid[2],
     ipoffset[2],
     ttl,
     proto;
   u16_t ipchksum;
   u16_t srcipaddr[2],
     destipaddr[2];
 #endif /* UIP_CONF_IPV6 */
 
   /* UDP header. */
   u16_t srcport,
     destport;
   u16_t udplen;
   u16_t udpchksum;
 } /*PACK_STRUCT_END*/;
 
 #ifdef __ICCARM__
         #pragma pack()
 #endif
 
 
 
 #define UIP_APPDATA_SIZE (UIP_BUFSIZE - UIP_LLH_LEN - UIP_TCPIP_HLEN)
 
 
 #define UIP_PROTO_ICMP  1
 #define UIP_PROTO_TCP   6
 #define UIP_PROTO_UDP   17
 #define UIP_PROTO_ICMP6 58
 
 /* Header sizes. */
 #if UIP_CONF_IPV6
 #define UIP_IPH_LEN    40
 #else /* UIP_CONF_IPV6 */
 #define UIP_IPH_LEN    20    /* Size of IP header */
 #endif /* UIP_CONF_IPV6 */
 #define UIP_UDPH_LEN    8    /* Size of UDP header */
 #define UIP_TCPH_LEN   20    /* Size of TCP header */
 #define UIP_IPUDPH_LEN (UIP_UDPH_LEN + UIP_IPH_LEN)    /* Size of IP +
                                                           UDP
                                                           header */
 #define UIP_IPTCPH_LEN (UIP_TCPH_LEN + UIP_IPH_LEN)    /* Size of IP +
                                                           TCP
                                                           header */
 #define UIP_TCPIP_HLEN UIP_IPTCPH_LEN
 
 
 #if UIP_FIXEDADDR
 extern const uip_ipaddr_t uip_hostaddr, uip_netmask, uip_draddr;
 #else /* UIP_FIXEDADDR */
 extern uip_ipaddr_t uip_hostaddr, uip_netmask, uip_draddr;
 #endif /* UIP_FIXEDADDR */
 
 
 
 #ifdef __ICCARM__
         #pragma pack(1)
 #endif
 
 
 #if defined ( __CC_ARM   )
 __packed
 #elif defined   (  __GNUC__  )
 __attribute__ ((__packed__))
 #endif
 struct uip_eth_addr {
   u8_t addr[6];
 } /*PACK_STRUCT_END*/;
 
 #ifdef __ICCARM__
         #pragma pack()
 #endif
 
 u16_t uip_chksum(u16_t *buf, u16_t len);
 
 u16_t uip_ipchksum(void);
 
 u16_t uip_tcpchksum(void);
 
 u16_t uip_udpchksum(void);
 
 
 #endif /* __UIP_H__ */
 
 

