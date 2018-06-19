#include "procesadoficheros.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qstring.h"
#include "vector"
#include "stdlib.h"
#include "stdio.h"
using namespace std;
using namespace cv;

QStringList SemanaLaboral={"Lunes" , "Martes" , "Miércoles" , "Jueves" , "Viernes"};
QStringList FindeSemana={"Sábado" , "Domingo"};

ProcesadoFicheros::ProcesadoFicheros()
{

}

ProcesadoFicheros::~ProcesadoFicheros()
{

}

char * ProcesadoFicheros::ToFloatString(const std::string& myString) {
   // Create a pointer to myString

    //char* p= new char[myString.size()];
    size_t t=sizeof(char);
    char* p = (char *)malloc(t);


    std::string::const_iterator it = myString.begin();

    for (int i=0; i<myString.size();++i)
    {
        if(it[i]==',')
        {
            p[i]='.';
            p=(char *)realloc(p,t);
            t++;
        }
        else
        {
            p[i]=it[i];
            p=(char *)realloc(p,t);
            t++;
        }
    }
   // Return the modified string
   return p;
}

bool ProcesadoFicheros::is_number(const std::string& s)
{
    int numero=0;
    std::string::const_iterator it = s.begin();
    while (it != s.end())
    {
           if((std::isdigit(*it))||(*it==','))
           {
               ++it;
               ++numero;
           }
           else
               ++it;
     }
    if (numero==s.length())
        return true;
    else
        return false;
}

int ProcesadoFicheros::to_sec(const std::string& tc)
{
    long hh=atol(tc.substr(0,2).c_str()),mm=atol(tc.substr(3,2).c_str()),ss=atol(tc.substr(6,2).c_str());

    return hh*3600+mm*60+ss;
}

void ProcesadoFicheros::LeeFichero(std::string fileName, std::string dirName)
{
    Mat featuresUnclustered;
    string action;
    string fullName=dirName+"/"+fileName;

    if(dirName.find_last_of("/") != std::string::npos)
            action = dirName.substr(dirName.find_last_of("/")+1);

    if(fileName.substr(fileName.find_first_of(".")+1) == "txt")
    {
        FILE * fp;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        QString linea=line;

        fp = fopen(fullName.c_str(), "r");
        if (fp == NULL)
           exit(EXIT_FAILURE);

    /*/////////////////////////////////////////////////////*/
        int number_of_lines = 0, number_of_colums = 0;
        Mat MatDescriptor_TMP_SVM(number_of_lines,++number_of_colums,CV_32FC1);
    /*////////////////////////////////////////////////////*/

        vector<vector<string> > MatrizTMP;

        int rows=0;
        int cols=0;
        rewind(fp);

        read = getline(&line, &len, fp);
        do
        {
            linea=line;
        } while ((!linea.contains("Total										",Qt::CaseSensitive)) && ((read = getline(&line, &len, fp)) != -1));
//        } while ((!linea.contains("Total\t\t\t\t\t\t\t\t\t\t",Qt::CaseSensitive)) && ((read = getline(&line, &len, fp)) != -1));

        while ((read = getline(&line, &len, fp)) != -1)
        {
            linea=line;
            //Extract parts separated by "\t"
            char * pch;
            pch = strtok (line,"\t\n\r");

            MatrizTMP.push_back(vector<string>());

            while (pch != NULL)
            {
                bool es_numero=is_number(pch);

                if ((es_numero)&&(cols==9))
                {
                    MatrizTMP[rows].push_back(" ");
                    ++cols;
                }
                else if ((es_numero)&&(cols>9))
                {
                    QString d;
                    d=QString::number(atof(ToFloatString((const char *)pch)));

                    MatrizTMP[rows].push_back(d.toStdString());
                    pch = strtok (NULL, "\t\n\r");
                    ++cols;
                }
                else
                {
                    MatrizTMP[rows].push_back(pch);
                    pch = strtok (NULL, "\t\n\r");
                    ++cols;
                }
            }

            free(pch);

            ++rows;
            cols=0;
        }

        fclose(fp);
        if (line)
            free(line);


/*Busca en la matriz los elementos repetidos para cada tipo de clase
* (Segmentos de 30 minutos y diferencia semana laboral de fin de semana) total 96 clases*/
        //vector<vector<vector<vector<vector<string>>>>>MatrizAudiencias;
        vector<vector<vector<vector<vector<string>>>>>MatrizAudiencias;
        MatrizAudiencias.push_back(vector<vector<vector<vector<string>>>>());   //Dias de diario
        MatrizAudiencias.push_back(vector<vector<vector<vector<string>>>>());   //Fin de semana
        for (int x=0;x<48;x++)
        {
            MatrizAudiencias[0].push_back(vector<vector<vector<string>>>());   //48 fragmentos de 30 minutos
            MatrizAudiencias[1].push_back(vector<vector<vector<string>>>());   //48 fragmentos de 30 minutos
        }
        int i=0, j=0;
        for (i = 0; i < rows; i++)
        {
            if (SemanaLaboral.contains(MatrizTMP[i][4].c_str())) //4 Día de la semana
            {
                int ii=to_sec(MatrizTMP[i][6].c_str()); //6 hora Fin
                int jj=to_sec(MatrizTMP[i][5].c_str()); //5 hora Inicio
                int kk=jj-ii;
                int ll=abs(kk);
                float mm=(float)ll / (float)1800;
                int nn=round(mm);
                double duracion = round((float)abs((to_sec(MatrizTMP[i][6].c_str()))-(to_sec(MatrizTMP[i][5].c_str())))/(float)1800);
                int tramo=round((float)(to_sec(MatrizTMP[i][6].c_str()))/(float)1800);
                MatrizAudiencias[0][tramo].push_back(vector<vector<string> >());    //Genera la nueva fila para ese horario
                for (j = 0; j < MatrizTMP[i].size(); j++)
                {
                    MatrizAudiencias[0][tramo][j].push_back(" ");
                    /*
                    //if (round(((to_sec(MatrizTMP[i][6].c_str()))-(to_sec(MatrizTMP[i][5].c_str())))/1800))    //5 Hora Inicio //6 Hora Fin
                    if (duracion<=1)
                    {
                        //MatrizAudiencias[0][tramo-1][j].push_back(vector<vector<string>>);
                    }
                    else
                    {

                    }
                    const char * c = MatrizTMP[i][0].c_str();
                    printf("%s\t", MatrizTMP[i][0].c_str());
                    */
                }
            }
            else if (FindeSemana.contains(MatrizTMP[i][4].c_str()))
            {
                MatrizAudiencias[1].push_back(vector<vector<vector<string> > >());
                const char * c = MatrizTMP[i][0].c_str();
                printf("%s\t", MatrizTMP[i][0].c_str());
            }
        }
/*///////////////////////////////////////////*/




        //featuresUnclustered.push_back(MatDescriptor_TMP_SVM.reshape(1,1));   // One line per action.
        featuresUnclustered.push_back(MatDescriptor_TMP_SVM);
        featuresUnclustered.push_back(MatDescriptor_TMP_SVM.reshape(1,1));   // One line per action.

        //Just in case featuresUnclustered not defined as <float>
            Mat descriptor;

            featuresUnclustered.convertTo(descriptor, CV_32FC1);

        /*///////////////////////////////////////////////////////////*/
            FileStorage fs(fullName.substr(0,fullName.find_first_of(".")) + ".yml", FileStorage::WRITE);
            fs << "Action" << descriptor;
            fs.release();
    }
}

void ProcesadoFicheros::process_line(QString line)
{
    if (line=="Fechas")
    {
        QString Fechas=line;
    }
}
