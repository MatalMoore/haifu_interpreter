#include <string>
#include <map>

// element strings
#define ELEM_DEFAULT_STRING "_"
#define ELEM_NONE_STRING "none"
#define ELEM_EARTH_STRING "earth"
#define ELEM_METAL_STRING "metal"
#define ELEM_WATER_STRING "water"
#define ELEM_WOOD_STRING "wood"
#define ELEM_FIRE_STRING "fire"

// element codes
#define ELEM_DEFAULT 0
#define ELEM_NONE 1
#define ELEM_EARTH 2
#define ELEM_METAL 3
#define ELEM_WATER 4
#define ELEM_WOOD 5
#define ELEM_FIRE 6

// converts a string to an element code
char toElement(const std::string& text);
// converts an element code to a string
std::string toElementString(const char element);

// returns the element after the parameter element in the create cycle
char progressElement_create(const char element);
// returns the element after the parameter element in the destroy cycle
char progressElement_destroy(const char element);
// returns the element after the parameter element in the fear cycle
char progressElement_fear(const char element);
// returns the element after the parameter element in the love cycle
char progressElement_love(const char element);

// returns whether element1 follows element 0 in the create cycle
bool testElements_create(const char element0, const char element1);
// returns whether element1 follows element 0 in the destroy cycle
bool testElements_destroy(const char element0, const char element1);
// returns whether element1 follows element 0 in the fear cycle
bool testElements_fear(const char element0, const char element1);
// returns whether element1 follows element 0 in the love cycle
bool testElements_love(const char element0, const char element1);