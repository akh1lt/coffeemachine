#include "ingredient.h"
#include <utility>
#include<map>
#include<string>
#include<cassert>
using namespace std;

/**
 * 1. Drinks are characterized by:
 * a. name
 * b. temperature
 * c. recipe
 */
enum DrinkType {
    COLD,
    HOT
};

class Drink {
private:
    string drink_name;
    DrinkType drink_type;
    map<Ingredient, int> recipe;

public:
    Drink(string drink_name, DrinkType drink_type, Ingredient base_ingredient, int base_quantity);
    Drink(string drink_name, DrinkType drink_type, map<Ingredient, int> recipe);
    void add_ingredient(Ingredient ingredient, int quantity);
    string get_drink_name();
    DrinkType get_drink_type();
    map<Ingredient, int> get_recipe();
};
