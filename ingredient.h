#include<map>
#include<string>
using namespace std;

/**
 * Ingredient set.
 */
enum Ingredient {
    WATER,
    MILK,
    ELAICHI,
    SUGAR,
    TEA_POWDER,
    COFFEE_POWDER
};

string get_ingredient_name(Ingredient ingredient);