#include <iostream>
#include <fstream>

using namespace std;

#include "Admin.h"

#include "SensorFumo.h"
#include "SensorHumidade.h"
#include "SensorLuz.h"
#include "SensorMissel.h"

#include "MotorCombustao.h"
#include "MotorEletrico.h"
#include "MotorInducao.h"

#include "Fabrica.h"
#include "Relogio.h"
#include "Uteis.h"

int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));
/*
    cout << "---------- TESTES DE HERANCA ENTRE UTILIZADORES ----------" << endl;
    Visitante *u = new Visitante(1, "ANA");

    cout << u->getNome() << endl;
    cout << "Listar:        " << u->getLISTAR()      << endl;
    cout << "Adicionar:     " << u->getADICIONAR()   << endl;
    cout << "Run:           " << u->getRUN()         << endl;
    cout << "Manutencao:    " << u->getMANUTENCAO()  << endl;
    cout << "Remover:       " << u->getREMOVER()     << endl;

    cout << endl << endl;
    Normal *n = new Normal(2, "LUIS");

    cout << n->getNome() << endl;
    cout << "Listar:        " << n->getLISTAR()      << endl;
    cout << "Adicionar:     " << n->getADICIONAR()   << endl;
    cout << "Run:           " << n->getRUN()         << endl;
    cout << "Manutencao:    " << n->getMANUTENCAO()  << endl;
    cout << "Remover:       " << n->getREMOVER()     << endl;

    cout << endl << endl;

    Admin *a = new Admin(3, "ZE");
    cout << a->getNome() << endl;
    cout << "Listar:        " << a->getLISTAR()      << endl;
    cout << "Adicionar:     " << a->getADICIONAR()   << endl;
    cout << "Run:           " << a->getRUN()         << endl;
    cout << "Manutencao:    " << a->getMANUTENCAO()  << endl;
    cout << "Remover:       " << a->getREMOVER()     << endl;

    cout << endl << endl;
*/

/*
    cout << "---------- TESTES DE HERANCA ENTRE MAQUINAS ----------" << endl;

    SensorFumo *s = new SensorFumo(2, "xansung", 50, 10, 20, 2);

    cout << "ID :           " << s->getID() << endl;
    cout << "Tipo:          " << s->GetTIPO() << endl;
    cout << "Marca :        " << s->getMARCA() << endl;
    cout << "Prob Avaria :  " << s->getPROBABILIDADE_AVARIA() << endl;
    cout << "Posicao :      (" << s->getX() << "," << s->getY() << ")" << endl;
    cout << "Valor :        " << s->getVALOR() << endl;

    MotorCombustao *m = new MotorCombustao(3, "aipole", 70, 20, 30, 12, 33, 35);
    cout << "ID :           " << m->getID() << endl;
    cout << "Tipo :         " << m->GetTIPO() << endl;
    cout << "Marca :        " << m->getMARCA() << endl;
    cout << "Prob Avaria :  " << m->getPROBABILIDADE_AVARIA() << endl;
    cout << "Posicao :      (" << m->getX() << "," << m->getY() << ")" << endl;
    cout << "Consumo Hora : " << m->getCONSUMO_HORA() << endl;
    cout << "Temp Aviso :   " << m->getTEMPERATURA_AVISO() << endl;
    cout << "Temp Paragem : " << m->getTEMPERATURA_PARAGEM() << endl;
*/

/*
    cout << "---------- TESTES DE ADDS ----------" << endl;
    Admin *adm = new Admin(1, "Ze");
    Fabrica *f = new Fabrica(adm);

    cout << "Utilizadores" << endl;
    cout << "Esperado: 1 0 0 1" << endl;

    // adicionar normal
    Normal *n1 = new Normal(2, "Luis");
    bool decisao = f->Add(n1);
    cout << decisao << endl;

    // nao da porque o ID esta repetido
    Normal *n2 = new Normal(2, "Joao");
    decisao = f->Add(n2);
    cout << decisao << endl;
    delete(n2);

    // nao da porque o ID e igual ao utilizador atual
    Normal *n3 = new Normal(1, "Tiago");
    decisao = f->Add(n3);
    cout << decisao << endl;
    delete(n3);

    // admin adiciona admin
    Admin *a = new Admin(3, "Sofia");
    decisao = f->Add(a);
    cout << decisao << endl;


    cout << endl << "Sensores e Motores" << endl;
    cout << "Esperado: 1 0 1 0" << endl;

    // adicionar um sensor
    SensorFumo *s1 = new SensorFumo(10, "xanxung", 5, 8, 25, 0);
    decisao = f->Add(s1);
    cout << decisao << endl;

    // da erro pq ID esta repetido
    SensorLuz *s2 = new SensorLuz(10, "xanxung", 5, 8, 25, 0);
    decisao = f->Add(s2);
    cout << decisao << endl;

    // funciona porque o ID dos sensores e separado do dos motores
    Motor *m1 = new MotorCombustao(10, "Roy", 10, 50, 50, 5, 81, 101);
    decisao = f->Add(m1);
    cout << decisao << endl;

    // da erro pq ID esta repetido (na lista de motores)
    MotorEletrico *m2 = new MotorEletrico(10, "EFACEC", 0.01, 10, 45, 1250, 81, 101);
    decisao = f->Add(m2);
    cout << decisao << endl;


    // TESTAR LISTAGEM
        cout << endl << endl;
        f->Listar();      // apenas escreve no ecra

        ofstream fich("listagem.txt");
        f->Listar(fich);    // apenas escreve no ficheiro
        fich.close();

    cout << endl << endl;
    cout << "---------- TESTES RELOGIO DE SIMULADOR ----------" << endl;
    Relogio r(8, 0);
    r.imprimir();
    cout << endl;

    cout << r.avancar(10) << " "; r.imprimir(); cout << endl;
    cout << r.avancar(50) << " "; r.imprimir(); cout << endl;
    cout << r.avancar(24 * 60) << " "; r.imprimir(); cout << endl;

    Relogio n(23, 50);
    n.imprimir();
    cout << endl;

    cout << Relogio(7, 59).dentroHorario(8, 16) << " ";
    cout << Relogio(8, 0).dentroHorario(8, 16) << " ";
    cout << Relogio(15, 59).dentroHorario(8, 16) << " ";
    cout << Relogio(16, 0).dentroHorario(8, 16) << " ";
*/


    // ficheiro de configuracao: por defeito o que esta na pasta do projeto (ou o 1o argumento)
    string ficheiroXML = "FicheiroXML.xml";
    if(argc > 1)    ficheiroXML = argv[1];

    cout << "---------- TESTE DO LOAD (XML) ----------" << endl;
    Admin *adm = new Admin(1, "Carlos");
    Fabrica *f = new Fabrica(adm);

    bool ok = f->Load(ficheiroXML);
    cout << "Load('" << ficheiroXML << "'): " << (ok ? "OK" : "FALHOU") << endl;

    // o que o Load ignorou ou corrigiu
    const list<string> &avisos = f->getAvisosLoad();
    list<string>::const_iterator itA;
    for(itA = avisos.begin(); itA != avisos.end(); itA++)
        cout << "  AVISO: " << *itA << endl;

    if(ok)
    {
        cout << endl << "---------- LISTAR ----------" << endl;
        f->Listar();

        cout << endl << "---------- LISTAR TIPO ----------" << endl;
        list<Motor *> *lm = f->Listar_Tipo("MOTOR-COMBUSTAO");
        cout << "Devolveu " << lm->size() << " motores" << endl;
        delete lm;      // apaga so a lista; os motores continuam a ser da fabrica

        cout << endl;
        delete f->Listar_Tipo("eletrico");

        // listagem para ficheiro
        ofstream fich("listagem.txt");
        f->Listar(fich);
        fich.close();
    }

    delete f;       // apaga utilizadores, motores e sensores da fabrica
    delete adm;     // o utilizador atual e nosso

/*  ----- testes anteriores (SHOW, RUN, START...) -----
    cout << "---------- TESTES DE METODOS ----------" << endl;
    // SHOW, RUN, START...

    MotorCombustao *m = new MotorCombustao(3, "aipole", 10, 20, 30, 22, 45, 60);
    m->SHOW();  cout << endl;
    m->START();
    m->RUN();
    m->RUN();
    m->SHOW();  cout << endl;

    Admin *adm = new Admin(1, "Carlos");
    Fabrica *f = new Fabrica(adm);
    f->Add(new MotorEletrico(4, "xanguexungue", 1, 12, 9, 10, 60, 80));
    f->Add(new SensorHumidade(2, "agape", 35, 12, 22, 10));
    f->Add(m);
    f->Listar();
    delete f;
    delete adm;




*/

    return 0;
}
