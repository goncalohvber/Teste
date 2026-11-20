#ifndef MOTOR_H
#define MOTOR_H

#include "Comum.h"

// estados possiveis de um motor (prefixo E_ para nao colidir com os metodos RUN, STOP...)
enum ESTADO_MOTOR { E_STOP, E_START, E_RUN, E_AVARIADO };

// classes de temperatura de um motor (prefixo C_)
enum CLASSE_TEMPERATURA { C_VERDE, C_AMARELO, C_VERMELHO };

// nomes para imprimir, pela mesma ordem dos enums
const string NOMES_ESTADO[4] = { "STOP", "START", "RUN", "AVARIADO" };
const string NOMES_COR[3]    = { "VERDE", "AMARELO", "VERMELHO" };




// Motor: herda de Comum e acrescenta apenas o que e so dele
// o tempo e dado pela Fabrica: cada passo de simulacao chama RUN com a duracao do passo (em horas)
class Motor : public Comum
{
    float CONSUMO_HORA;
    float TEMPERATURA_AVISO;        // a partir daqui o motor esta AMARELO
    float TEMPERATURA_PARAGEM;      // a partir daqui o motor esta VERMELHO e para

    float temperaturaAtual;         // temperatura neste momento
    float horasTrabalhadas;         // total de horas em RUN
    float consumoAcumulado;         // consumo total acumulado
    float consumoDia;               // dia atual
    ESTADO_MOTOR estado;


    protected:
        // quantos graus a temperatura sobe por cada hora em RUN
        // cada tipo de motor pode reimplementar para aquecer a uma velocidade diferente
        virtual float getAQUECIMENTO();


    public:
        Motor();
        Motor(int _id, string _marca, float _probAvaria, int _x, int _y, float _consumoHora, float _tempAviso, float _tempParagem);
        virtual ~Motor();

        string GetTIPO() { return "MOTOR"; }
        float getCONSUMO_HORA()         { return CONSUMO_HORA; }
        float getTEMPERATURA_AVISO()    { return TEMPERATURA_AVISO; }
        float getTEMPERATURA_PARAGEM()  { return TEMPERATURA_PARAGEM; }


        ESTADO_MOTOR getESTADO()        { return estado; }
        float getTEMPERATURA_ATUAL()    { return temperaturaAtual; }
        float getHORAS_TRABALHADAS()    { return horasTrabalhadas; }
        float getCONSUMOACUMULADO()     { return consumoAcumulado; }
        float getCONSUMO_DIA()          { return consumoDia; }

        void setTEMPERATURA_ATUAL(float _temp)  { temperaturaAtual = _temp; }

        // usado pelo Load da Fabrica para repor o estado que vem no ficheiro
        // (as transicoes normais fazem-se com START, RUN, STOP...)
        void setESTADO(ESTADO_MOTOR _estado)    { estado = _estado; }

        // classe de temperatura (VERDE, AMARELO ou VERMELHO) a partir da temperatura atual
        CLASSE_TEMPERATURA getCLASSE_TEMPERATURA();

        // metodos do motor (cada motor pode reimplementar)
        virtual void START();
        virtual void RUN(float horas = 1);          // funciona durante X horas de simulacao
        virtual void RESTART();
        virtual void STOP();
        virtual CLASSE_TEMPERATURA ESTOU_QUENTE();  // devolve a classe de temperatura

        void SHOW(ostream &f = std::cout);
        void AVARIAR();
        void NOVO_DIA();

};

#endif // MOTOR_H
