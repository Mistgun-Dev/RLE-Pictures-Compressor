#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h> 
#include <math.h>

#define __OSX__
#ifndef __OSX__
#include <GL/gl.h>	
#else
#include <GL/glut.h>
#endif

#define ESCAPE 27

/*!\file ima.h
 * \brief Fichier regroupant les structures et variables globales,
 *  ainsi que les prototypes des fonctions du programmes
 * \author Jean-Jacques Bourdin
 * \date 20 december 2016
 */

struct Image {
  unsigned long sizeX; // taille en absice de l'image
  unsigned long sizeY; // taille en ordonnée de l'image
  GLubyte *header; // data du Header de l'image
  unsigned long sizeHeader; // taille du Header de l'image
  GLubyte *data; //data de l'image
  GLubyte *original_data; // data original au cas ou on souhaite repasser de HSV à l'original
};
typedef struct Image Image;

/*Variables globale du programme*/
Image *image;  /*!< Variable globale acceuillant l'image à traiter */
int originalPics; /*!< Variable itnitialisé à 0, pase à 1 si il y a une conversion HSV */

void Display(void);

/*Prototypes des fonctions du fichier ppm.c*/
int sizeOfHeader(FILE *in);
int ImageLoad_PPM(char *filename, Image *image);
void return_picture(Image *img);
void refresh_original();

/*Prototypes des fonctions du fichier compress.c*/
int max(int a, int b);
int min(int a, int b);
float compress();
void RGB_TO_HSV(GLubyte *r, GLubyte *g, GLubyte *b);
void convertRGB_to_HSV();

/*Prototypes des fonctions du fichier decompress.c*/
int convertHSV_to_RGB();
void HSV_TO_RGB(FILE *out, GLubyte h, GLubyte s, GLubyte v);
int decompress();
