#ifndef UTEIS_H_INCLUDED
#define UTEIS_H_INCLUDED

#include <iostream>
#include <string>
#include <cstdlib>      // rand, srand, strtod, strtol
#include <ctime>
using namespace std;

/// Funcoes utilitarias (todas estaticas): aleatorios, esperas e conversao de texto
class Uteis
{
public:
    static int Aleatorio(int min, int max)
    {
        return min + rand() % (max - min + 1);
    }

    static void wait ( int mlseconds )
    {
        clock_t endwait;
        endwait = clock () + mlseconds;
        while (clock() < endwait);
    }

    static void wait_segundos ( int seconds )
    {
        wait(seconds * CLOCKS_PER_SEC);
    }

    // ------------------ TEXTO (usado na leitura do XML) ------------------

    /// remove espacos, tabs e mudancas de linha do inicio e do fim
    static string Trim(const string &s)
    {
        const string brancos = " \t\r\n";
        string::size_type ini = s.find_first_not_of(brancos);
        if(ini == string::npos)     return "";
        string::size_type fim = s.find_last_not_of(brancos);
        return s.substr(ini, fim - ini + 1);
    }

    /// devolve o texto em maiusculas
    static string Maiusculas(const string &s)
    {
        string r = s;
        for(string::size_type i = 0; i < r.size(); i++)
        {
            if(r[i] >= 'a' && r[i] <= 'z')
                r[i] = (char)(r[i] - 'a' + 'A');
        }
        return r;
    }

    /// converte "85" ou "85%" para numero. devolve false se nao for um numero valido
    static bool ParaFloat(const string &texto, float &valor)
    {
        string s = Trim(texto);
        if(s.empty())   return false;

        char *fim;
        double d = strtod(s.c_str(), &fim);
        if(fim == s.c_str())    return false;       // nao leu nenhum numero

        if(*fim == '%')     fim++;                  // aceita o simbolo de percentagem
        if(*Trim(fim).c_str() != '\0')  return false;   // sobrou lixo a seguir ao numero

        valor = (float)d;
        return true;
    }

    /// converte um inteiro (ex: "100"). devolve false se nao for valido
    static bool ParaInt(const string &texto, int &valor)
    {
        string s = Trim(texto);
        if(s.empty())   return false;

        char *fim;
        long l = strtol(s.c_str(), &fim, 10);
        if(fim == s.c_str() || *fim != '\0')    return false;

        valor = (int)l;
        return true;
    }

    /// converte um par "a,b" (ex: "50,50" ou "0, 80"). devolve false se nao for valido
    static bool ParaPar(const string &texto, float &a, float &b)
    {
        string::size_type virgula = texto.find(',');
        if(virgula == string::npos)     return false;

        return ParaFloat(texto.substr(0, virgula), a) && ParaFloat(texto.substr(virgula + 1), b);
    }
};

#endif // UTEIS_H_INCLUDED
