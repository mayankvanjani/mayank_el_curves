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
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 circle: add a circle to the edge matrix - 
	    takes 3 arguments (cx, cy, r)
	 hermite: add a hermite curve to the edge matrix -
	    takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
	 bezier: add a bezier curve to the edge matrix -
	    takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 xrotate: create an x-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 yrotate: create an y-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 zrotate: create an z-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
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
                  struct matrix * pm,
                  screen s) {

  FILE *f;
  char line[256];
  color c;
  char *line_arr[100];
  int param[10];
  char *token;
  int counter = 0;
  int j;
  clear_screen(s);

  c.red = 0;
  c.green = 0;
  c.blue = 0;

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, sizeof(line), f) != NULL ) {
    // line[strlen(line)-1]='\0';
    //    printf( "%s", line);
    line_arr[counter] = strdup(line);
    counter++;
  }

  //STARTING
  /*
  printf( "\n\n%s", line_arr[0] );
  char *test = line_arr[0];
  printf( "%lu\n", strlen( test ));
  if (strcmp( line_arr[0], "circle\n") == 0) {
    printf("WORKED\n");
  }
  */
  for (j = 0; j < counter; j++) {
    //    printf( "%s", line_arr[j] );

    if ( strcmp( line_arr[j], "line\n" ) == 0 ) {
      char *test_line = line_arr[j+1];
      test_line[strlen(test_line)-1] = ' ';
      token = strtok( test_line, " ");
      int z = 0;
      while (token != NULL ){
	param[z] = atoi(token);
	token = strtok(NULL, " ");
	z++;
      }
      add_edge( pm, param[0], param[1], param[2], param[3], param[4], param[5] ); 
    }
    else if ( strcmp( line_arr[j], "circle\n" ) == 0 ) {
      char *test_line = line_arr[j+1];
      test_line[strlen(test_line)-1] = ' ';
      //      printf( "\ncircle test_line: %s\n", test_line);
      token = strtok( test_line, " ");
      int z = 0;
      while (token != NULL ){
        param[z] = atoi(token);
	//	printf( "param[%d]: %d\n", z, param[z]);
        token = strtok( NULL, " ");
        z++;
      }
      printf( "add_circle( cx:%d, cy:%d, r:%d )\n", param[0],param[1],param[2]);     
      add_circle( pm, param[0], param[1], param[2], .01);
    }
    else if ( strcmp( line_arr[j], "hermite\n" ) == 0 ) {
      char *test_line = line_arr[j+1];
      test_line[strlen(test_line)-1] = ' ';
      //      printf( "\nhermite test_line: %s\n", test_line);
      token = strtok( test_line, " ");
      int z = 0;
      while (token != NULL ){
        param[z] = atoi(token);
        token = strtok(NULL, " ");
        z++;
      }
      printf( "HERMITE: add_curve(x0:%d y0:%d x1:%d y1:%d x2:%d y2:%d x3:%d y3:%d )\n"
	      ,param[0],param[1],param[2],param[3],param[4],param[5],param[6],param[7] );     
      add_curve(pm,param[0],param[1],param[2],param[3],param[4],param[5],param[6],param[7],.01,0);
    }
    else if ( strcmp( line_arr[j], "bezier\n" ) == 0 ) {
      char *test_line = line_arr[j+1];
      test_line[strlen(test_line)-1] = ' ';
      //      printf( "\nbezier test_line: %s\n", test_line);
      token = strtok( test_line, " ");
      int z = 0;
      while (token != NULL ){
        param[z] = atoi(token);
        token = strtok(NULL, " ");
        z++;
      }
      printf( "BEZIER: add_curve(x0:%d y0:%d x1:%d y1:%d x2:%d y2:%d x3:%d y3:%d )\n"
	      ,param[0],param[1],param[2],param[3],param[4],param[5],param[6],param[7] );
      add_curve(pm,param[0],param[1],param[2],param[3],param[4],param[5],param[6],param[7],.01,1);
    }
    else if ( strcmp( line_arr[j], "ident\n" ) == 0 ) {
      ident(transform);
    }
    else if ( strcmp( line_arr[j], "scale\n" ) == 0 ) {
      char *test_line = line_arr[j+1];
      test_line[strlen(test_line)-1] = ' ';
      token = strtok( test_line, " ");
      int z = 0;
      while (token != NULL ){
        param[z] = atoi(token);
        token = strtok(NULL, " ");
        z++;
      }
      struct matrix * mult = make_scale( param[0], param[1], param[2] );
      matrix_mult(mult, transform);
    }
    else if ( strcmp( line_arr[j], "translate\n" ) == 0 ) {
      char *test_line = line_arr[j+1];
      test_line[strlen(test_line)-1] = ' ';
      token = strtok( test_line, " ");
      int z = 0;
      while (token != NULL ){
        param[z] = atoi(token);
        token = strtok(NULL, " ");
        z++;
      }
      struct matrix * mult = make_translate( param[0], param[1], param[2] );
      matrix_mult(mult, transform);
    }
    else if ( strcmp( line_arr[j], "xrotate\n" ) == 0 ) {
      char *test_line = line_arr[j+1];
      test_line[strlen(test_line)-1] = ' ';
      token = strtok( test_line, " ");
      int z = 0;
      while (token != NULL ){
        param[z] = atoi(token);
        token = strtok(NULL, " ");
        z++;
      }
      struct matrix * mult = make_rotX( param[0] );
      matrix_mult(mult, transform);
    }
    else if ( strcmp( line_arr[j], "yrotate\n" ) == 0 ) {
      char *test_line = line_arr[j+1];
      test_line[strlen(test_line)-1] = ' ';
      token = strtok( test_line, " ");
      int z = 0;
      while (token != NULL ){
        param[z] = atoi(token);
        token = strtok(NULL, " ");
        z++;
      }
      struct matrix * mult = make_rotY( param[0] );
      matrix_mult(mult, transform);
    }
    else if ( strcmp( line_arr[j], "zrotate\n" ) == 0 ) {
      char *test_line = line_arr[j+1];
      test_line[strlen(test_line)-1] = ' ';
      token = strtok( test_line, " ");
      int z = 0;
      while (token != NULL ){
        param[z] = atoi(token);
        token = strtok(NULL, " ");
        z++;
      }
      struct matrix * mult = make_rotZ( param[0] );
      matrix_mult(mult, transform);
    }
    else if ( strcmp( line_arr[j], "apply\n" ) == 0 ) {
      matrix_mult( transform, pm);
    }
    else if ( strcmp( line_arr[j], "display\n" ) == 0 ) {
      draw_lines( pm, s, c );
      display(s);
    }
    else if ( strcmp( line_arr[j], "save\n" ) == 0 ) {
      char *test_line = line_arr[j+1];
      save_ppm( s, test_line );
      save_extension( s, test_line);
    }
    else if ( strcmp( line_arr[j], "quit\n" ) == 0 ) {
      printf("Got Here, and DONE!\n");
      return;
    }

  }

}

  
