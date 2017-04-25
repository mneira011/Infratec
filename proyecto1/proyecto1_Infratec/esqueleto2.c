// isis1304-111-proyecto2.cpp: define el punto de entrada de la aplicaci�n de consola.
// Se recuerda a los estudiantes que el proyecto debe ser desarrollado obligatoriamente en grupos de 3.
//
// DESARROLLADO POR:
// Mauricio Neira, 201424001
// Juan Felipe Ramos, 201616932
// Amilkar Eslava, 201611332

#define _CRT_SECURE_NO_DEPRECATE
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

// La representacion de la imagen
typedef struct img
{
	int ancho;
	int alto;
	unsigned char *informacion;
} Imagen;
//Cantidad de caracteres en el mensaje insertado
int lenPal = 0;
//Esta funcion convierte un caracter ascii en decimal a un string de 8 bits que corresponde
//al numero representado en binario
char *asciiABinario(int numDecimal);
//Esta funcion devuelve un string en binario que
//representa el mensaje
char *darMensajeBinario(unsigned char mensaje[],int n);
//esta funcion pasa el estring de 8 bits correspondiende a
//el valor correspondiente (int) en decimal
int binarioADec(char *palBinario);

// Funcion que carga el bmp en la estructura Imagen
void cargarBMP24 (Imagen * imagen, 	char * nomArchivoEntrada);

// Funcion que guarda el contenido de la estructura imagen en un archivo binario
void guardarBMP24 (Imagen * imagen, char * nomArchivoSalida);

//Funcion que inserta un mensaje en la imagen usando n bits por Byte
void insertarMensaje(Imagen * img , unsigned char mensaje[], int n);

//Funcion que lee un mensaje de una imagen dando la longitud del mensaje y el numero de bits por byte usados
void leerMensaje(Imagen * img, unsigned char msg[], int l, int n);

unsigned char sacarNbits(unsigned char mensaje[],int bitPos,int n);

// Programa principal
// NO MODIFICAR
int main(int argc, char* argv[]) {
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

	printf("Indique la accion\n\t1) insertar mensaje\n\t2) leer mensaje\n\n");

	gets(op);

    if(op[0] == '1') {
        printf("ingrese el mensaje a insertar\n");

        gets(msg);

        printf("longitud mensaje: %d\n",strlen(msg));

        num=0;
        printf("ingrese el numero de bits por Byte\n");
        scanf("%d",&num);

        insertarMensaje(img,msg,num);

        guardarBMP24 (img, nomArch1);

        printf("mensaje insertado\n");
    } else if(op[0] =='2') {
        printf("ingrese la longitud del mensaje insertado\n");

        scanf("%d",&l);

        printf("ingrese el numero de bits por Byte\n");
        scanf("%d",&n);

        for(i=0;i<l;i++) {
            msg[i]=0;
        }

        leerMensaje(img,msg, l, n);

        msg[l]=0;

        printf("el mensaje es: %s\n",msg);
    } else {
		printf("%s","Hubo un error al cargar el archivo\n");
	}
	system ("pause");
	return 0;
}

/**
* Inserta un mensaje, de a n bits por componente de color, en la imagen apuntada por img
* parametro img Apuntador a una imagen en cuyos pixeles se almacenar� el mensaje.
* parametro mensaje Apuntador a una cadena de caracteres con el mensaje.
* parametro n Cantidad de bits del mensaje que se almacenar�n en cada componente de color de cada pixel. 0 < n <= 8.
*/
void insertarMensaje( Imagen * img , unsigned char mensaje[], int n ) {
	//TODO  Desarrollar completo en C
	//un apuntador al mensaje de entrada
	char *pMensaje = mensaje;
	//esto apunta a la palabra en binario que tenemos
	//que insertar
	char *pal = darMensajeBinario(pMensaje,n);

	//este loop itera sobre los caracteres
	//e inserta en la imagen los bits correspondientes
	for(int i = 0; i<lenPal ; i +=n){
		//esta variable representa la palabra en binario (al reves)
		//que toca insertar en la casilla
		char temp[9];
		//esto obtiene los bits menos significativos
		for(int j = 0 ; j<n ; j++){
				temp[7-j] = pal[i+j];
		}
		//esto rellena con ceros a la izq para que quede la palabra
		//de 8 bits
		for(int j = 0; j<(8-n); j++){
			temp[j] = '0';
		}
		//terminamos la palabra con el simbolo de terminacion
		temp[8] = '\0';
		//creamos un apuntado a la palabra
		char *pTemp = temp;
		//obtengo los bits menos significativos de la
		//imagen que voy a remplazar
		int res = img->informacion[i/n] % (1<<n);
		//creo un entero con unicamente los valores
		//mas significativos de la casilla
		int valImg = img->informacion[i/n]- res ;
		//le inserto los digitos que codifican el mensaje
		valImg = valImg +  binarioADec(pTemp);
		//le meto este valor a la imagen
		img->informacion[i/n] = valImg;

	}
	//imprimo la longitud del mensaje
	printf("La longitud del mensaje insertado es de: %d\n",lenPal/8 );
}

/**
* Extrae un mensaje de tama�o l, guardado de a n bits por componente de color, de la imagen apuntada por img
* parametro img Apuntador a una imagen que tiene almacenado el mensaje en sus pixeles.
* parametro msg Apuntador a una cadena de caracteres donde se depositar� el mensaje.
* parametro l Tama�o en bytes del mensaje almacenado en la imagen.
* parametro n Cantidad de bits del mensaje que se almacenan en cada componente de color de cada pixel. 0 < n <= 8.
*/
void leerMensaje( Imagen * img, unsigned char msg[], int l, int n ) {
	//TODO  Desarrollar completo en C
	//longitud de bits
	int lenBits = l*8;
	//numero de casillas
	int casillas = lenBits/n;
	if(lenBits%n!=0){
		casillas +=1;
	}
	//mensaje en binario
	char ansBin[l*8+1];

	//iteramos sobre todas las casillas
	for(int i = 0; i<casillas;i++){
		//iteramos sobre la cantidad de bits por byte
		for(int j = 0; j<n;j++){
				//le agregamos (al reves pues insertamos al reves)
				//al mensaje en binario el valor correspondiente en binario
				ansBin[j+i*n] = asciiABinario(img -> informacion[i]%(1<<n))[7-j] ;
		}


	}
	//terminamos la palabra con el caracter de finalizacion
	ansBin[l*8] = '\0';
	//creamos una apuntador a esta palabra
	char *binaryArray = ansBin;

	//iteramos sobre la palabra en binario
	for(int i =0 ; i< lenBits;i+=8){
		//una letra del mensaje representada
		//en un string de bits
		char letraAscii[9];
		//iteramos sobre segmentos de 8 bits
		for(int j = 0; j<8 ; j++){

			letraAscii[j] = binaryArray[i+j];
		}
		letraAscii[8] = '\0';
		//pasamos el string en binario a
		//el caracter correspondiente
		char letra =binarioADec(letraAscii);
		//creamos un apuntador a esta letra
		char *pointerLetra = &letra;
		//insertamos la letra en el mensaje
		msg[i/8] = letra;


	}

}

/**
* Extrae n bits a partir del bit que se encuentra en la posici�n bitpos en la secuencia de bytes que
* se pasan como par�metro
* parametro secuencia Apuntador a una secuencia de bytes.
* parametro n Cantidad de bits que se desea extraer. 0 < n <= 8.
* parametro bitpos Posici�n del bit desde donde se extraer�n los bits. 0 <= n < 8*longitud de la secuencia
* retorno Los n bits solicitados almacenados en los bits menos significativos de un unsigned char
*/
unsigned char sacarNbits( unsigned char secuencia[], int bitpos, int n ) {
	//TODO [Opcional Sugerido] Desarrollar completo en C
}

// Lee un archivo en formato BMP y lo almacena en la estructura img
// NO MODIFICAR
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

// Esta funcion se encarga de guardar una estructura de Imagen con formato de 24 bits (formato destino) en un archivo binario
// con formato BMP de Windows.
// NO MODIFICAR
void guardarBMP24 (Imagen * imagen, char * nomArchivoSalida) {
	unsigned char bfType[2];
	unsigned int bfSize, bfReserved, bfOffBits, biSize, biWidth, biHeight, biCompression, biSizeImage, biXPelsPerMeter, biYPelsPerMeter, biClrUsed, biClrImportant;
	unsigned short biPlanes, biBitCount;
	FILE * archivoSalida;
	int y, x;
	int relleno = 0;

	int residuo = (4 - (imagen->ancho) % 4)&3; // Se debe calcular los bits residuales del bmp, que quedan al forzar en palabras de 32 bits


	bfType[2];       // Tipo de Bitmap
	bfType[0] = 'B';
	bfType[1] = 'M';
	bfSize = 54 + imagen -> alto * ((imagen -> ancho)/3) * sizeof (unsigned char);       // Tamanio total del archivo en bytes
	bfReserved = 0;   // Reservado para uso no especificados
	bfOffBits = 54;    // Tamanio total del encabezado
	biSize = 40;      // Tamanio del encabezado de informacion del bitmap
	biWidth = (imagen -> ancho)/3;     // Ancho en pixeles del bitmap
	biHeight = imagen -> alto;    // Alto en pixeles del bitmap
	biPlanes = 1;    // Numero de planos
	biBitCount = 24;  // Bits por pixel (1,4,8,16,24 or 32)
	biCompression = 0;   // Tipo de compresion
	biSizeImage = imagen -> alto * imagen -> ancho;   // Tamanio de la imagen (sin ecabezado) en bits
	biXPelsPerMeter = 2835; // Resolucion del display objetivo en coordenada x
	biYPelsPerMeter = 2835; // Resolucion del display objetivo en coordenada y
	biClrUsed = 0;       // Numero de colores usados (solo para bitmaps con paleta)
	biClrImportant = 0;  // Numero de colores importantes (solo para bitmaps con paleta)

	archivoSalida = fopen (nomArchivoSalida, "w+b"); // Archivo donde se va a escribir el bitmap
	if (archivoSalida == 0) {
		printf ("No ha sido posible crear el archivo: %s\n", nomArchivoSalida);
		exit (-1);
	}

	fwrite (bfType, sizeof(char), 2, archivoSalida); // Se debe escribir todo el encabezado en el archivo. En total 54 bytes.
	fwrite (&bfSize, sizeof(int), 1, archivoSalida);
	fwrite (&bfReserved, sizeof(int), 1, archivoSalida);
	fwrite (&bfOffBits, sizeof(int), 1, archivoSalida);
	fwrite (&biSize, sizeof(int), 1, archivoSalida);
	fwrite (&biWidth, sizeof(int), 1, archivoSalida);
	fwrite (&biHeight, sizeof(int), 1, archivoSalida);
	fwrite (&biPlanes, sizeof(short), 1, archivoSalida);
	fwrite (&biBitCount, sizeof(short), 1, archivoSalida);
	fwrite (&biCompression, sizeof(int), 1, archivoSalida);
	fwrite (&biSizeImage, sizeof(int), 1, archivoSalida);
	fwrite (&biXPelsPerMeter, sizeof(int), 1, archivoSalida);
	fwrite (&biYPelsPerMeter, sizeof(int), 1, archivoSalida);
	fwrite (&biClrUsed, sizeof(int), 1, archivoSalida);
	fwrite (&biClrImportant, sizeof(int), 1, archivoSalida);

	// Se escriben en el archivo los datos RGB de la imagen.
	for (y = 0; y < imagen -> alto; y++) {
		for (x = 0; x < imagen -> ancho; x++) {
			int pos = y * imagen -> ancho + x;
			fwrite (&imagen -> informacion[pos], sizeof(unsigned char), 1, archivoSalida);
		}
	    fwrite(&relleno, sizeof(unsigned char), residuo, archivoSalida);
	}
	fclose(archivoSalida);
}


char *darMensajeBinario(unsigned char mensaje[],int n){
	//numero de caracteres
  int numChars = strlen(mensaje);
	//mensaje representado por un string en binario
  char palBinario[10000*8] = "";
	//apuntador al string anterior
  char *ans = palBinario;
	int cont = 0;
	//concatenamos la palabra en binario a
	//palBinario
  for(int i =0 ;i<numChars;i++){
    char *temp = asciiABinario(mensaje[i]);

    strcat(palBinario,temp);
		// printf("palBinario: %s\n"	,temp );
		cont +=8 ;
  }
	//rellenamos con ceros para que sea un multiplo de
	//los bits por byte
  int aIterar = (n-(strlen(palBinario)%n));
	aIterar = aIterar%n;
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

  for(int i = 0; i < 8; i++){
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
