#include <iostream>
#include <string>
#include <vector>
#include <map>
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

    Item(string name, string type, int price, int durability = 0, int attack = 0, int resistance = 0, string details = "This is an item description example", string rarity = "Common")
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
    vector<vector<Item *>> grid;
    Shop(int rows = 5, int cols = 5) : rows{rows}, cols{cols}, grid(rows, vector<Item *>(cols))
    {
        vector<Item> items = {
            Item("Sword", "WEAPON", 40, 100, 20, 0, "A powerful sword", "Rare"),
            Item("Axe", "WEAPON", 35, 90, 25, 0, "A sharp axe", "Common"),
            Item("Dagger", "WEAPON", 25, 80, 15, 0, "A small dagger", "Common"),
            Item("Bow", "WEAPON", 30, 80, 30, 0, "A sturdy bow", "Uncommon"),
            Item("Mace", "WEAPON", 45, 95, 22, 0, "A heavy mace", "Rare"),
            Item("Staff", "WEAPON", 35, 85, 28, 0, "A magical staff", "Uncommon"),
            Item("Helmet", "HELMET", 20, 85, 0, 15, "A sturdy helmet", "Common"),
            Item("Chainmail", "CHESTPLATE", 50, 120, 0, 25, "Durable chainmail", "Rare"),
            Item("Plate chestplate", "CHESTPLATE", 60, 150, 0, 30, "Heavy plate chestplate", "Rare"),
            Item("Leather chestplate", "CHESTPLATE", 40, 100, 0, 20, "Light leather chestplate", "Common"),
            Item("Legguards", "LEGGINS", 25, 70, 0, 10, "Sturdy legguards", "Common"),
            Item("Iron Boots", "BOOTS", 30, 75, 0, 12, "Solid iron boots", "Uncommon"),
            Item("Leather Boots", "BOOTS", 20, 60, 0, 8, "Light leather boots", "Common"),
            Item("Healing Potion", "SUPPORT", 15, 0, 0, 0, "Restores health", "Common"),
            Item("Mana Potion", "SUPPORT", 20, 0, 0, 0, "Restores mana", "Uncommon"),
            Item("Speed Potion", "SUPPORT", 25, 0, 0, 0, "Increases speed", "Rare"),
            Item("Strength Potion", "SUPPORT", 30, 0, 0, 0, "Increases strength", "Rare"),
            Item("Fire Scroll", "SUPPORT", 35, 0, 0, 0, "Casts fire spell", "Rare"),
            Item("Ice Scroll", "SUPPORT", 35, 0, 0, 0, "Casts ice spell", "Rare"),
            Item("Lightning Scroll", "SUPPORT", 35, 0, 0, 0, "Casts lightning spell", "Rare"),
            Item("Healing Scroll", "SUPPORT", 30, 0, 0, 0, "Casts healing spell", "Rare"),
            Item("Revive Scroll", "SUPPORT", 40, 0, 0, 0, "Revives fallen ally", "Epic"),
            Item("Summoning Scroll", "SUPPORT", 45, 0, 0, 0, "Summons a creature", "Epic"),
        };

        int count = 0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                // Use the items vector to populate the shop grid
                if (count < items.size())
                {
                    grid[i][j] = new Item(items[count]);
                }
                else
                {
                    // If items vector is smaller than grid size, fill with default item
                    grid[i][j] = nullptr;
                }
                count++;
            }
        }
    }

    void display()
    {
        system("cls");
        cout << "Shop inventory:\n";

        // Calculate the maximum width for each column
        vector<int> maxColWidth(cols, 0);
        for (int j = 0; j < cols; j++)
        {
            for (int i = 0; i < rows; i++)
            {
                if (grid[i][j] != nullptr)
                {
                    int nameLength = grid[i][j]->name.length();
                    if (nameLength > maxColWidth[j])
                    {
                        maxColWidth[j] = nameLength;
                    }
                }
            }
        }

        // Display the shop inventory
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (i == pointerRow && j == pointerCol)
                {
                    cout << "> [" << (grid[i][j] != nullptr ? grid[i][j]->name : "");
                }
                else
                {
                    cout << "[" << (grid[i][j] != nullptr ? grid[i][j]->name : "");
                }

                // Adjust spacing based on the length of the longest item name in the column
                int spacesToAdd = maxColWidth[j] - (grid[i][j] != nullptr ? grid[i][j]->name.length() : 0);
                for (int k = 0; k < spacesToAdd + 1; k++) // Add an extra space after the name
                {
                    cout << " ";
                }

                if (i == pointerRow && j == pointerCol)
                {
                    cout << "] <\t\t";
                }
                else
                {
                    cout << "]\t\t";
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

    void showItemDetails(int row, int col)
    {
        if (row < rows && col < cols && grid[row][col] != nullptr)
        {
            Item* item = grid[row][col];
            cout << "Item Name: " << item->name << endl;
            cout << "Durability: " << item->durability << endl;
            cout << "Price: " << item->price << endl;
            cout << "Details: " << item->details << endl;
            cout << "Type: " << item->type << endl;
            if (item->type == "WEAPON")
            {
                cout << "Attack: " << item->attack << endl;
            }
            else if (item->type == "HELMET" || item->type == "CHESTPLATE" || item->type == "LEGGINS" || item->type == "BOOTS")
            {
                cout << "Resistance: " << item->resistance << endl;
            }
            cout << "Rarity: " << item->getRarityColor() << item->rarity << "\t\033[0m" << endl;
        }
        else
        {
            cout << "No item found" << endl;
        }
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
public:
    vector<vector<Item*>> grid;
    Equipment(int rows = 5, int cols = 5) : rows(rows), cols(cols), grid(rows, vector<Item*>(cols))
    {
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                grid[i][j] = nullptr;
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
                newGrid[newRowSize - 1][j] = nullptr;
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

        // Calculate the maximum width for each column
        vector<int> maxColWidth(cols, 0);
        for (int j = 0; j < cols; j++)
        {
            for (int i = 0; i < rows; i++)
            {
                if (grid[i][j] != nullptr)
                {
                    int nameLength = grid[i][j]->name.length();
                    if (nameLength > maxColWidth[j])
                    {
                        maxColWidth[j] = nameLength;
                    }
                }
            }
        }

        // Display the equipment
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (i == pointerRow && j == pointerCol)
                {
                    cout << "> [" << (grid[i][j] != nullptr ? grid[i][j]->name : "");
                }
                else
                {
                    cout << "[" << (grid[i][j] != nullptr ? grid[i][j]->name : "");
                }

                // Adjust spacing based on the length of the longest item name in the column
                int spacesToAdd = maxColWidth[j] - (grid[i][j] != nullptr ? grid[i][j]->name.length() : 0);
                for (int k = 0; k < spacesToAdd + 1; k++) // Add an extra space after the name
                {
                    cout << " ";
                }

                if (i == pointerRow && j == pointerCol)
                {
                    cout << "] <\t\t";
                }
                else
                {
                    cout << "]\t\t";
                }
            }
            cout << endl;
        }
    }
    void popInventory()
    {
        // Calculate the maximum width for each column
        vector<int> maxColWidth(cols, 0);
        for (int j = 0; j < cols; j++)
        {
            for (int i = 0; i < rows; i++)
            {
                if (grid[i][j] != nullptr)
                {
                    int nameLength = grid[i][j]->name.length();
                    if (nameLength > maxColWidth[j])
                    {
                        maxColWidth[j] = nameLength;
                    }
                }
            }
        }

        // Display the equipment
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cout << "[" << (grid[i][j] != nullptr ? grid[i][j]->name : "");

                // Adjust spacing based on the length of the longest item name in the column
                int spacesToAdd = maxColWidth[j] - (grid[i][j] != nullptr ? grid[i][j]->name.length() : 0);
                for (int k = 0; k < spacesToAdd + 1; k++) // Add an extra space after the name
                {
                    cout << " ";
                }
                cout << "]\t\t";
            }
            cout << endl;
        }
    }
    void move(int row1, int col1)
    {
        if (row1 >= 0 && row1 < rows && col1 >= 0 && col1 < cols)
        {
            char userInput;
            int row2 = getPointerRow(); // Initialize the second row
            int col2 = getPointerCol(); // Initialize the second column

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
            else if (item->type == "HELMET" || item->type == "CHESTPLATE" || item->type == "LEGGINS" || item->type == "BOOTS")
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
    Item* weapon;
    Item* support;
    Item* helmet;
    Item* chestplate;
    Item* leggins;
    Item* boots;
    Equipment* eq;
public:
    Player()
    {
        gold = 100;
        HP = 100;
        weapon = nullptr;
        support = nullptr;
        helmet = nullptr;
        chestplate = nullptr;
        leggins = nullptr;
        boots = nullptr;
        eq = new Equipment();
    }
    vector<vector<Item*>>& getEquipmentGrid()
    {
        return eq->grid;
    }

    Equipment* getEq()
    {
        return eq;
    }

    int getAttack() const {
        return (weapon != nullptr) ? weapon->attack : 0;
    }

    Item* getWeapon() const {
        return weapon;
    }

    void decreaseWeaponDurability()
    {
        if (weapon->durability > 1)
        {
            weapon->durability -= 1;
        }
        else if (weapon->durability == 1)
        {
            cout << "Your " << weapon->name << " has broken up!" << endl;
            delete weapon;
            weapon = nullptr;
        }
        else
        {
            weapon->durability = 0;
        }
    }

    Item* getHelmet()
    {
        return helmet;
    }

    void setHelmet(Item* newHelmet)
    {
        helmet = newHelmet;
    }

    void decreaseHelmetDurability(Player& player)
    {
        if (player.getHelmet() != nullptr) {
            if (player.getHelmet()->durability > 1)
            {
                player.getHelmet()->durability -= 1;
            }
            else if (player.getHelmet()->durability == 1)
            {
                cout << "Your " << player.getHelmet()->name << " has broken!" << endl;
                delete player.getHelmet();
                player.setHelmet(nullptr);
            }
            else
            {
                player.getHelmet()->durability = 0;
            }
        }
    }

    Item* getChestplate()
    {
        return chestplate;
    }

    void setChestplate(Item* newChestplate)
    {
        chestplate = newChestplate;
    }

    void decreaseChestplateDurability(Player& player)
    {
        if (player.getChestplate() != nullptr) {
            if (player.getChestplate()->durability > 1)
            {
                player.getChestplate()->durability -= 1;
            }
            else if (player.getChestplate()->durability == 1)
            {
                cout << "Your " << player.getChestplate()->name << " has broken!" << endl;
                delete player.getChestplate();
                player.setChestplate(nullptr);
            }
            else
            {
                player.getChestplate()->durability = 0;
            }
        }
    }

    Item* getLeggins()
    {
        return leggins;
    }

    void setLeggins(Item* newLeggins)
    {
        leggins = newLeggins;
    }

    void decreaseLegginsDurability(Player& player)
    {
        if (player.getLeggins() != nullptr) {
            if (player.getLeggins()->durability > 1)
            {
                player.getLeggins()->durability -= 1;
            }
            else if (player.getLeggins()->durability == 1)
            {
                cout << "Your " << player.getLeggins()->name << " has broken!" << endl;
                delete player.getLeggins();
                player.setLeggins(nullptr);
            }
            else
            {
                player.getLeggins()->durability = 0;
            }
        }
    }

    Item* getBoots()
    {
        return boots;
    }

    void setBoots(Item* newBoots)
    {
        boots = newBoots;
    }

    void decreaseBootsDurability(Player& player)
    {
        if (player.getBoots() != nullptr) {
            if (player.getBoots()->durability > 1)
            {
                player.getBoots()->durability -= 1;
            }
            else if (player.getBoots()->durability == 1)
            {
                cout << "Your " << player.getBoots()->name << " has broken!" << endl;
                delete player.getBoots();
                player.setBoots(nullptr);
            }
            else
            {
                player.getBoots()->durability = 0;
            }
        }
    }


    int getDefense() const {
        int totalDefense = 0;
        totalDefense += (helmet != nullptr) ? helmet->durability : 0;
        totalDefense += (chestplate != nullptr) ? chestplate->durability : 0;
        totalDefense += (leggins != nullptr) ? leggins->durability : 0;
        totalDefense += (boots != nullptr) ? boots->durability : 0;
        return totalDefense;
    }

    int getGold() const {
        return gold;
    }

    void setGold(int newGold)
    {
        gold = newGold;
    }

    int getHP() const {
        return HP;
    }

    void reduceHP(int damage)
    {
        // Calculate the actual damage after considering resistance
        int actualDamage = damage - (damage * getDefense() / 100);

        // Ensure actual damage is at least 1 to prevent healing
        actualDamage = max(1, actualDamage);

        HP -= actualDamage;
    }

    void setMainWeapon(int i, int j)
    {
        Item* temp = weapon;
        weapon = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }

    void setMainSupport(int i, int j)
    {
        Item* temp = support;
        support = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }

    void setMainHelmet(int i, int j)
    {
        Item* temp = helmet;
        helmet = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }

    void setMainChestplate(int i, int j)
    {
        Item* temp = chestplate;
        chestplate = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }

    void setMainLeggins(int i, int j)
    {
        Item* temp = leggins;
        leggins = eq->grid[i][j];
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

    void displayPlayerStats()
    {
        system("cls");
        cout << "Your gold: " << gold << endl;
        cout << "HP: " << HP << endl;
        cout << "Weapon: " << (weapon != nullptr ? weapon->name : "") << endl;
        cout << "Support: " << (support != nullptr ? support->name : "") << endl;
        cout << "Helmet: " << (helmet != nullptr ? helmet->name : "") << endl;
        cout << "Chestplate: " << (chestplate != nullptr ? chestplate->name : "") << endl;
        cout << "Leggins: " << (leggins != nullptr ? leggins->name : "") << endl;
        cout << "Boots: " << (boots != nullptr ? boots->name : "") << endl;
    }

    void buy(int row, int col, Shop& shop)
    {
        Item* newItem;
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
                shop.showItemDetails(row, col);
                cout << "Do you want to buy " << itemToBuy->name << " for " << itemToBuy->price << "$? \nY/N\n";
                cin >> choice;
                switch(choice)
                {
                case 'Y':
                case 'y':
                    gold -= itemToBuy->price;
                    // Create a new item for the player's equipment and copy the data
                    newItem = new Item(*itemToBuy);
                    // Remove the bought item from the shop's inventory
                    delete shop.grid[row][col];
                    shop.grid[row][col] = nullptr;
                    // Find an empty slot in the player's equipment to place the new item
                    for (int i = 0; i < eq->getRows(); i++)
                    {
                        for (int j = 0; j < eq->getCols(); j++)
                        {
                            if (eq->grid[i][j] == nullptr)
                            {
                                eq->grid[i][j] = newItem;
                                return;
                            }
                        }
                    }
                    cout << "Cannot find an empty slot in the player's equipment" << endl;
                    delete newItem; // Deallocate memory if no empty slot is found
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

class Enemy
{
private:
    int HP;
    int level;
    int attack;
    int resistance;

public:
    Enemy() : HP(5), level(1), attack(3), resistance(0) {}

    void attackPlayer(Player& player)
    {
        int damageToPlayer = attack - player.getDefense();
        if (damageToPlayer < 0)
            damageToPlayer = 0; // Ensure damage is non-negative

        // Inflict damage on the player
        player.reduceHP(damageToPlayer);
    }

    void levelUp()
    {
        HP = 5;
        level++;
        HP += HP * level;
        attack += level;
        if (level >= 3)
        {
            resistance += 3;
        }
    }

    void reduceHP(int damage)
    {
        // Calculate the actual damage after considering resistance
        int actualDamage = damage - (damage * resistance / 100);

        // Ensure actual damage is at least 1 to prevent healing
        actualDamage = max(1, actualDamage);

        HP -= actualDamage;
    }

    int getAttack() const {
        return attack;
    }

    int getHP() const {
        if (HP < 0)
        {
            return 0;
        }
        return HP;
    }

    int getLevel() const {
        return level;
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
    int enemyRow;
    int enemyCol;
public:
    GameBoard(int s) : size(s), playerRow(s / 2), playerCol(s / 2), shopRow(rand() % s), shopCol(rand() % s), enemyRow(rand() % s), enemyCol(rand() % s) {}

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

                else if (i == enemyRow && j == enemyCol)
                    cout << "E ";

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
    int getEnemyRow() const {
        return enemyRow;
    }
    int getEnemyCol() const {
        return enemyCol;
    }
};


class Game
{
private:
    Player player;
    Enemy enemy;
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
                        if(player.getEq()->grid[selectedItemRow][selectedItemCol]->type == "WEAPON")
                            player.setMainWeapon(selectedItemRow, selectedItemCol);

                        else if(player.getEq()->grid[selectedItemRow][selectedItemCol]->type == "SUPPORT")
                            player.setMainSupport(selectedItemRow, selectedItemCol);

                        else if(player.getEq()->grid[selectedItemRow][selectedItemCol]->type == "HELMET")
                            player.setMainHelmet(selectedItemRow, selectedItemCol);

                        else if(player.getEq()->grid[selectedItemRow][selectedItemCol]->type == "CHESTPLATE")
                            player.setMainChestplate(selectedItemRow, selectedItemCol);

                        else if(player.getEq()->grid[selectedItemRow][selectedItemCol]->type == "LEGGINS")
                            player.setMainLeggins(selectedItemRow, selectedItemCol);

                        else if(player.getEq()->grid[selectedItemRow][selectedItemCol]->type == "BOOTS")
                            player.setMainBoots(selectedItemRow, selectedItemCol);

                        else
                        {
                            cout << "Invalid item type" << endl;
                        }
                        inItemOptions = false;
                        showInventory = true;
                    }
                    if (selectedOption == 4)
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
                    if (selectedOption == 5)
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
        cout << (selectedOption == 3 ? "> " : "  ") << "4. Select as main " << selectedItem->type << endl;
        cout << "Options for equipment:" << endl;
        cout << (selectedOption == 4 ? "> " : "  ") << "5. Expand inventory" << endl;
        cout << (selectedOption == 5 ? "> " : "  ") << "6. Display stats" << endl;
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

    void play()
    {
        srand(time(NULL));
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
                        system("cls");
                        gameBoard.display();
                    }
                    continue; // Continue to wait for input after displaying inventory
                }
                else if (input == 27)
                {
                    option = pauseGame(option);
                    if (option == 0) {
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

                // Adding shop and enemy logic to game
                int shopRow = gameBoard.getShopRow();
                int shopCol = gameBoard.getShopCol();
                int enemyRow = gameBoard.getEnemyRow();
                int enemyCol = gameBoard.getEnemyCol();

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
                else if ((newRow == enemyRow && newCol == enemyCol) && input == 13)
                {
                    bool selecting = true;
                    while(selecting == true)
                    {
                        system("cls");
                        cout << "You have encountered an enemy!" << endl;
                        cout << "Choose your action:" << endl;
                        cout << (selectedIndex == 0 ? "> " : "  ") << "1. Attack enemy" << endl;
                        cout << (selectedIndex == 1 ? "> " : "  ") << "2. Edit inventory" << endl;
                        cout << (selectedIndex == 2 ? "> " : "  ") << "3. Display player stats" << endl;
                        cout << (selectedIndex == 3 ? "> " : "  ") << "4. Escape the enemy" << endl;
                        char userInput = _getch();

                        switch (userInput)
                        {
                        case 'w':
                        case 'W':
                        case 72: // Up arrow
                            selectedIndex = (selectedIndex - 1 + 4) % 4;
                            break;
                        case 's':
                        case 'S':
                        case 80: // Down arrow
                            selectedIndex = (selectedIndex + 1) % 4;
                            break;
                        case 13:
                            if (selectedIndex == 0)
                            {
                                fightEnemy(player, enemy);
                                selecting = false;
                            }
                            else if (selectedIndex == 1)
                            {
                                showInGameInventory();
                            }
                            else if (selectedIndex == 2)
                            {
                                player.displayPlayerStats();
                                player.getEq()->popInventory();
                                cout << "Press any key to continue...";
                                _getch();
                            }
                            else if (selectedIndex == 3)
                            {
                                system("cls");
                                gameBoard.display();
                                selecting = false;
                            }
                            break;
                        }
                    }
                }
                else
                {
                    switch (input) {
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
    }

    void fightEnemy(Player& player, Enemy& enemy)
    {
        bool defending = false;
        while (true)
        {
            system("cls");

            int damageToEnemy = 0;

            cout << "Player Stats:" << endl;
            cout << "HP: " << player.getHP() << endl;

            cout << endl;

            cout << "Enemy Stats:" << endl;
            cout << "HP: " << enemy.getHP() << endl;
            cout << "Level: " << enemy.getLevel() << endl;

            cout << "\nChoose your action:" << endl;
            cout << "1. Attack" << endl;
            cout << "2. Defend" << endl;

            char choice;
            cin >> choice;

            if (choice == '1') {
                if (player.getWeapon() == nullptr) 
                {
                    cout << "\nYou don't have a weapon equipped!\nDo you want to escape?\nY/N\n";
                    char escapeChoice;
                    cin >> escapeChoice;
                    if (escapeChoice == 'Y' || escapeChoice == 'y')
                    {
                        cout << "You escaped the enemy" << endl;
                        cout << "Press any key to continue...";
                        _getch();
                        break; // Exit the loop and return to the game
                    }
                    else
                    {
                        cout << "You have died from the enemy's hand :(";
                        _getch();
                        gameOver();
                    }
                }

                damageToEnemy = player.getAttack();
                enemy.reduceHP(damageToEnemy);
                player.decreaseWeaponDurability();
                cout << "\nYou attacked the enemy! Enemy HP: " << enemy.getHP() << endl;
            }
            else if (choice == '2')
            {
                cout << "\nYou chose to defend." << endl;
                defending = true;
            }
            else
            {
                cout << "\nInvalid choice. Please choose again." << endl;
                continue;
            }

            if (enemy.getHP() > 0 && !defending)
            {
                enemy.attackPlayer(player);
                player.decreaseHelmetDurability(player);
                player.decreaseChestplateDurability(player);
                player.decreaseLegginsDurability(player);
                player.decreaseBootsDurability(player);
                cout << "Enemy attacked! Player HP: " << player.getHP() << endl;
            }
            else if (defending)
            {
                cout << "You defended enemy's attack. Player HP: " << player.getHP() << endl;
            }

            if (player.getHP() <= 0)
            {
                gameOver();
                break;
            }
            else if (enemy.getHP() <= 0)
            {
                win(player, enemy);
                break;
            }

            cout << "\nPress any key to continue...";
            _getch();
        }
    }


    void gameOver()
    {
        system("cls");
        cout << "Game Over!" << endl;
        exit(0);
    }

    void win(Player& player, Enemy& enemy)
    {
        int goldEarned = player.getGold() + (10 * enemy.getLevel());
        player.setGold(goldEarned);
        enemy.levelUp();

        cout << "You defeated the enemy and earned " << goldEarned << " gold!" << endl;

        cout << "Do you want to continue fighting? Y/N\n";
        char choice;
        cin >> choice;

        if (choice == 'Y' || choice == 'y')
        {
            fightEnemy(player, enemy);
        }
        else if (choice == 'N' || choice == 'n')
        {
            system("cls");
            gameBoard.display();
            _getch();
        }
        else
        {
            cout << "Invalid option" << endl;
            win(player, enemy); // Recursively call the function to ask again
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
    Game game;
    game.play();

    return 0;
}
