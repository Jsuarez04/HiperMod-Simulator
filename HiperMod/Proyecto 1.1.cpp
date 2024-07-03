#include <iostream>
#include <fstream>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <queue>
#include <stack>
#include <ctime>
#include <cstdlib>
#include <cstring>
using namespace std;

//	ESTRUCTURA DE LOS PRODUCTOS
struct Productos
{
    int id;
    char descripcion[100];
    double precio;
    int stock;
    int ventas = 0;
}productos[10];

//	ESTRUCTURA DE CLIENTES
struct Clientes
{
    char nombre[40];
    int cv;
    float dinero_disponible;
    double PagoT = 0;
    stack<string> Carrito;
    int tiempo_de_compra = 0;
    int tiempo_de_llegada = 0;
}clientes[11];


// DECLARACION DE LA COLA DE CLIENTES
queue<Clientes> colaClientes;

// DEFINICION GLOBAL DE VARIABLES
int clientes_presentes= 0;
int compradores =0;								//Clientes que excedieron el tiempo y no les alcanso el dinero
int tiempo_excedido = 0;										//Agrega a los clientes que excedieron el tiempo
int espaciado;
float totalVenta =0;
//	PROTOTIPOS DE FUNCIONES
void cambiarColor(int colorFondo, int colorTexto);				//Cambia el color de los textos
void mostrar_nombre_supermercado();								//Muestra el Nombre del Supermercado
void encolar(const Clientes& cliente);							//Agrega a los clientes en un cola
void BaseDeDatos_Clientes();									//Muestra la base de datos de todoslos compradores cargados
void BaseDeDatos_Productos();									//CARGA TODOS LOS PRODUCTOS AL SISTEMA
void Creditos();												//Creditos de todos los participantes
void menu(int& eleccion);										//Menu del usuario
void iniciarSimulacion();										//Inicializar la simulacion de compra
void GenerarTiempoDeCompra();									//Generar Tiempo de compra Aleatorio
void esperando_compra(int tiempo);								//Funcion para pausar el programa mientras se realiza  una compra
void Otros();
void mostrarClientes();

//		FUNCION PRINCIPAL
int main()
{
	int eleccion;
	
	//	ENCABEZADO
    cambiarColor(1, 9);
    mostrar_nombre_supermercado();
	cambiarColor(1,15);
	// SELECCION DEL MENU
    do
    {
        menu(eleccion);
        switch (eleccion)
		{
            case 1: iniciarSimulacion(); 	break;	//OPCION 1 LLAMA A LA FUNCION INICIARSIMULACION
            case 2:	Creditos();				break;	//OPCION 2 MUESTRA LOS PARTICIPANTES DEL PROYECTO
            case 3: Otros();				break;	//MUESTRA OPCION DE BASES DE DATOS
            default: cout << "\nOpcion invalida";
        }
        system("cls");
    }while(eleccion != 4);
    
	for (int j = 0; j < 35; j++) //Centrando el texto
		{
            cout << " ";
        }
    cout << "Saliendo del programa, muchas gracias por visitarnos";
    getch();
    return 0;
}


//			TITULO HIPERMOD
void mostrar_nombre_supermercado()
{
    char nombre[8][70] =
	{
        {"   _   _  _                    ___  ___            _ "},
        {"  | | | |(_)                   |  \\/  |           | |"},
        {"  | |_| | _  _ __    ___  _ __ | .  . |  ___    __| |"},
        {"  |  _  || || '_ \\  / _ \\| '__|| |\\/| | / _ \\  / _` |"},
        {"  | | | || || |_) ||  __/| |   | |  | || (_) || (_| |"},
        {"  \\_| |_/|_|| .__/  \\___||_|   \\_|  |_/ \\___/  \\\\___|"},
        {"            | |                                      "},
        {"            |_|                                      "},
    };
	
	// CAMBIO DE COLOR DEL LOGO
    cambiarColor(0, 13);
    
    
    // CONSEGUIR TAMAÑO DE LA CONSOLA
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    int consoleWidth = consoleInfo.dwSize.X;

    // CALCULO DEL TAMAÑO NECESARIO PARA EL LOGO
    int espaciosEnBlanco = (consoleWidth - 70) / 2;
	int espaciado = espaciosEnBlanco;
    for (int i = 0; i < 8; i++)
	{
        // Imprimir espacios en blanco para centrar el texto
        for (int j = 0; j < espaciosEnBlanco; j++)
		{
            cout << " ";
        }

        for (int j = 0; j < 70; j++)
		{
            cout << nombre[i][j];
        }
        cout << "\n";
    }
    
    //	RETRASO EN EL SISTEMA PARA APRECIAR EL LOGO :D
    Sleep(1500);
    system("cls");
}

/*		FUNCION PARA CAMBIAR EL FONDO DEL TEXTO			*/
void cambiarColor(int colorFondo, int colorTexto)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorFinal = colorTexto + colorFondo * 16;
    SetConsoleTextAttribute(hConsole, colorFinal);
}


/*		FUNCION PARA GENERAL TIEMPO DE COMPRA ALEATORIO	*/
void GenerarTiempoDeCompra()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < 11; i++)
	{
        clientes[i].tiempo_de_compra = rand() % 15 + 1;
        if(clientes[i].tiempo_de_compra > 10)
    	{
    		tiempo_excedido++;
		}
		fflush(stdin);
    }
}

//				FUNCION PARA ENCOLAR A LOS CLIENTES
void encolar(const Clientes& cliente)
{
    colaClientes.push(cliente);
}


/*		FUNCION PARA LA INIALIZACION DEL PROGRAMA DE COMPRAS			*/
void iniciarSimulacion()
{	

	//	PARA QUE EL USUARIO INGRESE AL SISTEMA
	fflush(stdin);
	cout << "\nNombre y apellido: ";	cin.getline(clientes[0].nombre,40,'\n');	//	SU NOMBRE
	cout << "\nCedula de identidad: V-";		cin>>clientes[0].cv;						//	SU CEDULA DE IDENTIDAD
	cout << "\nSaldo disponible: $"; cin>>clientes[0].dinero_disponible;	//	SU DINERO DISPONIBLe
	
	// LLAMO A LA FUNCION PARA AGREGAR A TODOS LOS PIBES
    BaseDeDatos_Clientes();
	
	//	LLAMO A LA FUNCION PARA AGREGAR LOS PRODUCTOS
	BaseDeDatos_Productos();
	
    while (!colaClientes.empty())
	{
        Clientes cliente = colaClientes.front();
        colaClientes.pop();
    }
    system("pause");
}

/*		FUNCION MENU PARA EL MENU			*/
void menu(int& eleccion)
{
    cout << "\tMENU PRINCIPAL: \n";
    cout << "1.-\tIniciar simulacion." << endl;
    cout << "2.-\tCreditos." << endl;
    cout << "3.-\tOtros." << endl;
    cout << "4.-\tSalir." << endl;
    cout << "\n-\tDigite su opcion: ";
    cin >> eleccion;
}

/*		FUNCION PARA LOS CREDITOS		*/
void Creditos(){
    cout << "\n\nCreadores: "<<endl;
    cout << "Omar Castellano 	CI: V-31.770.825" << endl;
    cout << "Manuel Pastrano 	CI: V-28.534.779" << endl;
    cout << "Julio Suarez 		CI: V-31.074.002" << endl;
    cout << "Juan Mora 		CI: V-31.577.117" << endl;
    cout << "Armando Serra 		CI: V-31.839.664" << endl;

    //cout << "\nPresiona cualquier tecla para salir. ";
    system("pause");
}

void Otros()
{
	int opcion;
	cout << "\n";
	cout << "1.-\tMostrar Productos cargados." << endl;
	cout << "2.-\tMostrar Clientes en el mercado." << endl;
	cout << "\n\tDigite una opcion: "; cin >> opcion;
	
	switch(opcion)
	{
		//	MOSTRAR PRODUCTOS CARGADOS
		case 1:BaseDeDatos_Productos(); break;
		
		//	MOSTRAR LOS CLIENTES EN LA BASE DE DATOS
		case 2: mostrarClientes(); break;
		default: cout << "\nOpcion invalida";
	}
	system("pause");
}

//		FUNCION PARA TIEMPOS ENTRE COMPRAS
void esperando_compra(int tiempo)
{	
	for(int i= 0; i<50; i ++){
		cout<<" ";
	}
	cout<<"Comprando..."<<endl;
	
	for(int i= 0; i<35; i ++){
		cout<<" ";
	}	
	for(int i=0; i<tiempo; i++)
	{
		cout<<"------";
		Sleep(1000);
	}
	cout<<"\n\n";
	system("cls");
}


/*					BASES DE DATOS									*/

//				MOSTRAR LA BASE DE DATOS DE LOS CLIENTES
void BaseDeDatos_Clientes()
{
	string inputString;
	
	
	//	LLAMO A LA FUNCION PARA AGREGAR LOS TIEMPOS A CADA CLIENTE
	GenerarTiempoDeCompra();
	//LLAMO A LA FUNCION PARA AGREGAR LOS PRODUCTOS
	
	/*		CARGAR PRODUCTOS			*/
	ifstream archivo("productos.txt");
	
    if (archivo.is_open())
	{
        //	ITERADOR PARA AGREGAR PRODUCTOS DESDE EL ARCHIVO A LA ESTRUCTURA PRODUCTOS
        int i = 0;
        while (i < 10 && archivo >> productos[i].id >> productos[i].descripcion >> productos[i].precio >> productos[i].stock)
		{
            i++;
        }
        archivo.close();
    }
    	//	EN CASO DE QUE EL ARCHIVO NO SE ECUENTRE EN LA MISMA CARPETA DEL PROGRAMA
	else
	{
        cout << "No se pudo abrir el archivo." << endl;
    }
	
	/*		CARGAR CLIENTES		*/
	ifstream file("clientes.txt");
    if (file.is_open())
	{
		//	ITERADOR PARA AGREGAR CLIENTES DESDE EL ARCHIVO A LA ESTRUCTURA CLIENTES
		int i = 1;
        while (i < 11 && file >> clientes[i].nombre >> clientes[i].cv >> clientes[i].dinero_disponible)
		{
            i++;
        }
        file.close();
	}
		//	EN CASO DE QUE EL ARCHIVO NO SE ECUENTRE EN LA MISMA CARPETA DEL PROGRAMA
	else
	{
        cout << "No se pudo abrir el archivo." << endl;
	}
	fflush(stdin);
	
	
	int j,k; //	J = PRODUCTO RANDOM, K = NUMERO DE ARTICULOS A AGREGAR AL CARRITO
	srand(static_cast<unsigned int>(time(nullptr)));	//	DEFINO SRAND PARA CREAR UN NUMERO RANDOM
	
	for(int m=0;m<11;m++)
	{
		for(int i = 0; i < k; i++)	//	i = VARIABLE DE ITERACION HASTA LLEGAR AL NUMERO K
		{
			k = rand() % 30 + 1;	//	GENERA UN NUMERO RANDOM PARA LA CANTIDAD DE PRODUCTOS A ALMACENAR
		    j = rand() % 9 + 0;	//	GENERA UN NUMERO RANDOM PARA EL PRODUCTO A ESCOGER
		    productos[j].stock -1;	//	ELIMINAMOS -1 AL STOCK DE DICHO ARTICULO SELECCIONADO
		    clientes[m].Carrito.push(productos[j].descripcion);
			clientes[m].PagoT += productos[j].precio;
			fflush(stdin);
		}
	}
	fflush(stdin);
	//	MOSTRAMOS LOS DATOS DEL CLIENTE
    cout << "\nBase de datos de los clientes:\n"<<endl;
    for (int i=0; i<11; i++)
	{
		if(clientes[i].tiempo_de_compra <= 10 && clientes[i].dinero_disponible >= clientes[i].PagoT)
		{
			cout << "\n";																	//ESPACIO ENTRE DATOS DE CADA CLIENTE
	        cout << "Nombre: " << clientes[i].nombre << endl;								//NOMBRE DEL CLIENTE
	        cout << "Cedula de identidad: V-" << clientes[i].cv << endl;					//CEDULA DEL CLIENTE
	        cout << "Total a pagar: " << clientes[i].PagoT << endl;							//PAGO TOTAL A REALIZAR
	        cout << "Tiempo de compra: " << clientes[i].tiempo_de_compra << " min" << endl;	//MUESTRA EL TIEMPO TOTAL DE COMPRA
	        
	        for(int l = 1;!clientes[i].Carrito.empty();l++)
			{
			    cout <<l<<" "<< clientes[i].Carrito.top() << endl;
			    clientes[i].Carrito.pop();
			    fflush(stdin);
		    }
		    totalVenta += clientes[i].PagoT;
	        compradores++;
	        clientes_presentes++;		    
		    esperando_compra(clientes[i].tiempo_de_compra);
	        //cout << "------------------------" << endl;
	        fflush(stdin);
	    }
	    else
	    {
	        clientes_presentes++;	    	
	    	cout << "\n";
	    	cout << "El cliente "<<clientes[i].nombre<<" ha excedido el tiempo de compra o no tiene los fondo suficiente, ha salido de la cola\n";
	    	Sleep(1500);
	    	system("cls");
	    	esperando_compra(clientes[i].tiempo_de_compra);
			fflush(stdin);
		}	
	}
	//	MUESTRA LA CANTIDAD QUE EXCEDIO EL TIEMPO DE COMPRA
	cout << "\nCantidad de Clientes que superaron el tiempo limite: " << tiempo_excedido << endl;
	cout << "\n\tCantidad de Clientes que compraron exitosamente: " << compradores << endl;
	cout << "\n\tCantidad de Clientes que se presentaron a comprar: " << clientes_presentes << endl;
	cout<<"\n\tTotal de ventas: $"<<totalVenta<<endl;	
}	
	
//	FUNCION PARA CARGAR LOS PRODUCTOS DESDE EL .TXT
void BaseDeDatos_Productos()
{
	/*		CARGAR PRODUCTOS			*/
	ifstream archivo("productos.txt");
	
    if (archivo.is_open())
	{
        //	ITERADOR PARA AGREGAR PRODUCTOS DESDE EL ARCHIVO A LA ESTRUCTURA PRODUCTOS
        int i = 0;
        while (i < 9 && archivo >> productos[i].id >> productos[i].descripcion >> productos[i].precio >> productos[i].stock)
		{
            i++;
        }
        archivo.close();
    }
    	//	EN CASO DE QUE EL ARCHIVO NO SE ECUENTRE EN LA MISMA CARPETA DEL PROGRAMA
	else
	{
        cout << "No se pudo abrir el archivo." << endl;
    }
    cout << "\n";
	for(int i=0; i<9; i++)
	{
	    cout<<".- "<<productos[i].descripcion;
	    cout<<" $"<<productos[i].precio<<endl;
	    cout<<" Stock: "<<productos[i].stock<<endl;
	    cout<<"\n";
	};
    fflush(stdin);
}

void mostrarClientes(){
	for (int i = 0; i<10; i++){
			cout << "\n";																	//ESPACIO ENTRE DATOS DE CADA CLIENTE
	        cout << "Nombre: " << clientes[i].nombre << endl;								//NOMBRE DEL CLIENTE
	        cout << "Cedula de identidad: V-" << clientes[i].cv << endl;					//CEDULA DEL CLIENTE
	        cout << "Dinero disponible: $" << clientes[i].dinero_disponible << endl;		//DINERO DEL CLIENTE
	        cout << "Total a pagar: " << clientes[i].PagoT << endl;							//PAGO TOTAL A REALIZAR
	        cout << "Tiempo de compra: " << clientes[i].tiempo_de_compra << " min" << endl;	//MUESTRA EL TIEMPO TOTAL DE COMPRA
	        cout << "Tiempo de llegada: " << clientes[i].tiempo_de_llegada << endl;			//TIEMPO DE LLEGAR AL MERCADO
	}
}