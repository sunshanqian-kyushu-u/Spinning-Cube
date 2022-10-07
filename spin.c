#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <string.h>

#define HALF_SIDE_LEHGTH 80
#define PI 3.1415926
#define RESOLUTION_X 800
#define RESOLUTION_Y 480

#define LOOP_RUN 1
#define LOOP_STOP 0

float cube[8][3] = {{-HALF_SIDE_LEHGTH, -HALF_SIDE_LEHGTH, HALF_SIDE_LEHGTH}, 
        {HALF_SIDE_LEHGTH, -HALF_SIDE_LEHGTH, HALF_SIDE_LEHGTH}, 
        {HALF_SIDE_LEHGTH, -HALF_SIDE_LEHGTH, -HALF_SIDE_LEHGTH}, 
        {-HALF_SIDE_LEHGTH, -HALF_SIDE_LEHGTH, -HALF_SIDE_LEHGTH}, 
        {-HALF_SIDE_LEHGTH, HALF_SIDE_LEHGTH, HALF_SIDE_LEHGTH}, 
        {HALF_SIDE_LEHGTH, HALF_SIDE_LEHGTH, HALF_SIDE_LEHGTH}, 
        {HALF_SIDE_LEHGTH, HALF_SIDE_LEHGTH, -HALF_SIDE_LEHGTH}, 
        {-HALF_SIDE_LEHGTH, HALF_SIDE_LEHGTH, -HALF_SIDE_LEHGTH}};
int pointpair[12][2] = {{0, 1}, {0, 3}, {0, 4}, {1, 2}, {1, 5}, {2, 3}, 
        {2, 6}, {3, 7}, {4, 5}, {4, 7}, {5, 6}, {6, 7}};

int loop_state_flag;

void signal_handler(int sig) {
    if(sig == SIGINT) { /* no need? */
        loop_state_flag = LOOP_STOP;
    }
}

void drawrpoint(int r_x, int r_y, unsigned char *mem) /* x and y is not in real space(screen space) */
{
    if(r_x < 0 || r_x > RESOLUTION_X - 1 || r_y < 0 || r_y > RESOLUTION_Y - 1)
    {
        /* over border and do nothing */
    }
    else
    {
        /* draw the point */
        unsigned char *r_mem = mem + (r_y * RESOLUTION_X + r_x) * 4;
        *r_mem = 0x00;
        *(r_mem + 1) = 0x00;
        *(r_mem + 2) = 0xFF;
        *(r_mem + 3) = 0x00;
    }
}

void drawpoint(int x, int y, unsigned char *mem) /* x and y is not in real space(screen space) */
{
    int r_x = x + (int)(RESOLUTION_X / 2); /* r_x and r_y is in real space */
    int r_y = (int)(RESOLUTION_Y / 2) - y;
    if(r_x < 0 || r_x > RESOLUTION_X - 1 || r_y < 0 || r_y > RESOLUTION_Y - 1)
    {
        /* over border and do nothing */
    }
    else
    {
        /* draw the point */
        unsigned char *r_mem = mem + (r_y * RESOLUTION_X + r_x) * 4;
        *r_mem = 0x00;
        *(r_mem + 1) = 0x00;
        *(r_mem + 2) = 0xFF;
        *(r_mem + 3) = 0x00;
    }
}

void drawline(int x_1, int y_1, int x_2, int y_2, unsigned char *mem) { /* not real space */
    if (x_1 == x_2 && y_1 == y_2) {
        /* the same point */
        drawpoint(x_1, y_1, mem);
    } else {
        int r_x1 = x_1 + (int)(RESOLUTION_X / 2); /* real space */
        int r_x2 = x_2 + (int)(RESOLUTION_X / 2); /* real space */
        int r_y1 = (int)(RESOLUTION_Y / 2) - y_1;
        int r_y2 = (int)(RESOLUTION_Y / 2) - y_2;

        int i;

        if (abs(r_x1 - r_x2) >= abs(r_y1 - r_y2)) {
            /* horizontal biger */
            if (r_x1 >= r_x2) {
                for(i = 0; i < r_x1 - r_x2 + 1; i++)
                {
                    drawrpoint(r_x2 + i, r_y2 + (int)((float)(r_y1 - r_y2) / (r_x1 - r_x2) * i), mem);
                }
            } else {
                for(i = 0; i < r_x2 - r_x1 + 1; i++)
                {
                    drawrpoint(r_x1 + i, r_y1 + (int)((float)(r_y2 - r_y1) / (r_x2 - r_x1) * i), mem);
                }
            }
        } else {
            /* vertical biger */
            if (r_y1 >= r_y2) {
                for(i = 0; i < r_y1 - r_y2 + 1; i++)
                {
                    drawrpoint(r_x2 + (int)((float)(r_x1 - r_x2) / (r_y1 - r_y2) * i), r_y2 + i, mem);
                }
            } else {
                for(i = 0; i < r_y2 - r_y1 + 1; i++)
                {
                    drawrpoint(r_x1 + (int)((float)(r_x2 - r_x1) / (r_y2 - r_y1) * i), r_y1 + i, mem);
                }
            }
        }
    }
}

void projcal(float angle) {
    float temp_cube1[8][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
            {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    float temp_cube2[8][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
            {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    
    int i;
    for(i = 0; i < 8; i++){
        temp_cube1[i][0] = cube[i][0];
        temp_cube1[i][1] = cos(angle * PI / 180) * cube[i][1] + sin(angle * PI / 180) * cube[i][2];
        temp_cube1[i][2] = -sin(angle * PI / 180) * cube[i][1] + cos(angle * PI / 180) * cube[i][2];

        temp_cube2[i][0] = cos(angle * PI / 180) * temp_cube1[i][0] - sin(angle * PI / 180) * temp_cube1[i][2];
        temp_cube2[i][1] = temp_cube1[i][1];
        temp_cube2[i][2] = sin(angle * PI / 180) * temp_cube1[i][0] + cos(angle * PI / 180) * temp_cube1[i][2];

        cube[i][0] = cos(angle * PI / 180) * temp_cube2[i][0] - sin(angle * PI / 180) * temp_cube2[i][1];
        cube[i][1] = sin(angle * PI / 180) * temp_cube2[i][0] + cos(angle * PI / 180) * temp_cube2[i][1];
        cube[i][2] = temp_cube2[i][2];
    }
}

void draw (float cube[8][3], unsigned char *mem) {
    int i;
    for (i = 0; i < 12; i++) {
        drawline(cube[pointpair[i][0]][0], cube[pointpair[i][0]][1], 
                cube[pointpair[i][1]][0], cube[pointpair[i][1]][1], mem);
    }
}

void spin (unsigned char *mem) {
    loop_state_flag = LOOP_RUN;
    /* draw the first image */
    draw(cube, mem);
    while (loop_state_flag == LOOP_RUN) {
        projcal(1); /* change 1 degree */
        draw(cube, mem);
        usleep(1000 * 33); /* wait 33 ms for 30 fps */
        /* clear screen */
        memset(mem, 0x00, RESOLUTION_X * RESOLUTION_Y * 4);
    }
}