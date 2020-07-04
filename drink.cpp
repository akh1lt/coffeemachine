#include "drink.h"

Drink::Drink(string drink_name, DrinkType drink_type, Ingredient base_ingredient, int base_quantity) {
    assert(base_quantity > 0);
    this->drink_name = drink_name;
    this->drink_type = drink_type;
    this->recipe.insert(make_pair(base_ingredient, base_quantity));
}

Drink::Drink(string drink_name, DrinkType drink_type, map<Ingredient, int> recipe) {
    assert(recipe.size() > 0);
    this->drink_name = drink_name;
    this->drink_type = drink_type;
    this->recipe = recipe;
}

string Drink::get_drink_name() {
    return this->drink_name;
}

DrinkType Drink::get_drink_type() {
    return this->drink_type;
}

void Drink::add_ingredient(Ingredient ingredient, int quantity) {
    this->recipe[ingredient] = quantity;
}

map<Ingredient, int> Drink::get_recipe() {
    return this->recipe;
}