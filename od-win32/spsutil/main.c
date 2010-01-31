
#include <stdio.h>


#include "types.h"
#include "caps_win32.h"

static uae_u16 mfmbuf[20000], tracktiming[20000];

#define FLOPPY_WRITE_LEN 12668

int main(int argc, char *argv[])
{
    int tracks;
    int i;
    int lasttrack;
    char msg[100], prevmsg[100];
    char tmp[100];

    if (argc < 2) {
	printf("capsutil.exe <filename.ipf>\n");
	return 0;
    }

    if (!caps_loadimage(argv[1], 0, &tracks))
	return 0;

    lasttrack = -1;
    prevmsg[0] = 0;
    for (i = 0; i <= tracks; i++) {
	int tracklength, multirev, gapoffset;
	int sp = 0;
	int longtrack = 0, timingtrack = 0, flakeybits = 0;

	msg[0] = 0;
	if (i < tracks) {
	    if (!caps_loadtrack(mfmbuf, tracktiming, 0, i, &tracklength, &multirev, &gapoffset))
		return 0;
	    if (tracklength > FLOPPY_WRITE_LEN * 8) {
		longtrack = 1;
		sp = 1;
		sprintf(tmp, " LT +%02.2d%%", (tracklength * 100 / (FLOPPY_WRITE_LEN * 8)) - 100);
		strcat(msg, tmp);
	    }
	    if (multirev) {
		flakeybits = 1;
		sp = 1;
		strcat(msg, " FB");
	    }
	    if (tracktiming[0]) {
		timingtrack = 1;
		sp = 1;
		strcat(msg, " TT");
	    }
	}
	if (!sp)
	    strcat(msg, " --");

	sprintf(tmp, " [%d]", tracklength);
	strcat(msg, tmp);

	if (strcmp(prevmsg, msg)) {
	    if (lasttrack >= 0)
		printf("%03.3d - %03.3d: %s\n", lasttrack, i - 1, msg);
	    lasttrack = i;
	    strcpy(prevmsg, msg);
	}
    }
    return 0;
}