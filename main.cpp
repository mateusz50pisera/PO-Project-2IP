#include <iostream>
#include <string>
#include <vector>
#include <map>
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
    string name;
    int durability;
    string details;
    int attack;
    int resistance;
    string type;
    int price;

    Item(string name, string type, int durability = 100, int attack = 0, int resistance = 0, string details = "No details")
        : name{name}, type{type}, durability{durability}, details{details}, attack{attack}, resistance{resistance} {}
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
                grid[i][j] = new Item("item" + to_string(count), "DEFAULT", 100, 10, 0, "Default item for sale");
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
                cout << "[" << grid[i][j]->name << ", $" << grid[i][j]->price << "]\t";
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

public:
    vector<vector<Item*>> grid;
    Equipment(int rows = 5, int cols = 5) : rows{rows}, cols{cols}, grid(rows, vector<Item*>(cols))
    {
        int count = 0;
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                grid[i][j] = new Item("item" + to_string(count), "DEFAULT", 100, 10, 0, "Default item for sale");
                count++;
            }
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
        if (rows < 10 && cols < 10) // Ensure the grid doesn't exceed 10x10
        {
            int newRowSize = rows + 1;
            int newColSize = cols + 1;

            // Create a new grid with expanded size
            vector<vector<Item*>> newGrid(newRowSize, vector<Item*>(newColSize, nullptr));

            // Copy existing items to the new grid
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    newGrid[i][j] = grid[i][j];
                }
            }

            // Initialize newly added elements in the last row
            for (int j = 0; j < newColSize; ++j) {
                newGrid[newRowSize - 1][j] = new Item("item" + to_string((newRowSize - 1) * newColSize + j), "DEFAULT", 100, 10, 0, "Default item for sale");
            }

            // Delete the old grid items
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    delete grid[i][j];
                }
            }

            // Clear the old grid
            grid.clear();

            // Assign the new grid to the equipment
            grid = newGrid;

            // Update the size of rows and columns
            setRows(newRowSize);
            setCols(newColSize);

            cout << "Inventory expanded to " << newRowSize << "x" << newColSize << endl;
        }
        else
        {
            cout << "Cannot expand inventory after 10x10" << endl;
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
    void display()
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
            grid[row][col] = new Item("none", "DEFAULT", 100, 0, 0, "No item available");
        }
        else
        {
            cout << "Invalid position or no item found in player's equipment." << endl;
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
        else if (gold < 300)
        {
            cout << "Not enough gold." << endl;
        }
        else
        {
            cout << "There's an error" << endl;
        }
    }

    void sort(bool asc = false){

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
                if (eq->getRows() * eq->getCols() >= 6 * 6) {
                    cout << "Inventory is full, cannot buy more items." << endl;
                    return;
                }

                gold -= itemToBuy->price;
                // Remove the bought item from the shop's inventory
                delete shop.grid[row][col];
                shop.grid[row][col] = new Item("none", "DEFAULT", 100, 0, 0, "No item available");
                // Add the bought item to the player's equipment
                eq->move(0, 0, row, col); // Move to first available slot in player's equipment
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

int main()
{
    Player P;
    Shop shop;
    P.gold = 300;
    P.showEq();
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
    P.displayPlayerStats();
    P.moveItem(4, 4, 2, 0);
    P.displayPlayerStats();
    P.showDetails(2, 0);
    P.showGold();
    P.buy(1, 1, shop);
    P.showEq();
    shop.display();
    P.sell(2, 0, shop);
    shop.display();
    P.removeItem(1, 1);
    P.showEq();
    P.expandInventory();
    P.sort(true);
    P.showEq();

    return 0;
}
