#include <ioLib.h>
#include <clock.h>
#include <time.h>

#include "main.h"
#include "macros.h"

#include "float.h"
#include "languages.h"
#include "settings.h"
#include "debug.h"

#include "firmware.h"
#include "firmware/camera.h"

#include "utils.h"
#include "bmp.h"

#include "screenshot.h"

static void screenshot_task(void) {
    beep();

    // Wait before capturing the screen
    int i;
    for (i = 0; i < 10; ++i) {
        LEDBLUE ^= 2;
        SleepTask(500);
    }

    // Default filename
    char filename[20] = "A:/12345678.BMP";
    time_t t;
    struct tm tm;

    time(&t);
    localtime_r(&t, &tm);

    // Update the file name with current date to prevent file name conflicts
    sprintf(filename, "A:/%02d%02d%02d%02d.MEM", tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    int file = FIO_OpenFile(filename, O_CREAT | O_WRONLY , 644);

    if (file == -1) {
    } else {
        unsigned char bmpfileheader[14] = {'B','M',0,0,0,0,0,0,0,0,54,0,0,0};
        unsigned char bmpinfoheader[40] = {40,0,0,0,0,0,0,0,0,0,0,0,1,0,24,0};

        int height = 240;
        int width = 360;
        int filesize = 54 + height*width*3;

        bmpfileheader[2]  = (unsigned char)(filesize);
        bmpfileheader[3]  = (unsigned char)(filesize >> 8);
        bmpfileheader[4]  = (unsigned char)(filesize >> 16);
        bmpfileheader[5]  = (unsigned char)(filesize >> 24);

        bmpinfoheader[4]  = (unsigned char)(width);
        bmpinfoheader[5]  = (unsigned char)(width >> 8);
        bmpinfoheader[6]  = (unsigned char)(width >> 16);
        bmpinfoheader[7]  = (unsigned char)(width >> 24);
        bmpinfoheader[8]  = (unsigned char)(height);
        bmpinfoheader[9]  = (unsigned char)(height >> 8);
        bmpinfoheader[10] = (unsigned char)(height >> 16);
        bmpinfoheader[11] = (unsigned char)(height >> 24);

        FIO_WriteFile(file,bmpfileheader,14);
        FIO_WriteFile(file,bmpinfoheader,40);

        int x,y;
        for (y = height; y; --y) {
            char line[1080];

            for (x = 0; x < width; ++x) {
                char current_pixel = VramAddress[x+y*360];
                switch (current_pixel) {
                    case 0x11: // lighter gray
                        line[x*3] = 0xE0;
                        line[x*3+1] = 0xE0;
                        line[x*3+2] = 0xE0;
                        break;
                    case 0x22: // dark gray almost black
                        line[x*3] = 0x26;
                        line[x*3+1] = 0x26;
                        line[x*3+2] = 0x26;
                        break;
                    case 0x33: // light gray
                        line[x*3] = 0xCC;
                        line[x*3+1] = 0xCC;
                        line[x*3+2] = 0xCC;
                        break;
                    case 0x44: // light grey background of menu
                        line[x*3] = 0x7D;
                        line[x*3+1] = 0x7D;
                        line[x*3+2] = 0x7D;
                        break;
                    case 0x55: // light green / lime green
                        line[x*3] = 0x00;
                        line[x*3+1] = 0xFF;
                        line[x*3+2] = 0x8C;
                        break;
                    case 0x66: // red
                        line[x*3] = 0x00;
                        line[x*3+1] = 0x00;
                        line[x*3+2] = 0xFF;
                        break;
                    case 0x77: // brown red / maroon
                        line[x*3] = 0x13;
                        line[x*3+1] = 0x29;
                        line[x*3+2] = 0x61;
                        break;
                    case 0x88: // light blue
                        line[x*3] = 0xFF;
                        line[x*3+1] = 0xE5;
                        line[x*3+2] = 0x00;
                        break;
                    case 0x99: // light gray
                        line[x*3] = 0xCC;
                        line[x*3+1] = 0xCC;
                        line[x*3+2] = 0xCC;
                        break;
                    case 0xAA: // darker gray
                        line[x*3] = 0x40;
                        line[x*3+1] = 0x40;
                        line[x*3+2] = 0x40;
                        break;
                    case 0xBB: // brown red / maroon
                        line[x*3] = 0x13;
                        line[x*3+1] = 0x29;
                        line[x*3+2] = 0x61;
                        break;
                    case 0xCC: // light blue
                        line[x*3] = 0xFF;
                        line[x*3+1] = 0xE5;
                        line[x*3+2] = 0x00;
                        break;
                    case 0xDD: // light orange / pale yellow
                        line[x*3] = 0x00;
                        line[x*3+1] = 0x00;
                        line[x*3+2] = 0xFF;
                        break;
                    case 0xEE: // orange
                        line[x*3] = 0x00;
                        line[x*3+1] = 0x00;
                        line[x*3+2] = 0xFF;
                        break;
                    case 0xFF: // white
                        line[x*3] = 0xFF;
                        line[x*3+1] = 0xFF;
                        line[x*3+2] = 0xFF;
                        break;
                    default:
                        line[x*3] = 0xFF;
                        line[x*3+1] = 0xFF;
                        line[x*3+2] = 0xFF;
                        break;
                }
            }

            FIO_WriteFile(file,line,width*3);
        }

        FIO_CloseFile(file);
    }
}

void take_screenshot(const menuitem_t *menuitem) {
    CreateTask("scrshoot", 0x1e, 0x1000, screenshot_task, 0);
    LEDBLUE ^= 2;
}