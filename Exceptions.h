#ifndef HW1_EXCEPTION_H
#define HW1_EXCEPTION_H
#include <exception>
using std::exception;

class InvalidInput: public exception {} ;
class AlreadyExists: public exception {};
class DoesNotExist: public exception {};
class NotEnoughGroups: public exception {};
class NoPlayers: public exception {};
#endif //HW1_EXCEPTION_H
