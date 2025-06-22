# Sistema de Recomendação Musical

Um sistema inteligente de recomendação de músicas desenvolvido em C++ que utiliza algoritmos de similaridade para sugerir músicas baseadas em diferentes critérios.

## 📋 Funcionalidades

### 🔐 Sistema de Usuários
- **Cadastro de novos usuários** com validação de nome único
- **Login seguro** com verificação de credenciais
- **Persistência de dados** em arquivo `usuarios.txt`

### 🎵 Banco de Dados Musical
- **Carregamento automático** de músicas do arquivo `musicas.txt`
- **Adição dinâmica** de novas músicas ao banco
- **Estrutura completa** com 8 campos por música:
  - Título
  - Artista
  - Álbum
  - Gênero
  - Ano
  - Idioma
  - Popularidade (1-10)
  - Duração (segundos)

### 🤖 Sistema de Recomendação Inteligente

O sistema oferece **3 tipos de recomendação**:

#### 1. **Por Música**
- Algoritmo de similaridade com **17 critérios diferentes**
- Sistema de pontuação ponderada
- Comparação por gênero, artista, álbum, idioma, popularidade, duração e ano

#### 2. **Por Gênero**
- Busca por gênero específico
- Detecção de gêneros relacionados (ex: Rock ↔ Pop Rock)

#### 3. **Por Artista**
- Todas as músicas do artista escolhido
- Artistas com nomes similares
- Músicas de gêneros relacionados

## 🧠 Algoritmo de Similaridade

O sistema utiliza um **algoritmo proprietário** que considera:

### Critérios Principais (Peso Alto)
- **Gênero exato**: +8 pontos
- **Artista exato**: +7 pontos
- **Álbum exato**: +6 pontos
- **Idioma exato**: +6 pontos

### Critérios Secundários (Peso Médio)
- **Gêneros relacionados**: +5 pontos
- **Popularidade similar**: +2-4 pontos
- **Ano exato**: +4 pontos
- **Mesma década**: +2 pontos

### Critérios Auxiliares (Peso Baixo)
- **Artistas similares**: +3 pontos
- **Duração similar**: +3 pontos
- **Anos próximos**: +1-2 pontos
- **Bônus especiais** para hits populares e músicas clássicas

### Relacionamentos Inteligentes
- **Gêneros**: Rock ↔ Pop Rock, Jazz ↔ Blues, Hip Hop ↔ Rap
- **Idiomas**: Português ↔ Espanhol, Inglês ↔ Americano
- **Álbuns**: Greatest Hits ↔ Best Of, Live ↔ Ao Vivo

## 🚀 Como Usar

### Pré-requisitos
- Compilador C++ (g++, Visual Studio, etc.)
- Arquivo `musicas.txt` com dados musicais

### Formato do arquivo `musicas.txt`
```
titulo;artista;album;genero;ano;idioma;popularidade;duracao
Bohemian Rhapsody;Queen;A Night At The Opera;Rock;1975;Ingles;10;355
```

### Compilação e Execução
```bash
# Compilar
g++ -o music_system main.cpp

# Executar
./music_system
```

### Fluxo de Uso

1. **Escolher**: Criar conta ou fazer login
2. **Selecionar tipo de recomendação**:
   - Por música específica
   - Por gênero
   - Por artista
3. **Inserir dados** conforme solicitado
4. **Visualizar recomendações** ordenadas por similaridade
5. **Navegar pelos resultados** (5 por vez)

## 📁 Estrutura de Arquivos

```
├── main.cpp              # Código principal
├── musicas.txt           # Banco de dados de músicas
├── usuarios.txt          # Dados dos usuários (criado automaticamente)
└── recomendacoes.txt     # Histórico de recomendações (criado automaticamente)
```

## 🔧 Funcionalidades Técnicas

### Normalização de Dados
- **Conversão para minúsculas** para comparações
- **Capitalização automática** de palavras
- **Tratamento de espaços** e formatação

### Algoritmos Utilizados
- **Bubble Sort** para ordenação por similaridade
- **Busca linear** otimizada
- **Cálculo de similaridade** multi-critério

### Validações
- **Entrada de dados** com verificação de tipos
- **Ranges de valores** (popularidade 1-10)
- **Arquivos obrigatórios** com tratamento de erros

## 📊 Exemplo de Saída

```
=== Recomendações ===

- Somebody To Love | Queen | A Day At The Races | Rock | 1976 | Ingles | Pop: 9 | 4:56 | Similaridade: 28
- We Will Rock You | Queen | News Of The World | Rock | 1977 | Ingles | Pop: 10 | 2:02 | Similaridade: 25
- Don't Stop Me Now | Queen | Jazz | Rock | 1978 | Ingles | Pop: 8 | 3:29 | Similaridade: 23
```

## 🎯 Características Avançadas

- **Sistema de pontuação inteligente** com pesos balanceados
- **Detecção de relacionamentos** entre gêneros e idiomas
- **Filtragem de duplicatas** automática
- **Histórico persistente** de recomendações
- **Interface amigável** com navegação por páginas
- **Adição dinâmica** de músicas não catalogadas

## 🤝 Contribuição

Este sistema foi desenvolvido para demonstrar conceitos de:
- Algoritmos de recomendação
- Manipulação de arquivos em C++
- Estruturas de dados
- Sistemas de autenticação simples
- Interfaces de linha de comando

## 📈 Possíveis Melhorias

- Interface gráfica (GUI)
- Banco de dados SQL
- API web para integração
- Machine Learning para recomendações
- Suporte a playlists
- Integração com serviços de streaming

---

*Sistema desenvolvido em C++ com foco em algoritmos de similaridade e experiência do usuário.*
