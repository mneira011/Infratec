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
int lenPal = 0;
char *asciiABinario(int numDecimal);
char *darMensajeBinario(unsigned char mensaje[],int n);
int binarioADec(char *palBinario);
void insertarMensaje( Imagen * img , unsigned char mensaje[], int n );
void leerMensaje(Imagen * img,char msg[], int l, int n);

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
	/*
	AQUI VOY A IMPLEMENTAR EL insertarMensaje, DESPUES LO PASO A FUNCION
	*/
	int tamArray = img->alto * img->ancho;
	char palabra[] = "jhggjgh";
	leerMensaje(img,palabra,13,3);
	char palabra2[]= "A alejandro le acabo de cambiar la vidaaaaaaaaa";
	insertarMensaje(img,palabra2,3);
	leerMensaje(img,msg,47,3);
	printf("%s\n", msg);
	// for(int i = 0; i<ta ; i++){
  //   printf("%d\n",img->informacion[i] );
  // }


	// for(int i = 0; i<tamArray ; i++){
  //   printf("%d\n",img->informacion[i] );
  // }




  // printf("%lu\n",sizeof(img->informacion) );
  // printf("%d\n",(img -> informacion[460800-1]) );
  // printf("%d\n", img->alto );
  //
  // printf("%d\n", img->ancho/3 );


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

void leerMensaje(Imagen * img,char msg[], int l, int n){

		int lenBits = l*8;
		int casillas = lenBits/n;
		if(lenBits%n!=0){
			casillas +=1;
		}
		char ansBin[l*8+1];
		// printf("%d\n",l*8 );
		int cont = 0;
		for(int i = 0; i<casillas;i++){
			// printf("%s\n", asciiABinario(img -> informacion[i]%(1<<n)));
			// char palabrita[n];
			// printf("%s\n",palabrita );
			for(int j = 0; j<n;j++){
				// printf("%d\n",j+i*n  ;
				// printf("%d\n",l*8 );
				// if((j+i*n ) <(l*8) ){
					// palabrita[j] =asciiABinario(img -> informacion[i]%(1<<n))[7-j] ;
					cont ++;
					ansBin[j+i*n] = asciiABinario(img -> informacion[i]%(1<<n))[7-j] ;
					// printf("%c\n", asciiABinario(img -> informacion[i]%(1<<n))[7-j] );
				// }
					// strcat(ansBin,palabrita);
			}

			// printf("%s\n",palabrita );
			// printf("%s\n",ansBin );
			// strcat(ansBin,palabrita);
		}
		ansBin[l*8] = '\0';
		char *binaryArray = ansBin;
		// printf("%d\n",cont );
		// printf("%s\n",binaryArray );

		for(int i =0 ; i< lenBits;i+=8){
			char letraAscii[9];
			for(int j = 0; j<8 ; j++){
				// printf("%d %c\n",i+j, binaryArray[i+j]	 );
				letraAscii[j] = binaryArray[i+j];
			}
			letraAscii[8] = '\0';
			char letra =binarioADec(letraAscii);
			char *pointerLetra = &letra;
			// strcat(msg,pointerLetra);
			msg[i/8] = letra;

			// printf("%s %c\n",letraAscii,binarioADec(letraAscii) );
			// printf("%s\n",letraAscii );
		}
		printf("%s\n",msg );
}
void insertarMensaje( Imagen * img , unsigned char mensaje[], int n ){

	char *pMensaje = mensaje;
	// printf("%s\n",mensaje);
	// printf("%s\n",pMensaje);
	char *pal = darMensajeBinario(pMensaje,n);
	// printf("%s\n",pal);
	for(int i = 0; i<lenPal ; i +=n){
		char temp[9];
		for(int j = 0 ; j<n ; j++){
				temp[7-j] = pal[i+j];
		}
		for(int j = 0; j<(8-n); j++){
			temp[j] = '0';
		}

		temp[8] = '\0';
		// printf("%s\n",temp );


		char *pTemp = temp;
		// printf("%s\n",pTemp );
		// printf("%d\n",binarioADec(pTemp) );
		//
		// printf("Dec: %d, binario: %s\n",binarioADec(pTemp) ,temp );
		// printf("Valor img: %d\n",img->informacion[i/n] );
		int res = img->informacion[i/n] % (1<<n);
		int valImg = img->informacion[i/n]- res + binarioADec(pTemp) ;
		// printf("%s\n",asciiABinario(valImg));

		// valImg = valImg - res + binarioADec(pTemp) ;

		// printf("original: %s valAinsertar: %s valfinal: %s\n", asciiABinario(img->informacion[i/n]),asciiABinario(binarioADec(pTemp)),asciiABinario(valImg));
		img->informacion[i/n] = valImg;

		// printf("Valor img: %d, modulo: %d, binario: %s, val-res: %s, temp: %s, newVal: %s\n ",img->informacion[i/n], res, asciiABinario(img->informacion[i/n]),asciiABinario(img->informacion[i/n]-res),temp,asciiABinario(img->informacion[i/n]-res+binarioADec(pTemp)));
	}
	printf("La longitud del mensaje insertado es de: %d\n",lenPal/8 );
}

char *darMensajeBinario(unsigned char mensaje[],int n){
  int numChars = strlen(mensaje);
  char palBinario[10000*8] = "";
  char *ans = palBinario;
	int cont = 0;
  for(int i =0 ;i<numChars;i++){
    char *temp = asciiABinario(mensaje[i]);

    strcat(palBinario,temp);
		// printf("palBinario: %s\n"	,temp );
		cont +=8 ;
  }

  int aIterar = (n-(strlen(palBinario)%n));

  for(int i =0 ; i<aIterar ; i++){
    strcat(palBinario,"0");
		cont +=1;
  }
	lenPal = cont;
  return ans;
}




/*
Esta funcion convierte un caracter ASCII
a un string que representa el numero en binario
*/
char *asciiABinario(int numDecimal){
  //declaramos el arreglo de chars donde iran los 0s y 1s
  char *ans = malloc(9*sizeof(char));
  //algoritmo para convertir el numero a binario
	ans[0]='0';
  for(int i = 0; i < 7; i++){
          int resp = numDecimal%2;
          if(resp==0){
              ans[8-i-1] = '0';
          }
          else{
            ans[8-i-1] = '1';
          }
          numDecimal = numDecimal/2;
  }
  ans[8] = '\0';
  return ans;
}

int binarioADec(char *palBinario){
	int ans = 0 ;
	for(int i =0; i<8 ;i++){
			if(palBinario[i]=='1'){
				ans += 1<<(7-i);
			}
	}
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
