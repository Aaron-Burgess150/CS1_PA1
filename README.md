# CS1_PA1
 
Problem Description: Soccer Tournament
Monster Country is organizing a large international soccer tournament, which will feature multiple matches.
Each match involves two teams, and each team consists of several players. The country aims to develop a system to store tournament
data, facilitate queries, and perform data analytics.
Your task is to write a program to handle this scenario, with a particular focus on dynamic memory allocation (DMA).
Please note that there are specific constraints imposed on this assignment, primarily to help you better understand DMA.
While alternative solutions could be used for some functionalities, our primary goal is not only to solve the problem but to
do so in a way that enhances your understanding of the class material.

Requirements:
A. Struct Definitions: You must use the following struct definitions. You are not allowed to modify these structs.
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

//this is a special struct that should only be used to process a particular command
(mentioned below in the function requirement)
typedef struct MatchWinnerTeam{
   Team *winnerTeam; //pointer to the winner team if any.
   int isDraw; //set to 1 if it was a draw. In that case winnerTeam will be NULL
}MatchWinnerTeam;

B. Commands:
The system should support the following commands:
a. max_goal_player: This command should output the name and total goals of the player who has scored the most
goals across all players. In case of a tie for the highest goal count, the player who appears first in the input should
be printed. It is guaranteed that there will be at least one player in the system before issuing this command.

b. winner <teamName1> <teamName2>: This command finds the winner of the match between
<teamName1> and <teamName2>. You can assume <teamName1> will be always alphabetically smaller than
<teamName2>. This command will print one of the following messages
   • If there is a winner: “winner of <teamName1> vs <teamName2> is
     <winnerName>”, where <winnerName> is the name of the team who won out of these two
     teams.
   • If the match was drawn: “<teamName1> vs <teamName2> match drawn”
   • If the there was not match between the give teams, then it should print: “no match between
     <teamName1> and <teamName2>”

c. hattrick: A hat trick is an accomplishment where a player scores three goals in a single match. This command
will display all the matches and the players who achieved a hat trick during the tournament.
   • If there is no hat trick, it should print
      “no hat trick found in the tournament”
   • If there are one on more hat trick, it should print the result in the following format:
       Hat trick found in <teamName1> vs <teamname2>
       <player name1> <total goals by the player in the tournament>
       <player name2> <total goals by the player in the tournament>
       <player name n> <total goals by the player in the tournament>
       //similarly it should display the information for other matches if there are hat tricks in other matches.

d. champion: The team with the highest point is the champion of this tournament. This command prints the
champion of the tournament in the format: “champion is <team name>”. If multiple teams scored the
champion points, then the first team based on the inputs will be printed. In order to find the champion, use the
following point system to award points to a team:
   • Win: 3 points
   • Draw: 1 point
   • Loss: 0 points

e. There are many other potential queries that could be explored, but we will stop here to prevent the assignment from
becoming too lengthy

C. Function requirements
   • You are required to implement the following functions with the prototypes mentioned below and must use them while
     processing the commands/queries and other operations in your code. You can add more functions as needed to better
     organize your code as well.:

1. Player* createPlayer(char *name): Dynamically allocates a player, stores the name in a dynamically
allocated string, initializes the total goals to zero, and returns the player pointer

2. Team* createTeam(char *name, int playerCount): This function takes the team name and the number of
players in the team, dynamically allocates memory for the team and the required arrays, and fills them with the
corresponding information. However, while it allocates memory for the players, it does not assign the players
to the team.

3. Player* getPlayerByName(char* name, Player** playerList, int numPlayers): Returns the player pointer
in the playerList with the given name passed to this function. If the player is not found, it should return NULL

4. Team* getTeamByTeamName(Team** teamList, int numTeams, char *tname): Returns the Team pointer
in the teamList with the given team name passed to this function. If the team is not found, it should return NULL

5. void recordGoal(Match *match, Player *player, int time): It receives the reference to a match, reference to
a player, add it to the goal list for this match, and updates necessary variables. [Hint: you can use realloc to
increase the size of the goals array of the match!]

6. int printHatTrickPlayers( Match *match): This function accepts a reference to a match and prints the match
details along with the name of any player who has scored a hat trick, if applicable. It then returns an integer
value—1 if a hat trick was found in the match, and 0 if no hat trick was achieved.
   ▪ Hints: This function could be a bit tricky for some of you. Here I am giving you some hints:
   ▪ Note that you are processing one match here and you want to count how many goals are scored by each
     player in them. So, it would be good idea to keep a list of players and their goal counts.
   ▪ Allocate an array of player pointers (let’s say pp) based on the goal count of this match (because there
     cannot be more than goal count scorer in a match!)
   ▪ Allocate an array of int (let’s say gc) to store the goal count for scorers. The size of this array should
     be same as above. Think, should you use calloc or malloc here.
   ▪ Use a tracker variable to keep track of how many index of the pp or gc is used
   ▪ For each goal in the match, get the scorer, if the scorer is not in the pp, add it to the pp, update gc for
     that scorer. If the scorer is already in pp, then just update the gc for that scorer.
   ▪ After processing all the goals of this match, then use the gc array for hat trick decision

7. MatchWinnerTeam* getWinningTeam(Match *matches, int matchCount, char *team1Name, char
*team2Name): This function receives the list of matches, match count, and two team names as strings. It returns
a dynamically allocated MatchWinnerTeam struct that stores the winner of the match played by teams or
indicates if it was a draw or not. The value of this struct is discussed in the struct definition above.

8. Team* getChampion(Team **teamList, int teamCount, Match *matchList, int matchCount): It returns
the champion of the tournament as defined the champion command description.
   ▪ This also could be tricky for many of you.
   ▪ Hints: For each match, you can use the getWinningTeam() function to know the win, loss, and draw
   ▪ Use a points array to track the points of each team and use the same index for the team based on
     teamList.

9. You must also write the necessary functions to free all the memory

10. You are welcome to add any additional functions if you wish, though it is not required.

D. Additional Restrictions and other coding requirements and hints (do not skip!):
a. Only one instance of each player and team should be created, and their pointers should be used consistently
throughout the program. In other words, there should be only one malloc call for each player.

b. Your code design must not use triple pointers (***). You should limit the pointer usage to single or double
pointers for simplicity and clarity.

c. All strings used in the assignment will consist of single words, all in lowercase letters and underscores. The
maximum length of a string is 20 characters.

d. For strings that are not statically allocated, you must dynamically allocate exactly the necessary amount of
memory and use the strcpy function to store the string like the way we have done in the class and discussed
in recordings. Any alternative approach for handling strings will result in a -100 penalty for your submission.

e. You must free all the memory to receive full credit.

f. The leak detector you have used for your lab 2 is great for day-to-day testing. However, including that leak
detector slows down your code and it can take pretty long if the input size is big and if you have several dynamic
memory allocations on your code. So, to test your code, we will use valgrind command inside codegrade. If
you want to know more about valgrind command, you can watch a video posted on the DMA module. For your
own testing, you can still use the memory leak detector, however, DO NOT INCLUDE leak_detecotor_c.h on
your code for programming assignment.

g. You do not need to comment line by line but comment every function and every “paragraph” of code.

h. You don’t have to hold any particular indentation standard, but you must indent and you must do so consistently
within your own code.

i. Make sure your file name is main.c, otherwise it will not compile on codegrade

j. Your code must work on codegrade and the output format must match to receive credit for a particular test case

k. You must maintain a player pointer list and team list in the main function
   i. Player** playerList
   ii. Team** teamList
