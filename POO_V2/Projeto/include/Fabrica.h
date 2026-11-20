#ifndef FABRICA_H
#define FABRICA_H

#include <iostream>
#include <string>
#include <list>
#include <map>

#include "Utilizador.h"
#include "Motor.h"
#include "Sensor.h"
#include "XmlNo.h"

using namespace std;

/// Fabrica: guarda e gere os utilizadores, os motores e os sensores.
///
/// Gestao de memoria: a Fabrica e DONA dos utilizadores, motores e sensores que estao
/// nas suas listas (apaga-os no destrutor). Nao e dona do utilizador atual (o que e
/// passado ao construtor): esse continua a ser de quem o criou.
class Fabrica
{
    /// limites de temperatura de um tipo de motor, tal como vem na tag <DEFINICOES> do XML
    struct ConfigMotor
    {
        float verdeMin, verdeMax;
        float amareloMin, amareloMax;
        float vermelhoMin, vermelhoMax;
        float probAvaria;
    };

    /// tudo o que se le da tag <DEFINICOES> (guardado aqui ate o Load acabar sem erros)
    struct Definicoes
    {
        string nomeEmpresa;
        int horaAbertura;
        int horaEncerramento;
        int vizinhanca;
        int dimensaoX;
        int dimensaoY;
        map<string, ConfigMotor> motores;       // chave = tag do tipo (MELETRICO, MCOMBUSTAO, MINDUCAO)
    };

    // utilizador que esta a usar o programa
    Utilizador *utilizadorAtual;

    // listas de objetos da fabrica
    list<Utilizador *>  Lutilizadores;
    list<Motor *>       Lmotores;
    list<Sensor *>      Lsensor;

    // configuracao da fabrica (vem do XML)
    string nomeEmpresa;
    int horaAbertura;
    int horaEncerramento;
    int vizinhanca;                 // distancia dentro da qual um aviso de humidade desliga motores
    int dimensaoX;
    int dimensaoY;

    // mensagens do ultimo Load (entradas ignoradas, valores estranhos...)
    list<string> avisosLoad;

    // uma fabrica nao se copia (as listas tem ponteiros dos quais e dona)
    Fabrica(const Fabrica &);
    Fabrica &operator=(const Fabrica &);

    // auxiliares
    void iniciar();

    // procura um objeto pelo ID numa lista (NULL se nao existir)
    template <typename T>
    T* procurarID (int _id, list<T *> &lista);

    // apaga todos os objetos de uma lista e esvazia-a
    template <typename T>
    static void libertar(list<T *> &lista);

    // auxiliares do Load
    void avisarLoad(const string &mensagem);
    bool lerDefinicoes(XmlNo *no, Definicoes &d);
    bool lerConfigMotor(XmlNo *no, ConfigMotor &c);
    void lerMotores(XmlNo *no, const Definicoes &d, list<Motor *> &destino);
    void lerSensores(XmlNo *no, list<Sensor *> &destino);
    Motor  *criarMotor(XmlNo *no, const Definicoes &d);
    Sensor *criarSensor(XmlNo *no);

    public:
        Fabrica();
        Fabrica(Utilizador *ut);
        virtual ~Fabrica();

        /// carrega a configuracao da fabrica (definicoes, motores e sensores) de um ficheiro XML.
        /// Acrescenta ao que ja existe. Devolve false (sem alterar nada) se o ficheiro nao abrir,
        /// estiver mal formado ou nao tiver as definicoes essenciais. Entradas invalidas ou de tipos
        /// desconhecidos sao ignoradas e ficam registadas em getAvisosLoad().
        bool Load(const string &ficheiro);

        /// mensagens geradas pelo ultimo Load
        const list<string> &getAvisosLoad()     { return avisosLoad; }

        string getNomeEmpresa()     { return nomeEmpresa; }
        int getHoraAbertura()       { return horaAbertura; }
        int getHoraEncerramento()   { return horaEncerramento; }
        int getVizinhanca()         { return vizinhanca; }
        int getDimensaoX()          { return dimensaoX; }
        int getDimensaoY()          { return dimensaoY; }

        /// se devolver false o objeto nao foi guardado, por isso quem chamou tem de o apagar
        bool Add(Utilizador *ut);
        bool Add(Motor *m);
        bool Add(Sensor *s);

        /// lista o estado atual da fabrica (utilizadores, motores e sensores)
        void Listar(ostream &f = std::cout);

        /// lista so os motores / so os sensores
        void ListarMotores(ostream &f = std::cout);
        void ListarSensores(ostream &f = std::cout);

        /// lista e devolve os motores de um tipo (ex: "MOTOR-COMBUSTAO" ou so "COMBUSTAO").
        /// A lista devolvida e nova (quem chama faz delete da LISTA); os motores continuam da Fabrica
        list<Motor *> *Listar_Tipo(string Tipo, ostream &f = std::cout);

        //void Desligar(int id_motor);
        //ESTADO_MOTOR Get_Estado(int id_motor);
        //bool Manutencao();
        //List<string> *Ranking_Dos_Fracos();
        //List<Motor *> *Ranking_Dos_Mais_Trabalhadores();
        //void Relatorio(string fich_xml);
        //int Aviso_Humidade(list<Motor *> &lm);
        //int Aviso_Fumo(list<Motor *> &lm, string fich_video);
        //int Aviso_Luz(string fich_video);
        //void Aviso_Missel(string fvideo, string festado = "Estado.txt");
        //void Listagem_HTML(string fich_html);
};

#endif // FABRICA_H
