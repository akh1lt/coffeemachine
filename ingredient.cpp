#include "ingredient.h"

/**
 * Enum to verbal mapping of the ingredients for user messaging.
 * @param ingredient
 * @return
 */
string get_ingredient_name(Ingredient ingredient) {
    switch(ingredient) {
        case MILK:
            return "milk";
        case ELAICHI :
            return "elaichi";
        case SUGAR:
            return "sugar";
        case TEA_POWDER:
            return "tea powder";
        case COFFEE_POWDER:
            return "coffee powder";
        default:
            return "water";
    }
}