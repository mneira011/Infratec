#include "stdlib.h"
#include "stdio.h"
#include "string.h"
typedef struct img
{
	int ancho;
	int alto;
	unsigned char *informacion;
} Imagen;

/*
Esta funcion convierte un caracter ASCII
a un string que representa el numero en binario
*/
char *asciiABinario(int numDecimal);
char *darMensajeBinario(unsigned char mensaje[],int n);


// Funcion que carga el bmp en la estructura Imagen
void cargarBMP24 (Imagen * imagen, char * nomArchivoEntrada);


//funciones del proyecto
//NO TOCAAAR



int main(int argc, char const *argv[]) {
  Imagen *img = (Imagen *) malloc (sizeof (Imagen));
	char msg[10000];
    char op[2];
	int num;
	int l;
	int i;
	int n;
	char nomArch1 [256] = "";
  if (argc != 2) {
		printf ("Faltan argumentos - Debe ser un archivo\n");
		system("pause");
		return -1;
	}

	strcat (nomArch1, argv[1]);

	// Cargar los datos
	cargarBMP24 (img, nomArch1);

  int tamArray = img->alto * img->ancho;
  // printf("%lu\n",sizeof(img->informacion) );
  // printf("%d\n",(img -> informacion[460800-1]) );
  // printf("%d\n", img->alto );
  //
  // printf("%d\n", img->ancho/3 );
  for(int i = 0; i<tamArray ; i++){
    printf("%d\n",img->informacion[i] );
  }
  // char stringPrueba[256] = "";
  // char a[] = "B";
  // char b[] = "a";
  //
  // strcat (stringPrueba, a);
  // strcat (stringPrueba, b);
  // printf("%s\n",stringPrueba );

  // printf("%s\n",darMensajeBinario("holaaaa",3));

  // char a = 'B';
  // char *binario = asciiABinario(a);
  // printf("%s\n",binario );
  return 0;
}

char *darMensajeBinario(unsigned char mensaje[],int n){
  int numChars = strlen(mensaje);
  char palBinario[10000*8] = "";
  char *ans = palBinario;
  for(int i =0 ;i<numChars;i++){
    char *temp = asciiABinario(mensaje[i]);

    strcat(palBinario,temp);
  }

  int aIterar = (n-(strlen(palBinario)%n));
  for(int i =0 ; i<aIterar ; i++){
    strcat(palBinario,"0");

  }

  return ans;
}




/*
Esta funcion convierte un caracter ASCII
a un string que representa el numero en binario
*/
char *asciiABinario(int numDecimal){
  //declaramos el arreglo de chars donde iran los 0s y 1s
  char *ans = malloc(8*sizeof(char));
  //algoritmo para convertir el numero a binario
  for(int i = 0; i < 7; i++){
          int resp = numDecimal%2;
          if(resp==0){
              ans[7-i-1] = '0';
          }
          else{
            ans[7-i-1] = '1';
          }
          numDecimal = numDecimal/2;
  }
  ans[7] = '\0';
  return ans;
}


void cargarBMP24 (Imagen * imagen, char * nomArchivoEntrada) {
	// bmpDataOffset almacena la posici�n inicial de los datos de la imagen. Las otras almacenan el alto y el ancho
	// en pixeles respectivamente
	int bmpDataOffset, bmpHeight, bmpWidth;
	int y;
	int x;
	int	residuo;

	FILE *bitmapFile;
	bitmapFile = fopen (nomArchivoEntrada, "rb");
	if (bitmapFile == NULL) {
		printf ("No ha sido posible cargar el archivo: %s\n", nomArchivoEntrada);
		exit (-1);
	}
  fseek (bitmapFile, 10, SEEK_SET); // 10 es la posici�n del campo "Bitmap Data Offset" del bmp
	fread (&bmpDataOffset, sizeof (int), 1, bitmapFile);

	fseek (bitmapFile, 18, SEEK_SET); // 18 es la posici�n del campo "height" del bmp
	fread (&bmpWidth, sizeof (int), 1, bitmapFile);
	bmpWidth = bmpWidth*3;

	fseek (bitmapFile, 22, SEEK_SET); // 22 es la posici�n del campo "width" del bmp
	fread (&bmpHeight, sizeof (int), 1, bitmapFile);

	residuo = (4 - (bmpWidth) % 4)&3; // Se debe calcular los bits residuales del bmp, que surjen al almacenar en palabras de 32 bits

	imagen -> ancho = bmpWidth;
	imagen -> alto = bmpHeight;
	imagen -> informacion = (unsigned char *) calloc (bmpWidth * bmpHeight, sizeof (unsigned char));

	fseek (bitmapFile, bmpDataOffset, SEEK_SET); // Se ubica el puntero del archivo al comienzo de los datos

	for (y = 0; y < bmpHeight; y++)	 {
		for ( x= 0; x < bmpWidth; x++)  {
			int pos = y * bmpWidth + x;
			fread (&imagen -> informacion [pos], sizeof(unsigned char ), 1, bitmapFile);
		}
		fseek(bitmapFile, residuo, SEEK_CUR); // Se omite el residuo en los datos
	}
	fclose (bitmapFile);
}
