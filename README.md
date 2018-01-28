# 6.179: IAP 2018, PSet 2

The purpose of this pset is to give you an example of how large systems can be composed from small, 
interconnected subcomponent "object"s. You should hope to learn how to make objects interact with 
eachother to perform meaninful operations, and how to use the C++ STL to your advantage in reducing
the amount of time you spend implementing various standard algoithms and data structures. 

I do not proport the starting code to be perfect, indeed a few different architectural and correctness issues 
have already come up. But, it is good enough to get the key points across.

## Walkthrough

I have constructed a walkthrough in two parts for this pset. The first part will give general tips for
implementing each of the methods, and the second will be more of a step by step guide to each method. 
Ideally, you won't need to consult either of these, but if you do find yourself stuck, I'd reccommend looking
only at the first part to start, and only reading the "step by step" section if all hope is lost.

This walkthrough will only deal with the most basic functionallity possible (no ties, no "?", no variable `startElo`, etc.) the rest is up to you.

### Implementation: Tips and Tricks

#### PlayerStats
This class is responsible for maintaining each player's stats for a given tournament and simulating how these 
stats change as the player interacts with other players.

This is the easiest class to implement, as there is only a simple output function.

**PlayerStats.h**
```c++
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
```

- ```std::ostream &operator<<(std::ostream &os, const PlayerStats &ps)```
   - We'll need to round the `ps.elo` value, but other than that this is a pretty simple case of chaining a bunch of `<<` operators together, and just printing each component of the provided format string to `os` piece by piece


#### Game
This class represents a single match in a tournament, and provides several static methods for building these matches.

These implementations are also fairly easy, but will require you to understand how to interact with a class when provided
only a public view, and how to make use of pass-by-reference semantics.

```c++
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
```

- ```static Game victory(Player winner, Player loser);``` & ```static Game tie(Player p1, Player p2);```
   - These are static methods, which means they have no existing object to operate on. Rather, they return a new `Player`. We can simply construct one with the code already provided, and return that constructed object as is.
-```friend std::ostream &operator<<(std::ostream &os, Game &g);```
   - Another fairly simple case of chaining a bunch of `<<` operations together. In this case, we just delegate the operation to the `Player`'s `<<` operator and let it handle how to print a player for us.
-```std::istream &operator>>(std::istream &is, Game &g)```
   - (Implemented in the `Main.cpp` section). Here we do not havw access to the constructors for Players, but as the previously created `victory` and `tie` classes are public, we can call them and simply assign the result to `g`. As `g` was passed by reference, the assigment will be reflected to the outside world.

#### Tournament

This class is responsible for maintaining a mapping of Player->Stat pairs for a given tournament. It
provides a means by which to run games using its players, and way to view all the players it contains
and the stats of a given player.

These implementations should show you the power of the STL, and require you to use up standard means of
performing basic operations (find, sort, etc.) on STL containers. Google/DDG/Bing/whatever is your friend!

It is recomended that you implement `statsOf` before `execute`, as execute relies on logic `statsOf` should implement.

```c++
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
```

- ```PlayerStats statsOf(Player) const;```
   - Here, we want to "find" (big hint there, you should look up C++'s maps for more info) a player in the tournament's `players` map. If it exists in the map, we can simply return the one we found.
   - Otherwise, we can pretend like the player was in the tournament by just returning a newly created PlayerStats object
   - Unfortunately, we cannot use the [] operator on players in this function, as it is a const function, and [] is non-const.
- ```std::vector<Player> allPlayers() const;```
   - Pretty simple function which just returns all the keys in the map in a vector. Reference materials for this should be easy to find
- ```void execute(Game);```
   - This updates the tournpments `players` map via computing new Elo's for the two players in the game. We can use the `simulateMatch` to determine what each player will have. To use player stats to compute the stats
   a player with stats `a` will have after losing to a player with stats `b`, use `new_stats = a.simulateMatch(b, LOSS)`
   - It is important that you save an initial copy of the player's stats, so that you do not compute the second player's change in elo based on the updates stats of the first player.

### Implementation: Step by Step
Coming soon, maybe.
