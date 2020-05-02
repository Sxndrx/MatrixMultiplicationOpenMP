#include <iostream>
#include <regex>
#include <fstream>
#include <vector>
#include <chrono>
#include "omp.h"

bool isCorrectInstruction(char **argv, int argc);
void readMatrixFile(std::string matrixFileName, int &rowN, int &colN, std::vector<double> &matrix);
void multiplyMatrixSeq(std::vector<double> &matrix1, std::vector<double>  &matrix2, std::vector<double>  &result, int rowA, int colA, int rowB, int colB);
void multiplyMatrixPar(std::vector<double>  &matrix1, std::vector<double>  &matrix2, std::vector<double>  &result, int rowA, int colA, int rowB, int colB);
void printMatrix(std::vector<double>  &matrix);

int main(int argc, char** argv){

    int rowA, rowB, colA, colB = 0;
    bool test = false;
    std::vector<double>  matrix1(rowA*colA);
    std::vector<double>  matrix2(rowB*colB); 
    if(isCorrectInstruction(argv, argc) || test){
        if(test){
           readMatrixFile("matrix1.txt", rowA, colA, matrix1);
           readMatrixFile("matrix2.txt", rowB, colB, matrix2);
        }else{
        readMatrixFile(argv[argc-2], rowA, colA, matrix1);
        readMatrixFile(argv[argc-1], rowB, colB, matrix2);
        }

        std::vector<double> result(rowA*colB);
        if(argc==4){
            auto t1 = std::chrono::high_resolution_clock::now();
            multiplyMatrixSeq(matrix1, matrix2, result, rowA, colA, rowB, colB);
            auto t2 = std::chrono::high_resolution_clock::now();
            std::cout<<"seq execution time: "<<std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()<<"\n";
        }else{
            auto t1 = std::chrono::high_resolution_clock::now();
            multiplyMatrixPar(matrix1, matrix2, result, rowA, colA, rowB, colB);
            auto t2 = std::chrono::high_resolution_clock::now();
            std::cout<<"par execution time: "<<std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()<<"\n";
        }
    }

    return 0;
}

bool isCorrectInstruction(char **argv, int argc){
std::regex instruction_regex("(-s )?\\w+\\.txt \\w+\\.txt ");
    std::string instruction="";
    for(int i=1; i<argc; i++){
        instruction+=argv[i];
        instruction+=" ";
    }
    return std::regex_match(instruction, instruction_regex) ? true : false;
}

void readMatrixFile(std::string matrixFileName, int &rowN, int &colN, std::vector<double>  &matrix){
    std::ifstream matrixFile(matrixFileName);
    if(matrixFile.is_open()){
        int i = 0;
        int j = 0;
        double number;
        while(matrixFile >> number){
            if(i>1){
                matrix[j]=number;
                j++;
            }else{
                if(i==0)
                    rowN = number;
                else{
                    colN = number;
                    matrix.resize(rowN*colN);
                }
            }
            if(j%3==0){
                i++;
            }
        }
        matrixFile.close();
    }
}

void multiplyMatrixSeq(std::vector<double>  &matrix1, std::vector<double>  &matrix2, std::vector<double>  &result, int rowA, int colA, int rowB, int colB){
    if(colA==rowB){
        for(int i = 0; i<rowA; i++){
            for(int j=0; j<colB; j++){
                result[i*colB+j]=0;
                for(int n=0; n<colA; n++){
                    result[i*colB+j]+=matrix1[i*colB+n]*matrix2[n*colB+j];
                }
            }
        }
    }
}

void multiplyMatrixPar(std::vector<double>  &matrix1, std::vector<double>  &matrix2, std::vector<double>  &result, int rowA, int colA, int rowB, int colB){
        if(colA==rowB){

        int i, j, n =0;
        #pragma omp parallel shared(result) private(i,j,n)
        {
            #pragma omp for schedule(static)
                for(i = 0; i<rowA; i++){
                    for(j=0; j<colB; j++){
                        double total = 0;
                        for(n=0; n<colA; n++){
                            total+=matrix1[i*colB+n]*matrix2[n*colB+j];
                        }
                    result[i*colB+j]=total;
                    }
                }
        }
       
    }
}

void printMatrix(std::vector<double>  &matrix){
    for (long unsigned int i=0; i<matrix.size(); i++){
        std::cout << " |"<<matrix[i]<<"|";
        if(i%3==2){
            std::cout<<"\n";
        }
    }
}