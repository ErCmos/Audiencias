#include "procesadoficheros.h"
using namespace std;
using namespace cv;

ProcesadoFicheros::ProcesadoFicheros()
{

}

ProcesadoFicheros::~ProcesadoFicheros()
{

}

void ProcesadoFicheros::LeeFichero(std::string fileName, std::string dirName)
{
    Mat featuresUnclustered;
    string action;

    if(dirName.find_last_of("/") != std::string::npos)
            action = dirName.substr(dirName.find_last_of("/")+1);

    if(fileName.substr(fileName.find_first_of(".")+1) == "txt")
    {
        FILE * fp;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        //string fullName=dirName+"/"+fileName;
        string fullName=fileName;

        fp = fopen(fullName.c_str(), "r");
        if (fp == NULL)
           exit(EXIT_FAILURE);

    /*/////////////////////////////////////////////////////*/
        int ch, number_of_lines = 0, number_of_colums = 0;
        read = getline(&line, &len, fp);
        char * pch;
        pch = strtok (line,"\t");
        while (pch != NULL)
        {
          number_of_colums++;
          pch = strtok (NULL, "\t\n");
        }

        free(pch);
        rewind(fp); //rewind the file to count the lines
        do
        {
            ch = fgetc(fp);
            if(ch == '\n')
                number_of_lines++;
        } while (ch != EOF);

    /*////////////////////////////////////////////////////*/
        Mat MatDescriptor_TMP_SVM(number_of_lines,number_of_colums,CV_32FC1);

        int rows=0;
        int cols=0;
        rewind(fp);
        QString linea=line;

        do
        {
            linea=line;
        } while ((!linea.contains("Total										",Qt::CaseSensitive)) && ((read = getline(&line, &len, fp)) != -1));

        while ((read = getline(&line, &len, fp)) != -1)
        {
            //Extract parts separated by "\t"
            char * pch;
            pch = strtok (line,"\t");

            action=line;

            while (pch != NULL)
            {
              //printf ("%s\n",pch);
              float value=atof(pch);
              MatDescriptor_TMP_SVM.at<float>(rows,cols)=value;
              pch = strtok (NULL, "\t\n");
              ++cols;
            }

            free(pch);

            ++rows;
            cols=0;
        }

        fclose(fp);
        if (line)
            free(line);

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
