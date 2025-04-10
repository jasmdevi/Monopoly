//MARK: Monopoly Game
/*Jasmine Narayan
EGN 3211 Section 02*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BOARD_SIZE 40
#define MAX_PLAYERS 4
#define STARTING_BALANCE 1000
#define GO_MONEY 200
#define LUXURY_TAX 100
#define JAIL_POSITION 10
#define GO_TO_JAIL_POSITION 30

typedef enum { 
    PROPERTY, 
    COMMUNITY_CHEST, 
    LUXURY_TAX_TILE, 
    GOTO_JAIL,
    EMPTY,
    GO 
} TileType;

typedef struct {
    char name[50];
    TileType type;
    int cost;
    int rent;
    int mortage;
    int owner;     // -1 if not owned, otherwise the player index
    int mortgaged; // 1 if mortgaged, 0 if not
} Tile;

typedef struct {
    char symbol;
    int balance;
    int position;
    int in_jail;
    int jail_turns;
    int properties_owned[BOARD_SIZE];
    int total_rolls;
    int total_roll_value;
    int times_in_jail;
} Player;

//MARK: Function Declaration
void debugGameState(Player players[], int numPlayers, Tile board[], int rounds);
void initializeBoard(Tile board[]);
void initializePlayers(Player players[], int numPlayers);
void rollDice(int* die1, int* die2);
void movePlayer(Player* player, int roll);
void handleTile(Player* player, Tile board[], Player players[], int numPlayers, int rounds);
void handleJail(Player* player, int roll);
void printBoard(Tile board[], Player players[], int numPlayers);
void printStatistics(Player players[], int numPlayers, int rounds, Tile board[]);
int checkGameOver(Player players[], int numPlayers, Tile board[]);

//MARK: Debug Game State
/**
 * @brief Prints the game state for debugging purposes, including the player's jail time.
 * 
 * @param players Array of Player structures representing the players.
 * @param numPlayers Number of players in the game.
 * @param board Array of Tile structures representing the game board.
 * @param rounds Number of rounds played.
 */
void debugGameState(Player players[], int numPlayers, Tile board[], int rounds) {
    printf("\n-- DEBUG STATE -- Round %d\n", rounds);
    
    // Print player details
    for (int i = 0; i < numPlayers; i++) {
        printf("\nPlayer %c: \nBalance = $%d, \nPosition = %d, \nTimes in Jail = %d\n", 
            players[i].symbol, players[i].balance, players[i].position, players[i].times_in_jail);


        // Calculate and print the average roll value
        float avgRollValue = players[i].total_rolls ? (float)players[i].total_roll_value / players[i].total_rolls : 0;
        printf("Average Roll Value: %.2f\n", avgRollValue);

        // Print properties owned by the player
        printf("Properties Owned: ");
        int propertiesOwnedCount = 0;
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (players[i].properties_owned[j]) {
                propertiesOwnedCount++;
                printf("%s, ", board[j].name);
            }
        }
        printf("Total = %d\n", propertiesOwnedCount);

        // Print the names of the mortgaged properties
        printf("Mortgaged Properties: ");
        int mortgagedCount = 0;
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (players[i].properties_owned[j] && board[j].mortgaged) {
                mortgagedCount++;
                printf("%s, ", board[j].name);
            }
        }
        if (mortgagedCount == 0) {
            printf("None");
        }
        printf("\n");
    }printf("\n");

    // Print each tile's state
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("Tile %d: %s, Type: %d", i, board[i].name, board[i].type);
        if (board[i].owner != -1) {
            printf(" (Owned by Player %c)", players[board[i].owner].symbol);
        }
        printf("\n");
    }
    printf("-- END DEBUG STATE --\n\n");
}

// //MARK: Test Main
// //comment out the main function below and uncomment this main function to test your code
// //this main function will force values and run the scenarios for them
// int main() {
//     srand(time(NULL));  // Seed for random number generation

//     // Test Case 1: Initialize game with 2 players
//     printf("Test Case 1: Initialize Game with 2 Players\n");
//     int numPlayers = 2;
//     Tile board[BOARD_SIZE];
//     Player players[MAX_PLAYERS];
//     initializeBoard(board);
//     initializePlayers(players, numPlayers);

//     // Print initial board and player balances
//     printBoard(board, players, numPlayers);

//     // Arrays to track roll data for each player
//     int playerTotalRolls[MAX_PLAYERS] = {0};
//     int playerNumRolls[MAX_PLAYERS] = {0};

//     // Test Case 2: Player 1 rolls dice and moves
//     printf("\nTest Case 2: Player 1 Rolls Dice and Moves\n");
//     int die1, die2;
//     rollDice(&die1, &die2);
//     int roll = die1 + die2;
//     playerTotalRolls[0] += roll;
//     playerNumRolls[0]++;
//     printf("Player 1 rolled: %d\n", roll);
//     movePlayer(&players[0], roll);
//     handleTile(&players[0], board, players, numPlayers, 0); // Round 0
//     //printBoard(board, players, numPlayers);

//     // Test Case 3: Player 2 rolls dice and moves
//     printf("\nTest Case 3: Player 2 Rolls Dice and Moves\n");
//     rollDice(&die1, &die2);
//     roll = die1 + die2;
//     playerTotalRolls[1] += roll;
//     playerNumRolls[1]++;
//     printf("Player 2 rolled: %d\n", roll);
//     movePlayer(&players[1], roll);
//     handleTile(&players[1], board, players, numPlayers, 0); // Round 0
//     //printBoard(board, players, numPlayers);

//     // Test Case 4: Player 1 purchases a property
//     printf("\nTest Case 4: Player 1 Purchases a Property\n");
//     players[0].balance = 1000;  // Set a balance large enough to buy a property
//     handleTile(&players[0], board, players, numPlayers, 0);
//     //printBoard(board, players, numPlayers);

//     // Test Case 5: Player 2 lands on a property owned by Player 1
//     printf("\nTest Case 5: Player 2 Pays Rent to Player 1\n");
//     rollDice(&die1, &die2);
//     roll = die1 + die2;
//     playerTotalRolls[1] += roll;
//     playerNumRolls[1]++;
//     printf("Player 2 rolled: %d\n", roll);
//     movePlayer(&players[1], roll);
//     handleTile(&players[1], board, players, numPlayers, 1); // Round 1
//     //printBoard(board, players, numPlayers);

//     // Test Case 6: Player 1 goes to jail
//     printf("\nTest Case 6: Player 1 Goes to Jail\n");
//     players[0].position = 10;  // Set player 1 to land on the "Go to Jail" tile
//     handleTile(&players[0], board, players, numPlayers, 1); // Round 1
//     //printBoard(board, players, numPlayers);

//     // Test Case 7: Player 1 tries to get out of jail
//     printf("\nTest Case 7: Player 1 Rolls to Get Out of Jail\n");
//     rollDice(&die1, &die2);  // Player 1 rolls the dice
//     roll = die1 + die2;
//     playerTotalRolls[0] += roll;
//     playerNumRolls[0]++;
//     printf("Player 1 rolled: %d\n", roll);
//     handleJail(&players[0], roll);  // Try to get out of jail
//     //printBoard(board, players, numPlayers);

//     // Test Case 8: Check Game Over Condition
//     printf("\nTest Case 8: Check Game Over Condition\n");
//     players[0].balance = 0;  // Set Player 1's balance to 0
//     int gameOver = checkGameOver(players, numPlayers, board);
//     if (gameOver) {
//         printf("Game Over! Player %c is bankrupt.\n", players[0].symbol);
//     } else {
//         printf("Game is still ongoing.\n");
//     }

//     // Test Case 9: Print statistics at the end of the game
//     printf("\nTest Case 9: Print Game Statistics\n");
//     printStatistics(players, numPlayers, 1, board);  // After 1 round

//     //because this is a simplified test version, print statistics does not fully calculate the average roll value
//     //it will print 0 for the average roll value because of constraints, the full version of the code will calculate the average roll value
//     // Print the average roll value for each player
//     for (int i = 0; i < numPlayers; i++) {
//         if (playerNumRolls[i] > 0) {
//             float avgRoll = (float) playerTotalRolls[i] / playerNumRolls[i];
//             printf("\nAverage Roll Value for Player %d: %.2f\n", i + 1, avgRoll);
//         } else {
//             printf("Player %d has not rolled yet.\n", i + 1);
//         }
//     }

//     return 0;
// }



// //MARK: Regular Main
int main() {
    srand(time(NULL));

    int numPlayers;
    printf("Enter number of players (2-4): ");
    scanf("%d", &numPlayers);
    if (numPlayers < 2 || numPlayers > 4) {
        printf("Invalid number of players.\n");
        return 1;
    }

    Tile board[BOARD_SIZE];
    Player players[MAX_PLAYERS];
    initializeBoard(board);
    initializePlayers(players, numPlayers);

    int rounds = 0;

    // Game loop: end when one player remains with a positive balance
    while (!checkGameOver(players, numPlayers, board)) {
        for (int i = 0; i < numPlayers; i++) {
            if (players[i].balance <= 0) {
                continue; // Skip bankrupt players
            }

            int die1, die2;
            rollDice(&die1, &die2);
            int roll = die1 + die2;
            players[i].total_rolls++;
            players[i].total_roll_value += roll;

            if (players[i].in_jail) {
                handleJail(&players[i], roll);
            } else {
                movePlayer(&players[i], roll);
                handleTile(&players[i], board, players, numPlayers, rounds);
            }

            printBoard(board, players, numPlayers);
            //run after each round to check the state of the game
    //comment out when not using to debug
    //debugGameState(players, numPlayers, board, rounds);
        }
        rounds++;
        
    }
    
    
    // Once the game is over, print the statistics
    printStatistics(players, numPlayers, rounds, board);
}

//MARK: Initialize Board
/**
 * @brief Initializes the game board with predefined tiles.
 * 
 * @param board Array of Tile structures representing the game board.
 */
void initializeBoard(Tile board[]) {
    Tile tiles[BOARD_SIZE] = {
        {"GO", GO, 0, 0, -1, 0},
        {"Mediterranean Avenue", PROPERTY, 60, 2, 30, -1, 0},
        {"Community Chest", COMMUNITY_CHEST, 0, 0, 0, -1, 0},
        {"Baltic Avenue", PROPERTY, 60, 4, 30, -1, 0},
        {"Income Tax", LUXURY_TAX_TILE, 0, 0, 0, -1, 0},
        {"Reading Railroad", PROPERTY, 200, 25, 100, -1, 0},
        {"Oriental Avenue", PROPERTY, 100, 6, 50, -1, 0},
        {"Chance", COMMUNITY_CHEST, 0, 0, 0, -1, 0},
        {"Vermont Avenue", PROPERTY, 100, 6, 50, -1, 0},
        {"Connecticut Avenue", PROPERTY, 120, 8, 60, -1, 0},
        {"GO to Jail", GOTO_JAIL, 0, 0, 0, -1, 0},
        {"St. Charles Place", PROPERTY, 140, 10, 70, -1, 0},
        {"Electric Company", PROPERTY, 150, 10, 75, -1, 0},
        {"States Avenue", PROPERTY, 140, 10, 70, -1, 0},
        {"Virginia Avenue", PROPERTY, 160, 12, 80, -1, 0},
        {"Pennsylvania Railroad", PROPERTY, 200, 25, 100, -1, 0},
        {"St. James Place", PROPERTY, 180, 14, 90, -1, 0},
        {"Community Chest", COMMUNITY_CHEST, 0, 0, 0, -1, 0},
        {"Tennessee Avenue", PROPERTY, 180, 14, 90, -1, 0},
        {"New York Avenue", PROPERTY, 200, 16, 100, -1, 0},
        {"Free Parking", EMPTY, 0, 0, 0, -1, 0},
        {"Kentucky Avenue", PROPERTY, 220, 18, 110, -1, 0},
        {"Chance", COMMUNITY_CHEST, 0, 0, 0, -1, 0},
        {"Indiana Avenue", PROPERTY, 220, 18, 110, -1, 0},
        {"Illinois Avenue", PROPERTY, 240, 20, 120, -1, 0},
        {"B&O Railroad", PROPERTY, 200, 25, 100, -1, 0},
        {"Atlantic Avenue", PROPERTY, 260, 22, 130, -1, 0},
        {"Ventnor Avenue", PROPERTY, 260, 22, 130, -1, 0},
        {"Water Works", PROPERTY, 150, 10, 75, -1,0},
        {"Marvin Gardens", PROPERTY, 280, 24, 140, -1, 0},
        {"GO TO JAIL", GOTO_JAIL, 0, 0, 0, -1, 0},
        {"Pacific Avenue", PROPERTY, 300, 26, 150, -1, 0},
        {"North Carolina Avenue", PROPERTY, 300, 26, 150, -1, 0},
        {"Community Chest", COMMUNITY_CHEST, 0, 0, 0, -1, 0},
        {"Pennsylvania Avenue", PROPERTY, 320, 28, 160, -1, 0},
        {"Short Line Railroad", PROPERTY, 200, 25, 100, -1, 0},
        {"Chance", COMMUNITY_CHEST, 0, 0, 0, -1, 0},
        {"Park Place", PROPERTY, 350, 35, 175, -1, 0},
        {"Luxury Tax", LUXURY_TAX_TILE, 0, 0, 0, -1, 0},
        {"Boardwalk", PROPERTY, 400, 50, 200, -1, 0},
    };

    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = tiles[i];
    }
}

//MARK: Initialize Players
void initializePlayers(Player players[], int numPlayers) {
    char symbols[] = {'@', '#', '$', '%'};
    for (int i = 0; i < numPlayers; i++) {
        players[i].symbol = symbols[i];
        players[i].balance = STARTING_BALANCE;
        players[i].position = 0;
        players[i].in_jail = 0;
        players[i].jail_turns = 0;
        players[i].total_rolls = 0;
        players[i].times_in_jail = 0;
        for (int j = 0; j < BOARD_SIZE; j++) {
            players[i].properties_owned[j] = 0;
        }
        players[i].total_roll_value = 0; // Initialize total_roll_value to 0
        }
    }

//MARK: RollDice
/**
 * @brief Rolls two dice and stores the results.
 * 
 * @param die1 Pointer to store the result of the first die.
 * @param die2 Pointer to store the result of the second die.
 */
void rollDice(int* die1, int* die2) {
    *die1 = rand() % 6 + 1;
    *die2 = rand() % 6 + 1;
}
//MARK: Move Player
/**
 * @brief Moves a player based on the roll value.
 * 
 * @param player Pointer to the Player structure to be moved.
 * @param roll The total value of the dice roll.
 */
void movePlayer(Player* player, int roll) {
    player->position = (player->position + roll) % BOARD_SIZE;
    if (player->position < roll) {
        player->balance += GO_MONEY;
        printf("Player %c passed GO and collected $%d\n", player->symbol, GO_MONEY);
    }
}
//MARK: Handle All Tiles
/**
 * @brief Handles the actions when a player lands on a tile.
 * 
 * @param player Pointer to the Player structure landing on the tile.
 * @param board Array of Tile structures representing the game board.
 * @param players Array of Player structures representing the players.
 * @param numPlayers Number of players in the game.
 */
void handleTile(Player* player, Tile board[], Player players[], int numPlayers, int rounds) {
    Tile* tile = &board[player->position];

    // If the tile is a property
    if (tile->type == PROPERTY) {
        //MARK: Handle Property
        // Check if the property is available for purchase (no owner)
        if (tile->owner == -1 && player->balance >= tile->cost) {
            player->balance -= tile->cost;
            tile->owner = player - players;
            player->properties_owned[player->position] = 1;
            printf("Player %c bought %s for $%d\n", player->symbol, tile->name, tile->cost);
        }
        // If the property is owned by another player and it's not mortgaged
        else if (tile->owner != player - players) {
            // Skip rent if the property is mortgaged
            if (!tile->mortgaged) {
                int rent = tile->rent;
                rent += (rent * (rounds / 10)) / 2.0; // Increase rent by 2% every 10 rounds

                if (player->balance < rent) {
                    printf("Player %c cannot afford the rent of $%d\n", player->symbol, rent);
                    // Mortgage properties to pay rent
                    for (int i = 0; i < BOARD_SIZE; i++) {
                        if (player->properties_owned[i] && !board[i].mortgaged) {
                            board[i].mortgaged = 1;
                            player->balance += board[i].mortage;
                            printf("Player %c mortgaged %s for $%d\n", player->symbol, board[i].name, board[i].mortage);
                            if (player->balance >= rent) {
                                break;
                            }
                        }
                    }
                }

                // If the player can afford the rent, pay it
                if (player->balance >= rent) {
                    player->balance -= rent;
                    players[tile->owner].balance += rent;
                    printf("Player %c paid $%d rent to Player %c for landing on %s\n", player->symbol, rent, players[tile->owner].symbol, tile->name);
                } else {
                    printf("Player %c is bankrupt and cannot pay the rent\n", player->symbol);
                    player->balance = 0;
                }
            } else {
                printf("Player %c landed on a mortgaged property and does not need to pay rent.\n", player->symbol);
            }
        }
    }
    //MARK: Handle Luxury Tax
    else if (tile->type == LUXURY_TAX_TILE) {
        player->balance -= LUXURY_TAX;
        printf("Player %c paid luxury tax of $%d\n", player->symbol, LUXURY_TAX);
    }
    // MARK: Handle JAIL tile
    else if (tile->type == GOTO_JAIL) {
        player->jail_turns = 0;
        player->times_in_jail++;
        player->in_jail = 1;
        player->jail_turns = 0;
        printf("Player %c goes to jail\n", player->symbol);
    }
    // MARK: Handle Utilities
    else if (tile->type == PROPERTY && (strcmp(tile->name, "Electric Company") == 0 || strcmp(tile->name, "Water Works") == 0)) {
        if (tile->owner != -1 && tile->owner != player - players) {
            int utilityCount = 0;
            for (int i = 0; i < BOARD_SIZE; i++) {
                if ((strcmp(board[i].name, "Electric Company") == 0 || strcmp(board[i].name, "Water Works") == 0) && board[i].owner == tile->owner) {
                    utilityCount++;
                }
            }
            int rentMultiplier = (utilityCount == 1) ? 4 : 10;
            int rent = rentMultiplier * player->total_roll_value;
            rent += (rent * (rounds / 10)) / 10; // Increase rent by 10% every 10 rounds

            if (player->balance < rent) {
                printf("Player %c cannot afford the rent of $%d\n", player->symbol, rent);
                // Mortgage properties to pay rent
                for (int i = 0; i < BOARD_SIZE; i++) {
                    if (player->properties_owned[i] && !board[i].mortgaged) {
                        board[i].mortgaged = 1;
                        player->balance += board[i].mortage;
                        printf("Player %c mortgaged %s for $%d\n", player->symbol, board[i].name, board[i].mortage);
                        if (player->balance >= rent) {
                            break;
                        }
                    }
                }
            }

            if (player->balance >= rent) {
                player->balance -= rent;
                players[tile->owner].balance += rent;
                printf("Player %c paid $%d rent to Player %c for landing on %s\n", player->symbol, rent, players[tile->owner].symbol, tile->name);
            } else {
                printf("Player %c is bankrupt and cannot pay the rent\n", player->symbol);
                player->balance = 0;
            }
        }
    }
    // MARK: Handle Railroads
    else if (tile->type == PROPERTY && (strcmp(tile->name, "Reading Railroad") == 0 || strcmp(tile->name, "Pennsylvania Railroad") == 0 || strcmp(tile->name, "B&O Railroad") == 0 || strcmp(tile->name, "Short Line Railroad") == 0)) {
        if (tile->owner != -1 && tile->owner != player - players) {
            int railroadCount = 0;
            for (int i = 0; i < BOARD_SIZE; i++) {
                if ((strcmp(board[i].name, "Reading Railroad") == 0 || strcmp(board[i].name, "Pennsylvania Railroad") == 0 || strcmp(board[i].name, "B&O Railroad") == 0 || strcmp(board[i].name, "Short Line Railroad") == 0) && board[i].owner == tile->owner) {
                    railroadCount++;
                }
            }
            int rent = 25 * (1 << (railroadCount - 1));
            rent += (rent * (rounds / 10)) / 10; // Increase rent by 10% every 10 rounds

            if (player->balance < rent) {
                printf("Player %c cannot afford the rent of $%d\n", player->symbol, rent);
                // Mortgage properties to pay rent
                for (int i = 0; i < BOARD_SIZE; i++) {
                    if (player->properties_owned[i] && !board[i].mortgaged) {
                        board[i].mortgaged = 1;
                        player->balance += board[i].mortage;
                        printf("Player %c mortgaged %s for $%d\n", player->symbol, board[i].name, board[i].mortage);
                        if (player->balance >= rent) {
                            break;
                        }
                    }
                }
            }

            if (player->balance >= rent) {
                player->balance -= rent;
                players[tile->owner].balance += rent;
                printf("Player %c paid $%d rent to Player %c for landing on %s\n", player->symbol, rent, players[tile->owner].symbol, tile->name);
            } else {
                printf("Player %c is bankrupt and cannot pay the rent\n", player->symbol);
                player->balance = 0;
            }
        }
    }
}


//MARK: Handle Jail
/**
 * @brief Handles the actions when a player is in jail.
 * 
 * @param player Pointer to the Player structure in jail.
 * @param roll The total value of the dice roll.
 */
void handleJail(Player* player, int roll) {
    if (roll % 2 == 0) {
        player->in_jail = 0;
        player->jail_turns = 0;
        printf("Player %c rolled doubles and is out of jail\n", player->symbol);
    } else {
        player->jail_turns++;
        if (player->jail_turns >= 3) {
            player->in_jail = 0;
            player->jail_turns = 0;
            printf("Player %c is out of jail after 3 turns\n", player->symbol);
        } else {
            printf("Player %c is still in jail\n", player->symbol);
        }
    }
}

//MARK: Print Board
/**
 * @brief Prints the current state of the game board and player balances.
 * 
 * @param board Array of Tile structures representing the game board.
 * @param players Array of Player structures representing the players.
 * @param numPlayers Number of players in the game.
 */
void printBoard(Tile board[], Player players[], int numPlayers) {
   
    printf("\nMonopoly Board:\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        int flag = 0;
        for (int j = 0; j < numPlayers; j++) {
            if (i == players[j].position) {
                printf("[%c] %s", players[j].symbol, board[i].name);
                flag = 1;
                continue;
            }
            
        }
        if (flag == 0) {
            printf("[ ] %s", board[i].name);
        }

        if (board[i].owner != -1) {
            printf(" (Owned by Player %c)", players[board[i].owner].symbol);
        }
        printf("\n");
    }
    
    printf("\n");

    // Print player balances
    for (int i = 0; i < numPlayers; i++) {
        printf("Player %c: $%d\n", players[i].symbol, players[i].balance);
    }
}
//MARK: Print Statistics
/**
 * @brief Prints the game statistics after the game is over.
 * 
 * @param players Array of Player structures representing the players.
 * @param numPlayers Number of players in the game.
 * @param rounds Number of rounds played.
 */
void printStatistics(Player players[], int numPlayers, int rounds, Tile board[]) {
    printf("\nGame Over! \nStatistics:\n");
    printf("Total Rounds: %d\n", rounds);
    for (int i = 0; i < numPlayers; i++) {

        printf("\nPlayer %c:\n", players[i].symbol);
        printf("Average Roll Value: %.2f\n", players[i].total_rolls ? (float)players[i].total_roll_value / players[i].total_rolls : 0);
        // printf("Total Rolls: %d\n", players[i].total_rolls);

                // Print the number of properties owned by the player
                int propertiesOwnedCount = 0;
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (players[i].properties_owned[j]) {
                    propertiesOwnedCount++;
                    }
                }
                printf("Properties Owned: %d\n", propertiesOwnedCount);

        //test that the right properties owned by the user are being printed
        // printf("Properties: ");
        // for (int j = 0; j < BOARD_SIZE; j++) {
        //     if (players[i].properties_owned[j]) {
        //         printf("%d ", j);
        //     }
        // }
        //print the names of the properties owned by the user, not just their index
        printf("Property Names: ");
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (players[i].properties_owned[j]) {
                printf("%s, ", board[j].name);
            }
        }
        // //test that the the number of properties that have been mortgaged by the player are being printed
        // printf("\nMortgaged Properties: ");
        // for (int j = 0; j < BOARD_SIZE; j++) {
        //     if (players[i].properties_owned[j] && board[j].mortgaged) {
        //         printf("%d ", j);
        //     }
        // }
        //print the names of the properties that have been mortgaged by the player
        printf("\nMortgaged Property Names: ");
        int mortgagedCount = 0;
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (players[i].properties_owned[j] && board[j].mortgaged) {
            printf("%s, ", board[j].name);
            mortgagedCount++;
            }
        }
        if (mortgagedCount == 0) {
            printf("None");
        }
        printf("\nMoney: $%d\n", players[i].balance);
        printf("Times in Jail: %d\n", players[i].times_in_jail);
       
}
}

//MARK: Update Ownership
/**
 * @brief Updates the property ownership status for all players.
 * 
 * @param players Array of Player structures representing the players.
 * @param numPlayers Number of players in the game.
 * @param board Array of Tile structures representing the game board.
 */
void updatePropertyOwnership(Player players[], int numPlayers, Tile board[]){
    for (int i = 0; i < numPlayers; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[j].owner == i) {
                players[i].properties_owned[j] = 1;
            } else {
                players[i].properties_owned[j] = 0;
            }
        }
    }
}

//MARK: Check Game Status
/**
 * @brief Checks if the game is over based on property ownership and player balances.
 * 
 * @param players Array of Player structures representing the players.
 * @param numPlayers Number of players in the game.
 * @param board Array of Tile structures representing the game board.
 * @return int 1 if the game is over, 0 otherwise.
 */
int checkGameOver(Player players[], int numPlayers, Tile board[]) {
    int remainingPlayers = 0;
    int winner = -1;

    // Check if each player is still in the game
    for (int i = 0; i < numPlayers; i++) {
        if (players[i].balance > 0) {
            remainingPlayers++;
            winner = i;
        }

        // Check if player has mortgaged all properties and cannot afford rent
        if (players[i].balance <= 0) {
            int allMortgaged = 1;
            // Check if the player has mortgaged all their properties
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (players[i].properties_owned[j] && !board[j].mortgaged) {
                    allMortgaged = 0; // If any property is not mortgaged, they haven't mortgaged everything
                    break;
                }
            }

            // If the player has mortgaged all properties, they are effectively bankrupt
            if (allMortgaged) {
                printf("Player %c is bankrupt and has mortgaged all properties.\n", players[i].symbol);
                players[i].balance = 0; // Set their balance to zero
                return 1; // Game Over
            }
        }
    }

    return 0; // Game is still ongoing
}

