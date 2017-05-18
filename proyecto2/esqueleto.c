// isis1304-111-proyecto2.cpp: define el punto de entrada de la aplicaci—n de consola.
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

int test1;

//PARA LEER IMAGEN
char *move;
char info;
int pos;
//PARA SACARNBITS
int posByte;
int  numB;
int rest;
char byte1;
char rta;
//PARA INSERTAR MENSAJE
int iImg;
int bitpos;
char nByte;
char m;
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
		mov al,op
		cmp al,  'w'
		je escribir
		cmp al, 'r'
		je leer
		jmp fin
escribir:
		mov eax, bitsPorByte
		push eax
		mov eax, msg
		push eax
		mov eax, img
		push eax
		push ebp
		mov ebp, esp
		mov iImg, 0
		mov eax, iImg
		push eax
		mov bitpos, 0
		mov eax, bitpos
		push eax
		mov eax, 0
		mov al, nByte
		push eax
		mov eax, 0
		mov al,m
		push eax
		call insertarMensaje
		pop eax
		pop eax
		pop eax
		pop eax
		pop eax
		pop eax
		jmp fin
leer:

		
		mov eax,0
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
		pop eax
		pop ecx
		pop eax
fin:

    }
	
	printf("coso%d\n",test1);
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
	printf("entro a insertar mensaje\n");
    __asm {
		//basandonos en la solucion propuesta por el monitor:
		mov eax, 0
		mov al, 255
		mov ecx, [ebp+12]
		shl al, cl
		mov [ebp-16], eax
        miWhile1:
		mov eax, [ebp-8]
		cdq
		mov edx, 8
		idiv edx
		mov ebx, [ebp+8]
		cmp [ebx+eax], 0
		je finMiWhile
		mov eax, [ebp+4]
		mov ebx, [eax+8]
		mov edx, [ebp-4]
		mov cl, BYTE ptr[ebx+edx]
		mov eax, [ebp-16]
		and al, cl
		mov ebx, [ebp-12]
		mov bl, al
		mov [ebp-12], ebx
		or bl, //aqui falta el resultado de sacarNbits(mensaje, bitpos, n)
		mov cl, bl
		mov eax, [ebp+4]
		mov ebx, [eax+8]
		mov edx, [ebp-4]
		mov BYTE ptr[ebx+edx], cl
		mov eax, [ebp-8]
		mov ecx, [ebp+12]
		add eax, ecx
		mov [ebp-8], eax
		mov edx, [ebp-4]
		inc edx
		mov[ebp-4], edx
		jmp miWhile1
		finMiWhile:       
    }
	printf("ebx:%d\n", test1);
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
	printf("entro a leerMensaje...\n");
	printf("n:%d\n", n);
	printf("l:%d\n", l);
	__asm {
		mov esi, msg
		mov eax, img;
		mov eax, [eax + 8];
		mov move, eax;
		mov eax, 8;                                eax = 8;
		sub eax, n;                                eax = al = 8 - n;
		mov pos, 0;
		mov ebx, l
		imul ebx, 8
		mov l, ebx
		InicioWhile :
			mov ebx, pos
			cmp l, ebx
			jl FinWhile
			mov ebx, move
			mov bl, [ebx]
			mov info, bl
			mov cl, al
			shl info, cl
			mov dl, info;                                   dl = info;
			cmp pos, 0
			je sigamos
			mov ebx, pos
			shl ebx, 29;                                    pos % 8;
			shr ebx, 29
			jmp continued
			sigamos:
			mov ebx,0
			continued:
			mov cl, bl
			shr dl, cl
			mov edi, pos
			shr edi, 3;										pos / 8
			or BYTE ptr[esi + edi], dl
			cmp eax, ebx;									eax = 8 - n ebx = pos % 8;
			jge finIf
			mov edx, 8
			sub edx, ebx
			mov ch, info;							    	ebx = info;
			mov cl, dl
			shl ch, cl
			or BYTE ptr[esi + edi + 1], ch
		finIf :
			inc move
			mov edx, pos
			add edx, n
			mov pos, edx
			jmp InicioWhile

		FinWhile :
	}
	printf("El mensaje es:\n");
	printf("%s\n", msg);
	printf("Fin de leer mensaje...\n");
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
		//ESTA PARTE LA HIZO AMILKAR, FALTA PROBAR 
		push ebx
		push ecx
		push edx
		push esi

		mov esi, mensaje
		mov eax, 8
		sub eax, n
		mov rest, eax
		mov eax, bitpos
		mov ebx, eax
		shl eax, 29
		shr eax, 29
		mov numB, eax
		shr ebx, 3
		mov al, [esi + ebx]
		mov ebx, numB
		mov cl, bl
		shl al, cl
		mov byte1, al
		mov eax, rest
		mov ebx, numB
		cmp eax, ebx
		jge fin
		mov ecx, bitpos
		shr ecx, 3
		mov ch, [esi + ecx + 1]
		mov edx, 8
		sub edx, numB
		mov cl, dl
		shr ch, cl
		or byte1 , ch
		fin :
		mov eax, 0
		mov al, byte1
		mov ebx, rest
		mov cl, bl
		shr al, cl

		pop esi
		pop edx
		pop ecx
		pop ebx
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
