#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

// ----------- Funções de Normalização -----------
string paraMinusculas(const string& str) {
    string resultado = str;
    for (char& c : resultado) {
        c = tolower(c);
    }
    return resultado;
}

string capitalizarPalavras(const string& str) {
    string resultado = str;
    bool novaPalavra = true;
    for (size_t i = 0; i < resultado.length(); ++i) {
        if (isspace(resultado[i])) {
            novaPalavra = true;
        } else if (novaPalavra) {
            resultado[i] = toupper(resultado[i]);
            novaPalavra = false;
        } else {
            resultado[i] = tolower(resultado[i]);
        }
    }
    return resultado;
}

// ----------- Estruturas -----------
struct Musica {
    string titulo;
    string artista;
    string genero;
    int ano;
};

struct Recomendacao {
    Musica musica;
    int similaridade;
};

// ----------- Funções Auxiliares -----------

bool artistaInternacional(const string& artista) {
    string famosos[] = {
        "Ed Sheeran", "Queen", "The Weeknd", "Led Zeppelin", "Nirvana", "Eagles",
        "Billie Eilish", "Michael Jackson", "Guns N' Roses", "Oasis", "Adele",
        "John Lennon", "A-ha"
    };
    for (int i = 0; i < 13; i++) {
        if (artista == famosos[i]) return true;
    }
    return false;
}

bool mesmaDecada(int ano1, int ano2) {
    return (ano1 / 10) == (ano2 / 10);
}

bool palavrasEmComum(const string& titulo1, const string& titulo2) {
    size_t pos = 0, espaco;
    while ((espaco = titulo1.find(' ', pos)) != string::npos) {
        string palavra = titulo1.substr(pos, espaco - pos);
        if (!palavra.empty() && titulo2.find(palavra) != string::npos) {
            return true;
        }
        pos = espaco + 1;
    }
    string palavraFinal = titulo1.substr(pos);
    if (!palavraFinal.empty() && titulo2.find(palavraFinal) != string::npos) {
        return true;
    }
    return false;
}

int calcularSimilaridade(const Musica& m1, const Musica& m2) {
    int score = 0;
    if (m1.genero == m2.genero) score += 5;
    if (m1.artista == m2.artista) score += 2;
    if (m1.ano == m2.ano) score += 2;
    else if (abs(m1.ano - m2.ano) <= 2) score += 1;
    if (mesmaDecada(m1.ano, m2.ano)) score += 1;
    if (artistaInternacional(m1.artista) && artistaInternacional(m2.artista)) score += 1;
    if (palavrasEmComum(m1.titulo, m2.titulo)) score += 1;
    return score;
}

// ----------- Arquivos de Música -----------

int carregarMusicas(Musica banco[], int max) {
    ifstream file("musicas.txt");
    if (!file.is_open()) {
        cout << "Erro ao abrir 'musicas.txt'!" << endl;
        return 0;
    }

    string linha;
    int total = 0;
    while (getline(file, linha) && total < max) {
        size_t pos1 = linha.find(';');
        size_t pos2 = linha.find(';', pos1 + 1);
        size_t pos3 = linha.find(';', pos2 + 1);

        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) continue;

        banco[total].titulo = linha.substr(0, pos1);
        banco[total].artista = linha.substr(pos1 + 1, pos2 - pos1 - 1);
        banco[total].genero = linha.substr(pos2 + 1, pos3 - pos2 - 1);
        banco[total].ano = stoi(linha.substr(pos3 + 1));
        total++;
    }

    file.close();
    return total;
}

void salvarMusica(const Musica& musica) {
    ofstream file("musicas.txt", ios::app);
    if (file.is_open()) {
        file << musica.titulo << ";" << musica.artista << ";" << musica.genero << ";" << musica.ano << endl;
        file.close();
    }
}

int buscarMusica(const Musica banco[], int total, const string& titulo, const string& artista) {
    string tituloMin = paraMinusculas(titulo);
    string artistaMin = paraMinusculas(artista);

    for (int i = 0; i < total; i++) {
        if (paraMinusculas(banco[i].titulo) == tituloMin &&
            paraMinusculas(banco[i].artista) == artistaMin) {
            return i;
        }
    }
    return -1;
}

// ----------- Sistema de Usuários -----------

void salvarUsuario(const string& usuario, const string& senha) {
    ofstream file("usuarios.txt", ios::app);
    if (file.is_open()) {
        file << usuario << " " << senha << endl;
        file.close();
    }
}

bool validarLogin(const string& usuario, const string& senha) {
    ifstream file("usuarios.txt");
    string u, p;
    while (file >> u >> p) {
        if (u == usuario && p == senha)
            return true;
    }
    return false;
}

bool usuarioExiste(const string& usuario) {
    ifstream file("usuarios.txt");
    string u, p;
    while (file >> u >> p) {
        if (u == usuario)
            return true;
    }
    return false;
}

// ----------- Recomendacao por artista -----------

void recomendarPorArtista(const Musica banco[], int totalMusicas, const string& artistaBusca, Recomendacao recomendadas[], int& totalRecomendadas) {
    string generosUnicos[50];
    int totalGeneros = 0;

    // Coleta os gêneros usados pelo artista
    for (int i = 0; i < totalMusicas; i++) {
        if (banco[i].artista == artistaBusca) {
            bool jaTem = false;
            for (int j = 0; j < totalGeneros; j++) {
                if (banco[i].genero == generosUnicos[j]) {
                    jaTem = true;
                    break;
                }
            }
            if (!jaTem) {
                generosUnicos[totalGeneros++] = banco[i].genero;
            }
        }
    }

    // 1º adicionar músicas do mesmo artista
    for (int i = 0; i < totalMusicas; i++) {
        if (banco[i].artista == artistaBusca) {
            recomendadas[totalRecomendadas++] = {banco[i], 5};
        }
    }

    // 2º adicionar músicas com o mesmo gênero, evitando duplicatas
    for (int i = 0; i < totalMusicas; i++) {
        if (banco[i].artista == artistaBusca) continue;
        for (int j = 0; j < totalGeneros; j++) {
            if (banco[i].genero == generosUnicos[j]) {
                recomendadas[totalRecomendadas++] = {banco[i], 3};
                break;
            }
        }
    }
}

void salvarRecomendacoes(const string& usuario, const Musica& base, Recomendacao recomendadas[], int total) {
    ofstream file("recomendacoes.txt", ios::app);
    if (file.is_open()) {
        file << "Usuário: " << usuario << endl;
        file << "Base: " << base.titulo << " - " << base.artista << endl;
        file << "Recomendações:" << endl;
        for (int i = 0; i < total; i++) {
            file << "- " << recomendadas[i].musica.titulo << " | "
                 << recomendadas[i].musica.artista << " | "
                 << recomendadas[i].musica.genero << " | "
                 << recomendadas[i].musica.ano << " | Similaridade: "
                 << recomendadas[i].similaridade << endl;
        }
        file << "--------------------------" << endl;
        file.close();
    }
}

// ----------- Função Principal -----------

int main() {
    const int MAX_MUSICAS = 100;
    Musica banco[MAX_MUSICAS];
    int totalMusicas = carregarMusicas(banco, MAX_MUSICAS);

    if (totalMusicas == 0) {
        cout << "Nenhuma música carregada. O banco está vazio.\n";
    }

    string usuario, senha;
    int opcaoLogin;

    cout << "=== Sistema de Login ===\n";
    cout << "1. Criar Conta\n";
    cout << "2. Fazer Login\n";
    cout << "Escolha: ";
    cin >> opcaoLogin;
    cin.ignore();

    if (opcaoLogin == 1) {
        do {
            cout << "Digite um nome de usuário: ";
            getline(cin, usuario);
            if (usuarioExiste(usuario)) {
                cout << "Este nome de usuário já existe. Escolha outro.\n";
            }
        } while (usuarioExiste(usuario));

        cout << "Digite uma senha: ";
        getline(cin, senha);
        salvarUsuario(usuario, senha);
        cout << "Conta criada com sucesso!\n";
    }

    cout << "\n=== Login ===\n";
    cout << "Usuário: ";
    getline(cin, usuario);
    cout << "Senha: ";
    getline(cin, senha);

    if (!validarLogin(usuario, senha)) {
        cout << "Login falhou. Encerrando o programa.\n";
        return 1;
    }

    cout << "\nLogin bem-sucedido! Bem-vindo, " << usuario << "!\n";

    int tipoPesquisa;
    cout << "\n=== Como deseja obter recomendações? ===\n";
    cout << "1. Com base em uma música\n";
    cout << "2. Com base em um gênero\n";
    cout << "3. Com base em um artista\n";
    cout << "Escolha: ";
    cin >> tipoPesquisa;
    cin.ignore();

    Musica musicaEscolhida;
    string generoBusca, artistaBusca;
    bool usarSimilaridade = true;

    if (tipoPesquisa == 1) {
        string tituloBusca;
        cout << "\n=== Buscar ou Adicionar Música ===\n";
        cout << "Título da música: ";
        getline(cin, tituloBusca);
        cout << "Artista: ";
        getline(cin, artistaBusca);

        tituloBusca = capitalizarPalavras(tituloBusca);
        artistaBusca = capitalizarPalavras(artistaBusca);

        int indice = buscarMusica(banco, totalMusicas, tituloBusca, artistaBusca);
        if (indice != -1) {
            cout << "\nMúsica encontrada no banco! Usando ela como base para recomendação.\n";
            musicaEscolhida = banco[indice];
        } else {
            cout << "\nMúsica não encontrada no banco.\n";
            musicaEscolhida.titulo = tituloBusca;
            musicaEscolhida.artista = capitalizarPalavras(artistaBusca);

            cout << "Digite o gênero da música: ";
            getline(cin, musicaEscolhida.genero);
            musicaEscolhida.genero = capitalizarPalavras(musicaEscolhida.genero);

            cout << "Digite o ano da música: ";
            while (!(cin >> musicaEscolhida.ano)) {
                cout << "Ano inválido. Digite novamente: ";
                cin.clear();
                while (cin.get() != '\n');
            }
            cin.ignore();

            salvarMusica(musicaEscolhida);
            cout << "Música adicionada com sucesso!\n";

            totalMusicas = carregarMusicas(banco, MAX_MUSICAS);
        }

    } else if (tipoPesquisa == 2) {
        usarSimilaridade = false;
        cout << "\nDigite o gênero para recomendação: ";
        getline(cin, generoBusca);
        generoBusca = capitalizarPalavras(generoBusca);
    } else if (tipoPesquisa == 3) {
        usarSimilaridade = false;
        cout << "\nDigite o nome do artista para recomendação: ";
        getline(cin, artistaBusca);
        artistaBusca = capitalizarPalavras(artistaBusca);
    } else {
        cout << "Opção inválida. Encerrando.\n";
        return 1;
    }

    Recomendacao recomendadas[MAX_MUSICAS];
    int totalRecomendadas = 0;

    for (int i = 0; i < totalMusicas; i++) {
        bool adicionar = false;
        int score = 0;

        if (usarSimilaridade) {
            if (paraMinusculas(banco[i].titulo) == paraMinusculas(musicaEscolhida.titulo) &&
                paraMinusculas(banco[i].artista) == paraMinusculas(musicaEscolhida.artista))
                continue;

            score = calcularSimilaridade(musicaEscolhida, banco[i]);
            if (score > 0) adicionar = true;
        } else if (!generoBusca.empty()) {
            if (banco[i].genero == generoBusca) {
                adicionar = true;
                score = 5;
            }
        } else if (!artistaBusca.empty()) {
            break; // pula para lógica especial
        }

        if (adicionar) {
            recomendadas[totalRecomendadas++] = {banco[i], score};
        }
    }

    // Se pesquisa por artista, executa recomendação especializada
    if (!usarSimilaridade && !artistaBusca.empty()) {
        totalRecomendadas = 0;
        recomendarPorArtista(banco, totalMusicas, artistaBusca, recomendadas, totalRecomendadas);
    }

    if (usarSimilaridade) {
        for (int i = 0; i < totalRecomendadas - 1; i++) {
            for (int j = 0; j < totalRecomendadas - i - 1; j++) {
                if (recomendadas[j].similaridade < recomendadas[j + 1].similaridade) {
                    Recomendacao temp = recomendadas[j];
                    recomendadas[j] = recomendadas[j + 1];
                    recomendadas[j + 1] = temp;
                }
            }
        }
    }

    cout << "\n=== Recomendações ===\n\n";

    if (totalRecomendadas == 0) {
        cout << "Nenhuma recomendação encontrada.\n";
    } else {
        int exibidas = 0;
        char resposta;

        do {
            int quantidade = (totalRecomendadas - exibidas >= 5) ? 5 : totalRecomendadas - exibidas;
            for (int i = 0; i < quantidade; i++) {
                cout << "- " << recomendadas[exibidas + i].musica.titulo << " | "
                     << recomendadas[exibidas + i].musica.artista << " | "
                     << recomendadas[exibidas + i].musica.genero << " | "
                     << recomendadas[exibidas + i].musica.ano
                     << " | Similaridade: " << recomendadas[exibidas + i].similaridade << endl;
            }

            exibidas += quantidade;

            if (exibidas >= totalRecomendadas) {
                cout << "\nFim das recomendações.\n";
                break;
            }

            cout << "\nDeseja ver mais recomendações? (s/n): ";
            cin >> resposta;

        } while (resposta == 's' || resposta == 'S');

        salvarRecomendacoes(usuario, musicaEscolhida, recomendadas, totalRecomendadas);
        cout << "\nRecomendações salvas no arquivo 'recomendacoes.txt'.\n";
    }

    return 0;
}
