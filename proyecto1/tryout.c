#include "stdlib.h"
#include "stdio.h"
#include "string.h"
/*
Esta funcion convierte un caracter ASCII
a un string que representa el numero en binario
*/
char *asciiABinario(int numDecimal);
char *darMensajeBinario(unsigned char mensaje[],int n);




//funciones del proyecto
//NO TOCAAAR 



int main(int argc, char const *argv[]) {
  // char stringPrueba[256] = "";
  // char a[] = "B";
  // char b[] = "a";
  //
  // strcat (stringPrueba, a);
  // strcat (stringPrueba, b);
  // printf("%s\n",stringPrueba );

  printf("%s\n",darMensajeBinario("holaaaa",3));

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
