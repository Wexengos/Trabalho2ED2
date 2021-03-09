#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <climits>
#include <stdio.h>
#include <locale.h>
#include "QuadTree.h"
#include "AVLTree.h"
#include "ArvB.h"
#include "RegistroCoordinates.h"

using namespace std;

using namespace std::chrono;
#define TAMANHOMAX 1431490
#define QUANTCIDADES 5571


//g++ -o teste -O3 *.cpp
//.teste
void menuPrincipal(Registro *registros,RegistroCoordinates *registrosCoord,Hashing *hashPrinci,QuadTree *quadPrinci);
void moduloTeste(Registro *registro, RegistroCoordinates *registroCidades,Hashing *hashPrinci,QuadTree *quadPrinci);


void criaArquivoSaidaTxt(string nomeArquivo)
{
    ofstream arq(nomeArquivo);
    arq.close();
}

void testeQuadTree(RegistroCoordinates *registroCidades, int N)
{

    QuadTree *quad = new QuadTree();

    //arq."Cidades Inseridades na QuadTree"<<endl;
    high_resolution_clock::time_point inicio = high_resolution_clock::now();
    for (int i = 0; i < N; i++)
    {
        NoArvQuad *no = new NoArvQuad();
        no->setLatitude(registroCidades[i].getLatitude());
        no->setLongitude(registroCidades[i].getLongitude());
        no->setNome(registroCidades[i].getcidade_nome());
        quad->insere(no);
      
    }
    high_resolution_clock::time_point fim = high_resolution_clock::now();
    int tem = duration_cast<duration<double>>(fim - inicio).count();

    if (N <= 20)
    {
        cout << "Saida Console" << endl;
        quad->imprime();
    }
    else
    {
        cout << "Arquivo TXT criado" << endl;
        ofstream arq; // ("a:lista.dat", ios::ate | ios::out | ios::in)
        arq.open("QuadTreeTXT", ios::ate | ios::out | ios::in);
        quad->imprimeTXT(arq);
        arq << "Tempo: " << tem << endl;
        arq.close();
    }

    criaArquivoSaidaTxt("QuadIntervaloTXT");
    ofstream arq1;
    arq1.open("QuadIntervaloTXT", ios::ate | ios::out | ios::in);
    quad->cidadesNoIntervalo(arq1, -35, -1, -35, -1);
    arq1<<"Contador: "<<quad->getCont() << endl;
    arq1.close();

    cout << "Final" << endl;
}

void testeHash(Registro *registro, int N)
{

    Hashing *hash = new Hashing(N);
    int cont = 0;
    register int chave, id;
    //arq."Cidades Inseridades na QuadTree"<<endl;
    high_resolution_clock::time_point inicio = high_resolution_clock::now();
    for (int i = 0; i < N; i++)
    {
        //cout<<"Cidade "<<registro[i].getCidade()<<"foi visitada? "<<registro[i].getVisitado()<<endl;
        chave = registro[i].getCodigoCidade() + registro[i].getDataInt();
        id = hash->insere(chave, &registro[i]);
        cont++;
        //cout<<"id=> "<<id<<endl;
        //cout<<"chave: "<<chave<<endl;
        //cout<<"id: "<<id<<endl;
    }
    high_resolution_clock::time_point fim = high_resolution_clock::now();
    double tem = duration_cast<duration<double>>(fim - inicio).count();
    if (N <= 20)
    {
        cout << "Saida no Console ===> Cidades Inseridas na Hash" << endl;
        hash->imprime();
    }
    else
    {
        cout << "Arquivo TXT criado" << endl;
        ofstream arq; // ("a:lista.dat", ios::ate | ios::out | ios::in)
        arq.open("HashingTXT", ios::ate | ios::out | ios::in);
        hash->imprimeTXT(arq);
        hash->casosTotaisCidade(330010);
        arq << "Tempo: " << tem << endl;
        arq.close();
    }
    cout << endl;
    cout << "Ch: " << hash->getChavesArmazenadas() << endl;
    cout << "Teste finalizado" << endl;
}
void testeAvl(Registro *registro, int N)
{
    Hashing *hash = new Hashing(N);
    int cont = 0;
    register int chave, id;
    AVLTree *avl = new AVLTree();

    for (int i = 0; i < N; i++)
    {
        chave = registro[i].getCodigoCidade() + registro[i].getDataInt();
        id = hash->insere(chave, &registro[i]);
        avl->insere(id, hash);
    }

    if (N <= 20)
    {
        cout << "Saida no Console ===> Cidades Inseridas na Hash" << endl;
        avl->imprime(hash);
    }
    else
    {
        cout << "Arquivo TXT criado" << endl;
        ofstream arq; // ("a:lista.dat", ios::ate | ios::out | ios::in)
        arq.open("AvlTXT", ios::ate | ios::out | ios::in);
        avl->imprimeTXT(hash, arq);
        arq.close();
    }
    cout << endl;
    cout << "Teste finalizado" << endl;
    hash->~Hashing();
}
void testeAVB(Registro *registro, int N)
{
    Hashing *hash = new Hashing(N);
    int cont = 0;
    register int chave, id;
    ArvB *arvB = new ArvB(3);

    for (int i = 0; i < N; i++)
    {
        chave = registro[i].getCodigoCidade() + registro[i].getDataInt();
        id = hash->insere(chave, &registro[i]);
        arvB->insereArvB(id, hash);
    }

    if (N <= 20)
    {
        cout << "Saida no Console ===> Cidades Inseridas na Hash" << endl;
        arvB->imprimeArv(hash);
    }
    else
    {
        cout << "Arquivo TXT criado" << endl;
        ofstream arq; // ("a:lista.dat", ios::ate | ios::out | ios::in)
        arq.open("AvBTXT", ios::ate | ios::out | ios::in);
        arvB->imprimeArvTXT(hash, arq);
        arq.close();
    }
    cout << endl;
    cout << "Teste finalizado" << endl;
    hash->~Hashing();
}


void etapa1(RegistroCoordinates *registrosCoordinate, QuadTree *quadPrincipal)
{       
        cout<<"Lendo Arquivo com as cidades e Armazenando na QuadTree"<<endl;
        registrosCoordinate->leArquivoCoordi(registrosCoordinate, QUANTCIDADES);
    
        for (int i = 0; i < QUANTCIDADES; i++)
        {
            NoArvQuad *no = new NoArvQuad();
            no->setLatitude(registrosCoordinate[i].getLatitude());
            no->setLongitude(registrosCoordinate[i].getLongitude());
            no->setNome(registrosCoordinate[i].getcidade_nome());
            quadPrincipal->insere(no);
        }
    /*
        cout << "Arquivo TXT criado" << endl;
        ofstream arq; // ("a:lista.dat", ios::ate | ios::out | ios::in)
        arq.open("QuadTreeTXT", ios::ate | ios::out | ios::in);
        quadPrincipal->imprimeTXT(arq);
    */

}
void etapa2(Registro *registros,Hashing *hashingPrincipal,vector<int> idhashing)
{   
    cout<<"Lendo Registros e Armazenando na Hashing Table"<<endl;
    registros->leArquivo(registros,TAMANHOMAX);
    register int chave, id;

    for (int i = 0; i < TAMANHOMAX; i++)
    {
        chave = registros[i].getCodigoCidade() + registros[i].getDataInt();
        id = hashingPrincipal->insere(chave, &registros[i]);
        //idhashing[i] = id;
    }   
    /*
        cout << "Arquivo TXT criado" << endl;
        ofstream arq; // ("a:lista.dat", ios::ate | ios::out | ios::in)
        arq.open("HashingTXT", ios::ate | ios::out | ios::in);
        hashingPrincipal->imprimeTXT(arq);
    */
}
void menuPrincipal(Registro *registros,RegistroCoordinates *registrosCoord,Hashing *hashPrinci,QuadTree *quadPrinci)
{   

    vector<int> idhashig;
    
    int menu;
    cout << "====================MENU PRINCIPAL====================" << endl;
    cout << "Digite:" << endl;
    cout << "[1] Carregar o arquivo de cidades " << endl;
    cout << "[2] Carregamento do arquivo pre-processado de casos" << endl;
    cout << "[3] Modulo de testes" << endl;
    cout << "[4] Analise das estruturas balanceadas" << endl;
    cout << "[5] Encerrar o programa" << endl;
    cin >> menu;
    
    switch(menu)
    {
    case 1:
        system("cls");
        etapa1(registrosCoord,quadPrinci);
        cout << "Etapa 1 concluida!" << endl;
        menuPrincipal(registros,registrosCoord,hashPrinci,quadPrinci);
        break;
        
    case 2:
        system("cls");
        etapa2(registros,hashPrinci,idhashig);
        cout << "Etapa 2 concluida!" << endl;
        menuPrincipal(registros,registrosCoord,hashPrinci,quadPrinci);
        break;
        
    case 3:

        system("cls");
        moduloTeste(registros, registrosCoord,hashPrinci,quadPrinci);
        cout << "Etapa 3 concluida!" << endl;
        break;
       
    case 4:
         break;
    case 5:
        cout << "Fechando Exec!" << endl;
        cout << "Muito obrigado Caniato" << endl;
        exit(1);
        break;
       
    default:
        cout << "Valor invalido" << endl;
        menuPrincipal(registros,registrosCoord,hashPrinci,quadPrinci);
        break;
    }
    cout << "Fim" << endl;
}
void moduloTeste(Registro *registro, RegistroCoordinates *registroCidades,Hashing *hashPrinci,QuadTree *quadPrinci)
{
    
    int menuTeste, n;
    cout << "===============Modulo de Teste===============" << endl;
    cout << "[1] Inserção de N cidades na quad tree" << endl;
    cout << "[2] Inserção de N registros na tabela hash " << endl;
    cout << "[3] Inserção de N chaves na árvore AVL" << endl;
    cout << "[4] Inserção de N chaves na árvore B" << endl;
    cout << "[5] Voltar para o menu principal" << endl;

    cin >> menuTeste;

    switch (menuTeste)
    {
    case 1:
        system("cls");
        cout << "Digite N cidades que sera inseridas" << endl;
        cin >> n;
        criaArquivoSaidaTxt("QuadTreeTXT");
        testeQuadTree(registroCidades, n);
        moduloTeste(registro, registroCidades,hashPrinci,quadPrinci);

        break;
    case 2:
         system("cls");
        cout << "Digite  N registro que sera inseridos" << endl;
        cin >> n;
        criaArquivoSaidaTxt("HashingTXT");
        testeHash(registro, n);
        moduloTeste(registro, registroCidades,hashPrinci,quadPrinci);

        break;
    case 3:
        system("cls");
        cout << "Digite N chaves que sera inseridas na AVL" << endl;
        cin >> n;
        criaArquivoSaidaTxt("AvlTXT");
        testeAvl(registro, n);
        moduloTeste(registro, registroCidades,hashPrinci,quadPrinci);
        break;

    case 4:
        system("cls");
        cout << "Digite N cidades que sera inseridas na ARV B" << endl;
        cin >> n;
        criaArquivoSaidaTxt("AvBTXT");
        testeAVB(registro, n);
        moduloTeste(registro, registroCidades,hashPrinci,quadPrinci);

        break;
    case 5:
        menuPrincipal(registro,registroCidades,hashPrinci,quadPrinci);
    default:
        break;
    }
}
int main(int argc, char *argv[])
{
     
    //RegistroCoordinates *registrosCoordinate = new RegistroCoordinates[QUANTCIDADES];
    //Registro *registros = new Registro[TAMANHOMAX];

    QuadTree *quadPrincipal = new QuadTree();
    RegistroCoordinates *registrosCoordinate = new RegistroCoordinates[QUANTCIDADES];
    Registro *registro = new Registro[TAMANHOMAX];
    Hashing *hashPrincipal = new Hashing(TAMANHOMAX);
    
    menuPrincipal(registro,registrosCoordinate,hashPrincipal,quadPrincipal);

    /*
    int tamanhoN[] = {10000, 50000, 100000, 500000, 1000000, TAMANHOMAX};
    Registro *registros = new Registro[tamanhoN[5]];
    registros->leArquivo(registros,tamanhoN[5]);
    //registros->transformaCasosAcumuladosEmCasosDiarios(registros,tamanhoN[5]);


    RegistroCoordinates *registrosCoordinate = new RegistroCoordinates[QUANTCIDADES];
    registrosCoordinate->leArquivoCoordi(registrosCoordinate,QUANTCIDADES);
    moduloTeste(registros,registrosCoordinate);
    */
    return 0;
}
