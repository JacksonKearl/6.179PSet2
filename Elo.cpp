#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>

// Forward declarations.
class Game;
class Player;
class PlayerStats;
class Tournament;
typedef double Elo;

// Typedefs and enums for convinience
typedef std::pair<Player, PlayerStats> PlayerWithStats;

enum PlayerResult
{
    LOSS,
    WIN,
    TIE
};

//////////////////////////////////////////////////////////////
// Begin type interfaces.
//////////////////////////////////////////////////////////////

////////////////////////
// "PlayerStats.h"
//
// This class is responsible for maintaining each
//   player's stats for a given tournament and simulating
//   how these stats change as the player interacts with
//   other players.
class PlayerStats
{
    double expectedScore(PlayerStats b) const;
    Elo elo;
    int wins;
    int losses;
    int ties;

  public:
    PlayerStats() : elo(1000), wins(0), losses(0), ties(0) {}
    PlayerStats(Elo elo) : elo(elo), wins(0), losses(0), ties(0) {}
    PlayerStats(Elo elo, int wins, int losses, int ties) : elo(elo), wins(wins), losses(losses), ties(ties) {}

    PlayerStats simulateMatch(PlayerStats &b, PlayerResult result) const;
    friend bool playerStatComparator(PlayerWithStats p1, PlayerWithStats p2);
    friend std::ostream &operator<<(std::ostream &os, const PlayerStats &ps);
};

////////////////////////
// "Tournament.h"
//
// This class is responsible for maintaining a mapping
//   of Player->Stat pairs for a given tournament. It
//   provides a means by which to run games using its players,
//   and way to view all the players it contains and the stats
//   of a given player.
class Tournament
{
    std::map<Player, PlayerStats> players;

  public:
    Elo startElo;
    Tournament(Elo startElo) : startElo(startElo){};
    void execute(Game);
    PlayerStats statsOf(Player) const;
    std::vector<Player> allPlayers() const;
};

////////////////////////
// "Player.h"
//
// This struct is responsible for storing all the details
//    that identify a player
// Player is a header-only object.
struct Player
{
    std::string name;

    friend std::istream &operator>>(std::istream &is, Player &p)
    {
        is >> p.name;
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const Player &p)
    {
        os << p.name;
        return os;
    }
};

////////////////////////
// "Game.h"
//
// This class represents a single match in a tournament, and
//    provides several static methods for building these matches
class Game
{
    Player p1;
    Player p2;
    bool isTie;
    Game(Player p1, Player p2, bool tie) : p1(p1), p2(p2), isTie(tie){};

  public:
    Game(){};

    static Game victory(Player winner, Player loser);
    static Game tie(Player p1, Player p2);
    friend std::ostream &operator<<(std::ostream &os, Game &g);
    friend void Tournament::execute(Game);
};

//////////////////////////////////////////////////////////////
// Begin type implementations.
//////////////////////////////////////////////////////////////

////////////////////////
// "PlayerStats.cpp"

// Compute the intermediary value for the Elo algorithm
double PlayerStats::expectedScore(PlayerStats b) const
{
    double scaledDiff = (b.elo - this->elo) / 1000.0;
    double expFactor = 10;
    return 1.0 / (1 + pow(expFactor, scaledDiff));
}

// Compute the new stats this player would have after either a
// WIN/LOSS/TIE against a player with the passed in stats.
PlayerStats PlayerStats::simulateMatch(PlayerStats &b, PlayerResult result) const
{
    double d = this->expectedScore(b);
    switch (result)
    {
    case WIN:
        return {this->elo + 32 * (1 - d), wins + 1, losses, ties};
        break;
    case LOSS:
        return {this->elo + 32 * (0 - d), wins, losses + 1, ties};
        break;
    case TIE:
        return {this->elo + 32 * (0.5 - d), wins, losses, ties + 1};
        break;
    }
}
// render these stats to the stream in the format:
// <elo (rounded integer)> (<wins>-<losses>-<ties>)
std::ostream &operator<<(std::ostream &os, const PlayerStats &ps)
{
    // TODO: this.
    return os;
}

////////////////////////
// "Tournament.cpp"

// I reccommend implementing statsOf before execute.

// we need this in order to store 'Player' objects in a std::map object.
bool operator<(const Player &lhs, const Player &rhs)
{
    // return something...
}

// Given a player, this should return that player's stats
// in this tournament. If that player does not exist, it
// should act at if it is a new player, and return a the
// initial default stats object for this tournament.
PlayerStats Tournament::statsOf(const Player player) const
{
    // if player is in the tournament...
    //     return something....
    // else...
    //     return something else...
}

// This should update the tournament's internal view of
// the two player's stats, based on a passed Game
void Tournament::execute(Game g)
{
    // update the map...
}

// This should return a vector of every player in this
// tournament. Order is not important.
std::vector<Player> Tournament::allPlayers() const
{
    //return something....
}

////////////////////////
// "Game.cpp"

// This should return a Game object with the required properties.
Game Game::victory(Player winner, Player loser)
{
    //return something....
}

// This should return a Game object with the required properties.
Game Game::tie(Player p1, Player p2)
{
    //return something....
}

// This should render a representation of the Game to the screen,
// formatted like:
// <player1 name> <"tied" or "beat" as needed> <player 2 name>
std::ostream &operator<<(std::ostream &os, Game &g)
{
    // TODO: implement this.
    return os;
}

////////////////////////
// "Main.cpp"

// I reccommend implementing these after everything else. (indeed you may need to)

// This should take text from the input stream and
// create a Game from it. The text will be of the form
// <player 1 name> <"tied" or "beat" as needed> <player 2 name><newline>
//
// Helpful note: player names are only one word, as are operations: ("beat", "tied").
// "is >> (some string val)"" will by defualt take one word at a time. Also, an
// overload to create Players from an input stream has already been provided.
std::istream &operator>>(std::istream &is, Game &g)
{
    // TODO: implement this.
    return is;
}

// This should output each player in the tournament, sorted
// by their elo (decending). Each line output should be of the format:
// <tab><player name>: <elo> (<wins>-<losses>-<ties>)<newline>
std::ostream &operator<<(std::ostream &os, const Tournament &t)
{
    // TODO: implement this.
    return os;
}

// driver. Dont touch this.
int main()
{
    Elo startingElo;
    std::cout << "Enter starting elo: ";
    std::cin >> startingElo;

    std::cout << "Enter number of tournaments: ";
    int numTournaments;
    std::cin >> numTournaments;
    std::vector<Tournament> tournaments;
    for (size_t i = 0; i < numTournaments; i++)
    {
        tournaments.emplace_back(startingElo);
    }

    std::cout << "Enter number of games: ";
    int numGames;
    std::cin >> numGames;

    std::cout << std::endl;

    for (size_t i = 0; i < numGames; i++)
    {
        int counter = 0;
        for (Tournament &tournament : tournaments)
        {
            counter++;
            Game game;
            std::cin >> game;
            std::cout << "Tournament " << counter
                      << ", Game " << i / numTournaments + 1 << ": "
                      << game << std::endl;

            tournament.execute(game);
        }
    }

    int counter = 0;
    for (Tournament &tournament : tournaments)
    {
        std::cout << std::endl;
        counter++;
        std::cout << "Final Stats for Tournament "
                  << counter << ": " << std::endl
                  << tournament;
    }
    return 0;
}
