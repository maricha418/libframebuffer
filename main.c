#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "config.h"

int fb_dev;
uint32_t* fbp_real;
uint32_t* fbp;
uint32_t bg_color;

int fixed_line_length;
int fixed_fb_size;

static struct termios orig_termios;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

int initfb(void);
int getfbinfo(void);

int initfb()
{
  fb_dev = open(FB_PATH, O_RDWR);
  if(fb_dev < 0) {
    perror("open");
    return -1;
  }
  printf("\033[?25l");
  fflush(stdout);
  struct termios raw;
  tcgetattr(STDIN_FILENO, &orig_termios);
  raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ECHONL);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

  getfbinfo(); 

  fixed_line_length = finfo.line_length / 4;
  fixed_fb_size = vinfo.yres * fixed_line_length;

  fbp_real = mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fb_dev, 0);
  fbp = malloc(finfo.smem_len);
  if(fbp == MAP_FAILED) {
    perror("mmap");
    return -1;
  }
}

int getfbinfo()
{
  ioctl(fb_dev, FBIOGET_VSCREENINFO, &vinfo);
  ioctl(fb_dev, FBIOGET_FSCREENINFO, &finfo);
  //finfo.line_length: x-mem-max;
  //vinfo.yres: y-max;
  //vinfo.xres: x-max;

  //length-max: line_length * yres or smem_len
  return 0;
}

int drawline(int line, uint32_t color)
{
  int fixed_line_length = finfo.line_length / 4;
  int start_loc = line * fixed_line_length;

  for(int i = start_loc; i < start_loc + fixed_line_length; i++)
  {
    fbp[i] = color;
  }

  return 0;
}

int fbsync(void)
{
  memcpy(fbp_real, fbp, finfo.smem_len);
  for(int i = 0; i < fixed_fb_size; i++)
    fbp[i] = bg_color;

  return 0;
}

int fbclean(void)
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
  printf("\033[?25h");
  fflush(stdout);
  free(fbp);
  munmap(fbp, finfo.smem_len);
  close(fb_dev);
}

int main()
{
  initfb();
  bg_color = 0xFFFFFF;

  for(int i = 0; i < vinfo.yres; i++)
  {
    if(i < 25) {
      drawline(i, 0x00FF00);
    } else {
      drawline(i, 0xFFFFFF);
    }
  }

  fbsync();
  fbclean();
  return 0;
}
