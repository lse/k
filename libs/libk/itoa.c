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
#include <stdlib.h>
#include <string.h>

static int conv(unsigned int n, char dig[])
{
	int i = 0;

	while (n) {
		dig[i] = n % 10 + '0';
		++i;
		n /= 10;
	}
	return i - 1;
}

char *itoa(int nb, int base)
{
	char buff[50];
	char *res;
	int i = 0;
	int j;
	int dec = 0;

	(void)base;

	if (!nb)
		buff[0] = '0';
	else if (nb < 0) {
		dec = 1;
		i = conv(-nb, buff);
	} else
		i = conv(nb, buff);
	res = malloc(i * sizeof(char) + 1);
	if (dec)
		res[0] = '-';
	for (j = 0; j <= i; ++j)
		res[dec + j] = buff[i - j];
	res[dec + i * sizeof(char) + 1] = 0;
	return res;
}
