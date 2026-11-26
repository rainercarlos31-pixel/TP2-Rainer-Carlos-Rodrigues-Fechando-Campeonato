#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "team.h"
#include "teamlist.h"
#include "match.h"
#include "matchlist.h"

#define TOTAL_TEAMS 10
#define TOTAL_MATCHES 45
#define LINE_SIZE 512
#define NAME_WIDTH 16

typedef struct {
    int id;
    int team1;
    int team2;
    int goals1;
    int goals2;
} MatchData;

static int load_teams(const char *filename, TeamList *teams) {
    FILE *file = fopen(filename, "r");
    char line[LINE_SIZE];
    char *comma, *end;
    int id, count = 0;
    Team *team;

    if (file == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir %s.\n", filename);
        return 0;
    }
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        comma = strchr(line, ',');
        if (comma == NULL) continue;
        *comma = '\0';
        id = (int) strtol(line, &end, 10);
        if (line[0] == '\0' || *end != '\0' || id < 0 || id >= TOTAL_TEAMS ||
            comma[1] == '\0' || tl_find_id(teams, id) != NULL) {
            fprintf(stderr, "Aviso: linha invalida ou ID repetido em %s.\n", filename);
            continue;
        }
        team = team_create(id, comma + 1);
        if (team == NULL || !tl_append(teams, team)) {
            if (team != NULL) team_free(team);
            fclose(file);
            return 0;
        }
        count++;
    }
    fclose(file);
    return count == TOTAL_TEAMS;
}

static int load_matches(const char *filename, TeamList *teams, MatchList *matches) {
    FILE *file = fopen(filename, "r");
    char line[LINE_SIZE], extra;
    MatchData data;
    Match *match;
    Team *team1, *team2;
    int count = 0, i;
    int played_pair[TOTAL_TEAMS][TOTAL_TEAMS] = {{0}};
    int match_ids[TOTAL_MATCHES];

    if (file == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir %s.\n", filename);
        return -1;
    }
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d,%d,%d,%d,%d %c", &data.id, &data.team1,
                   &data.team2, &data.goals1, &data.goals2, &extra) != 5) {
            if (line[0] != '\n' && line[0] != '\r' && line[0] != '\0')
                fprintf(stderr, "Aviso: linha de partida invalida ignorada.\n");
            continue;
        }
        if (data.team1 < 0 || data.team1 >= TOTAL_TEAMS ||
            data.team2 < 0 || data.team2 >= TOTAL_TEAMS ||
            data.team1 == data.team2 || data.goals1 < 0 || data.goals2 < 0) {
            fprintf(stderr, "Aviso: partida %d possui dados invalidos e foi ignorada.\n", data.id);
            continue;
        }
        if (count >= TOTAL_MATCHES || played_pair[data.team1][data.team2]) {
            fprintf(stderr, "Aviso: partida %d excedente ou confronto repetido; ignorada.\n", data.id);
            continue;
        }
        for (i = 0; i < count; i++) {
            if (match_ids[i] == data.id) break;
        }
        if (i < count) {
            fprintf(stderr, "Aviso: ID de partida %d repetido; partida ignorada.\n", data.id);
            continue;
        }
        team1 = tl_find_id(teams, data.team1);
        team2 = tl_find_id(teams, data.team2);
        if (team1 == NULL || team2 == NULL) {
            fprintf(stderr, "Aviso: partida %d ignorada (ID de time inexistente).\n", data.id);
            continue;
        }
        match = match_create(data.id, data.team1, data.team2, data.goals1, data.goals2);
        if (match == NULL || !ml_append(matches, match)) {
            if (match != NULL) match_free(match);
            fclose(file);
            return -1;
        }
        match_ids[count] = data.id;
        played_pair[data.team1][data.team2] = 1;
        played_pair[data.team2][data.team1] = 1;
        team_register_match(team1, data.goals1, data.goals2);
        team_register_match(team2, data.goals2, data.goals1);
        count++;
    }
    fclose(file);
    return count;
}

/* Conta caracteres UTF-8 para manter a coluna alinhada no terminal. */
static int utf8_length(const char *text) {
    int length = 0;
    while (*text != '\0') {
        if (((unsigned char) *text & 0xC0) != 0x80) length++;
        text++;
    }
    return length;
}

static void print_name(const char *name) {
    int padding = NAME_WIDTH - utf8_length(name);
    printf("%s", name);
    while (padding-- > 0) putchar(' ');
}

static void print_table(TeamList *teams) {
    int i;
    Team *team;
    printf("%-4s %-3s %-16s %3s %3s %3s %4s %4s %4s %4s\n",
           "Pos", "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
    for (i = 0; i < tl_size(teams); i++) {
        team = tl_get(teams, i);
        printf("%-4d %-3d ", i + 1, team_id(team));
        print_name(team_name(team));
        printf(" %3d %3d %3d %4d %4d %4d %4d\n",
               team_wins(team), team_draws(team), team_losses(team),
               team_goals_scored(team), team_goals_conceded(team),
               team_goal_difference(team), team_points(team));
    }
}

static int save_classification(const char *filename, TeamList *teams) {
    FILE *file = fopen(filename, "w");
    int i;
    Team *team;
    if (file == NULL) {
        fprintf(stderr, "Erro: nao foi possivel criar %s.\n", filename);
        return 0;
    }
    fprintf(file, "Pos,ID,Time,V,E,D,GM,GS,S,PG\n");
    for (i = 0; i < tl_size(teams); i++) {
        team = tl_get(teams, i);
        fprintf(file, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%d\n", i + 1,
                team_id(team), team_name(team), team_wins(team), team_draws(team),
                team_losses(team), team_goals_scored(team), team_goals_conceded(team),
                team_goal_difference(team), team_points(team));
    }
    if (fclose(file) != 0) {
        fprintf(stderr, "Erro: falha ao finalizar %s.\n", filename);
        return 0;
    }
    return 1;
}

static void print_summary(TeamList *teams, MatchList *matches, int played) {
    int i, max_difference = -1;
    Team *leader, *best_attack, *best_defense, *team, *team1, *team2;
    Match *match, *biggest = NULL;

    printf("\n===== Resumo do campeonato =====\n");
    printf("Partidas disputadas: %d de %d\n", played, TOTAL_MATCHES);
    if (played == 0) {
        printf("Nenhuma partida foi disputada.\n");
        return;
    }

    leader = tl_get(teams, 0);
    best_attack = leader;
    best_defense = leader;
    for (i = 1; i < tl_size(teams); i++) {
        team = tl_get(teams, i);
        if (team_goals_scored(team) > team_goals_scored(best_attack)) best_attack = team;
        if (team_goals_conceded(team) < team_goals_conceded(best_defense)) best_defense = team;
    }
    for (i = 0; i < ml_size(matches); i++) {
        match = ml_get(matches, i);
        if (match_goal_difference(match) > max_difference) {
            max_difference = match_goal_difference(match);
            biggest = match;
        }
    }

    if (played == TOTAL_MATCHES)
        printf("Campeão: %s (%d pontos)\n", team_name(leader), team_points(leader));
    else
        printf("Líder: %s (%d pontos)\n", team_name(leader), team_points(leader));
    printf("Melhor ataque: %s (%d gols marcados)\n",
           team_name(best_attack), team_goals_scored(best_attack));
    printf("Melhor defesa: %s (%d gols sofridos)\n",
           team_name(best_defense), team_goals_conceded(best_defense));
    if (biggest != NULL) {
        team1 = tl_find_id(teams, match_team1(biggest));
        team2 = tl_find_id(teams, match_team2(biggest));
        printf("Maior goleada: %s %d x %d %s\n", team_name(team1),
               match_goals1(biggest), match_goals2(biggest), team_name(team2));
    }
}

int main(void) {
    TeamList *teams = tl_create();
    MatchList *matches = ml_create();
    int played;

    if (teams == NULL || matches == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        tl_free(teams);
        ml_free(matches);
        return 1;
    }
    if (!load_teams("times.csv", teams)) {
        fprintf(stderr, "Erro: times.csv deve conter exatamente os 10 times com IDs de 0 a 9, sem repeticao.\n");
        tl_free(teams);
        ml_free(matches);
        return 1;
    }
    played = load_matches("partidas.csv", teams, matches);
    if (played < 0) {
        tl_free(teams);
        ml_free(matches);
        return 1;
    }

    tl_sort(teams);
    print_table(teams);
    print_summary(teams, matches, played);
    if (!save_classification("classificacao.csv", teams)) {
        tl_free(teams);
        ml_free(matches);
        return 1;
    }
    ml_free(matches);
    tl_free(teams);
    return 0;
}
