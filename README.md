# 🎵 Sistema de Recomendação Musical em C++

Este projeto é um sistema de recomendação de músicas desenvolvido em C++ com suporte a:

- ✅ Cadastro e login de usuários
- 🔎 Recomendação por música, gênero ou artista
- 📂 Armazenamento de músicas em arquivos `.txt`
- 💡 Cálculo de similaridade entre músicas com base em diversos critérios
- 📄 Geração de um histórico de recomendações por usuário

---

## 📁 Estrutura dos Arquivos

- `main.cpp`: Código-fonte principal do sistema
- `musicas.txt`: Base de dados das músicas, com o formato:
Título;Artista;Gênero;Ano
- `usuarios.txt`: Dados de login de usuários (formato: `usuario senha`)
- `recomendacoes.txt`: Histórico de recomendações feitas

---

## 🧠 Como funciona a recomendação?

A recomendação pode ser feita de 3 maneiras:

### 1. 🎶 Baseado em uma música
- O sistema compara gênero, artista, ano e palavras no título.
- Cada critério soma pontos para a "similaridade".

### 2. 🎧 Baseado em um gênero
- Lista todas as músicas do mesmo gênero.

### 3. 👨‍🎤 Baseado em um artista
- Exibe músicas do mesmo artista e de gêneros semelhantes.

---

## ⚙️ Como compilar e executar

Você pode compilar com `g++`:

```bash
g++ main.cpp -o recomendador
./recomendador
