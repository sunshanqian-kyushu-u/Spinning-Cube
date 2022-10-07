#ifndef __SPIN_H
#define __SPIN_H

void spin (unsigned char *mem);
void draw (float cube[8][3], unsigned char *mem);
void projcal(float angle);
void drawline(int x_1, int y_1, int x_2, int y_2, unsigned char *mem);
void drawpoint(int x, int y, unsigned char *mem);
void drawrpoint(int r_x, int r_y, unsigned char *mem);
void signal_handler(int sig);

#endif