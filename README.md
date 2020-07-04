# Execution
g++ -std=c++11 -w -c ingredient.cpp -o ingredient.o
g++ -std=c++11 -w -c drink.cpp -o drink.o
g++ -std=c++11 -w -c coffee_machine.cpp -o coffee_machine.o
g++ -std=c++11 -w -c test_refill.cpp -o test_refill.o
g++ ingredient.o drink.o coffee_machine.o test_refill.o -o test_refill
g++ -std=c++11 -w -c test_can_only_make_single_drink.cpp -o test_can_only_make_single_drink.o
g++ ingredient.o drink.o coffee_machine.o test_can_only_make_single_drink.o -o test_can_only_make_single_drink
g++ -std=c++11 -w -c test_busy.cpp -o test_busy.o
g++ ingredient.o drink.o coffee_machine.o test_busy.o -o test_busy

# Note
1. The standard outputs from tests might be interleaved
2. It was a hack to add sleep in the test programs to stop compiler optimization from screwing with the order of 
commands provided to the machine

# Tests
1. test_can_only_make_single_drink
    a. command: ./test_can_only_make_single_drink
    b. validates concurrent execution
    c. threads compete for the common ingredients
    d. sample executions
(a)
[DISPLAY] preparing : coffee
[DISPLAY] Can't prepare : tea. Refill ingredients: water;milk;sugar;
[DISPLAY] Can't prepare : cold coffee. Refill ingredients: water;sugar;coffee powder;
[DISPLAY] ready to drink : coffee
(b)
[DISPLAY] preparing : tea
[DISPLAY] Can't prepare : cold coffee. Refill ingredients: water;sugar;
[DISPLAY] Can't prepare : coffee. Refill ingredients: water;milk;sugar;
[DISPLAY] ready to drink : tea

2. test_busy
    a. command: ./test_busy
    b. checks bound on the number of active drink requests
    c. sample executions
(a)
[DISPLAY] Machine busy, retry later!!
[DISPLAY] Machine busy, retry later!!
[DISPLAY] Machine busy, retry later!!
[DISPLAY] preparing : coffee
[DISPLAY] preparing : tea
[DISPLAY] preparing : cold coffee
[DISPLAY] ready to drink : coffee
[DISPLAY] ready to drink : cold coffee
[DISPLAY] ready to drink : tea
[DISPLAY] preparing : tea
[DISPLAY] ready to drink : tea
(b)
[DISPLAY] Machine busy, retry later!!
[DISPLAY] Machine busy, retry later!!
[DISPLAY] preparing : cold coffee
[DISPLAY] preparing : coffee
[DISPLAY] preparing : tea
[DISPLAY] ready to drink : cold coffee[DISPLAY] ready to drink : tea
[DISPLAY] preparing : tea

[DISPLAY] ready to drink : coffee
[DISPLAY] ready to drink : tea
[DISPLAY] preparing : tea
[DISPLAY] ready to drink : tea

3. test_refill
    a. command: ./test_refill
    b. verifies refill signal & drink preparation after refill
    c. sample execution
(a)
[DISPLAY] preparing : tea
[DISPLAY] ready to drink : tea
[DISPLAY] preparing : coffee
[DISPLAY] ready to drink : coffee
[DISPLAY] preparing : cold coffee
[DISPLAY] ready to drink : cold coffee
[DISPLAY] Can't prepare : coffee. Refill ingredients: water;sugar;
[DISPLAY] Can't prepare : coffee. Refill ingredients: water;
[DISPLAY] preparing : coffee
[DISPLAY] ready to drink : coffee