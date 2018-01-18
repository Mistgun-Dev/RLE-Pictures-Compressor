#include "ima.h"


/*!\file main.c
 * \brief Fichier contenant la fonction main ainsi que les fonctions
 * nécéssaires au chargement du contexte Opengl.
 * \author Jean-Jacques Bourdin
 * \date 12 december 2016
 */


/*!\brief Fonction qui gère les évènements clavier du programme*/
void Keyboard(unsigned char key, int x, int y)  {
  switch(key){
  case ESCAPE :
    exit(0);                   
    break;
  default:
    fprintf(stderr, "Unused key\n");
  }
}

/*\brief Fonction qui gère les évènements souris du programme*/
void Mouse(int button, int state, int x, int y) {

  switch(button){
  case GLUT_LEFT_BUTTON:
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    break;    
  }
  glutPostRedisplay();
}

/*!\brief Fonction qui charge les données en mémoire, et initialise le contexte Opengl*/
int Init(char *s) {

  originalPics = 1;
  image = (Image *) malloc(sizeof(Image));
  if (image == NULL) {
    fprintf(stderr, "Out of memory\n");
    return(-1);
  }
  if (ImageLoad_PPM(s, image)==-1) 
	return(-1);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);
  
  return (0);
}

/*!\brief Fonction qui redimensionne la taille de la fenêtre en fonction de la taille de l'image*/
int ReInit() {

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);
  
  return (0);
}

/*!brief Fonction qui affiche l'image chargée à l'écran*/
void Display(void) {

  glClear(GL_COLOR_BUFFER_BIT);
  glDrawPixels(image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, 
	       image->data);

  glFlush();
}

/*!\brief Fonction qui actualise le contexte Opengl lors du redimensionnement de la fenêtre*/
void Reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble)h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/*!\brief 
  Fonction qui gère les différentes options du menu accessible par le clique gauche
  - Compression sur RGB
  - Compressions sur HSV
  - Decompression
  - Informations de l'image
  - Quitter*/
void menuFunc(int item) {
  float res;
  
  switch(item){ 
  case 0:
    printf("\nCompression RGB ...\n");
    if(originalPics == -1)
      {
	refresh_original();
	Display();
      }
    res = compress();
    fprintf(stderr,"Le taux de compression sur RGB est de : %.2f%% \n",res);
    fprintf(stderr,"Le fichier compress a été généré \n");
    break;
  case 1:
    printf("\nCompression en mode HSV ...\n");
    if(originalPics != -1)
      convertRGB_to_HSV();
    else
      fprintf(stderr,"L'image a déja été convertie en HSV\n");
     
    res = compress();
    fprintf(stderr,"Le taux de compression après conversion en HSV est de : %.2f%% \n",res);
    fprintf(stderr,"Le fichier compress a été généré \n");
    break;
  case 2:
    printf("\nDecompression ...\n");
    res = decompress();
    if(originalPics != 1)
      convertHSV_to_RGB();
       
    if(res == 1)
	printf("Decompression terminé, le fichier decompressé a été généré \n");
    break;
  case 3:
    printf("\nInformations sur l'image ...\n");
    printf("Taille de l image : %lu %lu\n", image->sizeX,  image->sizeY);
    printf("Taille totale en octet : %lu\n", image->sizeX * image->sizeY * 3 + image->sizeHeader);
    printf("taille du Header : %lu\n", image->sizeHeader);
    break;
  case 4:
    free(image);
    exit(0);
    break;
  default:
    break;
  }
}

/*!brief Fonction main ...*/
int main(int argc, char **argv) 
{ 
  if (argc<2) 
    {
      fprintf(stderr, "Usage : rle nom_de_fichier.ppm\n");  
      exit(0);
    }
  
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowPosition(100, 100);  
  glutCreateWindow("Compression RLE");  

  Init(argv[1]);
  
  glutCreateMenu(menuFunc);
  glutAddMenuEntry("Compression RGB (original)", 0);
  glutAddMenuEntry("Compression HSV (conversion)", 1);
  glutAddMenuEntry("Decompression", 2);
  glutAddMenuEntry("Informations", 3);
  glutAddMenuEntry("Quitter", 4);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutDisplayFunc(Display);  
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);
  
  glutMouseFunc(Mouse);
  glutMainLoop();  

  return 1;
}
