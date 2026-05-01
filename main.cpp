#include <iostream>
#include "Simulador.h"
#include "Excepciones.h"

using namespace std;

int main(int argc, char* argv[]) {
    string archivoEntrada = "datos.txt";
    string carpetaSalida = "./reportes";

    if (argc >= 2) archivoEntrada = argv[1];
    if (argc >= 3) carpetaSalida  = argv[2];

    try {
        Simulador sim(archivoEntrada, carpetaSalida);
        sim.ejecutar();
        cout << "Simulacion completada. Reportes en: " << carpetaSalida << "\n";

    } catch (const ArchivoInvalido& e) {
        cerr << "[Error de archivo]    " << e.what() << "\n";
        return 1;

    } catch (const FormatoInvalido& e) {
        cerr << "[Error de formato]    " << e.what() << "\n";
        return 1;

    } catch (const OperacionContradictoria& e) {
        cerr << "[Error de operacion]  " << e.what() << "\n";
        return 1;

    } catch (const ErrorSistema& e) {
        cerr << "[Error del sistema]   " << e.what() << "\n";
        return 1;
    }

    return 0;
}