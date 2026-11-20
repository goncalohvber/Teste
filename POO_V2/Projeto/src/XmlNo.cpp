#include "XmlNo.h"

#include <fstream>
#include <sstream>

// limite de niveis de tags dentro de tags (evita estoirar a pilha com um ficheiro estranho)
static const int MAX_PROFUNDIDADE = 64;

XmlNo::XmlNo(const string &_nome)
{
    nome = _nome;
}

// o no e dono dos filhos: apaga-os (e eles apagam os seus filhos, etc.)
XmlNo::~XmlNo()
{
    list<XmlNo *>::iterator it;
    for(it = filhos.begin(); it != filhos.end(); it++)
        delete *it;
    filhos.clear();
}

// --------------------- CONSULTA ---------------------

XmlNo *XmlNo::Filho(const string &_nome) const
{
    list<XmlNo *>::const_iterator it;
    for(it = filhos.begin(); it != filhos.end(); it++)
    {
        if((*it)->nome == _nome)
            return *it;
    }
    return NULL;
}

string XmlNo::TextoDe(const string &_nome, const string &defeito) const
{
    XmlNo *f = Filho(_nome);
    if(f == NULL)   return defeito;
    return f->texto;
}

// --------------------- PARSER ---------------------

void XmlNo::SaltarBrancos(const string &s, string::size_type &pos)
{
    while(pos < s.size() && (s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\r' || s[pos] == '\n'))
        pos++;
}

// salta o que nao e elemento: <?xml ... ?>, <!-- comentario --> e <!DOCTYPE ...>
// devolve false se uma destas marcas nao fechar
bool XmlNo::SaltarMarcas(const string &s, string::size_type &pos)
{
    for(;;)
    {
        SaltarBrancos(s, pos);

        string::size_type fim;
        if(s.compare(pos, 2, "<?") == 0)
        {
            fim = s.find("?>", pos);
            if(fim == string::npos)     return false;
            pos = fim + 2;
        }
        else if(s.compare(pos, 4, "<!--") == 0)
        {
            fim = s.find("-->", pos);
            if(fim == string::npos)     return false;
            pos = fim + 3;
        }
        else if(s.compare(pos, 9, "<!DOCTYPE") == 0)
        {
            fim = s.find('>', pos);
            if(fim == string::npos)     return false;
            pos = fim + 1;
        }
        else
            return true;
    }
}

// troca &lt; &gt; &amp; &quot; &apos; pelos carateres verdadeiros
string XmlNo::DescodificarEntidades(const string &s)
{
    static const char *entidades[5] = { "&lt;", "&gt;", "&amp;", "&quot;", "&apos;" };
    static const char  simbolos[5]  = { '<',    '>',    '&',     '"',      '\'' };

    string r;
    string::size_type i = 0;
    while(i < s.size())
    {
        bool trocou = false;
        if(s[i] == '&')
        {
            for(int e = 0; e < 5 && !trocou; e++)
            {
                string ent = entidades[e];
                if(s.compare(i, ent.size(), ent) == 0)
                {
                    r += simbolos[e];
                    i += ent.size();
                    trocou = true;
                }
            }
        }
        if(!trocou)
        {
            r += s[i];
            i++;
        }
    }
    return r;
}

// le um elemento completo, desde o "<nome>" ate ao "</nome>".
// pos deve estar em cima do '<'. No fim fica logo a seguir ao "</nome>".
// se algo estiver mal devolve NULL (e nao fica memoria perdida)
XmlNo *XmlNo::LerElemento(const string &s, string::size_type &pos, int profundidade)
{
    if(profundidade > MAX_PROFUNDIDADE)     return NULL;
    if(pos >= s.size() || s[pos] != '<')    return NULL;
    pos++;

    // --- nome da tag
    string::size_type ini = pos;
    while(pos < s.size() && s[pos] != ' ' && s[pos] != '\t' && s[pos] != '\r' && s[pos] != '\n' && s[pos] != '>' && s[pos] != '/')
        pos++;
    if(pos >= s.size() || pos == ini)   return NULL;

    XmlNo *no = new XmlNo(s.substr(ini, pos - ini));

    // --- atributos: nao sao usados, so se avanca ate ao fim da tag (respeitando aspas)
    char aspas = 0;
    while(pos < s.size() && (aspas != 0 || (s[pos] != '>' && s[pos] != '/')))
    {
        if(aspas == 0 && (s[pos] == '"' || s[pos] == '\''))     aspas = s[pos];
        else if(aspas != 0 && s[pos] == aspas)                  aspas = 0;
        pos++;
    }
    if(pos >= s.size())     { delete no; return NULL; }

    // tag vazia: <nome/>
    if(s[pos] == '/')
    {
        if(s.compare(pos, 2, "/>") != 0)    { delete no; return NULL; }
        pos += 2;
        return no;
    }
    pos++;      // passa o '>'

    // --- conteudo: texto, comentarios, CDATA e elementos filhos
    string texto;
    for(;;)
    {
        // texto normal ate ao proximo '<'
        string::size_type prox = s.find('<', pos);
        if(prox == string::npos)    { delete no; return NULL; }     // faltou fechar a tag
        texto += DescodificarEntidades(s.substr(pos, prox - pos));
        pos = prox;

        if(s.compare(pos, 2, "</") == 0)                // fecho: </nome>
        {
            string::size_type fim = s.find('>', pos);
            if(fim == string::npos)     { delete no; return NULL; }

            string nomeFecho = s.substr(pos + 2, fim - pos - 2);
            // alguns ficheiros tem espacos antes do '>' (ex: </NOME >) e isso e valido
            string::size_type ultimo = nomeFecho.find_last_not_of(" \t\r\n");
            nomeFecho = (ultimo == string::npos) ? "" : nomeFecho.substr(0, ultimo + 1);

            if(nomeFecho != no->nome)   { delete no; return NULL; }    // tags mal emparelhadas

            pos = fim + 1;
            break;
        }
        else if(s.compare(pos, 4, "<!--") == 0)         // comentario
        {
            string::size_type fim = s.find("-->", pos);
            if(fim == string::npos)     { delete no; return NULL; }
            pos = fim + 3;
        }
        else if(s.compare(pos, 9, "<![CDATA[") == 0)    // texto sem interpretacao
        {
            string::size_type fim = s.find("]]>", pos);
            if(fim == string::npos)     { delete no; return NULL; }
            texto += s.substr(pos + 9, fim - pos - 9);
            pos = fim + 3;
        }
        else if(s.compare(pos, 2, "<?") == 0)           // instrucao no meio do ficheiro
        {
            string::size_type fim = s.find("?>", pos);
            if(fim == string::npos)     { delete no; return NULL; }
            pos = fim + 2;
        }
        else                                            // novo elemento filho
        {
            XmlNo *filho = LerElemento(s, pos, profundidade + 1);
            if(filho == NULL)   { delete no; return NULL; }
            no->filhos.push_back(filho);
        }
    }

    // espacos e mudancas de linha entre as tags nao fazem parte do valor
    string::size_type a = texto.find_first_not_of(" \t\r\n");
    if(a == string::npos)
        no->texto = "";
    else
        no->texto = texto.substr(a, texto.find_last_not_of(" \t\r\n") - a + 1);

    return no;
}

XmlNo *XmlNo::LerTexto(const string &conteudo)
{
    string::size_type pos = 0;

    // o ficheiro pode comecar com o BOM do UTF-8 (3 bytes invisiveis)
    if(conteudo.compare(0, 3, "\xEF\xBB\xBF") == 0)
        pos = 3;

    if(!SaltarMarcas(conteudo, pos))    return NULL;

    XmlNo *raiz = LerElemento(conteudo, pos, 0);
    if(raiz == NULL)    return NULL;

    // depois da raiz so pode haver brancos e comentarios
    if(!SaltarMarcas(conteudo, pos) || pos != conteudo.size())
    {
        delete raiz;
        return NULL;
    }
    return raiz;
}

XmlNo *XmlNo::LerFicheiro(const string &ficheiro)
{
    ifstream f(ficheiro.c_str(), ios::in | ios::binary);
    if(!f.is_open())    return NULL;

    stringstream buffer;
    buffer << f.rdbuf();
    return LerTexto(buffer.str());
}
