#ifndef PROCESADOFICHEROS_H
#define PROCESADOFICHEROS_H

#include "string"
#include <string.h>
#include <qstring.h>
#include <opencv2/opencv.hpp>

class ProcesadoFicheros
{
public:
    ProcesadoFicheros();
    ~ProcesadoFicheros();

    //////////////// CrearDiccionario ///////////////////
    void LeeFichero(std::string fileName, std::string dirName);
    char * ToFloatString(const std::string& myString);
    bool is_number(const std::string& s);
    int to_sec(const std::string& tc);
    ////////////////////////////////////////////////
};

#endif // PROCESADOFICHEROS_H
