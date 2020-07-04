#include"drink.h"
#include <utility>
#include<map>
#include<string>
#include<cassert>
#include<semaphore.h>
#include<pthread.h>
#include<thread>
#include<vector>
#include<iostream>
#include<chrono>
#include<queue>
using namespace std;

/**
 * DESIGN:
 * 1. The machine has a fixed number of coffee makers.
 * 2. Coffee maker is a sub component which can independently handle most of the steps involved in preparation of a
 * drink.
 * 3. The machine has two type of temperature controllers. Which will be shared by all the makers.
 *    (a) heater
 *    (b) cooler
 * 4. The number of temperature controllers is decided during the manufacture time.
 * 5. The machine has preset number of slots for a given set of ingredients.
 * 7. The preparation time is preset
 * 8. There is a service limit on the number of requests the machine can handle.
 *
 * INTERFACE:
 * 1. Turn off the machine
 * 2. Request for a drink
 * 3. Add a new drink recipe using the ingredients supported by the machine
 * 4. Refill the ingredients
 *
 * SYNCHRONIZATION CONSTRAINTS:
 * 1. Servers/makers block on the empty request queue.
 * 2. Access to all shared data structures is controlled by mutex.
 * 3. Access to heater or cooler needs to be synchronized.
 * 4. Actual drink preparation needs to happen concurrently.
 *
 * ASSUMPTIONS FOR SIMPLIFICATION:
 * 1. The drink preparation time is hard coded & not flexible.
 * 2. The refill amount is not flexible and fixed, which is same as the maximum capacity supported by the machine.
 * 3. Temperature is associated with the whole of the drink instead of any specific ingredient.
 * 4. Refill responsibility falls under the user of the machine.
 * 5. Ingredients & their capacities can't be altered.
 * 6. Number of outlets = number of coffee makers(worker threads).
 */

class CoffeeMachine {
private:
    int PREPARATION_TIME = 1;
    string TERMINATE = "SIG_TERMINATE";

    int num_outlets;
    int service_limit; // number of active requests the machine can track
    queue<string> drink_requests; // fifo to serve the drink requests
    pthread_mutex_t mutx; // mutex for the critical sections
    pthread_cond_t request_available; // conditional variable for communication between the server threads & machine
    sem_t cookers[2];
    vector<thread> servers;

    map<Ingredient, int> ingredient_slots;
    map<Ingredient, int> ingredient_stock;
    map<string, Drink> drinks_served;
    void serve_drink();

public:
    CoffeeMachine(int num_outlets, int num_heaters, int num_coolers, int service_limit,
                  map<Ingredient, int> ingredient_slots);
    void refill_ingredient(Ingredient ingredient);
    void add_drink(Drink drink);
    void request_drink(string drink_name);
    void turn_off();
};