#include <iostream>
#include <string>
using namespace std;

class Item {
public:
    string name;
    Item(string name) : name{name} {}
};

class Equipment {
    int rows;
    int cols;
public:
    Item*** grid;
    Equipment(int rows = 5, int cols = 5) : rows{rows}, cols{cols} {
        grid = new Item**[rows];
        for(int i = 0; i < rows; i++) {
            grid[i] = new Item*[cols];
        }
        int count = 0;
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                grid[i][j] = new Item("item " + to_string(count));
                count++;
            }
        }
    }
    ~Equipment() {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                delete grid[i][j];
            }
        }
        for(int j = 0; j < rows; j++) {
            delete[] grid[j];
        }
        delete[] grid;
    }
    void display() {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                cout << "[" << (grid[i][j] != nullptr ? grid[i][j]->name : "none") << "]\t";
            }
            cout << endl;
        }
    }

// Added move method that swaps the positions between two items
    void move(int row1, int col1, int row2, int col2) {
        if(row1 < rows && col1 < cols && row2 < rows && col2 < cols) {
            Item* temp = grid[row1][col1];
            grid[row1][col1] = grid[row2][col2];
            grid[row2][col2] = temp;
        }
        else {
            cout << "Invalid values entered" << endl;
        }
    }
};

class Player {
    int HP;
    Item* mainHand;
    Item* sword;
    Item* helmet;
    Item* armor;
    Item* pants;
    Item* boots;
    Equipment* eq;
public:
    Player() {
        HP = 100;
        mainHand = nullptr;
        sword = nullptr;
        helmet = nullptr;
        armor = nullptr;
        pants = nullptr;
        boots = nullptr;
        eq = new Equipment();
    }
    void setMainWeapon(int i, int j) {
        Item* temp = mainHand;
        mainHand = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }
    void setMainSword(int i, int j) {
        Item* temp = sword;
        sword = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }
    void setMainHelmet(int i, int j) {
        Item* temp = helmet;
        helmet = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }
    void setMainArmor(int i, int j) {
        Item* temp = armor;
        armor = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }
    void setMainPants(int i, int j) {
        Item* temp = pants;
        pants = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }
    void setMainBoots(int i, int j) {
        Item* temp = boots;
        boots = eq->grid[i][j];
        eq->grid[i][j] = temp;
    }
    void showEq() {
        eq->display();
    }
// Added moveItem method that inherits from the equipment move method
    void moveItem(int row1, int col1, int row2, int col2) {
        eq->move(row1, col1, row2, col2);
    }
    void displayPlayerStats() {
        cout << "HP: " << HP << endl;
        cout << (mainHand != nullptr ? mainHand->name : "Fist") << endl;
        cout << (sword != nullptr ? sword->name : "Sword") << endl;
        cout << (helmet != nullptr ? helmet->name : "Helmet") << endl;
        cout << (armor != nullptr ? armor->name : "Armor") << endl;
        cout << (pants != nullptr ? pants->name : "Pants") << endl;
        cout << (boots != nullptr ? boots->name : "Boots") << endl;
        eq->display();
    }
    ~Player() {
        delete eq;
    }
};

int main()
{
    Player P;
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
    return 0;
}
