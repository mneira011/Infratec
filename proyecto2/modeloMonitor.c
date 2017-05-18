// isis1304-111-proyecto2.cpp: define el punto de entrada de la aplicación de consola.
//
// DESARROLLADO POR:
// Nombre, carnet
// Nombre, carnet
// Nombre, carnet

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


// Función que carga el bmp en la estructura Imagen
void cargarBMP24 (Imagen * imagen, char * nomArchivoEntrada);

// Funcion que guarda el contenido de la estructura imagen en un archivo binario
void guardarBMP24 (Imagen * imagen, char * nomArchivoSalida);

void decidir(char nomArch1[], char op, int bitsPorByte, char arg4[], Imagen * img);

//Funcion que inserta un mensaje en la imagen usando n bits por Byte
void insertarMensaje(Imagen * img , char mensaje[], int n);

//Funcion que lee un mensaje de una imagen dando la longitud del mensaje y el numero de bits por byte usados
void leerMensaje(Imagen * img, char msg[], int l, int n);

unsigned char sacarNbits(char mensaje[],int bitPos,int n);

// Programa principal
// NO MODIFICAR
int main(int argc, char* argv[]) {
	Imagen *img = (Imagen *) malloc (sizeof (Imagen));
	char arg4[10000] = "";
	char op;
	int bitsPorByte;
	int i;
	int n;
	char nomArch1 [256] = "";

	if (argc != 5) {
		printf ("Faltan argumentos - Debe ser un archivo\n");
		system("pause");
		return -1;
	}

	strcat(nomArch1, argv[1]);
	op = argv[2][0];
	sscanf(argv[3], "%d", &bitsPorByte);
	strcpy(arg4, argv[4]);

	printf("op: %c\n", op);
	printf("bits per Byte: %d\n", bitsPorByte);
	printf("arg4: %s\n", arg4);

	// Cargar los datos
	cargarBMP24 (img, nomArch1);

	decidir(nomArch1, op, bitsPorByte, arg4, img);
	system("pause");
	return 0;
}


void decidir(char nomArch1[], char op, int bitsPorByte, char arg4[], Imagen * img) {
	int i, length;
	if (op == '1') {
		insertarMensaje(img, arg4, bitsPorByte);
		printf("longitud mensaje: %d\n", strlen(arg4));
		printf("mensaje insertado\n");
	} else if (op == '2') {
		sscanf(arg4, "%d", &length);
		for (i = 0; i<length; i++) {
			arg4[i] = 0;
		}
		leerMensaje(img, arg4, length, bitsPorByte);
		arg4[length] = 0;
		printf("el mensaje es: %s\n", arg4);
	} else {
		printf("%s", "Hubo un error al cargar el archivo\n");
	}
}

void insertarMensaje(Imagen * img , char mensaje[], int n) {
     int iImg = 0;
     int bitpos = 0;
     unsigned char nByte;

	 unsigned char m = 255 << n;

     while(mensaje[(bitpos/8)] != 0) {
		 nByte = img -> informacion[iImg] & m;
		 img -> informacion[iImg] = nByte|sacarNbits(mensaje, bitpos, n);
		 bitpos += n;
         iImg++;
     }
}

void leerMensaje(Imagen * img, char msg[], int l, int n) {
	int iImg=0;
	int bitpos = 0;
	int byte = 0;
	unsigned char nByte;

	for(;byte<l;) {
		nByte = img->informacion[iImg];
		if(bitpos+n < 8) {
			nByte <<= (8-n);
			nByte >>= bitpos;
			msg[byte] = msg[byte]|nByte;
			bitpos = bitpos + n;
		}
		else {
			nByte <<= (8-n);
			nByte >>= bitpos;
			msg[byte] = msg[byte]|nByte;
			byte++;
			nByte = img->informacion[iImg];
			nByte <<= (16-(bitpos+n));
			msg[byte] = msg[byte]|nByte;
			bitpos = bitpos + n - 8;
		}
		iImg++;
	}
	msg[l] = 0;
}


unsigned char sacarNbits(char mensaje[],int bitpos,int n) {
    int byte, bitInByte, k, i, tmp;
    unsigned char c, ff = 0xFF;
    byte = bitpos / 8;
    bitInByte = (bitpos - 8 * byte);
    k = 8 - bitInByte;
    if (n > k) {
        tmp = n - k;
        c = mensaje[byte] & (ff >> bitInByte);
        c <<= tmp;
        c |= mensaje[byte + 1] >> (8-tmp);
    } else
        c = mensaje[byte] >> (k - n);
    return c;
}

// Lee un archivo en formato BMP y lo almacena en la estructura img
// NO MODIFICAR
void cargarBMP24 (Imagen * imagen, char * nomArchivoEntrada) {
	// bmpDataOffset almacena la posición inicial de los datos de la imagen. Las otras almacenan el alto y el ancho
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

	fseek (bitmapFile, 10, SEEK_SET); // 10 es la posición del campo "Bitmap Data Offset" del bmp
	fread (&bmpDataOffset, sizeof (int), 1, bitmapFile);

	fseek (bitmapFile, 18, SEEK_SET); // 18 es la posición del campo "height" del bmp
	fread (&bmpWidth, sizeof (int), 1, bitmapFile);
	bmpWidth = bmpWidth*3;

	fseek (bitmapFile, 22, SEEK_SET); // 22 es la posición del campo "width" del bmp
	fread (&bmpHeight, sizeof (int), 1, bitmapFile);
	//bmpHeight = -bmpHeight; // Se multiplica 1 para convertirlo en valor positivo

	residuo = (4 - (bmpWidth) % 4)&3; // Se debe calcular los bits residuales del bmp, que surjen al almacenar en palabras de 32 bits

	imagen -> ancho = bmpWidth;
	imagen -> alto = bmpHeight;
	imagen -> informacion = (unsigned char *) calloc (bmpWidth * bmpHeight, sizeof (unsigned char));

	fseek (bitmapFile, bmpDataOffset, SEEK_SET); // Se ubica el puntero del archivo al comienzo de los datos

	for (y = 0; y < bmpHeight; y++) {
		for ( x= 0; x < bmpWidth; x++) {
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
