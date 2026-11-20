#include "Motor.h"

Motor::Motor()
: Comum()
{
    CONSUMO_HORA = 0;
    TEMPERATURA_AVISO = 0;
    TEMPERATURA_PARAGEM = 0;

    estado = E_STOP;
    temperaturaAtual = 0;
    horasTrabalhadas = 0;
    consumoAcumulado = 0;
    consumoDia = 0;
}

Motor::Motor(int _id, string _marca, float _probAvaria, int _x, int _y, float _consumoHora, float _tempAviso, float _tempParagem)
: Comum(_id, _marca, _probAvaria, _x, _y)
{
    CONSUMO_HORA = _consumoHora;
    TEMPERATURA_AVISO = _tempAviso;
    TEMPERATURA_PARAGEM = _tempParagem;

    estado = E_STOP;
    temperaturaAtual = 0;
    horasTrabalhadas = 0;
    consumoAcumulado = 0;
    consumoDia = 0;
}

Motor::~Motor()
{
    //dtor
}

// aquecimento por defeito (graus por hora em RUN)
// os motores especificos podem mudar
float Motor::getAQUECIMENTO()
{
    return 2;
}

CLASSE_TEMPERATURA Motor::getCLASSE_TEMPERATURA()
{
    if(temperaturaAtual >= TEMPERATURA_PARAGEM)     return C_VERMELHO;
    if(temperaturaAtual >= TEMPERATURA_AVISO)       return C_AMARELO;
    return C_VERDE;
}

// -----------------------------------------------------------------------------------------------

// so arranca se estiver parado e nao estiver demasiado quente
void Motor::START()
{
    if(estado == E_STOP && getCLASSE_TEMPERATURA() != C_VERMELHO)
        estado = E_START;
}

// funciona durante X horas de simulacao: so acontece se o motor estiver ligado
void Motor::RUN(float horas)
{
    if(estado != E_START && estado != E_RUN)    return;

    estado = E_RUN;
    temperaturaAtual += getAQUECIMENTO() * horas;
    horasTrabalhadas += horas;
    consumoAcumulado += CONSUMO_HORA * horas;
    consumoDia += CONSUMO_HORA * horas;

    // verifica a temperatura
    // se estiver vermelho o motor para
    ESTOU_QUENTE();
}

void Motor::RESTART()
{
    STOP();
    START();
}

// um motor avariado nao sai do estado de avariado com o stop
void Motor::STOP()
{
    if(estado != E_AVARIADO)
        estado = E_STOP;
}


// -----------------------------------------------------------------------------------------------


// devolve a classe da temperatura
// em vermelho o motor desliga se
// e feito um aviso ao gestor pela fabrica
CLASSE_TEMPERATURA Motor::ESTOU_QUENTE()
{
    CLASSE_TEMPERATURA cor = getCLASSE_TEMPERATURA();

    if(cor == C_VERMELHO)
        STOP();

    return cor;
}


void Motor::SHOW(ostream &f)
{
    Comum::SHOW(f);
    f << " - " << NOMES_ESTADO[estado] << " - temp " << temperaturaAtual << " (" << NOMES_COR[getCLASSE_TEMPERATURA()] << ")" << " - horas " << horasTrabalhadas << " - consumo dia " << consumoDia << " - total " << consumoAcumulado;
}



void Motor::AVARIAR()
{
    estado = E_AVARIADO;
}

// o consumo do dia volta a zero (o acumulado mantem-se)
void Motor::NOVO_DIA()
{
    consumoDia = 0;
}



