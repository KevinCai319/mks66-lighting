#include <stdio.h>
#include <stdlib.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "math.h"

/*======== void add_box() ==========
  Inputs:   struct matrix * tris
            double x
            double y
            double z
            double width
            double height
            double depth

  add the points for a rectagular prism whose
  upper-left-front corner is (x, y, z) with width,
  height and depth dimensions.
  ====================*/
void add_box(struct matrix *tris,
             double x, double y, double z,
             double width, double height, double depth)
{
  const double tx = x+width;
  const double ty = y-height;
  const double tz = z-depth;

  //FRONT FACE
  add_tri(tris,
          x,y,z,
          x,ty,z,
          tx,y, z);
  add_tri(tris,
        x,ty,z,
        tx, ty, z,
        tx, y, z);
  //BACK FACE
  add_tri(tris,
          tx,y,tz,
          x,ty,tz,
          x,y, tz);
  add_tri(tris,
        tx,y,tz,
        tx, ty, tz,
        x, ty, tz);
  //TOP FACE
  add_tri(tris,
          x,y,tz,
          x,y,z,
          tx,y, tz);
  add_tri(tris,
        x,y,z,
        tx, y, z,
        tx, y, tz);
  //BOTTOM FACE
  add_tri(tris,
          tx,ty,tz,
          x,ty,z,
          x,ty, tz);
  add_tri(tris,
        tx,ty,tz,
        tx, ty, z,
        x, ty, z);
  //LEFT FACE
  add_tri(tris,
          x,y,tz,
          x,ty,tz,
          x,y,z);
  add_tri(tris,
        x,ty,tz,
        x, ty, z,
        x, y, z);
  //RIGHT FACE
  add_tri(tris,
          tx,y,z,
          tx,ty,tz,
          tx,y,tz);
  add_tri(tris,
        tx,y,z,
        tx, ty, z,
        tx, ty, tz);
}

/*======== void add_sphere() ==========
  Inputs:   struct matrix * points
            double cx
            double cy
            double cz
            double r
            int step  

  adds all the points for a sphere with center (cx, cy, cz)
  and radius r using step points per circle/semicircle.

  should call generate_sphere to create the necessary points
  ====================*/
void add_sphere(struct matrix *tris,
                double cx, double cy, double cz,
                double r, int step)
{
  struct matrix *sphere = generate_sphere(cx, cy, cz, r, step);
  int point;
  const int stp = (step+1);
  for (point = 0; point < sphere->lastcol - stp; point++)
  {
    if(point % (stp) != 0){
      add_tri(tris, sphere->m[0][point], sphere->m[1][point], sphere->m[2][point],
                    sphere->m[0][point+stp], sphere->m[1][point+stp], sphere->m[2][point+stp],
                    sphere->m[0][point+step], sphere->m[1][point+step], sphere->m[2][point+step]
              );
    }
    if((point+1) % (stp) != 0){
            add_tri(tris, sphere->m[0][point], sphere->m[1][point], sphere->m[2][point],
                    sphere->m[0][point+1], sphere->m[1][point+1], sphere->m[2][point+1],
                    sphere->m[0][point+stp], sphere->m[1][point+stp], sphere->m[2][point+stp]
              );
    }
  }
}

/*======== void generate_sphere() ==========
  Inputs:   struct matrix * points
            double cx
            double cy
            double cz
            double r
            int step
  Returns: Generates all the points along the surface
           of a sphere with center (cx, cy, cz) and
           radius r using step points per circle/semicircle.
           Returns a matrix of those points
  ====================*/
struct matrix *generate_sphere(double cx, double cy, double cz,
                               double r, int step)
{

  struct matrix *sphere = new_matrix(4, 0);
  int cir = -1;
  int p = -1;

  while (++p <= step)
  {
    cir = -1;
    while (++cir <= step)
    {
      double p1 = (double)p / step * M_PI * 2;
      double cir1 = (double)cir / step * M_PI;
      add_point(sphere, cx + r * cos(cir1), cy + r * sin(cir1) * cos(p1), cz + r * sin(cir1) * sin(p1));
    }
  }
  return sphere;
}

/*======== void add_torus() ==========
  Inputs:   struct matrix * points
            double cx
            double cy
            double cz
            double r1
            double r2
            double step
  Returns:

  adds all the points required for a torus with center (cx, cy, cz),
  circle radius r1 and torus radius r2 using step points per circle.

  should call generate_torus to create the necessary points
  ====================*/
void add_torus(struct matrix *tris,
               double cx, double cy, double cz,
               double r1, double r2, int step)
{
  struct matrix *torus = generate_torus(cx, cy, cz, r1, r2, step);
  int point;
  const int stp = (step+1);
  for (point = 0; point < torus->lastcol - stp; point++)
  {
    if((point+1)%stp != 0){
      add_tri(tris,torus->m[0][point],torus->m[1][point],torus->m[2][point],
      torus->m[0][point+stp],torus->m[1][point+stp],torus->m[2][point+stp],
      torus->m[0][point+1],torus->m[1][point+1],torus->m[2][point+1]);
    }
    if(point%stp != 0 ){
      add_tri(tris, torus->m[0][point+step],torus->m[1][point+step],torus->m[2][point+step],
      torus->m[0][point+stp],torus->m[1][point+stp],torus->m[2][point+stp],
      torus->m[0][point],torus->m[1][point],torus->m[2][point]);
    }
  }
  return;
}

/*======== void generate_torus() ==========
  Inputs:   struct matrix * points
            double cx
            double cy
            double cz
            double r
            int step
  Returns: Generates all the points along the surface
           of a torus with center (cx, cy, cz),
           circle radius r1 and torus radius r2 using
           step points per circle.
           Returns a matrix of those points
  ====================*/
struct matrix *generate_torus(double cx, double cy, double cz,
                              double r1, double r2, int step)
{
  struct matrix *torus = new_matrix(4, 0);
  int t;
  int p = -1;
  while (++p <= step)
  {
    t = -1;
    while (++t <= step)
    {
      double p1 = (double)p / step * 2 * M_PI;
      double t1 = (double)t / step * 2 * M_PI;
      add_point(torus, cx + cos(p1) * (r1 * cos(t1) + r2),
                cy + r1 * sin(t1),
                cz - sin(p1) * (r1 * cos(t1) + r2));
    }
  }
  return torus;
}

/*======== void add_circle() ==========
  Inputs:   struct matrix * edges
            double cx
            double cy
            double r
            double step

  Adds the circle at (cx, cy) with radius r to edges
  ====================*/
void add_circle(struct matrix *edges,
                double cx, double cy, double cz,
                double r, int step)
{
  double x0, y0, x1, y1, t;
  int i;

  x0 = r + cx;
  y0 = cy;
  for (i = 1; i <= step; i++)
  {
    t = (double)i / step;
    x1 = r * cos(2 * M_PI * t) + cx;
    y1 = r * sin(2 * M_PI * t) + cy;

    add_edge(edges, x0, y0, cz, x1, y1, cz);
    x0 = x1;
    y0 = y1;
  }
}

/*======== void add_curve() ==========
Inputs:   struct matrix *edges
         double x0
         double y0
         double x1
         double y1
         double x2
         double y2
         double x3
         double y3
         double step
         int type

Adds the curve bounded by the 4 points passsed as parameters
of type specified in type (see matrix.h for curve type constants)
to the matrix edges
====================*/
void add_curve(struct matrix *edges,
               double x0, double y0,
               double x1, double y1,
               double x2, double y2,
               double x3, double y3,
               int step, int type)
{
  double t, x, y;
  int i;
  struct matrix *xcoefs;
  struct matrix *ycoefs;

  xcoefs = generate_curve_coefs(x0, x1, x2, x3, type);
  ycoefs = generate_curve_coefs(y0, y1, y2, y3, type);

  /* print_matrix(xcoefs); */
  /* printf("\n"); */
  /* print_matrix(ycoefs); */

  for (i = 1; i <= step; i++)
  {
    t = (double)i / step;
    x = xcoefs->m[0][0] * t * t * t + xcoefs->m[1][0] * t * t +
        xcoefs->m[2][0] * t + xcoefs->m[3][0];
    y = ycoefs->m[0][0] * t * t * t + ycoefs->m[1][0] * t * t +
        ycoefs->m[2][0] * t + ycoefs->m[3][0];

    add_edge(edges, x0, y0, 0, x, y, 0);
    x0 = x;
    y0 = y;
  }

  free_matrix(xcoefs);
  free_matrix(ycoefs);
}

/*======== void add_tri() ==========
Inputs:   struct matrix * tris
          int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2
          NOTE: coordinates should be in counterclockwise order!
Returns: 
add the triangle made of (x0, y0, z0) , (x1, y1, z1) , (x2, y2, z2)
should use add_point
====================*/
void add_tri(struct matrix *tris,
             double x0, double y0, double z0,
             double x1, double y1, double z1,
             double x2, double y2, double z2)
{
    add_point(tris, x0, y0, z0);
    add_point(tris, x1, y1, z1);
    add_point(tris, x2, y2, z2);
}

/*======== void add_point() ==========
Inputs:   struct matrix * points
         int x
         int y
         int z 
Returns: 
adds point (x, y, z) to points and increment points.lastcol
if points is full, should call grow on points
====================*/
void add_point(struct matrix *points, double x, double y, double z)
{

  if (points->lastcol == points->cols)
    grow_matrix(points, points->lastcol + 100);

  points->m[0][points->lastcol] = x;
  points->m[1][points->lastcol] = y;
  points->m[2][points->lastcol] = z;
  points->m[3][points->lastcol] = 1;
  points->lastcol++;
} //end add_point

/*======== void add_edge() ==========
Inputs:   struct matrix * points
          int x0, int y0, int z0, int x1, int y1, int z1
Returns: 
add the line connecting (x0, y0, z0) to (x1, y1, z1) to points
should use add_point
====================*/
void add_edge(struct matrix *points,
              double x0, double y0, double z0,
              double x1, double y1, double z1)
{
  add_point(points, x0, y0, z0);
  add_point(points, x1, y1, z1);
}

/*======== void draw_lines() ==========
Inputs:   struct matrix * points
         screen s
         color c 
Returns: 
Go through points 2 at a time and call draw_line to add that line
to the screen
====================*/
void draw_lines(struct matrix *points, zbuffer zbuf, screen s, color c)
{
  if (points->lastcol < 2)
  {
    printf("Need at least 2 points to draw a line!\n");
    return;
  }

  int point;
  for (point = 0; point < points->lastcol - 1; point += 2)
    draw_line(points->m[0][point],
              points->m[1][point],
              points->m[0][point + 1],
              points->m[1][point + 1],
              points->m[0][point + 2],
              points->m[1][point + 2],
              zbuf, s, c);
} // end draw_lines

int check_valid_tri(double x0, double y0, double z0,
             double x1, double y1, double z1,
             double x2, double y2, double z2){

            double ax = x1-x0;
            double ay = y1-y0;
            double az = z1-z0;

            double bx = x2-x0;
            double by = y2-y0;
            double bz = z2-z0;

            double nx = ay*bz - az*by;
            double ny = az*bx - ax*bz;
            double nz = ax*by - ay*bx;
            return (nz > 0);
}

/*======== void draw_tris() ==========
Inputs:   struct matrix * points
         screen s
         color c 
Returns: 
Go through points 2 at a time and call draw_line to add that line
to the screen
====================*/
void draw_tris(struct matrix *points,zbuffer zbuf, screen s, color c)
{
  const int last = points->lastcol;
  if (last < 3)
  {
    printf("Need at least 3 points to draw a tri!\n");
    return;
  }
  if (last % 3)
  {
    printf("ERROR: Invalid input for triangle matrix \n");
    return;
  }
  int point;
  for (point = 0; point < last - 2; point += 3)
  {
    draw_tri(points->m[0][point],points->m[1][point],points->m[2][point],
             points->m[0][point+1],points->m[1][point+1],points->m[2][point+1],
             points->m[0][point+2],points->m[1][point+2],points->m[2][point+2],zbuf, s,c);
  }
} // end draw_tris

//extra functions just to make code a bit more manageable
int getB(int x0, int x1, int y0, int y1){
  return (y0 > y1 || (y0 == y1 && x1 < x0));
}
int getT(int x0, int x1, int x2, int y0, int y1, int y2){
  if(y0 > y1){
    if(y0 > y2 || (!getB(x2,x0,y2,y0)))
      return 0;
    return 2;
  }
  if(y1 > y2){
    return getB(x1,x0,y1,y0);
  }
  if((y1 == y2 && x1 < x2)){
    return 1;
  }
  return 2;
}

void draw_tri(double x0,double y0,double z0,double x1,double y1,double z1,double x2,double y2,double z2,zbuffer zbuf, screen s, color c){
  if(check_valid_tri(x0,y0,z0,x1,y1,z1,x2,y2,z2)){
    double tx,ty,tz,bx,by,bz,mx,my,mz;
    switch(getT(x0,x1,x2,y0,y1,y2)){
      case 0:
        tx = x0;ty = y0;tz = z0;
        if(getB(x1,x2,y1,y2)){bx = x2;by = y2;bz = z2;mx = x1;my = y1;mz = z1;
                        }else{bx = x1;by = y1;bz = z1;mx = x2;my = y2;mz = z2;}
        break;
      case 1:
        tx = x1;ty = y1;tz = z1;
        if(getB(x0,x2,y0,y2)){bx = x2;by = y2;bz = z2;mx = x0;my = y0;mz = z0;
                        }else{bx = x0;by = y0;bz = z0;mx = x2;my = y2;mz = z2;}
        break;
      default:
        tx = x2;ty = y2;tz = z2;
        if(getB(x0,x1,y0,y1)){bx = x1;by = y1;bz = z1;mx = x0;my = y0;mz = z0;
                        }else{bx = x0;by = y0;bz = z0;mx = x1;my = y1;mz = z1;}
        break;
    }
    const double d0y = (ty-by+1);
    const double d1y = (my-by+1);
    const double d2y = (ty-my+1);
    const double dx = (tx-bx) / d0y;
    const double dz = (tz-bz) / d0y;
    const double  dmx = (mx-bx)/ d1y;
    const double  dmz = (mz-bz)/ d1y;
    const double  dtx = (tx-mx)/ d2y;
    const double  dtz = (tz-mz)/ d2y;
    int y = by;
    double kx0,kz0,kx1,kz1;
    kx0 = bx;
    kx1 = kx0;
    kz0 = bz;
    kz1 = kz0;
    const int hy = my;
    const int gy = ty;
    color c = genColor();
    //this should save some time
    while(y != hy){
      sline(kx0,kx1,kz0,kz1,++y,zbuf, s,c);
      kx0 += dx;
      kz0 += dz;
      kx1 += dmx;
      kz1 += dmz;
    }
    kx1 = mx;
    kz1 = mz;
    while(y <= gy){
      sline(kx0,kx1,kz0,kz1,++y,zbuf,s,c);
      kx0 += dx;
      kz0 += dz;
      kx1 += dtx;
      kz1 += dtz;
    }
    
  }
}
color genColor(){
    color c;
    c.red = 200 + random()* 55;
    c.green = 200 + random()* 55;
    c.blue = 200 + random()* 55;
    return c;
}
void sline(int x0, int x1, double z0, double z1, int y,zbuffer zbuf, screen s, color c){
  if(x0>x1){
    sline(x1,x0,z1,z0,y,zbuf, s,c);
  }
  const double diff = (z1-z0)/(x1-x0+1);
  while(x0 <= x1){
    plot(zbuf,s,c,x0++,y, z0);
    z0 += diff;
  }
}
void draw_line(int x0, int y0, int x1, int y1,double z0, double z1, zbuffer zbuf, screen s, color c)
{

  int x, y, d, A, B;
  double cz = z0;
  //swap points if going right -> left
  int xt, yt;
  if (x0 > x1)
  {
    xt = x0;
    yt = y0;
    x0 = x1;
    y0 = y1;
    x1 = xt;
    y1 = yt;
  }

  x = x0;
  y = y0;
  A = 2 * (y1 - y0);
  B = -2 * (x1 - x0);

  //octants 1 and 8
  if (abs(x1 - x0) >= abs(y1 - y0)){
    const double diff = (z1-z0)/(x1-x0+1);
    //octant 1
    if (A > 0)
    {

      d = A + B / 2;
      while (x < x1)
      {
        plot(zbuf, s, c, x, y, cz);
        if (d > 0)
        {
          y += 1;
          d += B;
        }
        x++;
        cz += diff;
        d += A;
      } //end octant 1 while
      plot(zbuf, s, c, x1, y1, cz);
    } //end octant 1

    //octant 8
    else
    {
      d = A - B / 2;

      while (x < x1)
      {
        //printf("(%d, %d)\n", x, y);
        plot(zbuf, s, c, x, y, cz);
        if (d < 0)
        {
          y -= 1;
          d -= B;
        }
        x++;
        cz += diff;
        d += A;
      } //end octant 8 while
      plot(zbuf, s, c, x1, y1, cz);
    } //end octant 8
  }   //end octants 1 and 8

  //octants 2 and 7
  else
  {
    const double diff = (z1-z0)/(y1-y0+1);
    //octant 2
    if (A > 0)
    {
      d = A / 2 + B;

      while (y < y1)
      {
        plot(zbuf, s, c, x, y, cz);
        if (d < 0)
        {
          x += 1;
          d += A;
        }
        y++;
        cz += diff;
        d += B;
      } //end octant 2 while
      plot(zbuf, s, c, x1, y1, cz);
    } //end octant 2

    //octant 7
    else
    {
      d = A / 2 - B;

      while (y > y1)
      {
        plot(zbuf, s, c, x, y, cz);
        if (d > 0)
        {
          x += 1;
          d += A;
        }
        y--;
        cz += diff;
        d -= B;
      } //end octant 7 while
      plot(zbuf, s, c, x1, y1, cz);
    } //end octant 7
  }   //end octants 2 and 7
} //end draw_line
