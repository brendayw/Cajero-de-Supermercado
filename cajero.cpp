#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

//Archivos
#define ArchivoProductos "productos.csv"
#define Ventas "ventas.csv"

//FUNCIONES
bool fechavalida(int dia, int mes, int anio);
void agregarproducto(string codigo, string descripcion, int precio);
bool leerproductos(string codigoBarras, string solicitado);
void editarprecio(string codigo, double nuevoprecio);
double iniciarcompra(int dia, int mes, int anio);
double descuento(double total);
void abonartarjeta(double abonarDinero);
void guardarventa(double totalcondescuento, int dia, int mes, int anio);
void retirarefectivo(double retirarDinero);
double cerrarCaja(double total, int dia, int mes, int anio);

int main() {
    //variables
    string rta, codigoingresado, codigoBarrasArchivo, descrip;
    double totalcondescuento, precioProd, nuevoprecio, dinero;
    int opcion, dia, mes, anio;

    //menu
    cout << "\t Bienvenido al cajero de PROYECTO FINAL" << endl;
    cout << "\n Ingrese la fecha (DD/MM/AAAA): ";
    cin >> dia >> mes >> anio;

    if (fechavalida(dia, mes, anio)) {
        cout << "Se ha iniciado la caja del dia: " << dia << "/" << mes << "/" << anio << endl;

        do {
            cout <<"\t MENÚ PRINCIPAL" << endl;
            cout << "1. Ingresar producto nuevo" << endl; //terminada
            cout << "2. Leer productos" << endl; //terminada
            cout << "3. Editar precio de producto" << endl; //terminado
            cout << "4. Iniciar compra" << endl; //terminada
            cout << "5. Retirar efectivo" << endl; //terminada
            cout << "6. Salir" << endl; //terminada
            cout << "Ingrese su opcion: ";
            cin >> opcion;

            switch (opcion) {
                case 1:
                    agregarproducto(codigoingresado, descrip, precioProd); //terminada
                    break;
                case 2:
                    leerproductos(codigoBarrasArchivo, codigoingresado); //terminada
                    break;
                case 3:
                    editarprecio(codigoingresado, nuevoprecio); //ver si funciona bien
                    break;
                case 4:
                    totalcondescuento = iniciarcompra(dia, mes, anio); //terminado
                    abonartarjeta(totalcondescuento); //terminado
                    guardarventa(totalcondescuento, dia, mes, anio); 
                    break;
                case 5:
                    cout << "Ingrese la cantidad de dinero a retirar: ";
                    cin >> dinero;
                    retirarefectivo(dinero);
                    break;
                case 6:
                    cout << "Esta seguro que desea salir? (Si/No)" << endl;
                    cin >> rta;
                    if (rta == "Si" || rta == "si") {
                        cerrarCaja(totalcondescuento, dia, mes, anio);
                        cout << "Gracias por usar el cajero. Adios!" << endl;
                        exit(0);
                    } else {
                        cout << "No se ha cerrado la caja" << endl;
                    }
                    break;  
                default:
                    cout << "Opcion no valida" << endl;
            }

        } while (opcion != 6 || rta == "NO" || rta == "no" || rta == "No");

    } else {
        cout << "La fecha ingresada no es valida" << endl;
    }

    return 0;
}

//FUNCIONES
/*---------------Valida fecha---------------*/
bool fechavalida(int dia, int mes, int anio) {
    if (mes >= 1 && mes <= 12) {
        if ((dia <=31) && (mes==1 || mes==3 || mes==5 || mes==7 || mes==8 || mes==10|| mes==12)) {
            return true;
        } else if ((dia <=30) && (mes==4 || mes==6 || mes==9 || mes==11)) {
            return true;
        } else if (mes == 2) {
            if ((anio % 4 == 0 && anio % 100 !=0) || (anio % 400 == 0 && dia <=29) || (dia <=28)) {
                return true;
            }
        }
    }
    return false;
}
/*---------------FIN valida fecha---------------*/

/*---------------Agrega Productos---------------*/
void agregarproducto(string codigo, string descripcion, int precio) {
    ofstream archivo(ArchivoProductos, ios::app);
   
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo" <<endl;
        return;
    }

    bool datosValidos = false;

    do {
        cout << "Ingrese el codigo de barras del producto: ";
        cin >> codigo;
        cin.ignore();
        cout << "Ingrese la descripcion del producto: ";
        getline(cin, descripcion);
        cout << "Ingrese el precio del producto: ";
        cin >> precio;
        cin.ignore();

        if (!codigo.empty() && !descripcion.empty() && precio > 0) {
            datosValidos = true;
        } else {
            cout << "Producto NO VALIDO. Ingrese los datos nuevamente" << endl;
        }
    } while (!datosValidos);


    archivo << codigo << ";" << descripcion << ";" << precio << endl;
    archivo.close();

    cout << "Producto agregado correctamente" << endl;
    
}
/*---------------FIN de agrega productos---------------*/

/*---------------Lee articulos---------------*/
bool leerproductos(string codigoBarrasArchivo, string solicitado) {
    ifstream archivo(ArchivoProductos);
    string linea;
    char delimitador = ';';

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo" <<endl;
    }

    cout << "Ingrese el codigo de barras del producto: ";
    cin >> solicitado;
    cin.ignore();

    getline(archivo, linea); //descarga la primera linea del archivo

    while (getline(archivo, linea)) {
        stringstream stream(linea);
        string codigoBarrasArchivo, desc,inicial, actual;
        double precio;

        getline(stream, codigoBarrasArchivo, delimitador);
        getline(stream, desc, delimitador);
        stream >> precio;
        getline(stream, inicial, delimitador);
        getline(stream, actual, delimitador);

        if (codigoBarrasArchivo == solicitado) {
            cout << "El producto es: "<<endl;
            cout << "Descripción: " << desc << endl;
            cout << "Precio: " << precio << endl;
            cout << "Codigo de barras: " << codigoBarrasArchivo << endl;
            archivo.close();
            return true; //sale de la funcion
        }

    }

    cout << "El producto no existe en el sistema." << endl;
    archivo.close();

    return false;
}
/*---------------FIN lee articulos---------------*/

/*---------------Edita precios---------------*/
void editarprecio(string codigo, double nuevoprecio) {
    ifstream archivoOriginal(ArchivoProductos);
    ofstream archivoTemporal("temp.csv");
    string linea;
    char delimitador = ';';

    if (!archivoOriginal.is_open() || !archivoTemporal.is_open()) {
        cout << "No se pudo abrir el archivo" <<endl;
        exit(1);
    }

    cout << "Ingrese el codigo de barras: ";
    cin >> codigo;
    cin.ignore();
    cout << "Ingrese el nuevo precio: ";
    cin >> nuevoprecio;
    cin.ignore();

    bool encontrado = false;
    string codigoBarrasArchivo, descripcion, precio, inicial, actual;

    while (getline(archivoOriginal, linea)) {
        stringstream stream(linea);

        getline(stream, codigoBarrasArchivo, delimitador);
        getline(stream, descripcion, delimitador);
        getline(stream, precio, delimitador);
        getline(stream, inicial, delimitador);
        getline(stream, actual, delimitador);

        if (codigoBarrasArchivo == codigo) {
            precio = to_string(nuevoprecio);
            encontrado = true;
        }

        archivoTemporal << codigoBarrasArchivo << ";" << descripcion << ";" << precio << ";" << inicial << ";" << actual << endl;
    }

    archivoOriginal.close();
    archivoTemporal.close();

    if (!encontrado) {
        cout << "Producto no encontrado" << endl;
        remove("temp.csv");
        return;
    }

    remove(ArchivoProductos);
    rename("temp.csv", ArchivoProductos);

    cout << "El precio fue actualizado correctamente." << endl;

}
/*---------------FIN Edita precio---------------*/

/*---------------Inicia compra---------------*/
double iniciarcompra(int dia, int mes, int anio) {
    ifstream archivo(ArchivoProductos);
    string linea, codigo;
    char delimitador = ';';
    double total = 0;
    bool encontrado = false;

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo" <<endl;
        exit(1);
    }

    do {
        cout << "Ingrese el codigo de barras: ";
        cin >> codigo;

        while (getline(archivo, linea)) {
            stringstream stream(linea);
            string codigoBarrasArchivo, descripcion, inicial, actual;
            int precio;

            getline(stream, codigoBarrasArchivo, delimitador);
            getline(stream, descripcion, delimitador);
            stream >> precio;
            getline(stream, inicial, delimitador);
            getline(stream, actual, delimitador);

            if (codigoBarrasArchivo == codigo) {
                total += precio;
                encontrado = true;
                break;
            }
        }

        if (!encontrado) {
            cout << "Producto no encontrado" << endl;
        }

        archivo.clear();
        archivo.seekg(0, ios::beg);

    } while (codigo != "0");

    archivo.close();
    cout << "El total a pagar es: $" << total << endl;

    total = descuento(total);
    return total;  
}
/*---------------FIN inicia compra---------------*/

/*---------------Descuento---------------*/
double descuento(double total) {
    const double porcentaje1 = 0.3;
    const double porcentaje2 = 0.2;
    const double porcentaje3 = 0.1;

    double descuento = total;

    if (total > 50000) {
        descuento = total - (total * porcentaje1);
        cout << "Descuento del 30% sobre la compra final" << endl;

    } else if (total <= 50000 && total > 30000) {
        descuento = total - (total * porcentaje2);
        cout << "Descuento del 20% sobre la compra final" << endl;

    } else if (total > 10000 && total <= 30000) {
        descuento = total - (total * porcentaje3);
        cout << "Descuento del 10% sobre la compra final" << endl;

    } else {
        cout << "No corresponde ningun descuento" << endl;
    }

    cout << "Descuento: $" << total - descuento << endl;
    cout << "El total a pagar con descuento es: $" << descuento << endl;
    
    return descuento;
}
/*---------------Fin descuento---------------*/

/*---------------Abonar tarjeta---------------*/
void abonartarjeta(double abonarDinero) {
    string rta, tarjeta;
    double saldoTarjeta = 250000;
   
    cout << "¿Desea abonar en efectivo o  tarjeta?" << endl;
    cin>> rta;

    if (rta == "TARJETA" || rta == "Tarjeta" || rta == "tarjeta") {
        string tipo;

        cout << "¿Débito o crédito?" << endl;
        cin >> rta;

        if (rta == "DEBITO" || rta == "Debito" || rta == "debito") {
            cout << "Ingrese su tarjeta debito: ";
            cin >> tarjeta;

            if (tarjeta.length() == 10) {
                char primerDigito = tarjeta[0];

                switch (primerDigito) {
                    case '3':
                        tipo = "American Express";
                    break;
                    case '4':
                        tipo = "Visa";
                    break;
                    case '5':
                        tipo = "MasterCard";
                    break;
                    default:
                        tipo = "No corresponde a ningun tipo de tarjeta";
                }

                cout << "Tipo de tarjeta: " << tipo << endl;
                cout << "Aguarde un momento..." << endl;
               
                if (saldoTarjeta >= abonarDinero) {
                    cout << "Se ha abonado el dinero correctamente" << endl;
                } else {
                    cout << "Tarjeta de Debito DENEGADA" << endl;
                    cout << "SALDO DISPONIBLE: $" << saldoTarjeta << endl;
                }

            } else {
                cout << "Tarjeta no valida" << endl;
            }

        } else {
            cout << "Ingrese su tarjeta de credito: ";
            cin >> tarjeta;

            if (tarjeta.length() == 10) {
                int cuota;

                cout << "¿En cuantas cuotas desea pagar?: " << endl;
                cout << "* 1 cuota (Cuota Simple)" << endl;
                cout << "* 3 cuotas sin interes" << endl;
                cout << "* 6 cuotas" << endl;
                cin >> cuota;

                if (cuota == 1 || cuota == 3 || cuota == 6) {
                    char primerDigito = tarjeta[0];

                    switch (primerDigito) {
                        case '3':
                            tipo = "American Express";
                        break;
                        case '4':
                            tipo = "Visa";
                        break;
                        case '5':
                            tipo = "MasterCard";
                        break;
                        default:
                            tipo = "No corresponde a ningun tipo de tarjeta";
                    }

                    cout << "Tipo de tarjeta: " << tipo << endl;
                    cout << "Aguarde un momento..." << endl;
                    cout << "..."<<endl;

                    if (saldoTarjeta >= abonarDinero / cuota) {
                        cout << "Medio de pago ACEPTADO" << endl;
                    } else {
                        cout << "Tarjeta de Credito DENEGADA" << endl;
                        cout << "SALDO DISPONIBLE INSUFICIENTE" << endl;
                    }

                } else
                {
                    cout << "Tarjeta no valida" << endl;
                }

            } else {
                cout << "Error al ingresar la cantidad de cuotas" << endl;
            }
        }
    }
}
/*---------------FIN abonar tarjeta---------------*/

/*---------------Guardar venta---------------*/
void guardarventa(double totalcondescuento, int dia, int mes, int anio) {
    ofstream archivo(Ventas, ios::app);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        exit(1);
    }

    archivo << totalcondescuento << ";" << dia << "/" << mes << "/" << anio << endl;

    archivo.close();
}
/*---------------FIN guardar venta---------------*/

/*---------------Retirar efectivo---------------*/
void retirarefectivo(double retirarDinero) {
    double saldoTarjeta = 250000; //saldo designado
    string tarjeta;

    cout << "Ingrese su tarjeta de debitp para retirar dinero: ";
    cin >> tarjeta;

    if (tarjeta == "") {
        cout << "No ha ingresado la tarjeda" << endl;

    } else if (tarjeta.size() < 10) {
        cout << "Tarjeta no valida" << endl;

    } else {

        if (saldoTarjeta >= retirarDinero) {
            cout << "Aguarde un momento..." << endl;
            cout << "..."<<endl;
            cout << "..."<<endl;
            cout << "Preparando dinero..." << endl;
            cout << "Se ha retirado el dinero correctamente" << endl;
            saldoTarjeta -= retirarDinero;
        } else {
            cout << "No tiene suficiente saldo" << endl;
            cout << "SALDO DISPONIBLE: $" << saldoTarjeta << endl;
        }

        cout << "Su saldo es de: $" << saldoTarjeta - retirarDinero << endl;
    }
}
/*---------------FIN retirar efectivo---------------*/

/*---------------Cerrar caja---------------*/
double cerrarCaja(double total, int dia, int mes, int anio) {
    ifstream archivo(Ventas);
    string linea, fecha;
    double totalcondescuento = 0;
    char delimitador = ';';

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de ventas." << endl;
        exit(1);
    }

    getline(archivo, linea);

    while (getline(archivo, linea)) {
        stringstream stream(linea);
        double ventatotal;

        stream >> ventatotal;
        stream.ignore(); //ignora al delimitador ";"
        getline(stream, fecha, delimitador); 

        stringstream fechaStream(fecha);
        int d, m, a;
        char delim = '/';
        fechaStream >> d >> delim >> m >> delim >> a;

        if (d == dia && m == mes && a == anio) {
            totalcondescuento += ventatotal;
        }
    }
    archivo.close();

    cout << "Has cerrado la caja del día." << endl;
    cout << "El total recaudado hoy es: $" << totalcondescuento << endl;
    return totalcondescuento;
}
/*---------------FIN cerrar caja---------------*/