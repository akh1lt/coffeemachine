#include "coffee_machine.h"

/**
 * Test for
 * 1. Verifying the refill signal in case of missing ingredients.
 * 2. Interface for the refill.
 */
int main() {

    // initialize the machine
    map<Ingredient, int> ingredient_slots;
    ingredient_slots[WATER] = 30;
    ingredient_slots[MILK] = 30;
    ingredient_slots[SUGAR] = 30;
    ingredient_slots[TEA_POWDER] = 30;
    ingredient_slots[COFFEE_POWDER] = 30;
    CoffeeMachine machine(1, 1, 1, 20, ingredient_slots);

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

    this_thread::sleep_for(chrono::seconds(5));

    // fails with a refill message
    machine.request_drink("coffee");
    this_thread::sleep_for(chrono::seconds(5));
    machine.refill_ingredient(SUGAR);

    // fails with a refill message
    machine.request_drink("coffee");
    this_thread::sleep_for(chrono::seconds(5));
    machine.refill_ingredient(WATER);

    // succeeds
    this_thread::sleep_for(chrono::seconds(5));
    machine.request_drink("coffee");

    // wait for completion & turns off
    this_thread::sleep_for(chrono::seconds(5));
    machine.turn_off();
}
