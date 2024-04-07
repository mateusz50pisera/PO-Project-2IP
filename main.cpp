#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <cstdlib>

using namespace std;

map<string, int> rarityMap = {
    {"Common", 1},
    {"Uncommon", 2},
    {"Rare", 3},
    {"Epic", 4},
    {"Legendary", 5}
};

class Item
{
public:
    Item() {}
    string name;
    int durability;
    string details;
    int attack;
    int resistance;
    string type;
    int price;
    string rarity; // Added rarity

    Item(string name, string type, int price, int durability = 100, int attack = 0, int resistance = 0, string details = "No details", string rarity = "Common")
        : name{name}, type{type}, price{price}, durability{durability}, details{details}, attack{attack}, resistance{resistance}, rarity{rarity} {}

    // Function to get the color code based on rarity
    string getRarityColor() const
    {
        // Check if rarity exists in the rarityMap
        if (rarityMap.find(rarity) != rarityMap.end())
        {
            switch (rarityMap.at(rarity)) {
            case 2: return "\033[32m"; // Green
            case 3: return "\033[34m"; // Blue
            case 4: return "\033[35m"; // Purple
            case 5: return "\033[33m"; // Yellow
            default: return ""; // No color for Common
            }
        }
        else
        {
            // Handle case where rarity is not found in rarityMap
            cout << "Rarity '" << rarity << "' not found in rarityMap." << endl;
            return ""; // Return empty string or default color
        }
    }
};

class Shop
{
    int rows;
    int cols;
public:
    vector<vector<Item*>> grid;
    Shop(int rows = 5, int cols = 5) : rows{rows}, cols{cols}, grid(rows, vector<Item*>(cols))
    {
        int count = 0;
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                grid[i][j] = new Item("item" + to_string(count), "DEFAULT", 100, 10, 0, 0, "Default item for sale");
                count++;
            }
        }
    }

    void display()
    {
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                Item* item = grid[i][j];
                string rarityColor = item->getRarityColor();
                cout << rarityColor << "[" << item->name << ", $" << item->price << "]\t\033[0m"; // Reset color after printing
            }
            cout << endl;
        }
    }

    int getRows() const
    {
        return rows;
    }

    int getCols() const
    {
        return cols;
    }

    ~Shop()
    {
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                delete grid[i][j];
            }
        }
    }
};

class Equipment
{
    int rows;
    int cols;
    int pointerRow;
    int pointerCol;
    // Helper function to make sorting items easier
    void sortItems(function<bool(const Item*, const Item*)> comparator, bool ascending)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                for (int k = 0; k < rows; k++)
                {
                    for (int l = 0; l < cols; l++)
                    {
                        if (grid[i][j] != nullptr && grid[k][l] != nullptr)
                        {
                            if (ascending)
                            {
                                if (comparator(grid[k][l], grid[i][j]))
                                {
                                    swap(grid[k][l], grid[i][j]);
                                }
                            } else
                            {
                                if (comparator(grid[i][j], grid[k][l]))
                                {
                                    swap(grid[k][l], grid[i][j]);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

public:
    vector<vector<Item*>> grid;
    Equipment(int rows = 5, int cols = 5) : rows(rows), cols(cols), grid(rows, vector<Item*>(cols))
    {
        int count = 0;
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                grid[i][j] = new Item(to_string(count), "DEFAULT", 100, 10, 0, 0, "Default item for sale");
                count++;
            }
        }
        pointerRow = 0;
        pointerCol = 0;
    }

    int getRows() const
    {
        return rows;
    }

    int getCols() const
    {
        return cols;
    }

    void setRows(int newRows)
    {
        rows = newRows;
    }

    void setCols(int newCols)
    {
        cols = newCols;
    }

    void expand() {
        if (rows < 10 && cols < 10)
        {
            int newRowSize = rows + 1;
            int newColSize = cols + 1;

            // Create a new grid with expanded size
            vector<vector<Item*>> newGrid(newRowSize, vector<Item*>(newColSize, nullptr));

            // Copy existing items to the new grid
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    newGrid[i][j] = grid[i][j];
                }
            }

            // Initialize newly added elements in the last row
            for (int j = 0; j < newColSize; j++)
            {
                newGrid[newRowSize - 1][j] = new Item("none", "DEFAULT", 100, 0, 0, 0, "No item available");  // Initialize with default item
            }

            // Replace the old grid with the new one
            grid = std::move(newGrid);
            rows = newRowSize;
            cols = newColSize;

            cout << "Inventory expanded to " << newRowSize << "x" << newColSize << endl;
        }
        else
        {
            cout << "Cannot expand inventory after 10x10" << endl;
        }
    }

    void display()
    {
        // Clear the screen
        system("cls");
        cout << "Your inventory:\n";

        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                if (i == pointerRow && j == pointerCol) {
                    cout << "> [" << (grid[i][j] != nullptr ? grid[i][j]->name : "none") << "] <\t\t";
                } else {
                    cout << "[" << (grid[i][j] != nullptr ? grid[i][j]->name : "none") << "]\t\t";
                }
            }
            cout << endl;
        }
    }
    void move(int row1, int col1, int row2, int col2)
    {
        if(row1 < rows && col1 < cols && row2 < rows && col2 < cols)
        {
            Item* temp = grid[row1][col1];
            grid[row1][col1] = grid[row2][col2];
            grid[row2][col2] = temp;
        }
        else
        {
            cout << "Invalid values entered" << endl;
        }
    }

    void movePointer(char direction)
    {
        switch(direction)
        {
        case 'w':
            if(pointerRow > 0)
                pointerRow--;
            break;
        case 'a':
            if(pointerCol > 0)
                pointerCol--;
            break;
        case 's':
            if(pointerRow < rows - 1)
                pointerRow++;
            break;
        case 'd':
            if(pointerCol < cols - 1)
                pointerCol++;
            break;
        default:
            cout << "Invalid direction" << endl;
            break;
        }
    }

    void showDetails(int row, int col)
    {
        if (row < rows && col < cols && grid[row][col] != nullptr)
        {
            Item* item = grid[row][col];
            cout << "Item Name: " << item->name << endl;
            cout << "Durability: " << item->durability << endl;
            cout << "Details: " << item->details << endl;
            cout << "Type: " << item->type << endl;
            if (item->type == "WEAPON")
            {
                cout << "Attack: " << item->attack << endl;
            }
            else if (item->type == "ARMOR")
            {
                cout << "Resistance: " << item->resistance << endl;
            }
            cout << "Rarity: " << item->getRarityColor() << item->rarity << "\t\033[0m" << endl;
        }
        else
        {
            cout << "Invalid position or no item found." << endl;
        }
    }

    void deleteItem(int row, int col)
    {
        if (row < rows && col < cols && grid[row][col] != nullptr)
        {
            // Delete the item from the player's equipment and replace it with a placeholder item
            cout << "Deleted item " << grid[row][col]->name << endl;
            delete grid[row][col];
            grid[row][col] = new Item("none", "DEFAULT", 100, 0, 0, 0, "No item available");
        }
        else
        {
            cout << "Invalid position or no item found in player's equipment." << endl;
        }
    }

    // Sorting methods depending on what user wants
    void sortByName(bool ascending = true)
    {
        auto comparator = [](const Item* a, const Item* b) {
            return a->name < b->name;
        };

        sortItems(comparator, ascending);
    }

    void sortByDurability(bool ascending = true)
    {
        auto comparator = [](const Item* a, const Item* b) {
            return a->durability < b->durability;
        };

        sortItems(comparator, ascending);
    }

    void sortByPrice(bool ascending = true)
    {
        auto comparator = [](const Item* a, const Item* b) {
            return a->price < b->price;
        };

        sortItems(comparator, ascending);
    }

    void sortByRarity(bool ascending = true)
    {
        auto comparator = [this](const Item* a, const Item* b) {
            return rarityMap[a->rarity] < rarityMap[b->rarity];
        };

        sortItems(comparator, ascending);
    }

    ~Equipment()
    {
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                delete grid[i][j];
            }
        }
    }
};

class Player
{
    int HP;
    Item* mainHand;
    Item* sword;
    Item* helmet;
    Item* armor;
    Item* pants;
    Item* boots;
    Equipment* eq;
public:
    int gold;
    Player()
    {
        HP = 100;
        mainHand = nullptr;
        sword = nullptr;
        helmet = nullptr;
        armor = nullptr;
        pants = nullptr;
        boots = nullptr;
        eq = new Equipment();
    }
    vector<vector<Item*>>& getEquipmentGrid()
    {
        return eq->grid;
    }

    void setMainWeapon(int i, int j)
    {
        Item* temp = mainHand;
        mainHand = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }

    void setMainSword(int i, int j)
    {
        Item* temp = sword;
        sword = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }

    void setMainHelmet(int i, int j)
    {
        Item* temp = helmet;
        helmet = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }

    void setMainArmor(int i, int j)
    {
        Item* temp = armor;
        armor = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }

    void setMainPants(int i, int j)
    {
        Item* temp = pants;
        pants = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }

    void setMainBoots(int i, int j)
    {
        Item* temp = boots;
        boots = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }

    void showEq()
    {
        eq->display();
    }

    void moveItem(int row1, int col1, int row2, int col2)
    {
        eq->move(row1, col1, row2, col2);
    }

    void expandInventory()
    {
        if (gold >= 300)
        {
            eq->expand();
            gold -= 300;
        }
        else
        {
            cout << "Not enough gold." << endl;
        }
    }

    void sort(bool asc = false, int option = 1)
    {
        switch (option) {
        case 1:
            eq->sortByName(asc);
            break;
        case 2:
            eq->sortByDurability(asc);
            break;
        case 3:
            eq->sortByPrice(asc);
            break;
        case 4:
            eq->sortByRarity(asc);
            break;
        default:
            cout << "Invalid criteria" << endl;
            break;
        }
    }

    void displayPlayerStats()
    {
        cout << "HP: " << HP << endl;
        cout << (mainHand != nullptr ? mainHand->name : "Fist") << endl;
        cout << (sword != nullptr ? sword->name : "Sword") << endl;
        cout << (helmet != nullptr ? helmet->name : "Helmet") << endl;
        cout << (armor != nullptr ? armor->name : "Armor") << endl;
        cout << (pants != nullptr ? pants->name : "Pants") << endl;
        cout << (boots != nullptr ? boots->name : "Boots") << endl;
        eq->display();
    }

    void showDetails(int row, int col)
    {
        eq->showDetails(row, col);
    }

    void showGold()
    {
        cout << "Your gold: " << gold << endl;
    }

    void buy(int row, int col, Shop& shop)
    {
        if (row < shop.getRows() && col < shop.getCols() && shop.grid[row][col] != nullptr)
        {
            Item* itemToBuy = shop.grid[row][col];
            if (gold >= itemToBuy->price)
            {
                // Checks if inventory is full of items
                if (eq->getRows() * eq->getCols() >= 6 * 6)
                {
                    cout << "Inventory is full, cannot buy more items." << endl;
                    return;
                }

                gold -= itemToBuy->price;
                // Remove the bought item from the shop's inventory
                delete shop.grid[row][col];
                shop.grid[row][col] = new Item("none", "DEFAULT", 100, 0, 0, 0, "No item available");
                // Add the bought item to the player's equipment
                eq->grid[row][col] = itemToBuy; // Assign the bought item directly to the player's equipment
                cout << "You bought " << itemToBuy->name << " for $" << itemToBuy->price << endl;
            }
            else
            {
                cout << "Not enough gold to buy " << itemToBuy->name << endl;
            }
        }
        else
        {
            cout << "Invalid position or no item found in shop." << endl;
        }
    }

    void sell(int row, int col, Shop& shop)
    {
        if (row < eq->getRows() && col < eq->getCols() && eq->grid[row][col] != nullptr)
        {
            Item* itemToSell = eq->grid[row][col];
            int sellPrice = itemToSell->price / 2;
            gold += sellPrice;
            cout << "You sold " << itemToSell->name << " for $" << sellPrice << endl;
            // Add the sold item back to the shop's inventory
            shop.grid[row][col] = itemToSell;
            eq->grid[row][col] = nullptr; // Remove the item from player's inventory
        }
        else
        {
            cout << "Invalid position or no item found in player's inventory." << endl;
        }
    }

    void removeItem(int row, int col)
    {
        eq->deleteItem(row, col);
    }

    ~Player()
    {
        delete eq;
    }
};

// Account doesn't work properly yet
class Account
{
    string login;
    string password;

public:
    Account(string login, string password) : login(login), password(password) {}

    bool Validator(string inputLogin, string inputPassword) const
    {
        return (inputLogin == login && inputPassword == password);
    }

public:

    void saveToFile(Player& player, const Shop& shop) const
    {
        ofstream file("save_data.txt");

        // Save player information
        file << "Player Gold: " << player.gold << endl;
        file << "Player Equipment:" << endl;
        for (int i = 0; i < player.getEquipmentGrid().size(); i++)
        {
            for (int j = 0; j < player.getEquipmentGrid()[i].size(); j++)
            {
                if (player.getEquipmentGrid()[i][j] != nullptr)
                {
                    file << "[" << i << "," << j << "] ";
                    file << "\"" << player.getEquipmentGrid()[i][j]->name << "\" ";
                    file << "\"" << player.getEquipmentGrid()[i][j]->type << "\" ";
                    file << player.getEquipmentGrid()[i][j]->price << " ";
                    file << player.getEquipmentGrid()[i][j]->durability << " ";
                    file << player.getEquipmentGrid()[i][j]->attack << " ";
                    file << player.getEquipmentGrid()[i][j]->resistance << " ";
                    file << "\"" << player.getEquipmentGrid()[i][j]->details << "\" ";
                    file << "\"" << player.getEquipmentGrid()[i][j]->rarity << "\"" << endl;
                }
            }
        }

        // Save shop information
        file << "Shop Items:" << endl;
        for (int i = 0; i < shop.grid.size(); i++)
        {
            for (int j = 0; j < shop.grid[i].size(); j++)
            {
                if (shop.grid[i][j] != nullptr)
                {
                    file << "[" << i << "," << j << "] ";
                    file << "\"" << shop.grid[i][j]->name << "\" ";
                    file << "\"" << shop.grid[i][j]->type << "\" ";
                    file << shop.grid[i][j]->price << " ";
                    file << shop.grid[i][j]->durability << " ";
                    file << shop.grid[i][j]->attack << " ";
                    file << shop.grid[i][j]->resistance << " ";
                    file << "\"" << shop.grid[i][j]->details << "\" ";
                    file << "\"" << shop.grid[i][j]->rarity << "\"" << endl;
                }
            }
        }

        file.close();
    }

    void loadFromFile(Player& player, Shop& shop)
    {
        ifstream file("save_data.txt");
        if (!file.is_open())
        {
            cout << "Unable to open save file. Starting a new game." << endl;
            return;
        }

        string line;
        while (getline(file, line))
        {
            if (line.find("Player Gold:") != string::npos)
            {
                stringstream ss(line);
                string prefix;
                int gold;
                ss >> prefix >> gold;
                player.gold = gold;
            }
            else if (line == "Player Equipment:")
            {
                while (getline(file, line) && line != "Shop Items:")
                {
                    stringstream ss(line);
                    string pos, itemName, itemType, itemDetails, itemRarity;
                    int itemPrice, itemDurability, itemAttack, itemResistance;
                    ss >> pos >> itemName >> itemType >> itemPrice >> itemDurability >> itemAttack >> itemResistance >> ws; // Read and discard whitespace
                    getline(ss, itemDetails, '\"'); // Read until the next quote
                    getline(ss, itemDetails, '\"'); // Read the contents between quotes
                    getline(ss, itemRarity); // Read the entire line for rarity
                    // Extract row and column from position string
                    int row = stoi(pos.substr(1, pos.find(',')));
                    int col = stoi(pos.substr(pos.find(',') + 1, pos.size() - pos.find(',') - 2));
                    // Assign item to player's equipment grid
                    player.getEquipmentGrid()[row][col] = new Item(itemName, itemType, itemPrice, itemDurability, itemAttack, itemResistance, itemDetails, itemRarity);
                }
            }
            else if (line == "Shop Items:")
            {
                while (getline(file, line))
                {
                    stringstream ss(line);
                    string pos, itemName, itemType, itemDetails, itemRarity;
                    int itemPrice, itemDurability, itemAttack, itemResistance;
                    ss >> pos >> itemName >> itemType >> itemPrice >> itemDurability >> itemAttack >> itemResistance >> ws; // Read and discard whitespace
                    getline(ss, itemDetails, '\"'); // Read until the next quote
                    getline(ss, itemDetails, '\"'); // Read the contents between quotes
                    getline(ss, itemRarity); // Read the entire line for rarity
                    // Extract row and column from position string
                    int row = stoi(pos.substr(1, pos.find(',')));
                    int col = stoi(pos.substr(pos.find(',') + 1, pos.size() - pos.find(',') - 2));
                    // Assign item to shop's grid
                    shop.grid[row][col] = new Item(itemName, itemType, itemPrice, itemDurability, itemAttack, itemResistance, itemDetails, itemRarity);
                }
            }
        }

        file.close();
    }
};

class GameBoard {
private:
    int size;
    int playerRow;
    int playerCol;

public:
    GameBoard(int s) : size(s), playerRow(s / 2), playerCol(s / 2) {}

    void updatePlayerPosition(int row, int col) {
        // Update player position logic
        playerRow = row;
        playerCol = col;
    }

    void display() {
        // Display game board logic
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i == playerRow && j == playerCol)
                    cout << "P ";
                else
                    cout << ". ";
            }
            cout << endl;
        }
    }

    int getSize() const {
        return size;
    }

    int getPlayerRow() const {
        return playerRow;
    }

    int getPlayerCol() const {
        return playerCol;
    }
};


class Game {
private:
    Player player;
    Shop shop;
    GameBoard gameBoard;
    Equipment eq;

public:
    Game() : player(), shop(), gameBoard(10) {}

    void displayMainMenu(int selectedIndex) {
        system("cls || clear"); // Clear the screen
        cout << "Main Menu\n";
        cout << (selectedIndex == 0 ? "> " : "  ") << "New Game\n";
        cout << (selectedIndex == 1 ? "> " : "  ") << "Load Save\n";
        cout << (selectedIndex == 2 ? "> " : "  ") << "Exit\n";
    }

    int mainMenu() {
        int selectedIndex = 0;

        while (true) {
            displayMainMenu(selectedIndex);

            char input = _getch(); // Get a single character input without waiting for Enter

            if (input == 13) { // Enter key
                if (selectedIndex == 2) // If "Exit" is selected
                    exit(0); // Exit the program
                else
                    return selectedIndex; // Return the selected index for other options
            }

            if (input == 'w' || input == 'W') // W key
                selectedIndex = (selectedIndex - 1 + 3) % 3;

            if (input == 's' || input == 'S') // S key
                selectedIndex = (selectedIndex + 1) % 3;
        }
    }

    bool showInGameInventory()
    {
        char userInput;
        bool showInventory = true;

        while(true)
        {
            if(showInventory)
            {
                eq.display();
                userInput = _getch();
                if (userInput == 27) // Check if the user pressed the escape key
                {
                    showInventory = false; // Hide the inventory
                    return true;
                    break; // Exit the loop
                }
                else
                {
                    eq.movePointer(userInput);
                }
            }
            else
            {
                showInventory = true;
                break; // Exit the loop
            }
        }
        return false;
    }

    void runGame() {
        int option = mainMenu();
        system("cls || clear");
        gameBoard.display();

        while (true) {
            if (_kbhit()) {
                char input = _getch();
                if (input == 'e' || input == 'E') { // If the 'e' key is pressed
                    if (showInGameInventory()) {
                        // Escape key was pressed while inventory was shown
                        system("cls || clear");
                        gameBoard.display(); // Display the game board
                    }
                    continue; // Continue to wait for input after displaying inventory
                } else if (input == 27) { // If the escape key is pressed
                    option = pauseGame(option);
                    if (option == 0) {
                        // Continue, so just break out of the pause loop
                        continue;
                    } else if (option == 1) {
                        mainMenu();
                        break; // Exit the game loop and return to main menu
                    }
                }

                // Movement controls
                int newRow = gameBoard.getPlayerRow();
                int newCol = gameBoard.getPlayerCol();

                switch (input) {
                case 'w':
                case 'W':
                    newRow = (newRow - 1 + gameBoard.getSize()) % gameBoard.getSize();
                    break;
                case 's':
                case 'S':
                    newRow = (newRow + 1) % gameBoard.getSize();
                    break;
                case 'a':
                case 'A':
                    newCol = (newCol - 1 + gameBoard.getSize()) % gameBoard.getSize();
                    break;
                case 'd':
                case 'D':
                    newCol = (newCol + 1) % gameBoard.getSize();
                    break;
                default:
                    break;
                }

                gameBoard.updatePlayerPosition(newRow, newCol);
                system("cls || clear");
                gameBoard.display(); // Display the updated game board
            }
        }
    }

    int pauseGame(int selectedIndex) {
        while (true) {
            system("cls || clear");
            cout << "Pause Menu\n";
            cout << (selectedIndex == 0 ? "> " : "  ") << "Continue\n";
            cout << (selectedIndex == 1 ? "> " : "  ") << "Exit to main menu";

            char input = _getch();
            if (input == 13) // Enter key
                return selectedIndex; // Return the selected index

            if (input == 'w' || input == 'W') // W key
                selectedIndex = (selectedIndex - 1 + 2) % 2;

            if (input == 's' || input == 'S') // S key
                selectedIndex = (selectedIndex + 1) % 2;
        }
    }

    void newGame() {
        system("cls || clear"); // Clear the screen
        cout << "Starting a new game...\n";

        // Initialize player position at the center of the game board
        int playerRow = gameBoard.getSize() / 2;
        int playerCol = gameBoard.getSize() / 2;
        gameBoard.updatePlayerPosition(playerRow, playerCol);

        // Display game board with player at the center
        gameBoard.display();
        _getch(); // Wait for a key press
    }

    void play() {
        runGame();
    }
};

int main()
{
    Game game;
    game.play();
    return 0;
}
