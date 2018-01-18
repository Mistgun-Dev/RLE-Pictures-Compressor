#include "ima.h"

/*!\file compress.c
 * \brief Fichier gérant la compression RLE et la conversion RGB en HSV
 * \author Nehari Mohamed
 * \date 20 december 2016
 */


/*!\brief Fonction qui retourne le minimum des deux valeurs passées en paramètre*/
int min(int a, int b)
{
  if(a<b)
    return a;
  return b;
}

/*!\brief Fonction qui retourne le maximum des deux valeurs passées en paramètre*/
int max(int a, int b)
{
  if(a>b)
    return a;
  return b;
}

/*!\brief Fonction qui convertis un pixel RGB en HSV */
void RGB_TO_HSV(GLubyte *r, GLubyte *g, GLubyte *b)
{
  float fR=*r, fG=*g, fB=*b;
  float fH, fS, fV;
  float fCMax = max(max(fR, fG), fB);
  float fCMin = min(min(fR, fG), fB);
  float fDelta = fCMax - fCMin;
  
  if(fDelta > 0) {
    if(fCMax == fR) {
      fH = 60 * (fmod(((fG - fB) / fDelta), 6));
    } else if(fCMax == fG) {
      fH = 60 * (((fB - fR) / fDelta) + 2);
    } else if(fCMax == fB) {
      fH = 60 * (((fR - fG) / fDelta) + 4);
    }
    
    if(fCMax > 0) {
      fS = fDelta / fCMax;
    } else {
      fS = 0;
    }
    
    fV = fCMax;
  } else {
    fH = 0;
    fS = 0;
    fV = fCMax;
  }
  
  if(fH < 0) {
    fH = 360 + fH;
  }
  
  fH *= 255;
  fH /= 360;
  fS *= 255;
  
  *r = (int)fH;
  *g = (int)fS;
  *b = (int)fV;
}


/*!\brief Fonction qui parcoure le tableau de données de l'image et converti chaque pixel en HSV*/
void convertRGB_to_HSV()
{
  int i;
  int size = image->sizeX * image->sizeY * 3;

  return_picture(image);
  
  originalPics = -1;

  for(i=0; i<size-3 ; i+=3)
    RGB_TO_HSV(&image->data[i], &image->data[i+1], &image->data[i+2]);

  return_picture(image);
  
  Display();
}



/*!\brief Fonction qui compresse les données de l'image, que l'image soit en HSV OU RGB la façon de compresser
  reste la meme, donc une fonction pour les deux modes.
  - retourne en sortie le ratio de compression de l'image*/
float compress()
{
  FILE *out = NULL;
  int seuil = 2;
  int repetition = 1, noRepetition = 0, loopTurn = 0, i,j;
  int size = image->sizeX * image->sizeY * 3;
  long sizeInitial, sizeCompress;
  float compressRatio;
 
  //mettre l'image dans le bon sens
  return_picture(image);

  out = fopen("compress","w");
  assert(out);

  //Ecrire le Header dans le fichier compressé
  for(i=0;i<image->sizeHeader; i++)
    fputc(image->header[i], out);
  
  //Compresser les pixels
  for(i=0;;)
    {
      //Tant que la valeur de la couleur du pixel suivant est identique
      while(repetition < 127 && image->data[i] == image->data[i+3] && i < size)
	{
	  repetition++;
	  i+=3;
	}

      //Ecrire les repetitions dans le fichier compréssé
      if(repetition >= seuil)
	{
	  fprintf(out,"%c%c",repetition,image->data[i]);
	  repetition = 1;
	  i+=3;
	}
	 
      //Tant que la valeur du pixel suivant est différente
      while(noRepetition < 128 && image->data[i] != image->data[i+3] && i < size)
	{
	  noRepetition++;
	  i+=3;
	}

      //Ecrire les non répétition dans le fichier compréssé
      if(noRepetition > 0)
	{
	  fprintf(out,"%c",-noRepetition);
	  
	  //Ecriture des pixels non répétés dans le fichier compréssé
	  for(j = i-(noRepetition*3); j<i ; j+=3)
	    fprintf(out,"%c",image->data[j]);
	  
	  noRepetition = 0;
	}

      //s'arreter si on dépasse la taille totale
      if(i >= size)
	{
	  //si les 3 couleurs ont été traitées, on arrete la boucle
	  if(++loopTurn == 3)
	    break;
	  //sinon, on compresse la couleur suivante
	  else
	    {
	      fprintf(out,"%c",0);
	      //Si compression sur R terminée, on passe à la compression sur G
	      if(loopTurn == 1)
		i = 1;
	      //Si compression sur G terminée, on passe à la compression sur B
	      if(loopTurn == 2)
		i = 2;
	    }
	}
    }

  //Calculer le taux de compression
  sizeInitial = image->sizeX * image->sizeY * 3 + image->sizeHeader;
  sizeCompress = ftell(out);
  compressRatio = (sizeInitial - sizeCompress);
  compressRatio /= sizeInitial;

  return_picture(image);
  fclose(out);

  return compressRatio*100;
}
