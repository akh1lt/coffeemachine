#include "coffee_machine.h"

/**
 * Test for
 * 1. Validating the concurrency of the drink preparation.
 * 2. Resource(ingredients) synchronization among the server threads.
 */
int main() {

    // initialize the machine
    map<Ingredient, int> ingredient_slots;
    ingredient_slots[WATER] = 30;
    ingredient_slots[MILK] = 30;
    ingredient_slots[SUGAR] = 30;
    ingredient_slots[TEA_POWDER] = 30;
    ingredient_slots[COFFEE_POWDER] = 30;
    CoffeeMachine machine(4, 4, 4, 20, ingredient_slots);

    // drink recipes
    Drink tea("tea", HOT, WATER, 30);
    tea.add_ingredient(SUGAR, 30);
    tea.add_ingredient(TEA_POWDER, 30);
    tea.add_ingredient(MILK, 30);

    Drink coffee("coffee", HOT, WATER, 30);
    coffee.add_ingredient(SUGAR, 30);
    coffee.add_ingredient(COFFEE_POWDER, 30);
    coffee.add_ingredient(MILK, 30);

    Drink cold_coffee("cold coffee", COLD, WATER, 30);
    cold_coffee.add_ingredient(SUGAR, 30);
    cold_coffee.add_ingredient(COFFEE_POWDER, 30);

    machine.add_drink(tea);
    machine.add_drink(coffee);
    machine.add_drink(cold_coffee);

    // only one drink succeeds
    machine.request_drink("tea");
    machine.request_drink("coffee");
    machine.request_drink("cold coffee");

    this_thread::sleep_for(chrono::seconds(5));

    // wait for completion & turn off
    this_thread::sleep_for(chrono::seconds(5));
    machine.turn_off();
}
