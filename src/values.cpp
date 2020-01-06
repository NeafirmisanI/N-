#include <iostream>
#include "mainClasses.hpp"

Value* Value::set_pos(Position* pos_s, Position* pos_e) {
    pos_start = pos_s;
    pos_end = pos_e;
    return this;
}

Value* Value::set_context(Context* context_) {
    context = context_;
    return this;
}
