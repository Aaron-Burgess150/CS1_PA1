/* COP 3502C Assignment 1
This program is written by: Aaron Burgess */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player {
    char *name; //Player name (dynamically allocated)
    int totalGoals; //Total goals scored by the player(needs to be counted based on the match)
}Player;

typedef struct Team {
    char *name; // Name of the team (dynamically allocated)
    Player **players; //An array of Player pointers where each pointer is pointing to a player
    int playerCount; // Number of players in the team
}Team;

typedef struct Goal {
    Player *scorer; // Pointer to the player who scored
    int time; // Time of the goal in minutes
}Goal;

typedef struct Match {
    Team *team1; // Pointer to Team 1
    Team *team2; // Pointer to Team 2
    Goal *goals; // Array of goals scored in the match
    int goalCount; // Total number of goals in the match
} Match;

typedef struct MatchWinnerTeam
{
    Team *winnerTeam; //pointer to the winner team if any.
    int isDraw; //set to 1 if it was a draw. In that case winnerTeam will be NULL
}MatchWinnerTeam;

//function prototypes
Player* createPlayer(char *name);
Team* createTeam(char *name, int playerCount);
Player* getPlayerByName(char* name, Player** playerList, int numPlayers);
Team* getTeamByTeamName(Team** teamList, int numTeams, char *tname);
void recordGoal(Match *match, Player *player, int time);
int printHatTrickPlayers( Match *match);
MatchWinnerTeam* getWinningTeam(Match *matches, int matchCount, char *team1Name, char *team2Name);
Team* getChampion(Team **teamList, int teamCount, Match *matchList, int matchCount);
//free functions for all dynamically allocated memory in each function
void freePlayer(Player *p);
void freeTeam(Team *t);
void freeGoal(Goal *g);
void freeMatch(Match *match);
void freeMatchWinnerTeam(MatchWinnerTeam *winnerTeam);

#define STRINGMAX 21

Player* createPlayer(char *name) { //needs free function to free space for name and player itself
    //allocate memory for player
    Player *player = (Player *)malloc(sizeof(Player));
    if (player == NULL) {
        printf("Space couldn't be allocated for the player in the createPlayer function!");
        return NULL;
    }

    //allocate memory for player's name and assign name
    player->name = (char *)malloc(strlen(name) + 1);
    if (player->name == NULL) {
        printf("Space couldn't be allocated for the player's name in the createPlayer function!");
        free(player);
        return NULL;
    }
    strcpy(player->name, name);
    //make new player's total goals 0
    player->totalGoals = 0;

    return player;
}

Team* createTeam(char *name, int playerCount) { //needs free function for the team players array, team name, and team itself
    //allocate memory for team
    Team *team = (Team *)malloc(sizeof(Team));
    if (team == NULL) {
        printf("Space couldn't be allocated for the team in the createTeam function!");
        return NULL;
    }

    //allocate memory for the team name and assign name
    team->name = (char *)malloc(strlen(name) + 1);
    if (team->name == NULL) {
        printf("Space couldn't be allocated for the team's name in the createTeam function!");
        free(team);
        return NULL;
    }
    strcpy(team->name, name);
    //assign the team with the number of players
    team->playerCount = playerCount;

    //assign memory for the array of players on the team (no need to name players)
    team->players = (Player **)malloc(team->playerCount * sizeof(Player *));
    if (team->players == NULL) {
        printf("Space couldn't be allocated for the team's roster in the createTeam function!");
        free(team->name);
        free(team);
        return NULL;
    }

    return team;
}

Player* getPlayerByName(char* name, Player** playerList, int numPlayers){
    for (int i = 0; i < numPlayers; i++) { //parse through array searching for player name
        if (strcmp(name, playerList[i]->name) == 0) {
            return playerList[i];
        }
    }
    return NULL; //name not found
}

Team* getTeamByTeamName(Team** teamList, int numTeams, char *tname) {
    for (int i = 0; i < numTeams; i++) { //parse through array searching for team name
        if (strcmp(tname, teamList[i]->name) == 0) {
            return teamList[i];
        }
    }
    return NULL; //name not found
}

void recordGoal(Match *match, Player *player, int time) { //might need a free method
    //update match->goals array (time and scorer), goalCount, and player's totalGoals
    match->goals = (Goal*)realloc(match->goals, (match->goalCount + 1) * sizeof(Goal));
    if (match->goals == NULL) {
        printf("Space couldn't be allocated for the match's goal in goals array in the recordGoal function!");
    }
    match->goalCount = match->goalCount + 1;
    match->goals[match->goalCount - 1].time = time;
    match->goals[match->goalCount - 1].scorer = player;
    player->totalGoals = player->totalGoals + 1;

}

int printHatTrickPlayers(Match *match) {
    /*
     * make an int to return (1 if there is a hat trick, 0 if no hat trick)
     * make a list of scorers (Player **pp)
     * make a list of number of goals per scorer (int *gc = (int*)calloc(number of scores, sizeof(int)) )
     * use a tracker variable to keep track of how many index of the pp or gc is used
     * when a score happens
     *      if the scorer isn't in pp, then add them to pp and update gc
     *      else (they are alr in pp), then just update gc
     *
     * parse through gc to see if there are characters with 3 or more goals in the match
     *      if there is a hat trick, change the returned int to 1
     *      print all players with a hat trick
     * return the int (0 if no hat trick)
     */

    int isHatTrick = 0;
    int tracker = 0;//to keep track of index of goal count and players
    Player *newPlayer; //player

    Player **pp = (Player**)malloc(match->goalCount * sizeof(Player*));
    if (pp == NULL) {
        printf("Space couldn't be allocated for pp in printHatTrickPlayers function!");
        return -1;
    }
    /*
    for (int i = 0; i < match->goalCount; i++) { //for every player, allocate space for the player
        pp[i] = (Player *)malloc(sizeof(Player));
        if (pp[i] == NULL) {
            printf("Space couldn't be allocated for pp[%d] in printHatTrickPlayers", i);
            return -1;
        }
    }
    */

    int *gc = (int*)calloc(match->goalCount, sizeof(int));
    if (gc == NULL) {
        printf("Space couldn't be allocated for gc in the printHatTrickPlayers function!");
        return -1;
    }

    for (int i = 0; i < match->goalCount; i++){ //for each goal
        newPlayer = match->goals[i].scorer;
        int exists = 0; // did this player already score a goal and is already in pp
        for (int j = 0; j < tracker; j++) { //for each player in pp
            if (strcmp(newPlayer->name, pp[j]->name) == 0) { //if newPlayer is in pp
                gc[j]++;
                exists = 1;
                break;
            }
        }
        if (exists == 0){ //newPlayer isn't in pp so we add him to pp
            pp[tracker] = newPlayer;
            gc[tracker]++;
            tracker++;
        }
    }

    for (int i = 0; i < tracker; i++) { //for every player in pp (indexes corresponds to gc)
        if (gc[i] > 2) {
            if (isHatTrick ==0) {
                printf("hat trick found in %s vs %s:\n", match->team1->name, match->team2->name);
            }
            isHatTrick = 1;
            printf("%s %d\n", pp[i]->name, pp[i]->totalGoals);
        }
    }

    //free everything
    free(gc);
    free(pp);

    return isHatTrick;
}

MatchWinnerTeam* getWinningTeam(Match *matches, int matchCount, char *team1Name, char *team2Name) { //needs a free function with imbedded team free (void freeWinningTeam(MatchWinnerTeam * winner) )
    MatchWinnerTeam *winner = (MatchWinnerTeam *)malloc(sizeof(MatchWinnerTeam));
    if (winner == NULL) {
        printf("Space couldn't be allocated for the winner in the MatchWinnerTeam function!");
        return NULL;
    }

    int match_index = -1;
    for (int i = 0; i < matchCount; i++) { //find the right match in the matches array
        if (((strcmp(matches[i].team1->name, team1Name) == 0) && (strcmp(matches[i].team2->name, team2Name) == 0)) || ((strcmp(matches[i].team1->name, team2Name) == 0) && (strcmp(matches[i].team2->name, team1Name)) == 0)){
            match_index = i;
        }
    }

    if (match_index == -1) { //no match in matches
        winner->winnerTeam = NULL;
        winner->isDraw = 0;
        return winner;
    }

    //keep track of teams and scores
    Team *t1 = matches[match_index].team1;
    Team *t2 = matches[match_index].team2;
    int t1_score = 0;
    int t2_score = 0;

    for (int i = 0; i < matches[match_index].goalCount; i++) { //for every goal in the match
        Player *scored = matches[match_index].goals[i].scorer; //player that scored that goal

        for (int j = 0; j<t1->playerCount; j++) {//for loop to go through t1 to see if scorer is on that team
            if (strcmp(t1->players[j]->name, scored->name) ==0) {//if he's on t1 --> t1_score++
                t1_score++;
            }
        }

        for (int j = 0; j<t2->playerCount; j++) { //for loop to go through t2 to see if scorer is on that team
            if (strcmp(t2->players[j]->name, scored->name) == 0) { //if he's on t2 --> t2_score++
                t2_score++;
            }
        }
    }

    if (t1_score > t2_score) {
        winner->winnerTeam = t1;
        winner->isDraw = 0; //may have to take out
    }
    else if (t1_score < t2_score) {
        winner->winnerTeam = t2;
        winner->isDraw = 0; //may have to take out
    }
    else {
        winner->winnerTeam = NULL;
        winner->isDraw = 1;
    }

    return winner;
}

Team* getChampion(Team **teamList, int teamCount, Match *matchList, int matchCount) {
    /*
     * make an int array to hold the number of champion points for each team (indexes match up with indexes in teamList)
     * parse through the matchList to get who won said match (or if there is a draw)
     *      make an MatchWinnerTeam pointer to hold the winner of each match ( = getWinningTeam(matchList, matchCount, matchList[i].team1, matchList[i].team2); )
     *      if winner->winnerTeam == NULL, then parse through teamList to find index of matchList[i].team1 (index1) and matchList[i].team2 (index2) and give both indexes in champPoints +1 points
     *      else parse through teamList to find index of winner->winnerTeam and give matching index in champPoints +3 points
     *      freeWinningTeam(winner);
     * go through champion points array and see which team has the highest number of champion  (Team *champion)
     * free champion points array (champPoints)
     * return champion
     */

    int *champPoints = (int *)calloc(teamCount, sizeof(int)); //array to hold champ points of teams
    if (champPoints == NULL) {
        printf("Space couldn't be allocated for champPoints array in the getChampion function!");
        return NULL;
    }

    for (int i = 0; i < matchCount; i++) { //for every match
        MatchWinnerTeam *winner = getWinningTeam(matchList, matchCount, matchList[i].team1->name, matchList[i].team2->name); //find the winner
        if ((winner->winnerTeam == NULL) && (winner->isDraw == 1)) { //no winner and match exists
            for (int j = 0; j < teamCount; j++) {
                if (strcmp(teamList[j]->name, matchList[i].team1->name) == 0) { //give team1 1 champPoint if there was a draw in the match
                    champPoints[j] += 1;
                }
            }

            for (int j = 0; j < teamCount; j++) {
                if (strcmp(teamList[j]->name, matchList[i].team2->name) == 0) { //give team2 1 champPoint if there was a draw in the match
                    champPoints[j] += 1;
                }
            }
        }
        else if (winner->winnerTeam != NULL){ //there is a winner
            for (int j = 0; j < teamCount; j++) {
                if (strcmp(teamList[j]->name, winner->winnerTeam->name) == 0){ //give winning team 3 champPoints since there was a winner
                    champPoints[j] += 3;
                }
            }
        }
        freeMatchWinnerTeam(winner);
    }

    int highestScore = 0;
    for (int i = 0; i < teamCount; i++) { //go through the champPoints array to see who has the highest number of them
        if (champPoints[i] > champPoints[highestScore]) {
            highestScore = i;
        }
    }

    free(champPoints);

    Team *champion = teamList[highestScore];
    return champion;
}

void freePlayer(Player *p) {
    free(p->name);
    free(p);
}

void freeTeam(Team *t) {
    free(t->name);
    for (int i = 0; i < t->playerCount; i++) {
        freePlayer(t->players[i]);
    }
    free(t->players);
    free(t);
}

void freeGoal(Goal *g) {
    //freePlayer(g->scorer);
    //free(g);
}

void freeMatch(Match *match) {
    //freeTeam(match->team1);
    //freeTeam(match->team2);
    if (match->goals != NULL){
        free(match->goals);
    }
    free(match);
}

void freeMatchWinnerTeam(MatchWinnerTeam *winnerTeam) {
    //freeTeam(winnerTeam->winnerTeam);
    free(winnerTeam);
}

int main(void) {
    int t = 0, m = 0, c = 0; //num of teams, num of matches, num of commands
    scanf("%d %d %d\n", &t, &m, &c);

    Player **playerList = NULL; //THIS MUST BE FREED bc realloc when assigning players
    Team **teamList = (Team**)malloc(t * sizeof(Team*)); //THIS MUST BE FREED

    if (teamList == NULL) {
        printf("Space couldn't be allocated for teamList in the main function");
        return 1;
    }

    int playerTotal = 0;
    int tracker_playerTotal = 0;
    for (int i = 0; i < t; i++) { //for loop to fill info for all teams
        char name[STRINGMAX];
        int playerCount;

        scanf("%s %d\n", name, &playerCount);
        playerTotal += playerCount; //new player total after player count of team is added
        playerList = (Player**)realloc(playerList, playerTotal * sizeof(Player*)); //realloc to add space

        if (playerList == NULL) {
            printf("Space couldn't be allocated for playerList in the main function! (realloc)");
            return 1;
        }

        teamList[i] = createTeam(name, playerCount); //new team

        for (int j = 0; j < teamList[i]->playerCount; j++) { //for loop for each player on the team
            scanf("%s\n", name);
            Player *player = createPlayer(name);
            teamList[i]->players[j] = player;
            playerList[tracker_playerTotal] = player;
            tracker_playerTotal++;
        }
    }

    Match *matchList = (Match*)malloc(m * sizeof(Match)); //THIS MUST BE FREED, array of matches, not match pointers
    if (matchList == NULL) {
        printf("Space couldn't be allocated for matchList in the main function");
        return 1;
    }

    for (int i = 0; i < m; i++) { //scan for all the matches
        char team1[STRINGMAX];
        char team2[STRINGMAX];

        scanf("%s %s", team1, team2);
        Team *t1 = getTeamByTeamName(teamList, t, team1); //find t1
        Team *t2 = getTeamByTeamName(teamList, t, team2); //find t2

        Match match;
        match.team1 = t1;
        match.team2 = t2;
        match.goals = NULL;
        match.goalCount = 0;

        while (1) { //keeps scanning for goals in the match
            char scorer[STRINGMAX];
            int time;
            scanf("%s", scorer);
            if (strcmp("end", scorer) == 0) {
                break;
            }
            scanf("%d", &time);
            Player *player_scorer = getPlayerByName(scorer, playerList, playerTotal);
            recordGoal(&match, player_scorer, time);
        }
        matchList[i] = match;
    }

    //commands
    for (int i = 0; i < c; i++) { //for loop for all the commands
        char command[STRINGMAX];
        scanf("%s", command);
        if (strcmp("max_goal_player", command) == 0) {
            int highestScore = 0;
            for (int j = 0; j < playerTotal; j++) { //for every player in playerList
                if (playerList[j]->totalGoals > playerList[highestScore]->totalGoals){
                    highestScore = j;
                }
            }
            printf("player with max goal %s %d\n", playerList[highestScore]->name, playerList[highestScore]->totalGoals);
        }

        else if (strcmp("winner", command) == 0) {
            char t1[STRINGMAX];
            char t2[STRINGMAX];
            scanf("%s %s", t1, t2);
            MatchWinnerTeam *winningTeam = getWinningTeam(matchList, m, t1, t2);

            if (winningTeam->winnerTeam != NULL) {
                printf("winner of %s vs %s is %s\n", t1, t2, winningTeam->winnerTeam->name);
            }
            else if ((winningTeam->winnerTeam == NULL) && (winningTeam->isDraw == 0)) { //no match found
                printf("no match between %s and %s\n", t1, t2);
            }
            else if ((winningTeam->winnerTeam == NULL) && (winningTeam->isDraw == 1)) { //match with a draw
                printf("%s vs %s match drawn\n", t1, t2);
            }
            free(winningTeam);

        }

        else if (strcmp("hattrick", command) == 0) {
            int isHatTrick = 0;
            for (int j = 0; j<m; j++) { //for all the matches
                if (printHatTrickPlayers(&matchList[j])){
                    isHatTrick = 1;
                }
            }

            if (isHatTrick == 0) {
                printf("no hat trick found in the tournament\n");
            }
        }

        else if (strcmp("champion", command) == 0) {
            Team *champion = getChampion(teamList, t, matchList, m);
            printf("champion is %s\n", champion->name);
        }
    }


    //freeing everything
    for(int i=0; i<t;i++){
        freeTeam(teamList[i]);
    }
    free(teamList);

    free(playerList);
    for(int i=0; i<m; i++){
        free(matchList[i].goals);
    }
    free(matchList);
    return 0;
}
