#include <iostream>
#include <regex>
#include <fstream>
#include <vector>

bool isCorrectInstruction(char **argv, int argc);
void readMatrixFile(std::string matrixFileName, int &rowN, int &colN, std::vector<std::vector<double>>  &matrix);
void multiplyMatrixSeq(std::vector<std::vector<double>>  &matrix1, std::vector<std::vector<double>>  &matrix2, std::vector<std::vector<double>>  &result, int rowA, int colA, int rowB, int colB);
void multiplyMatrixPar(std::vector<std::vector<double>>  &matrix1, std::vector<std::vector<double>>  &matrix2, std::vector<std::vector<double>>  &result, int rowA, int colA, int rowB, int colB);
void printMatrix(std::vector<std::vector<double>>  &matrix);

int main(int argc, char** argv){

    int rowA, rowB, colA, colB = 0;
    bool test = false;
    std::vector<std::vector<double>>  matrix1(rowA, std::vector<double>(colA));
    std::vector<std::vector<double>>  matrix2(rowB, std::vector<double>(colB)); 

    if(isCorrectInstruction(argv, argc) || test){
        if(test){
           readMatrixFile("matrix1.txt", rowA, colA, matrix1);
           readMatrixFile("matrix2.txt", rowB, colB, matrix2);
        }else{
        readMatrixFile(argv[argc-2], rowA, colA, matrix1);
        readMatrixFile(argv[argc-1], rowB, colB, matrix2);
        }

        std::vector<std::vector<double>> result(rowA, std::vector<double>(colB));
        if(argv[2]=="-s"){
            multiplyMatrixSeq(matrix1, matrix2, result, rowA, colA, rowB, colB);
        }else{

        }
        printMatrix(matrix1);
        printMatrix(matrix2);

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

void readMatrixFile(std::string matrixFileName, int &rowN, int &colN, std::vector<std::vector<double>>  &matrix){
    std::ifstream matrixFile(matrixFileName);
    if(matrixFile.is_open()){
        int i = 0;
        int j = 0;
        double number;
        while(matrixFile >> number){
            if(i>1){
                matrix[(i-2)%rowN][j%colN]=number;
                j++;
            }else{
                if(i==0)
                    rowN = number;
                else{
                    colN = number;
                    matrix.resize(rowN, std::vector<double>(colN));
                }
            }
            if(j%3==0){
                i++;
            }
        }
        matrixFile.close();
    }
}

void multiplyMatrixSeq(std::vector<std::vector<double>>  &matrix1, std::vector<std::vector<double>>  &matrix2, std::vector<std::vector<double>>  &result, int rowA, int colA, int rowB, int colB){
    if(colA==rowB){
        for(int i = 0; i<rowA; i++){
            for(int j=0; j<colB; j++){
                result[i][j]=0;
                for(int n=0; n<colA; n++){
                    result[i][j]+=matrix1[i][n]*matrix2[n][j];
                }
            }
        }
    }
}

void multiplyMatrixPar(std::vector<std::vector<double>>  &matrix1, std::vector<std::vector<double>>  &matrix2, std::vector<std::vector<double>>  &result, int rowA, int colA, int rowB, int colB){
        if(colA==rowB){
            
        for(int i = 0; i<rowA; i++){
            for(int j=0; j<colB; j++){
                result[i][j]=0;
                for(int n=0; n<colA; n++){
                    result[i][j]+=matrix1[i][n]*matrix2[n][j];
                }
            }
        }
    }
}

void printMatrix(std::vector<std::vector<double>>  &matrix){
    for (auto vec: matrix){
        std::cout << " |";
        for(auto value: vec){
            std::cout << " " << value;        
        }
        std::cout << " |" << "\n";
    }
}