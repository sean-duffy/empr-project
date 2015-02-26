/*opyright (c) 2002, Adam Dunkels.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions
  * are met:
  * 1. Redistributions of source code must retain the above copyright
  *    notice, this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above
  *    copyright notice, this list of conditions and the following
  *    disclaimer in the documentation and/or other materials provided
  *    with the distribution.
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
  * $Id: webclient.h,v 1.2 2006/06/11 21:46:37 adam Exp $
  *
  */
 #ifndef __WEBCLIENT_H__
 #define __WEBCLIENT_H__
 
 
 #include "webclient-strings.h"
 #include "uipopt.h"
 
 #define WEBCLIENT_CONF_MAX_URLLEN 100
 
 struct webclient_state {
   u8_t timer;
   u8_t state;
   u8_t httpflag;
 
   u16_t port;
   char host[40];
   char file[WEBCLIENT_CONF_MAX_URLLEN];
   u16_t getrequestptr;
   u16_t getrequestleft;
   
   char httpheaderline[200];
   u16_t httpheaderlineptr;
 
   char mimetype[32];
 };
 
 typedef struct webclient_state uip_tcp_appstate_t;
 #define UIP_APPCALL webclient_appcall
 
 void webclient_datahandler(char *data, u16_t len);
 
 void webclient_connected(void);
 
 void webclient_timedout(void);
 
 void webclient_aborted(void);
 
 void webclient_closed(void);
 
 
 
 void webclient_init(void);
 
 unsigned char webclient_get(char *host, u16_t port, char *file);
 
 void webclient_close(void);
 void webclient_appcall(void);
 
 char *webclient_mimetype(void);
 
 char *webclient_filename(void);
 
 char *webclient_hostname(void);
 
 unsigned short webclient_port(void);
 
 
 
 #endif /* __WEBCLIENT_H__ */
 
