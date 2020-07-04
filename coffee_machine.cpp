#include "coffee_machine.h"

/**
 * Initializes ingredients with maximum capacity.
 * @param num_outlets
 * @param num_heaters
 * @param num_coolers
 * @param service_limit
 * @param ingredient_slots
 */
CoffeeMachine::CoffeeMachine(int num_outlets, int num_heaters, int num_coolers, int service_limit,
                             map<Ingredient, int> ingredient_slots) {
    assert(num_outlets > 0);
    assert(num_heaters > 0);
    assert(num_coolers > 0);
    assert(service_limit > 1);
    for (auto slot : ingredient_slots) {
        assert(slot.second > 0);
    }

    this->service_limit = service_limit;
    this->num_outlets = num_outlets;
    this->ingredient_slots = ingredient_slots;
    this->ingredient_stock = ingredient_slots;

    mutx = PTHREAD_MUTEX_INITIALIZER;
    sem_init(&cookers[COLD], 0, num_coolers);
    sem_init(&cookers[HOT], 0, num_heaters);
    request_available = PTHREAD_COND_INITIALIZER;
    for (int i = 0; i < num_outlets; i++) {
        servers.push_back(thread(&CoffeeMachine::serve_drink, this));
    }
}

/**
 * 1. Method for destroying the thread pool.
 * 2. Sends TERMINATE message as a new drink request.
 * 3. Waits for all the threads to exit.
 */
void CoffeeMachine::turn_off() {
    for (int i = 0; i < servers.size(); i++) {
        request_drink(TERMINATE);
    }
    for (int i = 0; i < servers.size(); i++) {
        servers[i].join();
    }
}

/**
 * 1. Interface for adding the ingredients.
 * 2. Constraints the user to add the ingredient with full capacity of the machine.
 * @param ingredient
 */
void CoffeeMachine::refill_ingredient(Ingredient ingredient) {
    pthread_mutex_lock(&mutx);
    assert(ingredient_slots.find(ingredient) != ingredient_slots.end());
    ingredient_stock[ingredient] = ingredient_slots[ingredient];
    pthread_mutex_unlock(&mutx);
}

/**
 * 1. Interface for plugging in new drink recipes to the machine.
 * @param drink
 */
void CoffeeMachine::add_drink(Drink drink) {
    pthread_mutex_lock(&mutx);
    for (auto ingredient : drink.get_recipe()) {
        assert(this->ingredient_stock[ingredient.first] >= ingredient.second);
    }
    drinks_served.insert(make_pair(drink.get_drink_name(), drink));
    pthread_mutex_unlock(&mutx);
}

/**
 * 1. Wait for a new drink requests
 * 2. If the request is a TERMINATE signal from the machine, the thread exits.
 * 3. Verifies if all the ingredients are available and atomically fetches them.
 * 4. Displays the user about all the missing ingredients.
 * 5. Displays user message once the drink is served.
 */
void CoffeeMachine::serve_drink() {
    while (true) {

        // Wait for a an order
        pthread_mutex_lock(&mutx);
        while (drink_requests.empty()) {
            pthread_cond_wait(&request_available, &mutx);
        }

        string drink_to_make = drink_requests.front();
        drink_requests.pop();

        // Check for turn off signal
        if (drink_to_make == TERMINATE) {
            pthread_mutex_unlock(&mutx);
            return;
        }


        pthread_mutex_unlock(&mutx);

        // Get order details
        Drink curr_drink = drinks_served.find(drink_to_make)->second;
        DrinkType drink_type = curr_drink.get_drink_type();
        bool can_make_the_drink = true;

        // Fetch the ingredients
        pthread_mutex_lock(&mutx);
        string missing_ingredients = "";
        for (auto ingredient : curr_drink.get_recipe()) {

            if (this->ingredient_stock[ingredient.first] < ingredient.second) {
                can_make_the_drink = false;
                missing_ingredients += get_ingredient_name(ingredient.first);
                missing_ingredients += ";";
            }
        }

        // Acquire all the ingredients required
        if (can_make_the_drink) {
            for (auto ingredient : curr_drink.get_recipe()) {
                this->ingredient_stock[ingredient.first] -= ingredient.second;
            }
        } else {
            // Let user know about the missing ingredients
            cout << "[DISPLAY] Can't prepare : " << drink_to_make << ". Refill ingredients: " << missing_ingredients << endl;
            fflush(stdout);
        }

        pthread_mutex_unlock(&mutx);

        // If at least one of the ingredients is missing, stop making the drink
        if (!can_make_the_drink) continue;

        // Cook & serve the drink
        cout << "[DISPLAY] preparing : " << drink_to_make << endl;
        fflush(stdout);

        sem_wait(&cookers[drink_type]);
        // mixing, combining, and heating or cooling the ingredients
        this_thread::sleep_for(chrono::seconds(PREPARATION_TIME));
        sem_post(&cookers[drink_type]);

        cout << "[DISPLAY] ready to drink : " << drink_to_make << endl;
        fflush(stdout);
    }
}

/**
 * 1. Add new drink request to the request queue.
 * 2. Check if the request is actually TERMINATE sent by the machine.
 * @param drink_name
 */
void CoffeeMachine::request_drink(string drink_name) {
    pthread_mutex_lock(&mutx);
    if (drink_name != TERMINATE && drink_requests.size() == service_limit) {
        cout << "[DISPLAY] Machine busy, retry later!!" << endl;
        fflush(stdout);
        pthread_mutex_unlock(&mutx);
        return;
    }

    assert(drink_name == TERMINATE || drinks_served.find(drink_name) != drinks_served.end());
    drink_requests.push(drink_name);
    pthread_mutex_unlock(&mutx);
    pthread_cond_signal(&request_available);
}