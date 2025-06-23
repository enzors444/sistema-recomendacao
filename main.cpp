/*
 * Projeto desenvolvido por:
 * - Enzo Ribeiro da Silva (Matrícula: 812)
 * - Gustavo (Matrícula: ?)
 * - Lucas Hokari (Matrícula: ?)
 * - João Gabriel (Matrícula: ?)
 * - João Pedro (Matrícula: ?)
 */

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
    string album;
    string genero;
    int ano;
    string idioma;
    int popularidade; // 1-10 (1=pouco conhecida, 10=muito popular)
    int duracao;      // em segundos
};

struct Recomendacao {
    Musica musica;
    int similaridade;
};

// ----------- Funções Auxiliares -----------

bool albumsSimilares(const string& album1, const string& album2) {
    string a1 = paraMinusculas(album1);
    string a2 = paraMinusculas(album2);
    
    // Álbuns relacionados comuns
    if ((a1.find("greatest hits") != string::npos && a2.find("best of") != string::npos) ||
        (a1.find("best of") != string::npos && a2.find("greatest hits") != string::npos)) return true;
    if ((a1.find("live") != string::npos && a2.find("ao vivo") != string::npos) ||
        (a1.find("ao vivo") != string::npos && a2.find("live") != string::npos)) return true;
    if ((a1.find("unplugged") != string::npos && a2.find("acustico") != string::npos) ||
        (a1.find("acustico") != string::npos && a2.find("unplugged") != string::npos)) return true;
    
    return false;
}

bool generosSimilares(const string& genero1, const string& genero2) {
    string g1 = paraMinusculas(genero1);
    string g2 = paraMinusculas(genero2);
    
    if ((g1 == "rock" && g2 == "pop rock") || (g1 == "pop rock" && g2 == "rock")) return true;
    if ((g1 == "jazz" && g2 == "blues") || (g1 == "blues" && g2 == "jazz")) return true;
    if ((g1 == "hip hop" && g2 == "rap") || (g1 == "rap" && g2 == "hip hop")) return true;
    if ((g1 == "trap" && g2 == "rap") || (g1 == "rap" && g2 == "trap")) return true;
    if ((g1 == "eletronica" && g2 == "techno") || (g1 == "techno" && g2 == "eletronica")) return true;
    if ((g1 == "samba" && g2 == "bossa nova") || (g1 == "bossa nova" && g2 == "samba")) return true;
    if ((g1 == "country" && g2 == "folk") || (g1 == "folk" && g2 == "country")) return true;
    if ((g1 == "heavy metal" && g2 == "metal") || (g1 == "metal" && g2 == "heavy metal")) return true;
    if ((g1 == "sertanejo" && g2 == "country") || (g1 == "country" && g2 == "sertanejo")) return true;
    if ((g1 == "trap" && g2 == "hip hop") || (g1 == "hip hop" && g2 == "trap")) return true;
    if ((g1 == "funk" && g2 == "hip hop") || (g1 == "hip hop" && g2 == "funk")) return true;
    if ((g1 == "funk" && g2 == "rap") || (g1 == "rap" && g2 == "funk")) return true;
    if ((g1 == "funk" && g2 == "trap") || (g1 == "trap" && g2 == "funk")) return true;
    if ((g1 == "grunge" && g2 == "rock") || (g1 == "rock" && g2 == "grunge")) return true;
    if ((g1 == "rock" && g2 == "metal") || (g1 == "metal" && g2 == "rock")) return true;
    if ((g1 == "rock" && g2 == "heavy metal") || (g1 == "heavy metal" && g2 == "rock")) return true;
    
    return false;
}

bool idiomasSimilares(const string& idioma1, const string& idioma2) {
    string i1 = paraMinusculas(idioma1);
    string i2 = paraMinusculas(idioma2);
    
    // Idiomas relacionados
    if ((i1 == "portugues" && i2 == "espanhol") || (i1 == "espanhol" && i2 == "portugues")) return true;
    if ((i1 == "frances" && i2 == "italiano") || (i1 == "italiano" && i2 == "frances")) return true;
    
    return false;
}

bool duracaoSimilar(int duracao1, int duracao2) {
    int diferenca = abs(duracao1 - duracao2);
    return diferenca <= 60; // Diferença de até 1 minuto
}

bool popularidadeSimilar(int pop1, int pop2) {
    return abs(pop1 - pop2) <= 2; // Diferença de até 2 pontos na escala
}


int calcularSimilaridade(const Musica& m1, const Musica& m2) {
    const int P_ARTISTA_EXATO = 9;
    const int P_ALBUM_EXATO = 5;
    const int P_GENERO_EXATO = 12;
    const int P_GENERO_SIMILAR = 9;
    const int P_IDIOMA_EXATO = 3;
    const int P_IDIOMA_SIMILAR = 1;
    const int P_ANO_EXATO = 6;
    const int P_ANO_MUITO_PROXIMO = 5;
    const int P_ANO_PROXIMO = 4;
    const int P_ANO_DECADA = 2;
    const int P_POPULARIDADE_SIMILAR = 2;
    const int P_DURACAO_SIMILAR = 2;
    const int B_MESMO_ALBUM_ARTISTA = 3;
    
    const int PONTUACAO_MAXIMA = 
        P_ARTISTA_EXATO +
        P_ALBUM_EXATO +
        P_GENERO_EXATO +
        P_IDIOMA_EXATO +
        P_ANO_EXATO +
        P_POPULARIDADE_SIMILAR +
        P_DURACAO_SIMILAR +
        B_MESMO_ALBUM_ARTISTA;
        
    int score = 0;


    // Critério: Artista
    if (m1.artista == m2.artista) {
        score += P_ARTISTA_EXATO;
    }

    // Critério: Álbum
    if (m1.album == m2.album) {
        score += P_ALBUM_EXATO;
    }

    // Bônus: Mesmo artista e mesmo álbum
    if (m1.artista == m2.artista && m1.album == m2.album) {
        score += B_MESMO_ALBUM_ARTISTA;
    }

    // Critério: Gênero
    if (m1.genero == m2.genero) {
        score += P_GENERO_EXATO;
    } else if (generosSimilares(m1.genero, m2.genero)) {
        score += P_GENERO_SIMILAR;
    }

    // Critério: Idioma
    if (m1.idioma == m2.idioma) {
        score += P_IDIOMA_EXATO;
    } else if (idiomasSimilares(m1.idioma, m2.idioma)) {
        score += P_IDIOMA_SIMILAR;
    }

    // Critério: Ano de Lançamento
    int diff_ano = abs(m1.ano - m2.ano);
    if (diff_ano == 0) {
        score += P_ANO_EXATO;
    } else if (diff_ano <= 3) {
        score += P_ANO_MUITO_PROXIMO;
    } else if (diff_ano <= 7) {
        score += P_ANO_PROXIMO;
    } else if ((m1.ano / 10) == (m2.ano / 10)) {
        score += P_ANO_DECADA;
    }

    // Critério: Popularidade
    if (popularidadeSimilar(m1.popularidade, m2.popularidade)) {
        score += P_POPULARIDADE_SIMILAR;
    }

    // Critério: Duração
    if (duracaoSimilar(m1.duracao, m2.duracao)) {
        score += P_DURACAO_SIMILAR;
    }
    
    // --- Normalização ---
    int score_normalizado = (score * 100.0) / PONTUACAO_MAXIMA;

    if (score_normalizado > 100) {
        return 100;
    } else if (score_normalizado < 0) {
        return 0;
    } else {
        return score_normalizado;
    }

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
        if (linha.empty()) continue;
        
        size_t pos1 = linha.find(';');
        size_t pos2 = linha.find(';', pos1 + 1);
        size_t pos3 = linha.find(';', pos2 + 1);
        size_t pos4 = linha.find(';', pos3 + 1);
        size_t pos5 = linha.find(';', pos4 + 1);
        size_t pos6 = linha.find(';', pos5 + 1);
        size_t pos7 = linha.find(';', pos6 + 1);

        if (pos7 != string::npos) {
            // Formato: titulo;artista;album;genero;ano;idioma;popularidade;duracao
            banco[total].titulo = linha.substr(0, pos1);
            banco[total].artista = linha.substr(pos1 + 1, pos2 - pos1 - 1);
            banco[total].album = linha.substr(pos2 + 1, pos3 - pos2 - 1);
            banco[total].genero = linha.substr(pos3 + 1, pos4 - pos3 - 1);
            banco[total].ano = stoi(linha.substr(pos4 + 1, pos5 - pos4 - 1));
            banco[total].idioma = linha.substr(pos5 + 1, pos6 - pos5 - 1);
            banco[total].popularidade = stoi(linha.substr(pos6 + 1, pos7 - pos6 - 1));
            banco[total].duracao = stoi(linha.substr(pos7 + 1));
            total++;
        }
    }

    file.close();
    return total;
}

void salvarMusica(const Musica& musica) {
    ofstream file("musicas.txt", ios::app);
    if (file.is_open()) {
        file << musica.titulo << ";" << musica.artista << ";" << musica.album << ";" 
             << musica.genero << ";" << musica.ano << ";" << musica.idioma << ";" 
             << musica.popularidade << ";" << musica.duracao << endl;
        file.close();
    }
}

int buscarMusicasPorPalavrasChave(const Musica banco[], int total, const string& palavrasChave, int resultados[]) {
    string chaveMin = paraMinusculas(palavrasChave);
    int totalResultados = 0;
    
    for (int i = 0; i < total; i++) {
        string tituloMin = paraMinusculas(banco[i].titulo);
        
        // Verifica se todas as palavras-chave estão no título
        bool contemTodasPalavras = true;
        size_t pos = 0;
        string palavra;
        string chaveTemp = chaveMin;
        
        // Separa as palavras da busca
        while ((pos = chaveTemp.find(' ')) != string::npos || !chaveTemp.empty()) {
            if (pos != string::npos) {
                palavra = chaveTemp.substr(0, pos);
                chaveTemp.erase(0, pos + 1);
            } else {
                palavra = chaveTemp;
                chaveTemp = "";
            }
            
            if (!palavra.empty() && tituloMin.find(palavra) == string::npos) {
                contemTodasPalavras = false;
                break;
            }
        }
        
        if (contemTodasPalavras) {
            resultados[totalResultados] = i;
            totalResultados++;
        }
    }
    
    return totalResultados;
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
    string albumsUnicos[50];
    int totalGeneros = 0;
    int totalAlbums = 0;

    // Coleta gêneros e álbuns do artista
    for (int i = 0; i < totalMusicas; i++) {
        if (banco[i].artista == artistaBusca) {
            // Coleta gêneros únicos
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
            
            // Coleta álbuns únicos
            bool jaTemAlbum = false;
            for (int j = 0; j < totalAlbums; j++) {
                if (banco[i].album == albumsUnicos[j]) {
                    jaTemAlbum = true;
                    break;
                }
            }
            if (!jaTemAlbum) {
                albumsUnicos[totalAlbums++] = banco[i].album;
            }
        }
    }

    // Adiciona músicas do próprio artista
    for (int i = 0; i < totalMusicas; i++) {
        if (banco[i].artista == artistaBusca) {
            recomendadas[totalRecomendadas++] = {banco[i], 10};
        }
    }

    // Adiciona músicas de gêneros exatos e similares
    for (int i = 0; i < totalMusicas; i++) {
        if (banco[i].artista == artistaBusca) continue;
        bool jaAdicionado = false;
        
        // Verifica se já foi adicionado
        for (int k = 0; k < totalRecomendadas; k++) {
            if (banco[i].titulo == recomendadas[k].musica.titulo && 
                banco[i].artista == recomendadas[k].musica.artista) {
                jaAdicionado = true;
                break;
            }
        }
        
        if (!jaAdicionado) {
            // Verifica gêneros exatos
            for (int j = 0; j < totalGeneros; j++) {
                if (banco[i].genero == generosUnicos[j]) {
                    recomendadas[totalRecomendadas++] = {banco[i], 6}; // Score 6 para gênero exato
                    jaAdicionado = true;
                    break;
                }
            }
            
            // Se não foi adicionado por gênero exato, verifica gêneros similares
            if (!jaAdicionado) {
                for (int j = 0; j < totalGeneros; j++) {
                    if (generosSimilares(banco[i].genero, generosUnicos[j])) {
                        recomendadas[totalRecomendadas++] = {banco[i], 3}; // Score 3 para gênero similar
                        break;
                    }
                }
            }
        }
    }
}

void salvarRecomendacoes(const string& usuario, const Musica& base, Recomendacao recomendadas[], int total) {
    ofstream file("recomendacoes.txt", ios::app);
    if (file.is_open()) {
        file << "Usuario: " << usuario << endl;
        file << "Base: " << base.titulo << " - " << base.artista << " - " << base.album
             << " (" << base.idioma << ", " << base.ano << ", Pop: " << base.popularidade 
             << ", " << base.duracao << "s)" << endl;
        file << "Recomendacoes:" << endl;
        for (int i = 0; i < total; i++) {
            file << "- " << recomendadas[i].musica.titulo << " | "
                 << recomendadas[i].musica.artista << " | "
                 << recomendadas[i].musica.album << " | "
                 << recomendadas[i].musica.genero << " | "
                 << recomendadas[i].musica.ano << " | "
                 << recomendadas[i].musica.idioma << " | Pop: "
                 << recomendadas[i].musica.popularidade << " | "
                 << recomendadas[i].musica.duracao << "s | Similaridade: "
                 << recomendadas[i].similaridade << endl;
        }
        file << "--------------------------" << endl;
        file.close();
    }
}

// ----------- Função Principal -----------

int main() {
    const int MAX_MUSICAS = 400;
    const int MAX_RESULTADOS = 100;
    Musica banco[MAX_MUSICAS];
    int totalMusicas = carregarMusicas(banco, MAX_MUSICAS);

    if (totalMusicas == 0) {
        cout << "Nenhuma musica carregada. O banco esta vazio.\n";
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
            cout << "Digite um nome de usuario: ";
            getline(cin, usuario);
            if (usuarioExiste(usuario)) {
                cout << "Este nome de usuario ja existe. Escolha outro.\n";
            }
        } while (usuarioExiste(usuario));

        cout << "Digite uma senha: ";
        getline(cin, senha);
        salvarUsuario(usuario, senha);
        cout << "Conta criada com sucesso!\n";
    }

    cout << "\n=== Login ===\n";
    cout << "Usuario: ";
    getline(cin, usuario);
    cout << "Senha: ";
    getline(cin, senha);

    if (!validarLogin(usuario, senha)) {
        cout << "Login falhou. Encerrando o programa.\n";
        return 1;
    }

    cout << "\nLogin bem-sucedido! Bem-vindo, " << usuario << "!\n";

    int tipoPesquisa;
    cout << "\n=== Como deseja obter recomendacoes? ===\n";
    cout << "1. Com base em uma musica\n";
    cout << "2. Com base em um genero\n";
    cout << "3. Com base em um artista\n";
    cout << "Escolha: ";
    cin >> tipoPesquisa;
    cin.ignore();

    Musica musicaEscolhida;
    string generoBusca, artistaBusca;
    bool usarSimilaridade = true;

    if (tipoPesquisa == 1) {
        string palavrasChave;
        cout << "\n=== Buscar Musica ===\n";
        cout << "Digite palavras-chave do titulo da musica: ";
        getline(cin, palavrasChave);

        // Busca por palavras-chave
        int resultados[MAX_RESULTADOS];
        int totalResultados = buscarMusicasPorPalavrasChave(banco, totalMusicas, palavrasChave, resultados);
        
        if (totalResultados == 0) {
            cout << "\nNenhuma musica encontrada com essas palavras-chave.\n";
            cout << "Vamos cadastrar uma nova musica.\n";
            
            // Cadastrar nova música
            cout << "Digite o titulo completo da musica: ";
            getline(cin, musicaEscolhida.titulo);
            musicaEscolhida.titulo = capitalizarPalavras(musicaEscolhida.titulo);
            
            cout << "Digite o nome do artista: ";
            getline(cin, musicaEscolhida.artista);
            musicaEscolhida.artista = capitalizarPalavras(musicaEscolhida.artista);
            
        } else if (totalResultados == 1) {
            cout << "\nMusica encontrada:\n";
            int minutos = banco[resultados[0]].duracao / 60;
            int segundos = banco[resultados[0]].duracao % 60;
            cout << "- " << banco[resultados[0]].titulo << " | "
                 << banco[resultados[0]].artista << " | "
                 << banco[resultados[0]].album << " | "
                 << banco[resultados[0]].genero << " | "
                 << banco[resultados[0]].ano << " | "
                 << banco[resultados[0]].idioma << " | Pop: "
                 << banco[resultados[0]].popularidade << " | "
                 << minutos << ":" << (segundos < 10 ? "0" : "") << segundos << endl;
            
            cout << "\nEsta e a musica que voce estava procurando? (s/n): ";
            char confirmacao;
            cin >> confirmacao;
            cin.ignore();
            
            if (confirmacao == 's' || confirmacao == 'S') {
                musicaEscolhida = banco[resultados[0]];
                cout << "Musica selecionada! Gerando recomendacoes...\n";
            } else {
                cout << "Vamos cadastrar uma nova musica.\n";
                cout << "Digite o titulo completo da musica: ";
                getline(cin, musicaEscolhida.titulo);
                musicaEscolhida.titulo = capitalizarPalavras(musicaEscolhida.titulo);
                
                cout << "Digite o nome do artista: ";
                getline(cin, musicaEscolhida.artista);
                musicaEscolhida.artista = capitalizarPalavras(musicaEscolhida.artista);
            }
        } else {
            cout << "\nVarias musicas encontradas:\n";
            for (int i = 0; i < totalResultados; i++) {
                int minutos = banco[resultados[i]].duracao / 60;
                int segundos = banco[resultados[i]].duracao % 60;
                cout << (i + 1) << ". " << banco[resultados[i]].titulo << " | "
                     << banco[resultados[i]].artista << " | "
                     << banco[resultados[i]].album << " | "
                     << banco[resultados[i]].genero << " | "
                     << banco[resultados[i]].ano << " | "
                     << banco[resultados[i]].idioma << " | Pop: "
                     << banco[resultados[i]].popularidade << " | "
                     << minutos << ":" << (segundos < 10 ? "0" : "") << segundos << endl;
            }
            
            cout << "0. Nenhuma das opcoes acima (cadastrar nova musica)\n";
            cout << "Escolha uma opcao (0-" << totalResultados << "): ";
            
            int escolha;
            cin >> escolha;
            cin.ignore();
            
            if (escolha >= 1 && escolha <= totalResultados) {
                musicaEscolhida = banco[resultados[escolha - 1]];
                cout << "Musica selecionada! Gerando recomendacoes...\n";
            } else {
                cout << "Vamos cadastrar uma nova musica.\n";
                cout << "Digite o titulo completo da musica: ";
                getline(cin, musicaEscolhida.titulo);
                musicaEscolhida.titulo = capitalizarPalavras(musicaEscolhida.titulo);
                
                cout << "Digite o nome do artista: ";
                getline(cin, musicaEscolhida.artista);
                musicaEscolhida.artista = capitalizarPalavras(musicaEscolhida.artista);
            }
        }
        
        // Se precisa cadastrar nova música, continua com os outros campos
        if (totalResultados == 0 || 
            (totalResultados == 1 && musicaEscolhida.titulo.empty()) ||
            (totalResultados > 1 && musicaEscolhida.titulo.empty())) {
            
            cout << "Digite o nome do album: ";
            getline(cin, musicaEscolhida.album);
            musicaEscolhida.album = capitalizarPalavras(musicaEscolhida.album);

            cout << "Digite o genero da musica: ";
            getline(cin, musicaEscolhida.genero);
            musicaEscolhida.genero = capitalizarPalavras(musicaEscolhida.genero);

            cout << "Digite o ano da musica: ";
            while (!(cin >> musicaEscolhida.ano)) {
                cout << "Ano invalido. Digite novamente: ";
                cin.clear();
                while (cin.get() != '\n');
            }
            cin.ignore();

            cout << "Digite o idioma da musica: ";
            getline(cin, musicaEscolhida.idioma);
            musicaEscolhida.idioma = capitalizarPalavras(musicaEscolhida.idioma);

            cout << "Digite a popularidade da musica (1-10): ";
            while (!(cin >> musicaEscolhida.popularidade) || musicaEscolhida.popularidade < 1 || musicaEscolhida.popularidade > 10) {
                cout << "Popularidade invalida (deve ser entre 1 e 10). Digite novamente: ";
                cin.clear();
                while (cin.get() != '\n');
            }
            cin.ignore();

            cout << "Digite a duracao da musica em segundos: ";
            while (!(cin >> musicaEscolhida.duracao) || musicaEscolhida.duracao <= 0) {
                cout << "Duracao invalida. Digite novamente: ";
                cin.clear();
                while (cin.get() != '\n');
            }
            cin.ignore();

            salvarMusica(musicaEscolhida);
            cout << "Musica adicionada com sucesso!\n";

            totalMusicas = carregarMusicas(banco, MAX_MUSICAS);
        }

   
    } else if (tipoPesquisa == 2) {
        usarSimilaridade = false;
        cout << "\nDigite o genero para recomendacao: ";
        getline(cin, generoBusca);
        generoBusca = capitalizarPalavras(generoBusca);
    } else if (tipoPesquisa == 3) {
        usarSimilaridade = false;
        cout << "\nDigite o nome do artista para recomendacao: ";
        getline(cin, artistaBusca);
        artistaBusca = capitalizarPalavras(artistaBusca);
    } else {
        cout << "Opcao invalida. Encerrando.\n";
        return 1;
    }

    Recomendacao recomendadas[MAX_MUSICAS];
    int totalRecomendadas = 0;

    // Processamento das recomendações baseado no tipo de pesquisa
    if (tipoPesquisa == 3) { // Por artista
        recomendarPorArtista(banco, totalMusicas, artistaBusca, recomendadas, totalRecomendadas);
    } else {
        for (int i = 0; i < totalMusicas; i++) {
            bool adicionar = false;
            int score = 0;

            if (usarSimilaridade) { // Por música
                if (paraMinusculas(banco[i].titulo) == paraMinusculas(musicaEscolhida.titulo) &&
                    paraMinusculas(banco[i].artista) == paraMinusculas(musicaEscolhida.artista))
                    continue;

                score = calcularSimilaridade(musicaEscolhida, banco[i]);
                if (score > 0) adicionar = true;
            } else if (!generoBusca.empty()) { // Por gênero
                if (banco[i].genero == generoBusca) {
                    adicionar = true;
                    score = 8;
                } else if (generosSimilares(banco[i].genero, generoBusca)) {
                    adicionar = true;
                    score = 5;
                }
            }

            if (adicionar) {
                recomendadas[totalRecomendadas++] = {banco[i], score};
            }
        }
    }

    // Ordenação por similaridade (bubble sort)
    for (int i = 0; i < totalRecomendadas - 1; i++) {
        for (int j = 0; j < totalRecomendadas - i - 1; j++) {
            if (recomendadas[j].similaridade < recomendadas[j + 1].similaridade) {
                Recomendacao temp = recomendadas[j];
                recomendadas[j] = recomendadas[j + 1];
                recomendadas[j + 1] = temp;
            }
        }
    }

    cout << "\n=== Recomendacoes ===\n\n";

    if (totalRecomendadas == 0) {
        cout << "Nenhuma recomendacao encontrada.\n";
    } else {
        int exibidas = 0;
        char resposta;

        do {
            int quantidade = (totalRecomendadas - exibidas >= 5) ? 5 : totalRecomendadas - exibidas;
            for (int i = 0; i < quantidade; i++) {
                int minutos = recomendadas[exibidas + i].musica.duracao / 60;
                int segundos = recomendadas[exibidas + i].musica.duracao % 60;
                cout << "- " << recomendadas[exibidas + i].musica.titulo << " | "
                     << recomendadas[exibidas + i].musica.artista << " | "
                     << recomendadas[exibidas + i].musica.album << " | "
                     << recomendadas[exibidas + i].musica.genero << " | "
                     << recomendadas[exibidas + i].musica.ano << " | "
                     << recomendadas[exibidas + i].musica.idioma << " | Pop: "
                     << recomendadas[exibidas + i].musica.popularidade << " | "
                     << minutos << ":" << (segundos < 10 ? "0" : "") << segundos
                     << " | Similaridade: " << recomendadas[exibidas + i].similaridade << endl;
            }

            exibidas += quantidade;

            if (exibidas >= totalRecomendadas) {
                cout << "\nFim das recomendacoes.\n";
                break;
            }

            cout << "\nDeseja ver mais recomendacoes? (s/n): ";
            cin >> resposta;

        } while (resposta == 's' || resposta == 'S');

        salvarRecomendacoes(usuario, musicaEscolhida, recomendadas, totalRecomendadas);
    }

    return 0;
}
