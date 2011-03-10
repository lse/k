/*
* Copyright (c) LSE
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY LSE AS IS AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL LSE BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <libk.h>

# define COM1 0x03f8
# define COM2 0x02f8
# define COM3 0x03e8
# define COM4 0x02e8
# define MAX_BUF 4096

static const unsigned char kbdus[128] = {0,27,'1','2','3','4','5','6','7','8',
					 '9','0','-','=','\b','\t','q','w','e','r','t','y','u',
					 'i','o','p','[',']','\n',' ','a','s','d','f','g',
					 'h','j','k','l',';','\'','`',0,'\\','z','x','c',
					 'v','b','n','m',',','.','/',' ','*',' ',' ',' ',' ',
					 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
					 ' ','-',' ',' ',' ','+',' ',' ',' ',' ',' ',' ',' ',
					 ' ',' ' };


inline static void set_color(int color)
{
    unsigned char c[3] = { CONS_ESCAPE, CONS_COLOR, color };

    write(c, 3);
}

inline static void chat_loop (char *buf, size_t *i)
{
  int k = 1;
  unsigned int j;

  if ((k = getkey()) != -1)
    {
      if (k == KEY_BACKSPACE)
      	{
	  if (*i == 0)
	    return;
	  write("\r", 1);
	  for (j = 0; j < *i; ++j)
	    write(" ", 1);
	  write("\r", 1);
	  --(*i);
	  write(buf, *i);
	  return;
	}

      buf[(*i)] = kbdus[k];
      ++(*i);
      write(kbdus + k, 1);
      if (k == KEY_ENTER)
      	{
      	  send(COM1, buf, *i);
	  *i = 0;
      	}
    }

  if (*i == MAX_BUF)
    {
      write ("\n", 1);
      send(COM1, buf, *i);
      *i = 0;
    }
}

inline static void reset_pos ()
{
    char c[3] = { CONS_ESCAPE, CONS_SETX, 0 };

    write (c, 3);
    c[1] = CONS_SETY;
    write (c, 3);
}

inline static void clear_screen(void)
{
    char c[2] = { CONS_ESCAPE, CONS_CLEAR };

    write (c, 2);
}

void entry (void)
{
  char buf[MAX_BUF];
  char buf2[MAX_BUF];
  unsigned int i = 0;
  int k = 0;
  t_melody*	intro = load_sound ("/intro.ksf");

  switch_text ();
  playsound (intro, -1);
  write ("Bonjour", 5);
  clear_screen();
  reset_pos ();

  while (1)
    {
      chat_loop(buf, &i);
      if ((k = recv (COM1, buf2, MAX_BUF)) > 0)
      	{
      	  set_color(CONS_FRONT(CONS_CYAN) | CONS_FRONT(CONS_LIGHT));
      	  write ("==>", 3);
      	  set_color(CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_BLACK));
      	  write (buf2, k);
      	}
    }
}
