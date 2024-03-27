#include <iostream>
#include <string>
using namespace std;

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
    Item*** grid;
    Shop(int rows = 5, int cols = 5) : rows{rows}, cols{cols}
    {
        grid = new Item**[rows];
        for(int i = 0; i < rows; i++)
        {
            grid[i] = new Item*[cols];
        }
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

    ~Shop()
    {
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                delete grid[i][j];
            }
        }
        for(int j = 0; j < rows; j++)
        {
            delete[] grid[j];
        }
        delete[] grid;
    }
};

class Equipment
{
    int rows;
    int cols;
public:
    Item*** grid;
    Equipment(int rows = 5, int cols = 5) : rows{rows}, cols{cols}
    {
        grid = new Item**[rows];
        for(int i = 0; i < rows; i++)
        {
            grid[i] = new Item*[cols];
        }
        int count = 0;
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                grid[i][j] = new Item("item" + to_string(count), "DEFAULT");
                count++;
            }
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
        for(int j = 0; j < rows; j++)
        {
            delete[] grid[j];
        }
        delete[] grid;
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
    int gold;
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
    ~Player()
    {
        delete eq;
    }
};

int main()
{
    Player P;
    Shop shop;
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
    shop.display();

    return 0;
}
