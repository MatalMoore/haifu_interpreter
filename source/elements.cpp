#include "elements.h"

using namespace std;

//-------------------------------------------------------------------------------
// toElement()
//-------------------------------------------------------------------------------
char toElement(const std::string& text) {
  if (text == ELEM_NONE_STRING) {
    return ELEM_NONE;
  }
  else if (text == ELEM_EARTH_STRING) {
    return ELEM_EARTH;
  }
  else if (text == ELEM_METAL_STRING) {
    return ELEM_METAL;
  }
  else if (text == ELEM_WATER_STRING) {
    return ELEM_WATER;
  }
  else if (text == ELEM_WOOD_STRING) {
    return ELEM_WOOD;
  }
  else if (text == ELEM_FIRE_STRING) {
    return ELEM_FIRE;
  }
  else {
    return ELEM_DEFAULT;
  }
}
//-------------------------------------------------------------------------------
// toElementString()
//-------------------------------------------------------------------------------
string toElementString(const char element) {
  switch (element) {
  case ELEM_NONE:
    return ELEM_NONE_STRING;
  case ELEM_EARTH:
    return ELEM_EARTH_STRING;
  case ELEM_METAL:
    return ELEM_METAL_STRING;
  case ELEM_WATER:
    return ELEM_WATER_STRING;
  case ELEM_WOOD:
    return ELEM_WOOD_STRING;
  case ELEM_FIRE:
    return ELEM_FIRE_STRING;
  default:
    return ELEM_DEFAULT_STRING;
  }
}

//-------------------------------------------------------------------------------
// progressElement_create()
//-------------------------------------------------------------------------------
char progressElement_create(const char element) {
  switch (element) {
  case ELEM_EARTH:
    return ELEM_METAL;
  case ELEM_METAL:
    return ELEM_WATER;
  case ELEM_WATER:
    return ELEM_WOOD;
  case ELEM_WOOD:
    return ELEM_FIRE;
  case ELEM_FIRE:
    return ELEM_EARTH;
  default:
    return ELEM_NONE;
  }
}
//-------------------------------------------------------------------------------
// progressElement_destroy()
//-------------------------------------------------------------------------------
char progressElement_destroy(const char element) {
  switch (element) {
  case ELEM_EARTH:
    return ELEM_WATER;
  case ELEM_METAL:
    return ELEM_WOOD;
  case ELEM_WATER:
    return ELEM_FIRE;
  case ELEM_WOOD:
    return ELEM_EARTH;
  case ELEM_FIRE:
    return ELEM_METAL;
  default:
    return ELEM_NONE;
  }
}
//-------------------------------------------------------------------------------
// progressElement_fear()
//-------------------------------------------------------------------------------
char progressElement_fear(const char element) {
  switch (element) {
  case ELEM_EARTH:
    return ELEM_WOOD;
  case ELEM_METAL:
    return ELEM_FIRE;
  case ELEM_WATER:
    return ELEM_EARTH;
  case ELEM_WOOD:
    return ELEM_METAL;
  case ELEM_FIRE:
    return ELEM_WATER;
  default:
    return ELEM_NONE;
  }
}
//-------------------------------------------------------------------------------
// progressElement_love()
//-------------------------------------------------------------------------------
char progressElement_love(const char element) {
  switch (element) {
  case ELEM_EARTH:
    return ELEM_FIRE;
  case ELEM_METAL:
    return ELEM_EARTH;
  case ELEM_WATER:
    return ELEM_METAL;
  case ELEM_WOOD:
    return ELEM_WATER;
  case ELEM_FIRE:
    return ELEM_WOOD;
  default:
    return ELEM_NONE;
  }
}

//-------------------------------------------------------------------------------
// testElements_create()
//-------------------------------------------------------------------------------
bool testElements_create(const char element0, const char element1) {
  return element1 == progressElement_create(element0);
}
//-------------------------------------------------------------------------------
// testElements_destroy()
//-------------------------------------------------------------------------------
bool testElements_destroy(const char element0, const char element1) {
  return element1 == progressElement_destroy(element0);
}
//-------------------------------------------------------------------------------
// testElements_fear()
//-------------------------------------------------------------------------------
bool testElements_fear(const char element0, const char element1) {
  return element1 == progressElement_fear(element0);
}
//-------------------------------------------------------------------------------
// testElements_love()
//-------------------------------------------------------------------------------
bool testElements_love(const char element0, const char element1) {
  return element1 == progressElement_love(element0);
}