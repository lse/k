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
#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *fin = NULL;
	FILE *fout = NULL;
	unsigned int nb, tone, duration;
	char *magic = ".KSF";

	if (argc != 3) {
		fprintf(stderr, "%s", "ksf 1.0\n");
		fprintf(stderr, "Usage: %s in.txt out.ksf\n", argv[0]);
		return (1);
	}

	if (!(fin = fopen(argv[1], "r")) || !(fout = fopen(argv[2], "w"))) {
		perror("fopen");
		return (1);
	}
	// dump magic number
	fwrite(magic, 4, 1, fout);
	// reserves four bytes to store number of tones
	fwrite(&nb, 4, 1, fout);

	for (nb = 0; !feof(fin); nb++) {
		fscanf(fin, "%d%d", &tone, &duration);
		fwrite(&tone, 4, 1, fout);
		fwrite(&duration, 4, 1, fout);
	}

	// seek back to the number of tones
	fseek(fout, 4, SEEK_SET);
	// write the new value
	fwrite(&nb, sizeof(int), 1, fout);

	fclose(fout);
	fclose(fin);

	return (0);
}
