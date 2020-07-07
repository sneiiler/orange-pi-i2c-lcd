/*
 * Copyright (c) 2015, Vladimir Komendantskiy
 * MIT License
 *
 * SSD1306 demo of block and font drawing.
 */

//
// fixed for OrangePiZero by HypHop
//

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdbool.h>

#define IP_SIZE 16

#include "oled.h"
#include "font.h"

int oled_demo(struct display_info *disp)
{

    oled_putstrto(disp, 12, 27 - 4 + 4, "Initializing...");

    oled_send_buffer(disp);

    return 0;
}

void show_error(int err, int add)
{
    //const gchar* errmsg;
    //errmsg = g_strerror(errno);
    printf("\nERROR: %i, %i\n\n", err, add);
    //printf("\nERROR\n");
}

void show_usage(char *progname)
{
    printf("\nUsage:\n%s <I2C bus device node >\n", progname);
}

int main(int argc, char **argv)
{

    // ----------
    char filename[32];
    struct display_info disp;

    // if (argc < 2)
    // {
    // 	show_usage(argv[0]);
    // 	return -1;
    // }

    memset(&disp, 0, sizeof(disp));

    sprintf(filename, "%s", "/dev/i2c-0");
    disp.address = OLED_I2C_ADDR;
    disp.font = font2;

    //	if (oled_open(&disp, filename) < 0 ||
    //	    oled_init(&disp)           < 0 ||
    //	    oled_demo(&disp)           < 0)
    //	{

    int e;

    e = oled_open(&disp, filename);

    if (e < 0)
    {
        show_error(1, e);
    }
    else
    {
        e = oled_init(&disp);
        if (e < 0)
        {
            show_error(2, e);
        }
        else
        {

            printf("-----------------\n");

            if (oled_demo(&disp) < 0)
            {
                show_error(3, 777);
            }
        }
    }

    return 0;
}
