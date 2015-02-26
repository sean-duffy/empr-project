/*opyright (c) 2004, Swedish Institute of Computer Science.
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
 * $Id: psock.h,v 1.3 2006/06/12 08:00:30 adam Exp $
 */

#ifndef __PSOCK_H__
#define __PSOCK_H__

#include "uipopt.h"
#include "pt.h"

 /*
 * The structure that holds the state of a buffer.
 *
 * This structure holds the state of a uIP buffer. The structure has
 * no user-visible elements, but is used through the functions
 * provided by the library.
 *
 */
struct psock_buf {
  u8_t *ptr;
  unsigned short left;
};

struct psock {
  struct pt pt, psockpt; /* Protothreads - one that's using the psock
                            functions, and one that runs inside the
                            psock functions. */
  const u8_t *sendptr;   /* Pointer to the next data to be sent. */
  u8_t *readptr;         /* Pointer to the next data to be read. */

  char *bufptr;          /* Pointer to the buffer used for buffering
                            incoming data. */

  u16_t sendlen;         /* The number of bytes left to be sent. */
  u16_t readlen;         /* The number of bytes left to be read. */

  struct psock_buf buf;  /* The structure holding the state of the
                            input buffer. */
  unsigned int bufsize;  /* The size of the input buffer. */

  unsigned char state;   /* The state of the protosocket. */
};

void psock_init(struct psock *psock, char *buffer, unsigned int buffersize);
#define PSOCK_INIT(psock, buffer, buffersize) \
  psock_init(psock, buffer, buffersize)

#define PSOCK_BEGIN(psock) PT_BEGIN(&((psock)->pt))

PT_THREAD(psock_send(struct psock *psock, const char *buf, unsigned int len));
#define PSOCK_SEND(psock, data, datalen)                \
    PT_WAIT_THREAD(&((psock)->pt), psock_send(psock, data, datalen))

#define PSOCK_SEND_STR(psock, str)                      \
    PT_WAIT_THREAD(&((psock)->pt), psock_send(psock, str, strlen(str)))

PT_THREAD(psock_generator_send(struct psock *psock,
                                unsigned short (*f)(void *), void *arg));

#define PSOCK_GENERATOR_SEND(psock, generator, arg)     \
    PT_WAIT_THREAD(&((psock)->pt),                                      \
                   psock_generator_send(psock, generator, arg))


#define PSOCK_CLOSE(psock) uip_close()

PT_THREAD(psock_readbuf(struct psock *psock));
#define PSOCK_READBUF(psock)                            \
  PT_WAIT_THREAD(&((psock)->pt), psock_readbuf(psock))

PT_THREAD(psock_readto(struct psock *psock, unsigned char c));
#define PSOCK_READTO(psock, c)                          \
  PT_WAIT_THREAD(&((psock)->pt), psock_readto(psock, c))

#define PSOCK_DATALEN(psock) psock_datalen(psock)

u16_t psock_datalen(struct psock *psock);

#define PSOCK_EXIT(psock) PT_EXIT(&((psock)->pt))

#define PSOCK_CLOSE_EXIT(psock)         \
  do {                                          \
    PSOCK_CLOSE(psock);                 \
    PSOCK_EXIT(psock);                  \
  } while(0)

#define PSOCK_END(psock) PT_END(&((psock)->pt))

char psock_newdata(struct psock *s);

#define PSOCK_NEWDATA(psock) psock_newdata(psock)

#define PSOCK_WAIT_UNTIL(psock, condition)    \
  PT_WAIT_UNTIL(&((psock)->pt), (condition));

#define PSOCK_WAIT_THREAD(psock, condition)   \
  PT_WAIT_THREAD(&((psock)->pt), (condition))

#endif /* __PSOCK_H__ */

