#include <ManejoJson.hpp>

//***********************************Convertir a Json******************************************************************
StaticJsonDocument<1024> DatosFecha; //se declara el espacio que se deja para procesar el Json
                                     //hay que dejar 768, pero debe ponerse un parametro en potencias de 2, entonces el mas
                                     //cercano es 1024

void ConvertirString_Jseon(String DatosResponseApi)
{
    extern String ProcesamientoJson; //variable global para indicar si se proceso el json o no

    DeserializationError error = deserializeJson(DatosFecha, DatosResponseApi); //convierte a Json los datos de respuesta de la Api
                                                                                //que fueron guardados como string y devuelve si hubo error
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str()); //dice cual es el error que hubo al convertir los datos a Json
        ProcesamientoJson = "Error en el procesamiento del Json";
        return;
    }
    ProcesamientoJson = "Procesamiento del Json Correctamente";
}
//*************************************Acomodacion fecha***************************************************************

String SacarParametrosFecha()
{
    String datetime = DatosFecha["datetime"]; //se sacan parametros del json
    int day_of_week = DatosFecha["day_of_week"];
    String Meses[] = {"", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    String Dias[] = {"", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo"};

    String year = datetime.substring(0, 4); //se sacan sub cadenas para obtener los diferentes parametros de la fecha
    String Mes_str = datetime.substring(5, 7);
    String DiaNumero = datetime.substring(8, 10);
    String Hora = datetime.substring(11, 16);
    int Mes_int = atoi(Mes_str.c_str());
    String Mes = Meses[Mes_int];
    String Dia = Dias[day_of_week];
    String Fecha = Dia + ", " + DiaNumero + " " + Mes + " de " + year + " -- " + Hora; //se entrega la fecha en este formato

    return Fecha; //retorna la fecha
}
