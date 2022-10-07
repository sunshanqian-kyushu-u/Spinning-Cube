#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <math.h>
#include <signal.h>

/* about fb0 */
#include <linux/fb.h>
#include <sys/mman.h>

#include "spin.h"

int main()
{
    int fp = 0;
    long screensize=0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    unsigned char *mem;

    fp = open("/dev/fb0", O_RDWR);
    if (fp < 0)
    {
        printf("Error : Open framebuffer device failed!\r\n");
        goto fail_open_fb;
    }

    if (ioctl(fp, FBIOGET_FSCREENINFO, &finfo))
    {
        printf("Error: Read fixed info failed!\r\n");
        goto fail_get_finfo;
    }

    if (ioctl(fp, FBIOGET_VSCREENINFO, &vinfo))
    {
        printf("Error: Read variable info failed!\r\n");
        goto fail_get_vinfo;
    }

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    /* map fb0 to memory */
    mem =(char *) mmap (NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fp, 0);
    if ((int) mem == -1)
    {
       printf ("Error: Map framebuffer device to memory failed!\r\n");
       goto fail_map_mem;
    }

    /* clear screen */
    memset(mem, 0x00, screensize);

    signal(SIGINT, signal_handler);
    spin(mem);

    munmap (mem, screensize); /* unmap */

fail_map_mem:

fail_get_vinfo:
fail_get_finfo:
    close(fp);
    
fail_open_fb:
    printf ("Normal exit succeed!\r\n");
    return 0;
}