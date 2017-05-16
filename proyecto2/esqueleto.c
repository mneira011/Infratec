// isis1304-111-proyecto2.cpp: define el punto de entrada de la aplicaci—n de consola.
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

int test1;

// Funci—n que carga el bmp en la estructura Imagen
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
        printf ("Faltan argumentos\n");
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

/**
 * Identifica la opci—n ingresada y realiza las acciones que debe hacer el programa.
 * Ver el enunciado.
 */
// ESCRIBIR EN ENSAMBLADOR, SE PUEDEN USAR NOMBRES SIMBOLICOS
void decidir(char nomArch1[], char op, int bitsPorByte, char msg[], Imagen * img) {
	//SE PUEDEN USAR NOMBRES SIBOLICOS 
	printf("entro a decidir\n");
	
    __asm {
		mov al,bir
		cmp al,  op
		cmp al, 'w'
		je escri'r'
		je leer
		jmp fin
escribir:
		mov eax, bitsPorByte
		push eax
		
		mov eax, msg
		push eax
		
		mov eax, img
		push eax
				
		call insertarMensaje
		
		pop eax
		pop eax
		pop eax


		jmp fin
leer:

		

		mov eax, bitsPorByte
		push eax
		
		mov ecx, 0 
		mov eax, msg
miWhile:
		
		mov ebx, 0
		mov bl, [eax]
		cmp bl, 0
		je miFin
		sub bl, '0'
		imul ecx, ecx, 10
		add ecx, ebx
		inc eax
		jmp miWhile
miFin:
		push ecx

		mov eax, msg
		push eax
		
		mov eax, img
		push eax

		call leerMensaje

		pop eax 
		pop ecx
		pop eax
		pop eax

		jmp fin
fin:

    }
	
	printf("eax vale: %d\n",test1);
}

/**
 * Inserta un mensaje, de a n bits por componente de color, en la imagen apuntada por img
 * parametro img Apuntador a una imagen en cuyos pixeles se almacenará el mensaje.
 * parametro mensaje Apuntador a una cadena de caracteres con el mensaje.
 * parametro n Cantidad de bits del mensaje que se almacenarán en cada componente de color de cada pixel. 0 < n <= 8.
 */
// ESCRIBIR EN ENSAMBLADOR, *NO* SE PUEDEN USAR NOMBRES SIMBOLICOS
void insertarMensaje(Imagen * img , char mensaje[], int n) {
	//SIN USAR NOMBRES SIMBOLICOS 
	printf("entro a instertar mensaje\n");
    __asm {

        
    }
}

/**
 * Extrae un mensaje de tama–o l, guardado de a n bits por componente de color, de la imagen apuntada por img
 * parametro img Apuntador a una imagen que tiene almacenado el mensaje en sus pixeles.
 * parametro msg Apuntador a una cadena de caracteres donde se depositará el mensaje.
 * parametro l Tama–o en bytes del mensaje almacenado en la imagen.
 * parametro n Cantidad de bits del mensaje que se almacenan en cada componente de color de cada pixel. 0 < n <= 8.
 */
// ESCRIBIR EN ENSAMBLADOR, SE PUEDEN USAR NOMBRES SIMBOLICOS
void leerMensaje(Imagen * img, char msg[], int l, int n) {
	//SE PUEDEN USAR NOMBRES SIBOLICOS 
	printf("entro a leer mensaje\n");
    __asm {
        
    }
}

/**
 * Extrae n bits a partir del bit que se encuentra en la posici—n bitpos en la secuencia de bytes que
 * se pasan como parámetro
 * parametro secuencia Apuntador a una secuencia de bytes.
 * parametro n Cantidad de bits que se desea extraer. 0 < n <= 8.
 * parametro bitpos Posición del bit desde donde se extraerán los bits. 0 <= n < 8*longitud de la secuencia
 * retorno Los n bits solicitados almacenados en los bits menos significativos de un unsigned char
 */
// PROCEDIMIENTO OPCIONAL
unsigned char sacarNbits(char mensaje[],int bitpos,int n) {
    // TODO
	//SE PUEDEN USAR NOMBRES SIBOLICOS 
    __asm {
        
    }
}

// Lee un archivo en formato BMP y lo almacena en la estructura img
// NO MODIFICAR
void cargarBMP24 (Imagen * imagen, char * nomArchivoEntrada) {
    // bmpDataOffset almacena la posici—n inicial de los datos de la imagen. Las otras almacenan el alto y el ancho
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
    
    fseek (bitmapFile, 10, SEEK_SET); // 10 es la posici—n del campo "Bitmap Data Offset" del bmp
    fread (&bmpDataOffset, sizeof (int), 1, bitmapFile);
    
    fseek (bitmapFile, 18, SEEK_SET); // 18 es la posici—n del campo "height" del bmp
    fread (&bmpWidth, sizeof (int), 1, bitmapFile);
    bmpWidth = bmpWidth*3;
    
    fseek (bitmapFile, 22, SEEK_SET); // 22 es la posici—n del campo "width" del bmp
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
