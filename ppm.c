#include "ima.h"

#define INSPIREPAR "RPFELGUEIRAS"
#define CREATOR "JJ"
#define RGB_COMPONENT_COLOR 255

/*!\file ppm.c
 * \brief Fichier permettant de charger une image en mémoire, et d'y éfféctuer des modifications
 * \author Jean-Jacques Bourdin
 * \date 20 december 2016
 */

/*!\brief Fonction qui retourne le nombre d'octet que compose le header de l'image*/
int sizeOfHeader(FILE *in)
{
  int acc = 0, eol = 0;
  rewind(in);
  int c;
  for(;;)
    {
      while (c == '#') {
	while (getc(in) != '\n')
	  acc++;
      }
      
      acc++;
      if((c=fgetc(in)) == '\n')
	eol++;
      if(eol == 3)
	break;
    }
  rewind(in);
  return acc;
}

/*Fonction qui teste et charge l'image PPM en mémoire ainsi que ses caractéristiques*/
int ImageLoad_PPM(char *filename, Image *img)
{
  char buff[16];
  FILE *fp;
  int c, size, i, rgb_comp_color;
  
  //open PPM file for reading
  fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Unable to open file '%s'\n", filename);
    exit(1);
  }
  
  //read image format
  if (!fgets(buff, sizeof(buff), fp)) {
    perror(filename);
    exit(1);
  }
  
  //check the image format
  if (buff[0] != 'P' || buff[1] != '6') {
    fprintf(stderr, "Invalid image format (must be 'P6')\n");
    exit(1);
  }
  
  //check for comments
  c = getc(fp);
  while (c == '#') {
    while (getc(fp) != '\n')
      ;
    c = getc(fp);
  }
  ungetc(c, fp);
  //read image size information
  if (fscanf(fp, "%lu %lu", &img->sizeX, &img->sizeY) != 2) {
    fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
    exit(1);
  }
  
  
  //read rgb component
  if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
    fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
    exit(1);
  }

  //check rgb component depth
  if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
    fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
    exit(1);
  }
 
  /* allocation memoire */
  size = img->sizeX * img->sizeY * 3 ;
  img->data = (GLubyte *) malloc ((size_t) (size) * sizeof (GLubyte));
  assert(img->data);
  img->original_data = (GLubyte *) malloc ((size_t) (size) * sizeof (GLubyte));
  assert(img->original_data);
  
  //récupérer la taille du Header
  img->sizeHeader = sizeOfHeader(fp);

  //lire le header de l'image
  img->header = (GLubyte *) malloc ((size_t) img->sizeHeader * sizeof (GLubyte)+1);
  for(i=0;i<img->sizeHeader;i++)
    img->header[i] = fgetc(fp);
  
  //read pixel data from file
  if (fread(img->data, (size_t) 1, (size_t) size, fp) == 0) 
    fprintf(stderr, "Error loading image '%s'\n", filename);
   
  return_picture(image);
  memcpy(img->original_data , img->data, size);

  fclose(fp);
  return 1;
}

/*Fonction qui effectue une rotation de 180° sur l'image*/
void return_picture(Image *img)
{
  GLubyte tmp, * ptrdeb, *ptrfin, *lastline;
  int size =  3 * (img->sizeX * img->sizeY);
  int b, c, sizex, sizey;
   
  sizex = img->sizeX;
  sizey = img->sizeY;
  lastline = img->data + size - sizex * 3;
  for (b = 0; b < sizey / 2; b++) {
    ptrdeb = img->data + b * sizex * 3;
    ptrfin = lastline - (b * sizex * 3);

    for (c = 0; c < 3 * sizex; c++) {
      tmp = *ptrdeb;
      *ptrdeb = *ptrfin;
      *ptrfin = tmp;
      ptrfin++;
      ptrdeb++;
    }		
  }
}

/*!\brief Fonction qui actualise les données original de l'image en RGB si il y a eu
  une conversion en HSV au préalable*/
void refresh_original()
{
  int i;
  int size = image->sizeX * image->sizeY * 3;

  for(i=0; i<size ;i++)
    image->data[i] = image->original_data[i];

  originalPics = 1;
}
