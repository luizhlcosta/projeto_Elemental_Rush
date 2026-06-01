# Elemental Rush

Elemental Rush é um jogo cooperativo em pixel art. Os jogadores controlam dois personagens elementais que devem trabalhar em conjunto para superar obstáculos, atravessar plataformas e completar cada fase.

## Sobre o Jogo

Em Elemental Rush, o objetivo é conduzir os dois personagens até a saída da fase utilizando suas habilidades e características únicas para resolver os desafios encontrados pelo caminho.

Durante a partida, é possível:

- Controlar dois personagens simultaneamente;
- Movimentar-se pelo mapa;
- Saltar entre plataformas;
- Evitar armadilhas e obstáculos;
- Coletar itens espalhados pelo cenário;
- Completar fases cooperativas.

O jogo possui mecânicas simples de plataforma e cooperação, focando na coordenação entre os personagens para avançar pelos desafios.

### Menus

- Enter para confirmar opções;
- Esc para retornar ou sair.

## Requisitos

Para compilar e executar o projeto é necessário possuir:

- GCC
- Makefile
- Raylib
- Ubuntu/Linux
- Libm
- Openal

## Instalação das Dependências

Atualize os pacotes:

```bash
sudo apt update
```

Instale as ferramentas de compilação:

```bash
sudo apt install build-essential gcc make
```

Instale a Raylib:

```bash
sudo apt install libraylib-dev
```

Instale o OpenAL:

```bash
sudo apt install libopenal-dev
```

Caso sua versão do Ubuntu não possua esse pacote disponível, consulte a documentação oficial da Raylib para instalação manual.

## Clonando o Repositório

```bash
git clone https://github.com/luizhlcosta/projeto_Elemental_Rush.git
```

Entre na pasta do projeto:

```bash
cd Elemental-Rush
```

## Compilação

Para compilar o jogo, execute:

```bash
make
```

Ao final da compilação será gerado o executável:

## Execução

Execute o jogo com:

```bash
./jogo
```

## Tecnologias Utilizadas

- Linguagem C
- Raylib
- GCC
- Makefile
- Openal
- Libm
- Ubuntu Linux

## Observações Importantes

- O jogo deve ser executado a partir da pasta raiz do projeto.
- Os arquivos de imagem e áudio são carregados utilizando caminhos relativos.
- Certifique-se de que todos os recursos do projeto estejam presentes antes da execução.
- O projeto foi desenvolvido com foco em mecânicas cooperativas de plataforma utilizando arte em pixel art.

## Créditos

Elemental Rush foi desenvolvido como um projeto acadêmico utilizando a linguagem C e a biblioteca Raylib. O jogo foi inspirado nas mecânicas clássicas de Fireboy and Watergirl, adaptadas para uma versão própria em pixel art.
