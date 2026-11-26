# Fechando o Campeonato

**Instituto Federal do Espírito Santo (IFES) — Campus Serra**  
**Curso:** Bacharelado em Sistemas de Informação  
**Aluno:** Rainer Carlos  
**Modalidade:** Individual  
**Prazo informado no enunciado:** 30/11/2026 às 23h59

---

## 1. Apresentação do projeto

Este projeto implementa, em linguagem C, um programa para apurar e apresentar a classificação de um campeonato de futebol disputado por dez equipes. Os dados dos times são carregados do arquivo `times.csv`, enquanto os resultados das partidas são lidos de `partidas.csv`.

A partir dos resultados válidos, o programa atualiza as estatísticas de cada equipe, calcula a pontuação e o saldo de gols, organiza a tabela de classificação conforme os critérios definidos no trabalho e apresenta um resumo do campeonato. Ao final, a classificação é gravada no arquivo `classificacao.csv`.

O programa é executado diretamente pelo terminal, sem menu ou entrada interativa de dados.

## 2. Estrutura do repositório

```text
fechando-campeonato/
├── Makefile              # Regras para compilar, executar e limpar o projeto
├── README.md             # Documentação do projeto
├── main.c                # Leitura dos arquivos, apuração e geração dos relatórios
├── team.c                # Implementação do TAD Team
├── team.h                # Interface pública do TAD Team
├── teamlist_dlist.c      # Implementação da lista duplamente encadeada de times
├── teamlist.h            # Interface pública do TAD TeamList
├── match.c               # Implementação do TAD Match
├── match.h               # Interface pública do TAD Match
├── matchlist.c           # Implementação da lista simplesmente encadeada de partidas
├── matchlist.h           # Interface pública do TAD MatchList
├── times.csv             # Cadastro dos dez times participantes
├── partidas.csv          # Arquivo de entrada ativo (inicia vazio)
├── partidas_vazio.csv    # Cenário oficial: campeonato não iniciado
├── partidas_parcial.csv  # Cenário oficial: 20 partidas
└── partidas_completo.csv # Cenário oficial: 45 partidas
```

### 2.1. Descrição dos arquivos

- **`main.c`** — contém o ponto de entrada do programa e coordena as etapas de leitura dos CSVs, registro dos resultados, ordenação da classificação, impressão do relatório e gravação do arquivo de saída.
- **`team.c` / `team.h`** — implementam e declaram o TAD `Team`, que representa uma equipe, mantém suas estatísticas e disponibiliza funções para consultar os dados, registrar partidas, calcular saldo de gols e calcular pontos.
- **`teamlist_dlist.c` / `teamlist.h`** — implementam e declaram o TAD `TeamList`, responsável por armazenar e organizar as equipes em uma lista duplamente encadeada.
- **`match.c` / `match.h`** — implementam e declaram o TAD `Match`, que representa uma partida por meio de seu identificador, dos IDs dos dois times e dos gols de cada equipe.
- **`matchlist.c` / `matchlist.h`** — implementam e declaram o TAD `MatchList`, que armazena as partidas em uma lista simplesmente encadeada e preserva a ordem em que são lidas do arquivo.
- **`Makefile`** — automatiza a compilação, a execução e a limpeza dos arquivos gerados.
- **`times.csv`** — contém os identificadores e os nomes das dez equipes do campeonato.
- **`partidas.csv`** — contém os resultados que serão apurados e deve permanecer no mesmo diretório do executável. Nesta entrega, começa com o cabeçalho, representando o campeonato não iniciado.
- **`partidas_vazio.csv`, `partidas_parcial.csv` e `partidas_completo.csv`** — arquivos oficiais dos três cenários de teste fornecidos para o trabalho. Para testar um cenário, copie o arquivo desejado para `partidas.csv`.
- **`classificacao.csv`** — é criado ou sobrescrito pelo programa com a classificação calculada.

## 3. TADs e estruturas de dados

O projeto utiliza quatro TADs para separar a representação das entidades do campeonato e as estruturas responsáveis por armazená-las.

### 3.1. TAD Team

O TAD `Team` representa uma equipe participante. Sua implementação mantém os seguintes dados:

- `id`: identificador numérico da equipe;
- `name`: nome da equipe, armazenado em memória dinâmica;
- `wins`: quantidade de vitórias;
- `draws`: quantidade de empates;
- `losses`: quantidade de derrotas;
- `goals_scored`: total de gols marcados;
- `goals_conceded`: total de gols sofridos.

O acesso aos dados é feito por funções próprias do TAD. A função `team_register_match()` atualiza as estatísticas de uma equipe conforme o placar de uma partida. As funções `team_goal_difference()` e `team_points()` calculam, respectivamente, o saldo de gols e a pontuação. Esses dois valores são calculados quando necessários e não ficam armazenados como campos da estrutura.

### 3.2. TAD TeamList — lista duplamente encadeada

O TAD `TeamList` armazena os times em uma **lista duplamente encadeada**. Cada nó mantém um ponteiro para um `Team`, um ponteiro para o próximo nó e outro para o nó anterior. A lista mantém referências ao primeiro e ao último nó, além da quantidade de elementos.

Entre as operações disponibilizadas estão:

- criação e destruição da lista;
- inserção de uma equipe no final;
- consulta da quantidade de equipes;
- acesso a uma equipe por posição;
- busca de uma equipe pelo ID;
- ordenação da lista segundo os critérios de classificação.

A ordenação é realizada reorganizando os nós da própria lista. Dessa forma, a implementação não substitui a estrutura exigida por um vetor ou por outra coleção dinâmica.

### 3.3. TAD Match

O TAD `Match` representa uma partida. Cada registro contém:

- `id`: identificador da partida;
- `team1` e `team2`: IDs das equipes participantes;
- `goals1` e `goals2`: gols marcados por cada equipe.

O TAD disponibiliza funções de criação, consulta dos campos, cálculo da diferença absoluta de gols e liberação da memória utilizada.

### 3.4. TAD MatchList — lista simplesmente encadeada

O TAD `MatchList` armazena as partidas em uma **lista simplesmente encadeada**. Cada nó contém um ponteiro para uma partida e um ponteiro para o próximo nó. A estrutura mantém referências ao primeiro e ao último elemento e registra o tamanho da lista.

A inserção ocorre no final, preservando a sequência de leitura do arquivo `partidas.csv`. Essa ordem também é utilizada para resolver empates na identificação da maior goleada: quando duas partidas apresentam a mesma diferença de gols, permanece a primeira encontrada no arquivo.

## 4. Formato dos arquivos CSV

Os arquivos de entrada devem estar no mesmo diretório em que o programa será executado. A primeira linha de cada arquivo é considerada cabeçalho e é ignorada durante a leitura.

### 4.1. `times.csv`

O cabeçalho esperado é:

```csv
ID,Nome
```

Em seguida, devem ser informados os dez times, com IDs de 0 a 9. O arquivo fornecido no projeto contém:

```csv
0,JAVAlis
1,ESCorpiões
2,SemCTRL
3,GOrilas
4,PYthons
5,SeQueLas
6,NETunos
7,LOOPardos
8,RUSTicos
9,REACTivos
```

### 4.2. `partidas.csv`

O cabeçalho esperado é:

```csv
ID,Time1,Time2,GolsTime1,GolsTime2
```

Cada linha representa uma partida e informa o ID do jogo, o ID do primeiro time, o ID do segundo time e os gols marcados por cada um. Os IDs dos times precisam corresponder aos cadastrados em `times.csv`.

Na leitura, o programa desconsidera linhas que não correspondem ao formato esperado, partidas com gols negativos, confrontos de um time contra ele mesmo, IDs de times fora do intervalo de 0 a 9, IDs de partidas repetidos, confrontos repetidos e partidas que indiquem IDs de equipes inexistentes. Para o cadastro dos times, são exigidos dez IDs únicos entre 0 e 9. As linhas inválidas são acompanhadas de mensagens de aviso.

**Atenção:** o `partidas.csv` incluído nesta pasta contém somente o cabeçalho e representa o cenário de campeonato não iniciado. Os três arquivos oficiais de teste também estão incluídos na pasta. Para testar cada cenário, copie o arquivo correspondente para `partidas.csv`, mantendo o cabeçalho e o formato indicado. O programa limita a apuração a 45 confrontos únicos.

### 4.3. `classificacao.csv`

Depois da apuração, o programa gera o arquivo `classificacao.csv`, com as colunas:

```csv
Pos,ID,Time,V,E,D,GM,GS,S,PG
```

As colunas representam a posição, o identificador e o nome do time, vitórias, empates, derrotas, gols marcados, gols sofridos, saldo de gols e pontos.

## 5. Funcionamento da apuração e critérios de classificação

O processamento segue esta sequência:

1. Cria as estruturas `TeamList` e `MatchList`.
2. Lê `times.csv` e cria os dez objetos `Team`.
3. Lê `partidas.csv`, verifica os dados de cada linha e localiza as equipes envolvidas.
4. Cria um objeto `Match` para cada partida aceita e o adiciona à `MatchList`.
5. Atualiza as estatísticas dos dois times envolvidos no resultado.
6. Ordena a lista de equipes conforme os critérios do campeonato.
7. Exibe a tabela e o resumo no terminal.
8. Gera `classificacao.csv` e libera a memória alocada.

A pontuação é calculada da seguinte forma:

- Vitória: 3 pontos.
- Empate: 1 ponto.
- Derrota: 0 ponto.

O saldo de gols é calculado subtraindo os gols sofridos dos gols marcados (`GM - GS`).

A classificação utiliza os critérios abaixo, nesta ordem:

1. Maior pontuação (`PG`).
2. Maior número de vitórias (`V`).
3. Maior saldo de gols (`S`).
4. Maior número de gols marcados (`GM`).
5. Menor ID do time.

## 6. Resumo do campeonato

O programa apresenta a quantidade de partidas apuradas em relação ao total previsto de 45 jogos. Quando as 45 partidas válidas e sem confrontos repetidos são contabilizadas, o primeiro colocado é apresentado como **campeão**. Quando o campeonato ainda está incompleto, é apresentado como **líder**.

O resumo também informa:

- **Melhor ataque:** equipe com o maior número de gols marcados;
- **Melhor defesa:** equipe com o menor número de gols sofridos;
- **Maior goleada:** partida com a maior diferença absoluta de gols entre as equipes.

Em caso de empate no melhor ataque ou na melhor defesa, é mantida a equipe que aparece primeiro na classificação, ou seja, a mais bem colocada segundo os critérios de ordenação. Em caso de empate na maior diferença de gols, é mantida a primeira partida encontrada na ordem do arquivo.

Se nenhuma partida válida for apurada, o programa informa a quantidade de jogos disputados e apresenta a mensagem de que nenhuma partida foi disputada, sem exibir os demais destaques.

## 7. Compilação e execução

### 7.1. Pré-requisitos

- Sistema Linux ou ambiente Ubuntu/WSL;
- GCC (compilador C);
- GNU Make.

No Ubuntu/WSL, caso as ferramentas ainda não estejam instaladas, pode-se utilizar:

```bash
sudo apt update
sudo apt install build-essential -y
```

### 7.2. Compilar

Dentro da pasta do projeto, execute:

```bash
make
```

O comando compila os arquivos-fonte e gera o executável `campeonato`.

Também é possível solicitar a compilação pelo alvo específico:

```bash
make compile
```

### 7.3. Executar

Com os arquivos `times.csv` e `partidas.csv` preparados, execute:

```bash
./campeonato
```

Ou utilize o alvo de execução do Makefile:

```bash
make run
```

Ao terminar, a classificação será exibida no terminal e gravada em `classificacao.csv`.

### 7.4. Limpar arquivos gerados

Para remover os arquivos objeto, o executável e o CSV de classificação gerado:

```bash
make clean
```

## 8. Principais decisões de implementação

- **Separação por TADs:** as equipes, as partidas e suas respectivas listas foram divididas em módulos próprios, com interfaces declaradas em arquivos `.h` e implementações em arquivos `.c`.
- **Lista duplamente encadeada para os times:** foi utilizada a estrutura exigida para `TeamList`. A ordenação altera as ligações entre os nós, sem converter a coleção em vetor.
- **Lista simplesmente encadeada para as partidas:** `MatchList` mantém a ordem de leitura dos resultados, necessária para preservar a prioridade da primeira partida em empates na maior goleada.
- **Pontuação e saldo calculados:** `PG` e `S` são obtidos por funções do TAD `Team`, evitando armazenar informações derivadas que podem ser calculadas a partir das estatísticas acumuladas.
- **Critérios de desempate centralizados:** a comparação entre equipes segue a ordem definida no enunciado, incluindo o menor ID como último critério.
- **Gerenciamento de memória:** as funções de destruição percorrem as listas, liberam os nós e também os objetos armazenados. O nome de cada equipe, alocado dinamicamente, é liberado junto com o respectivo `Team`.
- **Compilação com avisos:** o Makefile utiliza `-Wall`, `-Wextra`, `-std=c11` e `-pedantic` para solicitar avisos do compilador e seguir o padrão C11.

## 9. Testes realizados

Os três cenários oficiais disponibilizados para o trabalho foram executados com os arquivos correspondentes:

- **Campeonato não iniciado (`partidas_vazio.csv`):** 0 de 45 partidas; tabela zerada e ordenada pelo ID; mensagem de nenhuma partida disputada.
- **Campeonato em andamento (`partidas_parcial.csv`):** 20 de 45 partidas; líder NETunos com 10 pontos.
- **Campeonato encerrado (`partidas_completo.csv`):** 45 de 45 partidas; campeão NETunos com 17 pontos, melhor ataque com 21 gols marcados, melhor defesa com 9 gols sofridos e maior goleada ESCorpiões 0 x 5 RUSTicos.

Nos três cenários, o programa terminou sem erros e gerou o arquivo `classificacao.csv` com cabeçalho e dez equipes. A compilação também foi realizada com `-Wall -Wextra -std=c11 -pedantic`, sem avisos.

Para repetir os testes, copie o arquivo de cenário desejado para `partidas.csv`, mantendo `times.csv` no mesmo diretório, e execute `make` seguido de `./campeonato`.
