#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
  Inputs:   char * filename 
  struct matrix * transform, 
  struct matrix * pm,
  screen s
  Returns: 
  Goes through the file named filename and performs all of the actions listed in that file.
  The file follows the following format:
  Every command is a single character that takes up a line
  Any command that requires arguments must have those arguments in the second line.
  The commands are as follows:
  line: add a line to the edge matrix - 
  takes 6 arguments (x0, y0, z0, x1, y1, z1)
  ident: set the transform matrix to the identity matrix - 
  scale: create a scale matrix, 
  then multiply the transform matrix by the scale matrix - 
  takes 3 arguments (sx, sy, sz)
  translate: create a translation matrix, 
  then multiply the transform matrix by the translation matrix - 
  takes 3 arguments (tx, ty, tz)
  rotate: create an rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 2 arguments (axis, theta) axis should be x y or z
  apply: apply the current transformation matrix to the 
  edge matrix
  display: draw the lines of the edge matrix to the screen
  display the screen
  save: draw the lines of the edge matrix to the screen
  save the screen to a file -
  takes 1 argument (file name)
  quit: end parsing
  See the file script for an example of the file format
  IMPORTANT MATH NOTE:
  the trig functions int math.h use radian mesure, but us normal
  humans use degrees, so the file will contain degrees for rotations,
  be sure to conver those degrees to radians (M_PI is the constant
  for PI)
  ====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  char *args[10];
  int i;
  char *l;
  struct matrix*m;
  clear_screen(s);

  color c;
  c.red = 100;
  c.blue = 50;
  c.green = 100;

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);
    if(!strncmp(line,"quit", 255)){
      return;
    }
    else if(!strncmp(line, "ident", 255)){
      ident(transform);
    }
    else if(!strncmp(line, "apply", 255)){
      matrix_mult(transform, edges);
    }
    else if(!strncmp(line, "display", 255)){
      clear_screen(s);
      draw_lines(edges, s, c);
      display(s);
    }
    else if(!strncmp(line, "save", 255)){
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      clear_screen(s);
      draw_lines(edges, s, c); 
      save_extension(s, line);
    }
    else if(!strncmp(line, "line", 255)){
      fgets(line, 255, f);
      line[strlen(line)-1]='\0'; 
      l = line;
      for(i = 0;i < 6;i++){
	args[i] = strsep(&l, " ");
      }
      add_edge(edges, atof(args[0]), atof(args[1]), atof(args[2]), atof(args[3]), atof(args[4]), atof(args[5]));
    }
    else if(!strncmp(line, "scale", 255)){
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      l = line;
      for(i = 0;i < 3;i++){
	args[i] = strsep(&l, " ");
      }
      m = make_scale(atof(args[0]), atof(args[1]), atof(args[2]));
      matrix_mult(m, transform);
    }
    else if(!strncmp(line, "translate", 255)){
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      l = line;
      for(i = 0;i < 3;i++){
	args[i] = strsep(&l, " ");
      }
      m = make_translate(atof(args[0]), atof(args[1]), atof(args[2]));
      matrix_mult(m, transform); 
    }
    else if(!strncmp(line, "rotate", 255)){
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      l = line;
      for(i = 0;i < 2;i++){
	args[i] = strsep(&l, " ");
      }
      if(!strncmp(args[0], "x",1) == 0){
	m = make_rotX(atof(args[1]));
      }
      else if(!strncmp(args[0], "y",1) == 0){
	m = make_rotY(atof(args[1]));
      }
      else if(!strncmp(args[0], "z",1) == 0){
	m = make_rotZ(atof(args[1]));
      }
      matrix_mult(m, transform);
    }
  }
}
