/*opyright (c) 2004-2005, Swedish Institute of Computer Science.
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
  * 3. Neither the name of the Institute nor the names of its contributors
  *    may be used to endorse or promote products derived from this software
  *    without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
  * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  * SUCH DAMAGE.
  *
  * This file is part of the uIP TCP/IP stack
  *
  * Author: Adam Dunkels <adam@sics.se>
  *
  * $Id: lc.h,v 1.2 2006/06/12 08:00:30 adam Exp $
  */
 
 #ifdef DOXYGEN
 
 #define LC_INIT(lc)
 
 #define LC_SET(lc)
 
 #define LC_RESUME(lc)
 
 #define LC_END(lc)
 
 #endif /* DOXYGEN */
 
 #ifndef __LC_H__
 #define __LC_H__
 
 #ifdef LC_CONF_INCLUDE
 #include LC_CONF_INCLUDE
 #else
 #include "lc-switch.h"
 #endif /* LC_CONF_INCLUDE */
 
 #endif /* __LC_H__ */
 
