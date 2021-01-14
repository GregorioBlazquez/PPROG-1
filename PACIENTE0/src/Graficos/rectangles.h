/*------------------------------------------------------------------
   Project:  General C utilities
   File:     rectangle.h
   Rev.      2.0
   Date:     Sept. 22, 2011


   Revisions:
   1.0       Sept. 22, 2011
   2.0       Oct. 1, 2014
             Removed some redundant and complex functions. Streamlined
	     the code to make it a better example.

   (C) Simone Santini, 2011-2014

   This file defines the functions that form a library to manage 
   vitual windows on a character screen. The window is not marked in any
   way on the screen (marking it out, if necessary, is a purpose of the 
   calling program), but allows the user to position the cursor and print
   inside a window (almost) as if it were an independent screen. 

   In the examples that preceed the functions, we assume that we have
   a screen with 10 rows and 40 columns and in it we open a "window"
   of 5 rows and 15 column beginning at position 2, 19. The situation
   is therefore this:

   0                                        39
    +----------------------------------------+
  0 |                                        |
    |                   +---------------+    |
    |                   |               |    |
    |                   |               |    |
    |                   |               |    |
    |                   |               |    |
    |                   |               |    |
    |                   +---------------+    |
    |                                        |
   9|                                        |
    +----------------------------------------+

    Of course, the library doesn't mark the window on the screen, and
    in order to remark this fact, we shall write down the screen as:

                       19             33
     0                  v             v    39
    +----------------------------------------+
  0 |                                        |
    |                                        |
    |                                        | < 2
    |                                        | 
    |                                        |
    |                                        |
    |                                        | < 6
    |                                        | 
    |                                        |
   9|                                        |
    +----------------------------------------+


    NOTE:
    These functions are implemented using ASCII ESC codes for the control
    of the cursor. This might produce some incompatibility problem with 
    Windows and MS/DOS systems.

    Note also that the ESC code consider that the cursor coordinates
    begin with 1 (1, 1 is the position of the top left character of
    the screen). However, following the standard computing convention,
    this library will consider that the coordinates begin with 0. So,
    (0,0) are the coordinates of the top-left-most character in the
    screen and for a screen of size, say 40x80, the bottom-right
    character has coordinates (39,79).

  -------------------------------------------------------------------
*/
#ifndef SC_WINDOW_H
#define SC_WINDOW_H
 
#include <stdio.h>
#include <stdlib.h>    
  
#define ERR -1
#define OKK 1
      
      
      
/*
  
  This is the structure used to store the information about a window:
  this is the structure returned by the function that creates a new
  window, and must be passed as a parameter to all functions that
  write in the window.
*/
typedef struct {
  int  c1;     /* column on the screen where the window begins */
  int  r1;     /* row on the screen where the window begins */
  int  nr;     /* number of rows in the window  */
  int  nc;     /* number of column in the window  */
  
  int  crs_r;  /* last position of the cursor in this window */
  int  crs_c;  /* last position of the cursor in this window */
  
  int  last_line;
  int  bg_color; /* the background colour for this window */
  int  fg_color; /* the foreground colour for this window */
 
  int  ch_effect; /* the character effect for this window */
} sc_rectangle;
  
  
/*
  Moves the cursor into a window position, translating it into
  absolute screen positions. Note the "+1" used to translate from the
  0-based library coordinates to the 1-based escape codes.
 */
void _move_to(sc_rectangle *sc, int r, int c);

/*
  Creates a window on the screen. 
  Parameters:
     r1, c2: row and column where the window begins;
     nr, nc: number of rows and columns of the window.
  
  Returns:
    a pointer to a newly allocated sc_rectangle structure with the 
    information for the window.
  
  Note that this function will do absolutely nothing on the screen
  itself. In particular, it will NOT clear the window. This function
  will simply create the structure, initialize its fields, and return
  it.
*/
sc_rectangle *win_new(int r1, int c1, int nr, int nc, int bg, int fg);
  
  
void win_cls(sc_rectangle *sc);
  
/*
  Releases the structure allocated for a window. After calling this
  function it is no longer possible to use the window.
  
  This function doesn't erase the window contents from the screen.
  
  Parameters:
      sc: the window structure on which the function operates.
  
  Returns:
      nothing.
*/
void win_delete(sc_rectangle *sc);
  
/*
  Sets the background color for the window
  
  Parameters:
      sc:  the window structure on which the function operates.
      col: the new background color;
  
  Returns:
      1: regular execution
      0: incorrect color parameter
*/
int win_bgcol(sc_rectangle *sc, int col);
  
  
/*
  Sets the foreground color for the window
  
  Parameters:
      sc:  the window structure on which the function operates.
      col: the new foreground color;
  
  Returns:
      1: regular execution
      0: incorrect color parameter
*/
int win_fgcol(sc_rectangle *sc, int col);
  
  
/*
  This function writes a string in a single line starting at a given
  position of the window, and without changing line. Each new line
  character contained in the string will be considered as a string
  termination, and if the string is too long to fit in the screen, it
  will be truncated. 
  
  So, if on the screen of the example we execute the functions:
  
  win_write_line_at(sc, 1, 7, "en un lugar de la mancha");
  win_write_line_at(sc, 3, 5, "oh!\nI didn't know");
   
  
  we get:
  
                       19             33
     0                  v             v    39
    +----------------------------------------+
  0 |                                        |
    |                                        |
    |                          en un lu      | < 2
    |                                        | 
    |                        oh!             |
    |                                        |
    |                                        | < 6
    |                                        | 
    |                                        |
   9|                                        |
    +----------------------------------------+
  
    Parameters:
      sc:  the window structure on which the function operates.
      r:   the row of the window at which the writing begins
      c:   the column of the window at which the writing begins
      str: the screen that we must write
  
    Returns:
      the number of characters actually written on the screen.
  
*/
int win_write_line_at(sc_rectangle *sc, int r, int c, char *str);

/*Well, this function does the same thing as the previous
one, but writes the string in a certain color...*/
int win_write_line_at_colour(sc_rectangle *sc, int r, int c, char *str, int colour, int background);

/*
  Writes a char at a given position of the window.

    Parameters:
      sc:  the window structure on which the function operates.
      r:   the row of the window at which the writing begins
      c:   the column of the window at which the writing begins
      ch: the character that we must write
  
    Returns:
      The constant 1
  
*/
int win_write_char_at(sc_rectangle *sc, int r, int c, char ch);

/*
  Writes an int at a given position of the window.

    Parameters:
      sc:  the window structure on which the function operates.
      r:   the row of the window at which the writing begins
      c:   the column of the window at which the writing begins
      n: the integer that we must write
  
    Returns:
      The constant 1
  
*/
int win_write_int_at(sc_rectangle *sc, int r, int c, int n);

/*
  Perdon por cambiar a español.
  Esta funcion crea y limpia 4 ventanas, que pasamos como argumentos de entrada,
  con un tamaño fijo en un tamaño fijo de terminal. Despues imprime los bordes.

    Parametros:
      sc1:  puntero a ventana 1 vacio donde se guarda la ventana 1 creada.
      sc2, sc3, sc4: igual que sc1 pero con su respectiva ventana
  
    Returns:
      1 si ha ido bien 0 si ha fallado
*/
int win_create4(sc_rectangle *sc1, sc_rectangle *sc2, sc_rectangle *sc3, sc_rectangle *sc4);

/*
  Writes the border of the window.

    Parameters:
      sc:  the window structure on which the function operates.
  
    Returns:
      The constant 1
  
*/
int win_write_border(sc_rectangle *sc);


/*
  Writes the borders of the 4 windows, combinating the borders that are together.

    Parameters:
      sc1, sc2, sc3, sc4:  the windows structure on which the function operates.
      sc1 is the top left, sc2 the top right, sc3 the bottom left, sc4 the bottom right
  
    Returns:
      The constant 1
    
    Result:
  ____________________
  |      SC1   | SC2 |
  |____________|_____|
  |      SC3   | SC4 |
  |____________|_____|
  
*/
int win_write_borders4(sc_rectangle *sc1, sc_rectangle *sc2, sc_rectangle *sc3, sc_rectangle *sc4);

/*
  This function writes a string in a single or multiple lines
  starting at a given row of the window, only in the second
  window of our game. This funtion is only for windows with 30 or 
  more colums.
  
  So, if on the screen of the example we execute the functions:
  
  win_write_line_sc2(sc2, "en un lugar de la mancha", 1);
  win_write_line_sc2(sc2, "esta El Bonillo", 2);
   
  
  we get:
  
  ____________________________________
  |      SC1   |         SC2         | <--line 0
  |            | en un lugar de la   | <--line 1  
  |            | mancha              |
  |            | esta El Bonillo     |
  |____________|_____________________|
  |      SC3   |         SC4         |
  |____________|_____________________|
  
    Parameters:
      sc:  the window structure on which the function operates.
      str: the screen that we must write
      r:   the row of the window at which the writing begins
           the row start in 0
  
    Returns:
      the number of rows actually written on the screen.
  
 */
int win_write_line_sc2(sc_rectangle *sc, char *str, int c);

int win_write_pista_sc4(sc_rectangle *sc, char *str, int c);

#endif  /* SC_WINDOW_H */
