#include "Fabrica.h"

#include <sstream>

#include "MotorCombustao.h"
#include "MotorEletrico.h"
#include "MotorInducao.h"
#include "SensorFumo.h"
#include "SensorHumidade.h"
#include "SensorLuz.h"
#include "SensorMissel.h"
#include "Uteis.h"

// --------------------- AUXILIARES ---------------------
// funcao que procura se ja existe algum id numa lista (de utilizadores, motores ou sensores)
template <typename T>
T* Fabrica::procurarID (int _id, list<T *> &lista)
{
    typename list<T *>::iterator it;

    for(it = lista.begin(); it != lista.end(); it++)
    {
        if((*it)->getID() == _id)
            return *it;
    }
    return NULL;
}

// apaga os objetos apontados pela lista e esvazia-a
template <typename T>
void Fabrica::libertar(list<T *> &lista)
{
    typename list<T *>::iterator it;

    for(it = lista.begin(); it != lista.end(); it++)
        delete *it;
    lista.clear();
}

// --------------------- CONSTRUTORES / DESTRUTOR ---------------------

// poe todos os atributos num estado conhecido (usado pelos dois construtores)
void Fabrica::iniciar()
{
    utilizadorAtual = NULL;
    nomeEmpresa = "";
    horaAbertura = 8;
    horaEncerramento = 18;
    vizinhanca = 0;
    dimensaoX = 0;
    dimensaoY = 0;
}

Fabrica::Fabrica()
{
    iniciar();
}

Fabrica::Fabrica(Utilizador *ut)
{
    iniciar();
    utilizadorAtual = ut;
}

Fabrica::~Fabrica()
{
    // a fabrica e dona de tudo o que esta nas listas
    // (o utilizador atual nao: foi criado por quem chamou o construtor)
    libertar(Lutilizadores);
    libertar(Lmotores);
    libertar(Lsensor);
}

// --------------------- ADICIONAR ---------------------
// funcoes para adicionar utilizadores, motores e sensores
// pega no objeto e acrescenta a respetiva lista
// devolve false se nao for permitido ou se o ID ja existir
// se devolver falso ainda tem se que apagar o objeto

bool Fabrica::Add(Utilizador *ut)
{
    if(ut == NULL || utilizadorAtual == NULL)   return false;

    // so quem tem permissao de ADD pode adicionar
    if(!utilizadorAtual->getADICIONAR())    return false;

    // apenas um admin pode adicionar outro admin
    if(ut->getREMOVER() && !utilizadorAtual->getREMOVER())  return false;

    // ID repetido
    if(ut->getID() == utilizadorAtual->getID() || procurarID(ut->getID(), Lutilizadores))   return false;

    // se verifica todas estas condicoes entao e adicionado
    Lutilizadores.push_back(ut);
    return true;
}

bool Fabrica::Add(Motor *m)
{
    if(m == NULL || utilizadorAtual == NULL)    return false;

    // so quem tem permissao de ADD pode adicionar
    if(!utilizadorAtual->getADICIONAR())    return false;

    // se o ID ja existir
    if(procurarID(m->getID(), Lmotores) != NULL)    return false;

    Lmotores.push_back(m);
    return true;
}

bool Fabrica::Add(Sensor *s)
{
    if(s == NULL || utilizadorAtual == NULL)    return false;

    // so quem tem permissao de ADD pode adicionar
    if(!utilizadorAtual->getADICIONAR())    return false;

    // se o ID ja existir
    if(procurarID(s->getID(), Lsensor) != NULL)    return false;

    Lsensor.push_back(s);
    return true;
}

// --------------------- LISTAR ---------------------
// por defeito listam no ecra porem tambem podem listar num ficheiro caso este seja passado
// so quem tem permissao de LISTAR pode listar (todos os tipos de utilizador tem)

// lista so os motores
void Fabrica::ListarMotores(ostream &f)
{
    if(utilizadorAtual == NULL || !utilizadorAtual->getLISTAR())
    {
        f << "Sem permissao para listar" << endl;
        return;
    }

    f << "--- MOTORES (" << Lmotores.size() << ") ---" << endl;
    list<Motor *>::iterator it;
    for(it = Lmotores.begin(); it != Lmotores.end(); it++)
    {
        (*it)->SHOW(f);
        f << endl;
    }
}

// lista so os sensores
void Fabrica::ListarSensores(ostream &f)
{
    if(utilizadorAtual == NULL || !utilizadorAtual->getLISTAR())
    {
        f << "Sem permissao para listar" << endl;
        return;
    }

    f << "--- SENSORES (" << Lsensor.size() << ") ---" << endl;
    list<Sensor *>::iterator it;
    for(it = Lsensor.begin(); it != Lsensor.end(); it++)
    {
        (*it)->SHOW(f);
        f << endl;
    }
}

// lista o estado atual da fabrica: utilizadores, motores e sensores
void Fabrica::Listar(ostream &f)
{
    if(utilizadorAtual == NULL || !utilizadorAtual->getLISTAR())
    {
        f << "Sem permissao para listar" << endl;
        return;
    }

    f << "UTILIZADOR ATUAL: " << utilizadorAtual->getNome() << endl;
    f << "FABRICA: " << nomeEmpresa << " (aberta das " << horaAbertura << "h as " << horaEncerramento << "h)" << endl;

    // utilizadores
    f << endl << "--- UTILIZADORES (" << Lutilizadores.size() << ") ---" << endl;
    list<Utilizador *>::iterator itU;
    for(itU = Lutilizadores.begin(); itU != Lutilizadores.end(); itU++)
    {
        f << (*itU)->getID() << " - " << (*itU)->getNome() << endl;
    }

    // motores e sensores
    f << endl;
    ListarMotores(f);
    f << endl;
    ListarSensores(f);
}

// lista e devolve os motores de um dado tipo
// aceita o nome completo ("MOTOR-COMBUSTAO") ou so o fim ("combustao")
// a lista devolvida e criada aqui: quem chama tem de fazer delete da lista (os motores nao!)
list<Motor *> *Fabrica::Listar_Tipo(string Tipo, ostream &f)
{
    list<Motor *> *resultado = new list<Motor *>();

    if(utilizadorAtual == NULL || !utilizadorAtual->getLISTAR())
    {
        f << "Sem permissao para listar" << endl;
        return resultado;
    }

    string tipo = Uteis::Maiusculas(Uteis::Trim(Tipo));

    f << "--- MOTORES DO TIPO " << tipo << " ---" << endl;
    list<Motor *>::iterator it;
    for(it = Lmotores.begin(); it != Lmotores.end(); it++)
    {
        string tipoMotor = (*it)->GetTIPO();
        if(tipoMotor == tipo || tipoMotor == "MOTOR-" + tipo)
        {
            (*it)->SHOW(f);
            f << endl;
            resultado->push_back(*it);
        }
    }

    if(resultado->empty())
        f << "(nenhum motor deste tipo)" << endl;

    return resultado;
}

// --------------------- LOAD (XML) ---------------------
// Estrutura do ficheiro:
//   <DADOS>
//      <DEFINICOES> ... </DEFINICOES>      horario, vizinhanca, dimensao, limites de temperatura
//      <MOTORES>    ... </MOTORES>         <MCOMBUSTAO>, <MELETRICO>, <MINDUCAO>
//      <SENSORES>   ... </SENSORES>        <SHUMIDADE>, <SLUZ>, <SFUMO>, <SMISSIL>
//   </DADOS>
//
// O Load e feito "tudo ou nada": primeiro le-se o ficheiro todo para objetos temporarios e so
// no fim e que a fabrica e alterada. O Load e a configuracao inicial da fabrica e nao uma
// acao do utilizador, por isso nao pede permissao de ADD (um visitante tambem tem de ver a fabrica).

void Fabrica::avisarLoad(const string &mensagem)
{
    avisosLoad.push_back(mensagem);
}

// converte o ID de um objeto para texto (para as mensagens)
static string identificar(XmlNo *no)
{
    return "<" + no->getNome() + "> com ID '" + no->TextoDe("ID") + "'";
}

// le um valor inteiro de uma tag filha. devolve false se faltar ou nao for um inteiro
static bool lerInteiro(XmlNo *pai, const string &tag, int &valor)
{
    return Uteis::ParaInt(pai->TextoDe(tag), valor);
}

bool Fabrica::Load(const string &ficheiro)
{
    avisosLoad.clear();

    // 1. ler o XML para uma arvore em memoria
    XmlNo *raiz = XmlNo::LerFicheiro(ficheiro);
    if(raiz == NULL)
    {
        avisarLoad("Nao foi possivel ler '" + ficheiro + "' (nao existe ou o XML esta mal formado)");
        return false;
    }
    if(raiz->getNome() != "DADOS")
    {
        avisarLoad("O elemento principal do XML devia ser <DADOS> e e <" + raiz->getNome() + ">");
        delete raiz;
        return false;
    }

    // 2. definicoes (sao obrigatorias: os motores dependem dos limites de temperatura)
    Definicoes def;
    XmlNo *noDef = raiz->Filho("DEFINICOES");
    if(noDef == NULL)
    {
        avisarLoad("Falta a tag <DEFINICOES>");
        delete raiz;
        return false;
    }
    if(!lerDefinicoes(noDef, def))
    {
        delete raiz;
        return false;
    }

    // 3. motores e sensores para listas temporarias
    list<Motor *> novosMotores;
    list<Sensor *> novosSensores;
    try
    {
        lerMotores(raiz->Filho("MOTORES"), def, novosMotores);
        lerSensores(raiz->Filho("SENSORES"), novosSensores);
    }
    catch(...)
    {
        // por exemplo falta de memoria: nao deixar nada perdido
        libertar(novosMotores);
        libertar(novosSensores);
        delete raiz;
        throw;
    }

    // 4. correu tudo bem: a fabrica passa a ter a nova configuracao
    nomeEmpresa = def.nomeEmpresa;
    horaAbertura = def.horaAbertura;
    horaEncerramento = def.horaEncerramento;
    vizinhanca = def.vizinhanca;
    dimensaoX = def.dimensaoX;
    dimensaoY = def.dimensaoY;

    Lmotores.splice(Lmotores.end(), novosMotores);          // passa os motores para a fabrica
    Lsensor.splice(Lsensor.end(), novosSensores);

    delete raiz;
    return true;
}

// le <DEFINICOES>. Devolve false se faltar algo essencial
bool Fabrica::lerDefinicoes(XmlNo *no, Definicoes &d)
{
    d.nomeEmpresa = no->TextoDe("NOME_EMPRESA");

    // horario
    if(!lerInteiro(no, "HORA_INICIO", d.horaAbertura) || !lerInteiro(no, "HORA_FECHO", d.horaEncerramento))
    {
        avisarLoad("Faltam (ou sao invalidas) as tags HORA_INICIO / HORA_FECHO");
        return false;
    }
    if(d.horaAbertura < 0 || d.horaEncerramento > 24 || d.horaAbertura >= d.horaEncerramento)
    {
        avisarLoad("Horario invalido: a hora de abertura tem de ser menor que a de fecho (0 a 24)");
        return false;
    }

    // vizinhanca (distancia de aviso de humidade)
    if(!lerInteiro(no, "VIZINHANCA_AVISO", d.vizinhanca) || d.vizinhanca < 0)
    {
        avisarLoad("VIZINHANCA_AVISO em falta ou invalida: assumido 0");
        d.vizinhanca = 0;
    }

    // dimensao "largura,altura"
    float dx, dy;
    if(Uteis::ParaPar(no->TextoDe("DIMENSAO_FABRICA"), dx, dy) && dx > 0 && dy > 0)
    {
        d.dimensaoX = (int)dx;
        d.dimensaoY = (int)dy;
    }
    else
    {
        avisarLoad("DIMENSAO_FABRICA em falta ou invalida: assumido 0,0");
        d.dimensaoX = 0;
        d.dimensaoY = 0;
    }

    // limites de temperatura de cada tipo de motor
    const char *tipos[3] = { "MELETRICO", "MCOMBUSTAO", "MINDUCAO" };
    for(int i = 0; i < 3; i++)
    {
        XmlNo *noTipo = no->Filho(tipos[i]);
        if(noTipo == NULL)
        {
            avisarLoad(string("Nas DEFINICOES nao existe <") + tipos[i] + ">: motores deste tipo serao ignorados");
            continue;
        }

        ConfigMotor c;
        if(lerConfigMotor(noTipo, c))
            d.motores[tipos[i]] = c;
        else
            avisarLoad(string("Limites de <") + tipos[i] + "> invalidos: motores deste tipo serao ignorados");
    }

    return true;
}

// le os limites VERDE/AMARELO/VERMELHO e a probabilidade de avaria de um tipo de motor
bool Fabrica::lerConfigMotor(XmlNo *no, ConfigMotor &c)
{
    if(!Uteis::ParaPar(no->TextoDe("VERDE"), c.verdeMin, c.verdeMax))           return false;
    if(!Uteis::ParaPar(no->TextoDe("AMARELO"), c.amareloMin, c.amareloMax))     return false;
    if(!Uteis::ParaPar(no->TextoDe("VERMELHO"), c.vermelhoMin, c.vermelhoMax))  return false;
    if(!Uteis::ParaFloat(no->TextoDe("PROB_AVARIA"), c.probAvaria))             return false;

    // cada intervalo tem de estar bem ordenado e as cores uma a seguir a outra
    if(c.verdeMin > c.verdeMax || c.amareloMin > c.amareloMax || c.vermelhoMin > c.vermelhoMax)     return false;
    if(!(c.verdeMin < c.amareloMin && c.amareloMin < c.vermelhoMin))                                return false;
    if(c.probAvaria < 0 || c.probAvaria > 100)                                                      return false;

    return true;
}

// percorre <MOTORES> e cria um motor por cada filho valido
void Fabrica::lerMotores(XmlNo *no, const Definicoes &d, list<Motor *> &destino)
{
    if(no == NULL)
    {
        avisarLoad("O XML nao tem a tag <MOTORES>");
        return;
    }

    list<XmlNo *>::const_iterator it;
    for(it = no->getFilhos().begin(); it != no->getFilhos().end(); it++)
    {
        Motor *m = criarMotor(*it, d);
        if(m == NULL)   continue;       // a razao ja ficou nos avisos

        // ID repetido (na fabrica ou no proprio ficheiro)
        if(procurarID(m->getID(), Lmotores) != NULL || procurarID(m->getID(), destino) != NULL)
        {
            avisarLoad("Motor " + identificar(*it) + " ignorado: ID repetido");
            delete m;
            continue;
        }
        destino.push_back(m);
    }
}

// percorre <SENSORES> e cria um sensor por cada filho valido
void Fabrica::lerSensores(XmlNo *no, list<Sensor *> &destino)
{
    if(no == NULL)
    {
        avisarLoad("O XML nao tem a tag <SENSORES>");
        return;
    }

    list<XmlNo *>::const_iterator it;
    for(it = no->getFilhos().begin(); it != no->getFilhos().end(); it++)
    {
        Sensor *s = criarSensor(*it);
        if(s == NULL)   continue;

        if(procurarID(s->getID(), Lsensor) != NULL || procurarID(s->getID(), destino) != NULL)
        {
            avisarLoad("Sensor " + identificar(*it) + " ignorado: ID repetido");
            delete s;
            continue;
        }
        destino.push_back(s);
    }
}

// cria UM motor a partir de uma tag <MCOMBUSTAO>, <MELETRICO> ou <MINDUCAO>
// devolve NULL (e deixa um aviso) se o tipo for desconhecido ou faltarem dados
Motor *Fabrica::criarMotor(XmlNo *no, const Definicoes &d)
{
    const string tag = no->getNome();

    if(tag != "MELETRICO" && tag != "MCOMBUSTAO" && tag != "MINDUCAO")
    {
        avisarLoad("Motor " + identificar(no) + " ignorado: tipo de motor desconhecido");
        return NULL;
    }

    // limites de temperatura deste tipo (vem das DEFINICOES)
    map<string, ConfigMotor>::const_iterator itCfg = d.motores.find(tag);
    if(itCfg == d.motores.end())
    {
        avisarLoad("Motor " + identificar(no) + " ignorado: sem limites de temperatura nas DEFINICOES");
        return NULL;
    }
    const ConfigMotor &cfg = itCfg->second;

    // campos obrigatorios
    int id;
    float x, y, consumo;
    string marca = no->TextoDe("MARCA");

    if(!lerInteiro(no, "ID", id) || id < 0 || marca.empty()
       || !Uteis::ParaPar(no->TextoDe("POSICAO"), x, y)
       || !Uteis::ParaFloat(no->TextoDe("CONSUMO_HORA"), consumo) || consumo < 0)
    {
        avisarLoad("Motor " + identificar(no) + " ignorado: faltam dados ou sao invalidos (ID, MARCA, POSICAO, CONSUMO_HORA)");
        return NULL;
    }

    // temperatura atual (se nao existir comeca a 0) e estado (se nao existir comeca em STOP)
    float temperatura = 0;
    if(no->Filho("VALOR_TEMP_ATUAL") != NULL && !Uteis::ParaFloat(no->TextoDe("VALOR_TEMP_ATUAL"), temperatura))
    {
        avisarLoad("Motor " + identificar(no) + " ignorado: VALOR_TEMP_ATUAL invalido");
        return NULL;
    }

    ESTADO_MOTOR estado = E_STOP;
    if(no->Filho("ESTADO_ATUAL") != NULL)
    {
        string nomeEstado = Uteis::Maiusculas(no->TextoDe("ESTADO_ATUAL"));
        bool encontrado = false;
        for(int i = 0; i < 4 && !encontrado; i++)
        {
            if(nomeEstado == NOMES_ESTADO[i])
            {
                estado = (ESTADO_MOTOR)i;
                encontrado = true;
            }
        }
        if(!encontrado)
        {
            avisarLoad("Motor " + identificar(no) + " ignorado: ESTADO_ATUAL invalido ('" + no->TextoDe("ESTADO_ATUAL") + "')");
            return NULL;
        }
    }

    // o limite de AVISO e o inicio do AMARELO e o de PARAGEM e o inicio do VERMELHO
    Motor *m;
    if(tag == "MELETRICO")
        m = new MotorEletrico(id, marca, cfg.probAvaria, (int)x, (int)y, consumo, cfg.amareloMin, cfg.vermelhoMin);
    else if(tag == "MCOMBUSTAO")
        m = new MotorCombustao(id, marca, cfg.probAvaria, (int)x, (int)y, consumo, cfg.amareloMin, cfg.vermelhoMin);
    else
        m = new MotorInducao(id, marca, cfg.probAvaria, (int)x, (int)y, consumo, cfg.amareloMin, cfg.vermelhoMin);

    m->setTEMPERATURA_ATUAL(temperatura);
    m->setESTADO(estado);

    // a cor que vem no ficheiro pode nao bater certo com os limites: manda a temperatura
    string corFicheiro = Uteis::Maiusculas(no->TextoDe("ESTADO_COR"));
    string corCalculada = NOMES_COR[m->getCLASSE_TEMPERATURA()];
    if(!corFicheiro.empty() && corFicheiro != corCalculada)
    {
        ostringstream msg;
        msg << "Motor " << identificar(no) << ": ESTADO_COR '" << corFicheiro << "' nao corresponde a temperatura "
            << temperatura << " (" << corCalculada << "). Usada a cor calculada pela temperatura";
        avisarLoad(msg.str());
    }

    // um motor em VERMELHO nao pode estar a trabalhar: protege-se (STOP), como manda o enunciado
    ESTADO_MOTOR antes = m->getESTADO();
    m->ESTOU_QUENTE();
    if(m->getESTADO() != antes)
        avisarLoad("Motor " + identificar(no) + " estava em VERMELHO: colocado em STOP");

    return m;
}

// cria UM sensor a partir de uma tag <SHUMIDADE>, <SLUZ>, <SFUMO> ou <SMISSIL>
// devolve NULL (e deixa um aviso) se o tipo for desconhecido ou faltarem dados
Sensor *Fabrica::criarSensor(XmlNo *no)
{
    const string tag = no->getNome();

    if(tag != "SHUMIDADE" && tag != "SLUZ" && tag != "SFUMO" && tag != "SMISSIL")
    {
        avisarLoad("Sensor " + identificar(no) + " ignorado: tipo de sensor desconhecido (so existem LUZ, HUMIDADE, FUMO e MISSIL)");
        return NULL;
    }

    int id;
    float x, y, probAvaria, valor, valorAviso;
    string marca = no->TextoDe("MARCA");

    if(!lerInteiro(no, "ID", id) || id < 0 || marca.empty()
       || !Uteis::ParaPar(no->TextoDe("POSICAO"), x, y)
       || !Uteis::ParaFloat(no->TextoDe("PROB_AVARIA"), probAvaria) || probAvaria < 0 || probAvaria > 100
       || !Uteis::ParaFloat(no->TextoDe("VALOR_ATUAL"), valor)          // aceita "85%"
       || !Uteis::ParaFloat(no->TextoDe("VALOR_AVISO"), valorAviso))
    {
        avisarLoad("Sensor " + identificar(no) + " ignorado: faltam dados ou sao invalidos (ID, MARCA, POSICAO, PROB_AVARIA, VALOR_ATUAL, VALOR_AVISO)");
        return NULL;
    }

    if(tag == "SHUMIDADE")      return new SensorHumidade(id, marca, probAvaria, (int)x, (int)y, valor, valorAviso);
    if(tag == "SLUZ")           return new SensorLuz(id, marca, probAvaria, (int)x, (int)y, valor, valorAviso);
    if(tag == "SFUMO")          return new SensorFumo(id, marca, probAvaria, (int)x, (int)y, valor, valorAviso);
    return new SensorMissel(id, marca, probAvaria, (int)x, (int)y, valor, valorAviso);
}
