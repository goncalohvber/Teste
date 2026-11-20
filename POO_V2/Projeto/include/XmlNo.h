#ifndef XMLNO_H
#define XMLNO_H

#include <string>
#include <list>

using namespace std;

/// Um no (elemento) de um ficheiro XML.
///
/// Serve para ler o ficheiro de configuracao da fabrica sem usar bibliotecas externas.
/// Cada no guarda o nome da tag, o texto que tem la dentro e os nos filhos.
/// O no e dono dos filhos: ao apagar a raiz, a arvore toda e libertada.
///
/// Exemplo:  <MOTOR><ID>10</ID></MOTOR>
///   no "MOTOR" -> filho "ID" com texto "10"
///
/// Suporta: prologo <?xml ...?>, comentarios, CDATA, atributos (sao ignorados),
/// tags vazias (<a/>) e as entidades &lt; &gt; &amp; &quot; &apos;
class XmlNo
{
    string nome;                // nome da tag
    string texto;               // texto que esta dentro da tag (sem espacos nas pontas)
    list<XmlNo *> filhos;       // elementos que estao dentro desta tag

    // um no tem donos dos filhos, por isso nao pode ser copiado
    XmlNo(const XmlNo &);
    XmlNo &operator=(const XmlNo &);

    // auxiliares do parser (pos e a posicao atual no texto, vai avancando)
    static void SaltarBrancos(const string &s, string::size_type &pos);
    static bool SaltarMarcas(const string &s, string::size_type &pos);
    static string DescodificarEntidades(const string &s);
    static XmlNo *LerElemento(const string &s, string::size_type &pos, int profundidade);

    public:
        XmlNo(const string &_nome);
        virtual ~XmlNo();

        string getNome() const                      { return nome; }
        string getTexto() const                     { return texto; }
        const list<XmlNo *> &getFilhos() const      { return filhos; }

        /// primeiro filho com esse nome (NULL se nao existir). O no continua a ser do pai
        XmlNo *Filho(const string &_nome) const;

        /// texto do primeiro filho com esse nome. Se nao existir devolve "defeito"
        string TextoDe(const string &_nome, const string &defeito = "") const;

        /// le o conteudo de um XML (ja em memoria) e devolve a raiz.
        /// Devolve NULL se o XML estiver mal formado. Quem chama tem de fazer delete da raiz
        static XmlNo *LerTexto(const string &conteudo);

        /// igual a LerTexto mas le primeiro o ficheiro. NULL se nao abrir ou estiver mal formado
        static XmlNo *LerFicheiro(const string &ficheiro);
};

#endif // XMLNO_H
