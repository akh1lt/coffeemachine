#include "coffee_machine.h"

/**
 * Test for checking the bound on number of active requests.
 */
int main() {
    // initialize the machine
    map<Ingredient, int> ingredient_slots;
    ingredient_slots[WATER] = 60;
    ingredient_slots[MILK] = 60;
    ingredient_slots[SUGAR] = 60;
    ingredient_slots[TEA_POWDER] = 60;
    ingredient_slots[COFFEE_POWDER] = 60;
    CoffeeMachine machine(3, 1, 1, 3, ingredient_slots);

    // drink recipes
    Drink tea("tea", HOT, WATER, 10);
    tea.add_ingredient(SUGAR, 10);
    tea.add_ingredient(TEA_POWDER, 10);
    tea.add_ingredient(MILK, 10);

    Drink coffee("coffee", HOT, WATER, 10);
    coffee.add_ingredient(SUGAR, 10);
    coffee.add_ingredient(COFFEE_POWDER, 10);
    coffee.add_ingredient(MILK, 10);

    Drink cold_coffee("cold coffee", COLD, WATER, 10);
    cold_coffee.add_ingredient(SUGAR, 10);
    cold_coffee.add_ingredient(COFFEE_POWDER, 10);

    machine.add_drink(tea);
    machine.add_drink(coffee);
    machine.add_drink(cold_coffee);


    // successful requests
    machine.request_drink("tea");
    machine.request_drink("coffee");
    machine.request_drink("cold coffee");

    // rejected for busy queue
    machine.request_drink("tea");
    machine.request_drink("coffee");
    machine.request_drink("cold coffee");

    this_thread::sleep_for(chrono::seconds(5));

    // succeeds for waiting enough time for the existing queue
    machine.request_drink("tea");

    // wait for completion & turn off
    this_thread::sleep_for(chrono::seconds(5));
    machine.turn_off();
}
