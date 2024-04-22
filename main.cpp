#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
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
    string rarity;

    Item(string name, string type, int price, int durability = 0, int attack = 0, int resistance = 0, string details = "No details", string rarity = "Common")
        : name{name}, type{type}, price{price}, durability{durability}, details{details}, attack{attack}, resistance{resistance}, rarity{rarity} {}

    string getRarityColor() const
    {
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
    int pointerRow;
    int pointerCol;
public:
    vector<vector<Item*>> grid;
    Shop(int rows = 5, int cols = 5) : rows{rows}, cols{cols}, grid(rows, vector<Item*>(cols))
    {
        int count = 0;
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                grid[i][j] = new Item("item" + to_string(count), "DEFAULT", 0, 0, 0, 0, "Default item for sale");
                count++;
            }
        }
    }

    void display() {
        system("cls");
        cout << "Shop inventory:\n";

        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                if (i == pointerRow && j == pointerCol)
                {
                    cout << "> [" << (grid[i][j] != nullptr ? grid[i][j]->name : "none") << "] <\t";
                }
                else
                {
                    cout << "[" << (grid[i][j] != nullptr ? grid[i][j]->name : "none") << "\t]\t";
                }
            }
            cout << endl;
        }
    }

    void movePointer(char direction)
    {
        switch(direction)
        {
        case 'w':
        case 'W':
        case 72:
            if(pointerRow > 0)
            {
                pointerRow--;
            }
            break;
        case 'a':
        case 'A':
        case 75:
            if(pointerCol > 0)
            {
                pointerCol--;
            }
            break;
        case 's':
        case 'S':
        case 80:
            if(pointerRow < rows - 1)
            {
                pointerRow++;
            }
            break;
        case 'd':
        case 'D':
        case 77:
            if(pointerCol < cols - 1)
            {
                pointerCol++;
            }
            break;
        }
    }

    int getRows() const {
        return rows;
    }

    int getCols() const {
        return cols;
    }

    int getPointerRow() const {
        return pointerRow;
    }

    int getPointerCol() const {
        return pointerCol;
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
                            }
                            else
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
                grid[i][j] = new Item("item" + to_string(count), "DEFAULT", 0, 0, 0, 0, "Default item for sale");
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

    int getPointerRow() const {
        return pointerRow;
    }

    int getPointerCol() const {
        return pointerCol;
    }

    vector<vector<Item*>>& getGrid()
    {
        return grid;
    }

    void setRows(int newRows)
    {
        rows = newRows;
    }

    void setCols(int newCols)
    {
        cols = newCols;
    }

    void expand()
    {
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
                newGrid[newRowSize - 1][j] = new Item("none", "DEFAULT", 0, 0, 0, 0, "No item available");  // Initialize with default item
            }

            // Replace the old grid with the new one
            grid = std::move(newGrid);
            rows = newRowSize;
            cols = newColSize;

            cout << "Inventory expanded to " << newRowSize << "x" << newColSize << endl;
        }
        else
        {
            cout << "You don't have enough gold to expand your inventory, or inventory size limit reached." << endl;
        }
    }


    void display()
    {
        system("cls");
        cout << "Your inventory:\n";

        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                if (i == pointerRow && j == pointerCol)
                {
                    cout << "> [" << (grid[i][j] != nullptr ? grid[i][j]->name : "none") << "] <\t";
                }
                else
                {
                    cout << "[" << (grid[i][j] != nullptr ? grid[i][j]->name : "none") << "\t]\t";
                }
            }
            cout << endl;
        }
    }
    void popInventory()
    {
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                cout << "[" << (grid[i][j] != nullptr ? grid[i][j]->name : "none") << "\t]\t";
            }
            cout << endl;
        }
    }
    void move(int row1, int col1)
    {
        if (row1 >= 0 && row1 < rows && col1 >= 0 && col1 < cols)
        {
            char userInput;
            int row2 = 0; // Initialize the second row
            int col2 = 0; // Initialize the second column

            // Initialize temp outside the switch statement
            Item* temp = nullptr;

            // Loop until the user selects the second item to swap
            while (true)
            {

                // Display the inventory
                display();

                // Get user input to navigate the inventory
                userInput = _getch();

                // Move the pointer according to user input
                movePointer(userInput);
                switch (userInput)
                {
                case 'w':
                case 'W':
                case 72:
                    if (row2 > 0)
                    {
                        row2--;
                    }
                    break;
                case 'a':
                case 'A':
                case 75:
                    if (col2 > 0)
                    {
                        col2--;
                    }
                    break;
                case 's':
                case 'S':
                case 80:
                    if (row2 < rows - 1)
                    {
                        row2++;
                    }
                    break;
                case 'd':
                case 'D':
                case 77:
                    if (col2 < cols - 1)
                    {
                        col2++;
                    }
                    break;
                case 13:
                    temp = grid[row1][col1];
                    grid[row1][col1] = grid[row2][col2];
                    grid[row2][col2] = temp;
                    return;
                case 27: // Escape key
                    return;
                default:
                    break;
                }
            }
        }
        else
        {
            cout << "Invalid position entered" << endl;
        }
    }

    void movePointer(char direction)
    {
        switch(direction)
        {
        case 'w':
        case 'W':
        case 72:
            if(pointerRow > 0)
            {
                pointerRow--;
            }
            break;
        case 'a':
        case 'A':
        case 75:
            if(pointerCol > 0)
            {
                pointerCol--;
            }
            break;
        case 's':
        case 'S':
        case 80:
            if(pointerRow < rows - 1)
            {
                pointerRow++;
            }
            break;
        case 'd':
        case 'D':
        case 77:
            if(pointerCol < cols - 1)
            {
                pointerCol++;
            }
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
            cout << "Invalid position or no item found" << endl;
        }
    }

    void deleteItem(int row, int col)
    {
        if (row < rows && col < cols)
        {
            // Check if the item is not a deleted item
            if (grid[row][col] != nullptr)
            {
                char choice;
                cout << "Do you want to delete item?\nY/N\n";
                cin >> choice;
                switch(choice)
                {
                case 'Y':
                case 'y':
                    delete grid[row][col];
                    grid[row][col] = nullptr;
                    cout << "Item deleted" << endl;
                    break;
                case 'N':
                case 'n':
                    break;
                default:
                    cout << "Invalid option" << endl;
                    break;
                }
            }
            else
            {
                cout << "No item found" << endl;
            }
        }
        else
        {
            cout << "Invalid position" << endl;
        }
    }

    // Sorting methods depending on what user wants
    void sortByName(bool ascending = true)
    {
        auto comparator = [](const Item* a, const Item* b)
        {
            return a->name < b->name;
        };

        sortItems(comparator, ascending);
    }

    void sortByDurability(bool ascending = true)
    {
        auto comparator = [](const Item* a, const Item* b)
        {
            return a->durability < b->durability;
        };

        sortItems(comparator, ascending);
    }

    void sortByPrice(bool ascending = true)
    {
        auto comparator = [](const Item* a, const Item* b)
        {
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
    int gold;
    int HP;
    Item* mainHand;
    Item* sword;
    Item* helmet;
    Item* armor;
    Item* pants;
    Item* boots;
    Equipment* eq;
public:
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

    Equipment* getEq()
    {
        return eq;
    }

    int getGold()
    {
        return gold;
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

    void popEq()
    {
        eq->popInventory();
    }

    void sort(bool asc = false, int option = 1)
    {
        switch (option)
        {
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
        system("cls");
        cout << "Your gold: " << gold << endl;
        cout << "HP: " << HP << endl;
        cout << (mainHand != nullptr ? mainHand->name : "Fist") << endl;
        cout << (sword != nullptr ? sword->name : "Sword") << endl;
        cout << (helmet != nullptr ? helmet->name : "Helmet") << endl;
        cout << (armor != nullptr ? armor->name : "Armor") << endl;
        cout << (pants != nullptr ? pants->name : "Pants") << endl;
        cout << (boots != nullptr ? boots->name : "Boots") << endl;
    }

    void buy(int row, int col, Shop& shop)
    {
        if (row < shop.getRows() && col < shop.getCols() && shop.grid[row][col] != nullptr)
        {
            char choice;
            Item* itemToBuy = shop.grid[row][col];
            if (gold >= itemToBuy->price)
            {
                // Checks if inventory is full of items
                if (eq->getRows() * eq->getCols() >= 6 * 6)
                {
                    cout << "Inventory is full, cannot buy more items" << endl;
                    return;
                }

                cout << "Do you want to buy " << itemToBuy->name << "for " << itemToBuy->price << "$? \nY/N\n";
                cin >> choice;
                switch(choice)
                {
                case 'Y':
                case 'y':
                    gold -= itemToBuy->price;
                    // Remove the bought item from the shop's inventory
                    delete shop.grid[row][col];
                    shop.grid[row][col] = new Item("none", "DEFAULT", 0, 0, 0, 0, "No item available");
                    // Add the bought item to the player's equipment
                    eq->grid[row][col] = itemToBuy; // Assign the bought item directly to the player's equipment
                    cout << "You bought " << itemToBuy->name << " for $" << itemToBuy->price << endl;
                    break;
                case 'N':
                case 'n':
                    break;
                default:
                    cout << "Invalid option" << endl;
                    break;
                }
            }
            else
            {
                cout << "Not enough gold to buy " << itemToBuy->name << endl;
            }
        }
        else
        {
            cout << "Invalid position or no item found in shop" << endl;
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
            // Add the sold item to the shop's inventory
            shop.grid[row][col] = itemToSell;
            eq->grid[row][col] = nullptr; // Remove the item from player's inventory
        }
        else
        {
            cout << "Invalid position or no item found in player's inventory" << endl;
        }
    }

    ~Player()
    {
        delete eq;
    }
};

class GameBoard
{
private:
    int size;
    int playerRow;
    int playerCol;
    int shopRow;
    int shopCol;
public:
    GameBoard(int s) : size(s), playerRow(s / 2), playerCol(s / 2), shopRow(rand() % s), shopCol(rand() % s) {}

    void updatePlayerPosition(int row, int col)
    {
        playerRow = row;
        playerCol = col;
    }

    void display()
    {
        // For displaying board
        cout << "+";
        for (int i = 0; i < (size * 2); i++)
        {
            cout << "=";
        }
        cout << "+" << endl;
        for (int i = 0; i < size; i++)
        {
            cout << "|";
            for (int j = 0; j < size; j++)
            {
                if (i == playerRow && j == playerCol)
                    cout << "O ";
                else if (i == shopRow && j == shopCol)
                    cout << "$ ";
                else
                    cout << ". ";
            }
            cout << "|" << endl;
        }
        cout << "+";
        for (int i = 0; i < (size * 2); i++)
        {
            cout << "=";
        }
        cout << "+" << endl;
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

    int getShopRow() const {
        return shopRow;
    }

    int getShopCol() const {
        return shopCol;
    }
};


class Game
{
private:
    Player player;
    GameBoard gameBoard;
    Shop shop;
    int gold = player.getGold();

public:
    Game() : player(), shop(), gameBoard(15) {}

    void displayMainMenu(int selectedIndex)
    {
        system("cls");
        cout << "RPG project game\n";
        cout << (selectedIndex == 0 ? "> " : "  ") << "New Game\n";
        cout << (selectedIndex == 1 ? "> " : "  ") << "Exit\n";
    }

    int mainMenu()
    {
        int selectedIndex = 0;

        while (true)
        {
            displayMainMenu(selectedIndex);

            char input = _getch(); // Get a single character input without waiting for Enter

            if (input == 13) // Enter key
            {
                if (selectedIndex == 1)
                {
                    exit(0);
                }
                else
                {
                    return selectedIndex; // Return the selected index for other options
                }
            }

            if (input == 'w' || input == 'W' || input == 72)
            {
                selectedIndex = (selectedIndex - 1 + 2) % 2;
            }

            if (input == 's' || input == 'S' || input == 80)
            {
                selectedIndex = (selectedIndex + 1) % 2;
            }
        }
    }

    bool showInGameInventory()
    {
        char userInput;
        bool showInventory = true;
        bool inItemOptions = false;
        int selectedItemRow = player.getEq()->getPointerRow();
        int selectedItemCol = player.getEq()->getPointerCol();
        int selectedOption = 0;


        while(true)
        {
            if (showInventory && !inItemOptions)
            {
                player.getEq()->display();
                userInput = _getch();
                if (userInput == 27) // Escape key
                {
                    showInventory = false;
                    return true;
                }
                else if (userInput == 13)
                {
                    if (player.getEq()->grid[selectedItemRow][selectedItemCol] != nullptr) // Display options for the selected item
                    {
                        inItemOptions = true;
                    }
                }
                else
                {
                    player.getEq()->movePointer(userInput);
                    selectedItemRow = player.getEq()->getPointerRow();
                    selectedItemCol = player.getEq()->getPointerCol();
                }
            }

            if (inItemOptions)
            {
                system("cls");

                // Handle item options menu here
                showItemOptions(selectedItemRow, selectedItemCol, selectedOption);
                userInput = _getch();
                switch (userInput)
                {
                case 'w':
                case 'W':
                case 72:
                    selectedOption = (selectedOption - 1 + 6) % 6;
                    break;
                case 's':
                case 'S':
                case 80:
                    selectedOption = (selectedOption + 1) % 6;
                    break;
                case 13: // Enter key
                    if (selectedOption == 0)
                    {
                        player.getEq()->showDetails(selectedItemRow, selectedItemCol);
                        cout << "Press any key to continue...";
                        _getch(); // Wait for any key press
                        selectedItemRow = player.getEq()->getPointerRow();
                        selectedItemCol = player.getEq()->getPointerCol();
                        inItemOptions = false;
                        showInventory = true;
                    }
                    if (selectedOption == 1)
                    {
                        player.getEq()->move(selectedItemRow, selectedItemCol);
                        cout << "Changed item's position" << endl;
                        cout << "Press any key to continue...";
                        _getch(); // Wait for any key press
                        selectedItemRow = player.getEq()->getPointerRow();
                        selectedItemCol = player.getEq()->getPointerCol();
                        inItemOptions = false;
                        showInventory = true;
                    }
                    if (selectedOption == 2)
                    {
                        player.getEq()->deleteItem(selectedItemRow, selectedItemCol);
                        cout << "Press any key to continue...";
                        _getch();
                        selectedItemRow = player.getEq()->getPointerRow();
                        selectedItemCol = player.getEq()->getPointerCol();
                        inItemOptions = false;
                        showInventory = true;
                    }
                    if (selectedOption == 3)
                    {
                        if (gold >= 300)
                        {
                            char choice;
                            cout << "Do you want to expand inventory?\nY/N\n";
                            cin >> choice;
                            switch(choice)
                            {
                            case 'Y':
                            case 'y':
                                player.getEq()->expand();
                                gold -= 300;
                            case 'N':
                            case 'n':
                                break;
                            default:
                                cout << "Invalid option" << endl;
                                break;
                            }
                            cout << "Press any key to continue...";
                            _getch();
                            inItemOptions = false;
                            showInventory = true;
                        }
                        else
                        {
                            cout << "Not enough gold" << endl;
                            cout << "Press any key to continue...";
                            _getch();
                            inItemOptions = false;
                            showInventory = true;
                        }
                    }
                    if (selectedOption == 4)
                    {
                        player.displayPlayerStats();
                        player.getEq()->popInventory();
                        cout << "Press any key to continue...";
                        _getch();
                        selectedItemRow = player.getEq()->getPointerRow();
                        selectedItemCol = player.getEq()->getPointerCol();
                        inItemOptions = false;
                        showInventory = true;
                    }
                    if (selectedOption == 5)
                    {
                        // Sorting doesn't work yet
                        system("cls");
                        int sortBy;
                        bool asc;
                        char choice;
                        cout << "Do you want to sort ascending?\nY/N\n";
                        cin >> choice;
                        if (choice == 'Y' || choice == 'y')
                        {
                            cout << "Pick number to choose by what do you want to sort items?\n1. Name\n2. Durability\n3. Price\n4. Rarity";
                            cin >> choice;
                            if (choice == 1)
                            {
                                player.getEq()->sortByName();
                            }
                            if (choice == 2)
                            {
                                player.getEq()->sortByDurability();
                            }
                            if (choice == 3)
                            {
                                player.getEq()->sortByPrice();
                            }
                            if (choice == 4)
                            {
                                player.getEq()->sortByRarity();
                            }
                        }
                        else if (choice == 'N' || choice == 'n')
                        {
                            asc = false;
                            cout << "Pick number to choose by what do you want to sort items?\n1. Name\n2. Durability\n3. Price\n4. Rarity\n";
                            cin >> choice;
                            if (choice == 1)
                            {
                                player.getEq()->sortByName(asc);
                                player.getEq()->popInventory();
                            }
                            if (choice == 2)
                            {
                                player.getEq()->sortByDurability(asc);
                            }
                            if (choice == 3)
                            {
                                player.getEq()->sortByPrice(asc);
                            }
                            if (choice == 4)
                            {
                                player.getEq()->sortByRarity(asc);
                            }
                        }
                        else
                        {
                            cout << "Invalid option" << endl;
                        }
                        inItemOptions = false;
                        showInventory = true;
                    }
                    break;
                case 27: // Escape key
                    inItemOptions = false;
                    showInventory = true;
                    break;
                default:
                    break;
                }
            }

            if (!showInventory && !inItemOptions)
            {
                break; // Exit the loop if neither inventory nor item options are being displayed
            }
        }
        return false;
    }

    void showItemOptions(int row, int col, int selectedOption)
    {
        // Retrieve the selected item from the inventory
        Item* selectedItem = player.getEq()->grid[row][col];

        // Display options for the selected item
        cout << "Item Options for " << selectedItem->name << ":" << endl;
        cout << (selectedOption == 0 ? "> " : "  ") << "1. Show Details" << endl;
        cout << (selectedOption == 1 ? "> " : "  ") << "2. Move item" << endl;
        cout << (selectedOption == 2 ? "> " : "  ") << "3. Remove Item" << endl;
        cout << "Options for equipment:" << endl;
        cout << (selectedOption == 3 ? "> " : "  ") << "4. Expand inventory" << endl;
        cout << (selectedOption == 4 ? "> " : "  ") << "5. Display stats" << endl;
        cout << (selectedOption == 5 ? "> " : "  ") << "6. Sort items" << endl;
    }

    void showShopOptions(int selectedOption)
    {
        shop.display();
        cout << "Shop options:" << endl;
        cout << (selectedOption == 0 ? "> " : "  ") << "1. Show Details" << endl;
        cout << (selectedOption == 1 ? "> " : "  ") << "2. Move item" << endl;
    }

    void showInGameShop()
    {
        int selectedOption = 0;
        int selectedRow = 0;
        int selectedCol = 0;
        bool showPlayerInventory = false;
        bool showShopInventory = false;
        char userInput;

        while (true)
        {
            system("cls");
            cout << "Shop options:" << endl;
            cout << (selectedOption == 0 ? "> " : "  ") << "1. Buy" << endl;
            cout << (selectedOption == 1 ? "> " : "  ") << "2. Sell" << endl;
            cout << (selectedOption == 2 ? "> " : "  ") << "3. Display player stats" << endl;
            userInput = _getch();

            switch (userInput)
            {
            case 'w':
            case 'W':
            case 72: // Up arrow
                selectedOption = (selectedOption - 1 + 3) % 3;
                break;
            case 's':
            case 'S':
            case 80: // Down arrow
                selectedOption = (selectedOption + 1) % 3;
                break;
            case 'a':
            case 'A':
            case 75: // Left arrow
                if (selectedOption == 0)
                    selectedCol = (selectedCol - 1 + shop.getCols()) % shop.getCols();
                break;
            case 'd':
            case 'D':
            case 77: // Right arrow
                if (selectedOption == 0)
                    selectedCol = (selectedCol + 1) % shop.getCols();
                break;
            case 13: // Enter key
                if (selectedOption == 0) // Buy
                {
                    showShopInventory = true;
                    while(showShopInventory == true)
                    {
                        shop.display();
                        userInput = _getch();
                        if (userInput == 27)
                        {
                            showShopInventory = false;
                        }
                        else if (userInput == 13)
                        {
                            player.buy(selectedRow, selectedCol, shop);
                            cout << "Press any key to continue...";
                            _getch();
                            showShopInventory = false;
                        }
                        else
                        {
                            shop.movePointer(userInput);
                            selectedRow = shop.getPointerRow();
                            selectedCol = shop.getPointerCol();
                        }
                    }
                }
                else if (selectedOption == 1) // Sell
                {
                    showPlayerInventory = true;
                    while(showPlayerInventory == true)
                    {
                        player.getEq()->display();
                        userInput = _getch();
                        if (userInput == 27)
                        {
                            showPlayerInventory = false;
                        }
                        else if (userInput == 13)
                        {
                            char choice;
                            cout << "Do you want to sell this item?\nY/N\n";
                            cin >> choice;
                            switch(choice)
                            {
                            case 'Y':
                            case 'y':
                                player.sell(selectedRow, selectedCol, shop);
                                cout << "Press any key to continue...";
                                _getch();
                                showPlayerInventory = false;
                                break;
                            case 'N':
                            case 'n':
                                break;
                            default:
                                cout << "Invalid option" << endl;
                                break;
                            }
                        }
                        else
                        {
                            player.getEq()->movePointer(userInput);
                            selectedRow = player.getEq()->getPointerRow();
                            selectedCol = player.getEq()->getPointerCol();
                        }
                    }
                }
                else if (selectedOption == 2)
                {
                    player.displayPlayerStats();
                    player.getEq()->popInventory();
                    cout << "Press any key to continue...";
                    _getch();
                    break;
                }
                break;
            case 27: // Escape key
                return;
            default:
                break;
            }
        }
    }

    void play() {
        int option = mainMenu();
        system("cls");
        gameBoard.display();
        int selectedIndex = 0;

        while (true)
        {
            if (_kbhit())
            {
                char input = _getch();
                if (input == 'e' || input == 'E')
                {
                    if (showInGameInventory())
                    {
                        // Escape key was pressed while inventory was shown
                        system("cls");
                        gameBoard.display();
                    }
                    continue; // Continue to wait for input after displaying inventory
                }
                else if (input == 27) // Escape key
                {
                    option = pauseGame(option);
                    if (option == 0)
                    {
                        system("cls");
                        gameBoard.display(); // Refresh the screen after returning from pause menu
                        option = 0;
                        continue;
                    }
                    else if (option == 1)
                    {
                        exit(0);
                    }
                }


                // Movement controls
                int newRow = gameBoard.getPlayerRow();
                int newCol = gameBoard.getPlayerCol();

                // Adding shop logic to game
                int shopRow = gameBoard.getShopRow();
                int shopCol = gameBoard.getShopCol();

                if ((newRow == shopRow && newCol == shopCol) && input == 13)
                {
                    system("cls");
                    cout << "Do you want to enter the shop?\nY/N" << endl;
                    char choice;
                    cin >> choice;
                    switch(choice)
                    {
                    case 'Y':
                    case 'y':
                        showInGameShop();
                        break;
                    case 'N':
                    case 'n':
                        gameBoard.display();
                        break;
                    default:
                        cout << "Invalid option" << endl;
                        break;
                    }
                }

                switch (input)
                {
                case 'w':
                case 'W':
                case 72: // Up arrow
                    newRow = (newRow - 1 + gameBoard.getSize()) % gameBoard.getSize();
                    break;
                case 's':
                case 'S':
                case 80: // Down arrow
                    newRow = (newRow + 1) % gameBoard.getSize();
                    break;
                case 'a':
                case 'A':
                case 75: // Left arrow
                    newCol = (newCol - 1 + gameBoard.getSize()) % gameBoard.getSize();
                    break;
                case 'd':
                case 'D':
                case 77: // Right arrow
                    newCol = (newCol + 1) % gameBoard.getSize();
                    break;
                default:
                    break;
                }

                gameBoard.updatePlayerPosition(newRow, newCol);
                system("cls");
                gameBoard.display(); // Display the updated game board
            }
        }
    }

    int pauseGame(int selectedIndex)
    {
        while (true)
        {
            system("cls");
            cout << "Pause Menu\n";
            cout << (selectedIndex == 0 ? "> " : "  ") << "Continue\n";
            cout << (selectedIndex == 1 ? "> " : "  ") << "Exit\n";

            char input = _getch();
            if (input == 13) // Enter key
            {
                return selectedIndex;
            }

            if (input == 'w' || input == 'W' || input == 72)
            {
                selectedIndex = (selectedIndex - 1 + 2) % 2;
            }

            if (input == 's' || input == 'S' || input == 80)
            {
                selectedIndex = (selectedIndex + 1) % 2;
            }
        }
    }

    void newGame()
    {
        system("cls");

        int playerRow = gameBoard.getSize() / 2;
        int playerCol = gameBoard.getSize() / 2;
        gameBoard.updatePlayerPosition(playerRow, playerCol);

        gameBoard.display();
        _getch();
    }
};

int main()
{
    srand(time(NULL));
    Game game;
    game.play();

    /*

    Player P;
    Shop shop;
    P.gold = 600;

    P.popEq();
    shop.display();

    P.displayPlayerStats();
    P.setMainWeapon(3, 4);
    P.setMainSword(4, 1);
    P.setMainHelmet(1, 3);
    P.setMainArmor(2, 1);
    P.setMainPants(0, 2);
    P.setMainBoots(4, 4);
    P.setMainWeapon(2, 0);
    P.setMainSword(1, 4);
    P.setMainHelmet(3, 1);
    P.setMainArmor(2, 3);
    P.setMainPants(2, 0);
    P.setMainBoots(4, 4);

    P.buy(0, 0, shop);

    P.sell(2, 0, shop);
    shop.display();

    P.popEq();
    cout << "After sorting:\n";
    P.sort(false, 4);
    P.popEq();

    */

    return 0;
}
