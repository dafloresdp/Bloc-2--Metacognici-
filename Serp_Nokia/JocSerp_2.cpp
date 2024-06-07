#include "miniwin.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace miniwin;
using namespace std;

const int XTAM = 65, YTAM = 40;
const int SZ = 12;
const int LONGITUD_INICIAL = 14; // Longitud inicial de la serpiente

void cuadrado(int x, int y, int c) {
    color(c);
    rectangulo_lleno(x * SZ, y * SZ, (x + 1) * SZ - 1, (y + 1) * SZ - 1);
}

bool hay_choque(const vector<pair<int, int>>& serpiente) {
    // Ordenar el vector de segmentos de la serpiente
    vector<pair<int, int>> serpiente_ordenada = serpiente;
    sort(serpiente_ordenada.begin() + 1, serpiente_ordenada.end()); // +1 para no incluir la cabeza

    // Búsqueda binaria para verificar si la cabeza choca con el cuerpo
    pair<int, int> cabeza = serpiente[0];
    return binary_search(serpiente_ordenada.begin() + 1, serpiente_ordenada.end(), cabeza);
}

int main() {
    vredimensiona(XTAM * SZ, YTAM * SZ);
    vector<pair<int, int>> serpiente;
    int x = 30, y = 20; // Cabecita
    for (int i = 0; i < LONGITUD_INICIAL; ++i) {
        serpiente.push_back({x - i, y});
    }

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

            // Añadir nueva posición de la cabeza
            serpiente.insert(serpiente.begin(), {x, y});
            // Eliminar el último segmento para mantener la longitud
            if (serpiente.size() > LONGITUD_INICIAL) {
                serpiente.pop_back();
            }
        }

        // Verificación de límites y colisión
        if (x < 0 || x >= XTAM || y < 0 || y >= YTAM || hay_choque(serpiente)) {
            color(ROJO); // Cambia el color del texto a rojo
            texto(10, 10, "Game Over :c");
            refresca();
            espera(2000); // Espera 2 segundos para que se pueda leer el mensaje
            break; // Salir
        }

        borra();
        // Dibujar la serpiente
        bool esCabeza = true;
        for (auto segmento : serpiente) {
            if (esCabeza) {
                cuadrado(segmento.first, segmento.second, ROJO); // La cabeza es roja
                esCabeza = false;
            } else {
                cuadrado(segmento.first, segmento.second, VERDE); // El cuerpo es verde
            }
        }
        // Pinta todo
        refresca();
        espera(10);
        t = tecla();
    }
    vcierra();

    return 0;
}
