#include <stdio.h>
#include <stdint.h>

#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define FB_PATH "/dev/fb0"

#define PRINT_FIELD(fmt, field) printf(#field ":" fmt "\n", vinfo.field)
#define PRINT_FFIELD(fmt, field) printf(#field ":" fmt "\n", finfo.field)

int fb_dev;
uint32_t* fbp;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

int initfb()
{
  fb_dev = open(FB_PATH, O_RDWR);
  if(fb_dev < 0) {
    perror("open");
    return -1;
  }
}

int getfbinfo()
{
  ioctl(fb_dev, FBIOGET_VSCREENINFO, &vinfo);
  ioctl(fb_dev, FBIOGET_FSCREENINFO, &finfo);
    puts("[fb_var_screeninfo]");
    PRINT_FIELD("%u", xres);
    PRINT_FIELD("%u", yres);
    PRINT_FIELD("%u", xres_virtual);
    PRINT_FIELD("%u", yres_virtual);
    PRINT_FIELD("%u", xoffset);
    PRINT_FIELD("%u", yoffset);
    PRINT_FIELD("%u", bits_per_pixel);
    PRINT_FIELD("%u", grayscale);
    PRINT_FIELD("%u", nonstd);
    PRINT_FIELD("%u", activate);
    PRINT_FIELD("%u", height);
    PRINT_FIELD("%u", width);
    PRINT_FIELD("%u", accel_flags);
    PRINT_FIELD("%u", pixclock);
    PRINT_FIELD("%u", left_margin);
    PRINT_FIELD("%u", right_margin);
    PRINT_FIELD("%u", upper_margin);
    PRINT_FIELD("%u", lower_margin);
    PRINT_FIELD("%u", hsync_len);
    PRINT_FIELD("%u", vsync_len);
    PRINT_FIELD("%u", sync);
    PRINT_FIELD("%u", vmode);
    PRINT_FIELD("%u", rotate);
    PRINT_FIELD("%u", colorspace);
    puts("------------------");
    puts("[fb_fix_screeninfo]");
    PRINT_FFIELD("%s", id);
    PRINT_FFIELD("%u", smem_len);
    PRINT_FFIELD("%u", type);
    PRINT_FFIELD("%u", type_aux);
    PRINT_FFIELD("%u", visual);
    PRINT_FFIELD("%u", xpanstep);
    PRINT_FFIELD("%u", ypanstep);
    PRINT_FFIELD("%u", ywrapstep);
    PRINT_FFIELD("%u", line_length);
    PRINT_FFIELD("%u", mmio_len);
    PRINT_FFIELD("%u", accel);
    PRINT_FFIELD("0x%04x", capabilities);
    puts("--------------");
  return 0;
}

int main()
{
  initfb();
  getfbinfo();
  return 0;
}
