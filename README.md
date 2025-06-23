# Sistema de Recomendação Musical 🎵

Um sistema inteligente de recomendação de músicas em C++ que utiliza algoritmos de similaridade para encontrar músicas compatíveis com os gostos do usuário.

## 📋 Funcionalidades

- **Sistema de Login/Cadastro**: Autenticação segura de usuários
- **Busca Inteligente**: Pesquisa por palavras-chave no título das músicas
- **Três Modos de Recomendação**:
  - Por música específica (algoritmo de similaridade)
  - Por gênero musical
  - Por artista
- **Algoritmo de Similaridade Avançado**: Considera múltiplos fatores como:
  - Artista e álbum
  - Gênero musical (incluindo gêneros similares)
  - Ano de lançamento
  - Idioma
  - Popularidade
  - Duração
- **Cadastro de Novas Músicas**: Expansão automática do banco de dados
- **Histórico de Recomendações**: Salva todas as recomendações em arquivo

## 🔧 Pré-requisitos

- Compilador C++ (GCC, Clang, MSVC)
- Padrão C++11 ou superior

## 📁 Estrutura de Arquivos

```
projeto/
├── main.cpp                 # Código fonte principal
├── musicas.txt             # Banco de dados de músicas (criado automaticamente)
├── usuarios.txt            # Cadastro de usuários (criado automaticamente)
├── recomendacoes.txt       # Histórico de recomendações (criado automaticamente)
└── README.md               # Este arquivo
```

## 🚀 Como Usar

### 1. Compilação
```bash
g++ -o recomendador main.cpp
```

### 2. Execução
```bash
./recomendador
```

### 3. Primeiro Uso
1. Escolha "Criar Conta" e cadastre-se
2. Faça login com suas credenciais
3. Escolha o tipo de recomendação desejada

### 4. Tipos de Recomendação

#### Por Música
- Digite palavras-chave do título
- Selecione a música desejada ou cadastre uma nova
- Receba recomendações baseadas em similaridade

#### Por Gênero
- Digite o gênero desejado
- Receba músicas do mesmo gênero ou similares

#### Por Artista
- Digite o nome do artista
- Receba músicas do artista e de gêneros relacionados

## 📊 Formato dos Dados

### Arquivo `musicas.txt`
Formato: `titulo;artista;album;genero;ano;idioma;popularidade;duracao`

Exemplo:
```
Bohemian Rhapsody;Queen;A Night At The Opera;Rock;1975;Inglês;10;355
```

### Arquivo `usuarios.txt`
Formato: `usuario senha`

### Arquivo `recomendacoes.txt`
Contém o histórico completo de todas as recomendações geradas.

## 🧮 Algoritmo de Similaridade

O sistema utiliza um algoritmo proprietário que pontua músicas baseado em:

| Critério | Peso Máximo |
|----------|-------------|
| Mesmo Artista | 9 pontos |
| Gênero Exato | 10 pontos |
| Gênero Similar | 4 pontos |
| Ano Exato | 6 pontos |
| Mesmo Álbum | 3 pontos |
| Idioma | 3 pontos |
| Popularidade Similar | 4 pontos |
| Duração Similar | 3 pontos |

### Gêneros Similares Reconhecidos
- Rock ↔ Pop Rock
- Jazz ↔ Blues
- Hip Hop ↔ Rap ↔ Trap ↔ Funk
- Eletrônica ↔ Techno
- Samba ↔ Bossa Nova
- Country ↔ Folk ↔ Sertanejo
- Heavy Metal ↔ Metal
- Grunge ↔ Rock

## 💡 Características Técnicas

- **Busca Flexível**: Aceita múltiplas palavras-chave
- **Normalização de Texto**: Converte automaticamente para formato padrão
- **Ordenação por Relevância**: Resultados ordenados por similaridade
- **Interface Interativa**: Navegação intuitiva por menus
- **Persistência de Dados**: Todos os dados são salvos em arquivos

## 🔍 Exemplo de Uso

```
=== Sistema de Login ===
1. Criar Conta
2. Fazer Login
Escolha: 1

Digite um nome de usuario: joao
Digite uma senha: 123456
Conta criada com sucesso!

=== Como deseja obter recomendacoes? ===
1. Com base em uma musica
2. Com base em um genero
3. Com base em um artista
Escolha: 1

Digite palavras-chave do titulo da musica: bohemian rhapsody

Musica encontrada:
- Bohemian Rhapsody | Queen | A Night At The Opera | Rock | 1975 | Inglês | Pop: 10 | 5:55

Esta e a musica que voce estava procurando? (s/n): s

=== Recomendacoes ===
- We Will Rock You | Queen | News Of The World | Rock | 1977 | Inglês | Pop: 9 | 2:02 | Similaridade: 85
- Stairway To Heaven | Led Zeppelin | Led Zeppelin IV | Rock | 1971 | Inglês | Pop: 10 | 8:02 | Similaridade: 72
...
```

## 🛠️ Personalização

### Adicionando Novos Gêneros Similares
Edite a função `generosSimilares()` no código fonte:

```cpp
if ((g1 == "novo_genero" && g2 == "genero_similar") || 
    (g1 == "genero_similar" && g2 == "novo_genero")) return true;
```

### Ajustando Pesos do Algoritmo
Modifique as constantes na função `calcularSimilaridade()`:

```cpp
const int P_ARTISTA_EXATO = 9;     // Peso para mesmo artista
const int P_GENERO_EXATO = 10;     // Peso para mesmo gênero
// ... outros pesos
```

## 👨‍💻 Desenvolvimento

Desenvolvido em C++ utilizando:
- Estruturas de dados nativas
- Algoritmos de ordenação
- Manipulação de arquivos
- Processamento de strings

---

*Sistema de Recomendação Musical - Conectando você às suas próximas músicas favoritas!* 🎶
