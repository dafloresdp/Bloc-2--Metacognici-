#include "miniwin.h"
#include <iostream>

using namespace miniwin;

const int XTAM = 65, YTAM = 40;
const int SZ = 12;

void cuadrado(int x, int y, int c) {
color(c);
rectangulo_lleno(x*SZ, y*SZ, (x+1)*SZ-1, (y+1)*SZ-1);
}
int main () {
vredimensiona(XTAM * SZ, YTAM * SZ);
int x = 30, y = 20 //Cabecita
int vx = 1, vy = 0; //Speed
int retraso = 0;
int t = tecla ();
while (t != ESCAPE) {
        retraso++;
    //Proceso de teclas, lógica i tinte del juego
   if (retraso == 10)
   {
       x += vx;
       y += vy;
       retraso= 0;
   }
   
    borra();
    cuadrado (x,y, VERDE);
    //Pinta todo
    refresca ();
    espera(10);
    t=tecla();
}
vcierra();


return 0;
}
