#include "ima.h"

/*!\file decompress.c
 * \brief Fichier gérant la décompression RLE et la conversion HSV en RGB
 * \author Nehari Mohamed
 * \date 20 december 2016
 */


/*!\brief Fonction qui décompresse le fichier compréssé.
  Trois fichiers temporaires sont créés lors de la decompression afin d'isoler les 3 couleurs R, G et B.
  Enfin, le regroupement des trois couleurs est éfféctué pour généré le fichier décompréssé.*/
int decompress()
{
  FILE *in = NULL, *out = NULL, *tempR=NULL, *tempB=NULL, *tempG=NULL, *tempFile=NULL;
  char c;
  int r, i, value, sizeHeader, turnLoop = 0;
  unsigned long size;
  int mark;
  
  tempR = fopen("tempR","w+");
  assert(tempR);
  tempG = fopen("tempG","w+");
  assert(tempG);
  tempB = fopen("tempB","w+");
  assert(tempB);
  tempFile = tempR;

  if((in = fopen("compress","rb")) == NULL)
    {
      fprintf(stderr,"Échec de la décompression : Fichier inexistant, ou echec du chargement\n");
      return -1;
    }
  out = fopen("decompress.ppm","wb");
  assert(out);  

  //Récupération de la taille du Header
  sizeHeader = sizeOfHeader(in);
  
  //Ecrire le header de l'image dans le fichier décompréssé
  while(sizeHeader--)
    fputc(fgetc(in),out);

  //Récupération de la taille du fichier pour le parcourir
  mark = ftell(in);
  fseek(in,0,SEEK_END);
  size = ftell(in);
  fseek(in,mark,SEEK_SET);
  fprintf(stderr,"size = %lu\n",size);

  c=fgetc(in);
  //Décompression des pixels
  for(i=0 ; i<size ; i++, c=fgetc(in))
    {
      
      //Si code de controle pour changement de couleur
      if(c == 0)
	{
	  turnLoop++;
	  if(turnLoop == 1) tempFile = tempG;
	  if(turnLoop == 2) tempFile = tempB;
	  continue;
	}

      //Si 'c' est positif, donc repetition d'un meme octet
      if(c > 0)
	{
	  i++;
	  value = fgetc(in);
	  while(c--)
	    fprintf(tempFile,"%c",value);
	  
	  continue;
	}
      
      //Si 'c' est négatif, donc écriture des octect sans répétition 
      if(c <= 0)
	{
	  c *= -1;
	  while(c--)
	    {
	      fprintf(tempFile,"%c",fgetc(in));
	      i++;
	    }
	}
    }

  rewind(tempR);
  rewind(tempG);
  rewind(tempB);

  //regroupement des trois champ en un seul fichier
  while((r = fgetc(tempR)) != EOF)
    {
      fputc(r,out);
      fputc(fgetc(tempG),out);
      fputc(fgetc(tempB),out);      
    }

  fclose(tempR);
  fclose(tempG);
  fclose(tempB);
  remove("tempR");
  remove("tempG");
  remove("tempB");
  fclose(in);
  fclose(out);
  return 1;
}



/*! \brief Convert HSV to RGB color space
  
  Converts a given set of HSV values `h', `s', `v' into RGB
  coordinates. The output RGB values are in the range [0, 1], and
  the input HSV values are in the ranges h = [0, 360], and s, v =
  [0, 1], respectively.
*/
void HSV_TO_RGB(FILE *out, GLubyte h, GLubyte s, GLubyte v)
{
  float fH=h, fS=s, fV=v;
  fH *= 360;
  fH /= 255;
  fS /= 255;
  fV /= 255;
  float fC = fV * fS; 
  float fR,fG,fB;
  float fHPrime = fmod(fH / 60.0, 6);
  float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
  float fM = fV - fC;
  
  if(0 <= fHPrime && fHPrime < 1) {
    fR = fC;
    fG = fX;
    fB = 0;
  } else if(1 <= fHPrime && fHPrime < 2) {
    fR = fX;
    fG = fC;
    fB = 0;
  } else if(2 <= fHPrime && fHPrime < 3) {
    fR = 0;
    fG = fC;
    fB = fX;
  } else if(3 <= fHPrime && fHPrime < 4) {
    fR = 0;
    fG = fX;
    fB = fC;
  } else if(4 <= fHPrime && fHPrime < 5) {
    fR = fX;
    fG = 0;
    fB = fC;
  } else if(5 <= fHPrime && fHPrime < 6) {
    fR = fC;
    fG = 0;
    fB = fX;
  } else {
    fR = 0;
    fG = 0;
    fB = 0;
  }
  
  fR += fM;
  fG += fM;
  fB += fM;

  fR *= 255;
  fG *= 255;
  fB *= 255;

  fprintf(out,"%c%c%c",(int)fR,(int)fG,(int)fB);
  static int i = 0;
  if(i<2)
    fprintf(stderr,"%d  %d  %d\n",(int)fR,(int)fG,(int)fB);
  i++;
}


/*!\brief Fonction qui parcoure chaque pixel de l'image, et les convertit en RGB*/
int convertHSV_to_RGB()
{
  FILE *in = NULL;
  FILE *out = NULL;
  int c;
  GLubyte h,s,v;
  int sizeHeader;

  
  if((in = fopen("decompress.ppm","r+")) == NULL)
    {
      fprintf(stderr,"Échec de la conversion en RGB : Fichier inexistant, ou echec du chargement\n");
      return -1;
    }

  out = fopen("decompressHSV.ppm","w+");
  assert(out);

  //Récupération de la taille du Header
  sizeHeader = sizeOfHeader(in);

  while(sizeHeader--)
    fputc(fgetc(in),out);
  
  //Décompression des pixels
  while((c = fgetc(in)) != EOF)

    {
      h = c;
      s = fgetc(in);
      v = fgetc(in);
      
      HSV_TO_RGB(out, h, s, v);
    }

  remove("decompress.ppm");
  return 1;
  fclose(in);
  fclose(out);
}
