#include "miniwin.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace miniwin;
using namespace std;

const int XTAM = 65, YTAM = 40;
const int SZ = 12;
const int LONGITUD_INICIAL = 6; // Longitud inicial de la serpiente
const int CRECIMIENTO = 5; // Número de segmentos que crece la serpiente al comer comida
const int PUNTOS_POR_COMIDA = 12; // Puntos por cada comida
const int PUNTOS_POR_AZULL = 22; // Puntos de la comida que te hace correr mucho
const int DURACION_RAPIDA = 500; // Duración del efecto rápido en ciclos
const int COMIDA_AZUL_FRECUENCIA = 5; // Frecuencia de aparición de la comida azul
const int DURACION_COMIDA_AZUL = 5000; // Duración de la comida azul en milisegundos
const int DURACION_COMIDA_RAPIDA = 7000; // Duración de la comida rápida en milisegundos
const int PUNTOS_POR_COMIDA_BLANCA = -8; // Puntos que quita la comida blanca
const int DURACION_COMIDA_BLANCA = 12000; // Duración de la comida blanca en milisegundos
const int DURACION_EFECTO_BLANCO = 15000; // Duración del efecto de ralentización en milisegundos
const int PUNTOS_POR_SERPIENTE_MUERTA = 160; // Puntos por la comida "serpiente muerta"
const int DURACION_SERPIENTE_MUERTA = 4000; // Duración de la comida "serpiente muerta" en milisegundos
const int FRECUENCIA_SERPIENTE_MUERTA = 10; // Frecuencia de aparición de la comida "serpiente muerta"

struct Punto {
    int x, y;
};

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

Punto al_azar() {
    Punto p = {rand() % XTAM, rand() % YTAM};
    return p;
}

int main() {
    vredimensiona(XTAM * SZ, YTAM * SZ);

    vector<pair<int, int>> serpiente;
    Punto comida = al_azar();
    Punto comida_rapida = { -1, -1 }; // Inicialmente fuera del tablero
    Punto comida_blanca = { -1, -1 }; // Inicialmente fuera del tablero
    Punto serpiente_muerta = { -1, -1 }; // Inicialmente fuera del tablero
    int puntuacion = 0; // Variable para almacenar la puntuación
    int comida_amarilla_contador = 0; // Contador para la comida amarilla
    int x = 30, y = 20; // Cabecita
    for (int i = 0; i < LONGITUD_INICIAL; ++i) {
        serpiente.push_back({x - i, y});
    }

    int vx = 1, vy = 0; // Speed
    int retraso = 8;
    int ciclos_rapidos = 0; // Contador de ciclos rápidos
    int t = tecla();
    clock_t inicio_comida_rapida = 0; // Tiempo de inicio de la comida rápida
    clock_t inicio_comida_blanca = 0; // Tiempo de inicio de la comida blanca
    clock_t inicio_efecto_blanco = 0; // Tiempo de inicio del efecto de ralentización
    clock_t inicio_serpiente_muerta = 0; // Tiempo de inicio de la comida "serpiente muerta"
    bool pausa = false; // Variable para controlar la pausa
    bool tecla_arriba_pulsada = false; // Variable para detectar si la tecla de arriba fue pulsada una vez
    bool continuar_juego = true; // Variable para controlar si el juego debe continuar
    clock_t tiempo_anterior_arriba = 0; // Tiempo de la última pulsación de la tecla de arriba

    while (t != ESCAPE) {
        ciclos_rapidos++;
        if (ciclos_rapidos == DURACION_RAPIDA) {
            ciclos_rapidos = 0;
            retraso = 8; // Restaurar el retraso original si no hay efecto de comida blanca
        }

        // Proceso de teclas, lógica y tinta del juego
        if (!pausa && continuar_juego) { // Solo si el juego no está en pausa y debe continuar
            if (t == ARRIBA) {
                if (!tecla_arriba_pulsada) {
                    tecla_arriba_pulsada = true;
                    tiempo_anterior_arriba = clock(); // Registra el tiempo de la primera pulsación
                } else {
                    // Si la tecla de arriba ya ha sido pulsada una vez
                    // verifica si ha sido pulsada dos veces en un intervalo de 1 segundo
                    if ((clock() - tiempo_anterior_arriba) / CLOCKS_PER_SEC < 0.0000001) {
                        // Si la segunda pulsación ocurre dentro de 1 segundo, pausa el juego
                        pausa = true; // Activa la pausa
                        tecla_arriba_pulsada = false; // Reinicia el estado de la tecla pulsada
                    } else {
                        // Si han pasado más de 1 segundo desde la última pulsación,
                        // registra la nueva pulsación como la primera
                        tecla_arriba_pulsada = true;
                        tiempo_anterior_arriba = clock(); // Registra el tiempo de la nueva pulsación
                    }
                }
                vx = 0; vy = -1;
            } else if (t == ABAJO) {
                vx = 0; vy = 1;
            } else if (t == IZQUIERDA) {
                vx = -1; vy = 0;
            } else if (t == DERECHA) {
                vx = 1; vy = 0;
            }

            if (ciclos_rapidos % retraso == 0) {
                x += vx;
                y += vy;

                // Añadir nueva posición de la cabeza
                serpiente.insert(serpiente.begin(), {x, y});
                // Verificar si la serpiente ha comido la comida normal
                if (serpiente[0].first == comida.x && serpiente[0].second == comida.y) {
                    for (int i = 0; i < CRECIMIENTO; ++i) {
                        serpiente.push_back(serpiente.back()); // Crece la serpiente
                    }
                    puntuacion += PUNTOS_POR_COMIDA; // Incrementa la puntuación
                    comida = al_azar(); // Generar nueva comida
                    comida_amarilla_contador++;

                    // Generar comida rápida cada COMIDA_AZUL_FRECUENCIA comidas normales
                    if (comida_amarilla_contador % COMIDA_AZUL_FRECUENCIA == 0) {
                        comida_rapida = al_azar();
                        inicio_comida_rapida = clock(); // Registrar el tiempo de inicio de la comida rápida
                    }

                    // Generar comida blanca cada 3 comidas amarillas
                    if (comida_amarilla_contador % 3 == 0) {
                        comida_blanca = al_azar();
                        inicio_comida_blanca = clock(); // Registrar el tiempo de inicio de la comida blanca
                    }

                    // Generar comida "serpiente muerta" cada FRECUENCIA_SERPIENTE_MUERTA comidas amarillas
                    if (comida_amarilla_contador % FRECUENCIA_SERPIENTE_MUERTA == 0) {
                        serpiente_muerta = al_azar();
                        inicio_serpiente_muerta = clock(); // Registrar el tiempo de inicio de la comida "serpiente muerta"
                    }
                } else if (serpiente[0].first == comida_rapida.x && serpiente[0].second == comida_rapida.y) {
                    retraso = 5; // Reducir el retraso
                    ciclos_rapidos = 0; // Reiniciar el contador de ciclos rápidos
                    puntuacion += PUNTOS_POR_AZULL;
                    comida_rapida = { -1, -1 }; // Mover comida rápida fuera del tablero
                } else if (serpiente[0].first == comida_blanca.x && serpiente[0].second == comida_blanca.y) {
                    puntuacion += PUNTOS_POR_COMIDA_BLANCA; // Quitar puntos
                    retraso = 15; // Aumentar el retraso
                    inicio_efecto_blanco = clock(); // Registrar el tiempo de inicio del efecto
                    comida_blanca = { -1, -1 }; // Mover comida blanca fuera del tablero
                } else if (serpiente[0].first == serpiente_muerta.x && serpiente[0].second == serpiente_muerta.y) {
                    puntuacion += PUNTOS_POR_SERPIENTE_MUERTA; // Incrementar puntuación
                    serpiente_muerta = { -1, -1 }; // Mover la comida "serpiente muerta" fuera del tablero
                } else {
                    // Eliminar el último segmento para mantener la longitud
                    serpiente.pop_back();
                }
            }

            // Verificación de límites y colisión
            if (x < 0 || x >= XTAM || y < 0 || y >= YTAM || hay_choque(serpiente)) {
                color(ROJO); // Cambia el color del texto a rojo
                int texto_ancho = 170; // Aproximación del ancho del texto
                int texto_alto = 50; // Aproximación del alto del texto
                int centro_x = (XTAM * SZ - texto_ancho) / 2;
                int centro_y = (YTAM * SZ - texto_alto) / 2;
                texto(centro_x, centro_y, "Game Over :c");
                refresca();
                espera(3000); // Espera 3 segundos para que se pueda leer el mensaje y las puntuaciones
                break; // Salir
            }

            // Verificación del tiempo de la comida rápida
            if (comida_rapida.x != -1 && comida_rapida.y != -1 && (clock() - inicio_comida_rapida) > DURACION_COMIDA_RAPIDA) {
                comida_rapida = { -1, -1 }; // Mover comida rápida fuera del tablero
            }

            // Verificación del tiempo de la comida blanca
            if (comida_blanca.x != -1 && comida_blanca.y != -1 && (clock() - inicio_comida_blanca) > DURACION_COMIDA_BLANCA) {
                comida_blanca = { -1, -1 }; // Mover comida blanca fuera del tablero
            }

            // Verificación del tiempo del efecto de la comida blanca
            if ((clock() - inicio_efecto_blanco) > DURACION_EFECTO_BLANCO) {
                retraso = 8; // Restaurar el retraso original
            }

            // Verificación del tiempo de la comida "serpiente muerta"
            if (serpiente_muerta.x != -1 && serpiente_muerta.y != -1 && (clock() - inicio_serpiente_muerta) > DURACION_SERPIENTE_MUERTA) {
                serpiente_muerta = { -1, -1 }; // Mover comida "serpiente muerta" fuera del tablero
            }
        }

        // Verificar si el juego está en pausa y esperar a que se desactive la pausa
        while (pausa) {
            t = tecla(); // Obtener la próxima tecla
            if (t == ARRIBA) {
                if (!tecla_arriba_pulsada) {
                    tecla_arriba_pulsada = true;
                } else {
                    pausa = false; // Desactiva la pausa
                    tecla_arriba_pulsada = false; // Reinicia el estado de la tecla pulsada
                }
            }
            espera(10); // Espera para evitar un uso intensivo del CPU
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

        // Dibujar la comida normal
        cuadrado(comida.x, comida.y, AMARILLO);
        // Dibujar la comida rápida si está en el tablero
        if (comida_rapida.x != -1 && comida_rapida.y != -1) {
            cuadrado(comida_rapida.x, comida_rapida.y, AZUL);
        }
        // Dibujar la comida blanca si está en el tablero
        if (comida_blanca.x != -1 && comida_blanca.y != -1) {
            cuadrado(comida_blanca.x, comida_blanca.y, BLANCO);
        }
        // Dibujar la comida "serpiente muerta" si está en el tablero
        if (serpiente_muerta.x != -1 && serpiente_muerta.y != -1) {
            cuadrado(serpiente_muerta.x, serpiente_muerta.y, ROJO);
        }

        // Dibujar la puntuación
        color(BLANCO);
        texto(10, 10, "Puntuación: " + to_string(puntuacion));

        // Pinta todo
        refresca();
        espera(10);
        t = tecla();
    }
    vcierra();

    return 0;
}
