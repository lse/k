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
#include <sound.h>
#include <stdlib.h>

struct melody *load_sound(const char *path)
{
	struct melody *melody = NULL;
	int fd = -1;
	int nb = -1;
	int i = -1;
	char *magic = ".KSF";
	char buf[5];

	if ((fd = open(path, O_RDONLY)) < 0)
		return NULL;

	buf[4] = 0;
	/* check the magic number */
	if ((read(fd, buf, 4) != 4) || (strcmp(magic, buf) != 0)) {
		close(fd);
		return NULL;
	}

	/* read the numbers of tones */
	if (read(fd, &nb, sizeof(int)) != sizeof(int)) {
		close(fd);
		return NULL;
	}

	/* allocate space to store the new melody */
	if (!(melody = malloc((nb + 1) * sizeof(struct melody)))) {
		close(fd);
		return NULL;
	}

	/* load the melody */
	for (i = 0; i < nb; i++) {
		if (read(fd, &melody[i].freq, sizeof(int)) != sizeof(int) ||
		    read(fd, &melody[i].duration, sizeof(int)) != sizeof(int)) {
			close(fd);
			free(melody);
			return NULL;
		}
	}

	/* put a null tones to indicate end of melody */
	melody[nb].freq = 0;
	melody[nb].duration = (unsigned long)-1;

	close(fd);

	return (melody);
}

void clear_sound(struct melody *melody)
{
	free(melody);
}
