#include "miniwin.h"
#include <iostream>

using namespace miniwin;

const int XTAM = 65, YTAM = 40;
const int SZ = 12;

void cuadrado(int x, int y, int c) {
    color(c);
    rectangulo_lleno(x * SZ, y * SZ, (x + 1) * SZ - 1, (y + 1) * SZ - 1);
}

int main() {
    vredimensiona(XTAM * SZ, YTAM * SZ);
    int x = 30, y = 20; // Cabecita
    int vx = 1, vy = 0; // Speed
    int retraso = 0;
    int t = tecla();
    while (t != ESCAPE) {
        retraso++;
        // Proceso de teclas, lógica y tinta del juego
        if (t == ARRIBA) {
            vx = 0; vy = -1;
        } else if (t == ABAJO) {
            vx = 0; vy = 1;
        } else if (t == IZQUIERDA) {
            vx = -1; vy = 0;
        } else if (t == DERECHA) {
            vx = 1; vy = 0;
        }

        if (retraso == 8) {
            x += vx;
            y += vy;
            retraso = 0;
        }

        // Verificación de límites
        if (x < 0 || x >= XTAM || y < 0 || y >= YTAM) {
            color(ROJO); //Color rojito
            texto(10, 10, "Game Over :c");
            refresca();
            espera(2000); // Esperamos  2 segs para que se lea el mensaje
            break; // Salir
        }

        borra();
        cuadrado(x, y, VERDE);
        // Pinta todo
        refresca();
        espera(10);
        t = tecla();
    }
    vcierra();

    return 0;
}
