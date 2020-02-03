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

std::tuple<Value*, Error*> Value::added_to(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::subbed_by(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::multed_by(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::dived_by(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::powed_by(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::modded_by(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::get_comparison_eq(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::get_comparison_ne(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::get_comparison_lt(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::get_comparison_gt(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::get_comparison_lte(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::get_comparison_gte(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::anded_by(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::ored_by(Value* other) {
    return std::make_tuple(nullptr, illegal_operation(other));
}

std::tuple<Value*, Error*> Value::notted() {
    return std::make_tuple(nullptr, illegal_operation());
}

std::tuple<Value*, Error*> Value::round() {
    return std::make_tuple(nullptr, illegal_operation());
}

std::tuple<Value*, Error*> Value::abs() {
    return std::make_tuple(nullptr, illegal_operation());
}

std::tuple<Value*, Error*> Value::floor() {
    return std::make_tuple(nullptr, illegal_operation());
}

std::tuple<Value*, Error*> Value::ceil() {
    return std::make_tuple(nullptr, illegal_operation());
}

std::tuple<Value*, Error*> Value::sin() {
    return std::make_tuple(nullptr, illegal_operation());
}

std::tuple<Value*, Error*> Value::cos() {
    return std::make_tuple(nullptr, illegal_operation());
}

std::tuple<Value*, Error*> Value::tan() {
    return std::make_tuple(nullptr, illegal_operation());
}

RTResult* Value::execute(std::vector<Value*> args) {
    return (new RTResult)->failure(illegal_operation());
}

Value* Value::copy() {
    throw ("No copy method defined");
}

bool Value::is_true() {
    return false;
}

Error* Value::illegal_operation(Value* other) {
    if (other == nullptr) other = this;
    return new RTError(pos_start, other->pos_end, "Illegal operation", context);
}

std::tuple<Value*, Error*> Number::added_to(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new Number(int_value + other->int_value))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::subbed_by(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new Number(int_value - other->int_value))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::multed_by(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new Number(int_value * other->int_value))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::dived_by(Value* other) {
    if (instanceof<Number>(other)) {
        if (other->int_value == 0) return std::make_tuple(nullptr, new RTError(other->pos_start, other->pos_end, "Division by 0", context));
        return std::make_tuple((new Number(int_value / other->int_value))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::powed_by(Value* other) {
    if (instanceof<Number>(other)) {
        if (value == 0 && other->int_value == 0) return std::make_tuple(nullptr, new RTError(other->pos_start, other->pos_end, "0 raised to the 0th power", context));
        return std::make_tuple((new Number((int8_t) (pow(int_value, other->int_value) + 0.5)))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::modded_by(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new Number(int_value % other->int_value))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::get_comparison_eq(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new Number((int8_t) (int_value == other->int_value)))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::get_comparison_ne(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new Number((int8_t) (int_value != other->int_value)))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::get_comparison_lt(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new Number((int8_t) (int_value < other->int_value)))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::get_comparison_gt(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new Number((int8_t) (int_value > other->int_value)))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::get_comparison_lte(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new Number((int8_t) (int_value <= other->int_value)))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::get_comparison_gte(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new Number((int8_t) (int_value >= other->int_value)))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::anded_by(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new Number((int8_t) (int_value && other->int_value)))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::ored_by(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new Number((int8_t) (int_value || other->int_value)))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> Number::notted() {
    return std::make_tuple((new Number((int8_t) ((int_value == 0) ? 1 : 0)))->set_context(context), nullptr);
}

std::tuple<Value*, Error*> Number::round() {
    return std::make_tuple((new Number(std::round(int_value)))->set_context(context), nullptr);
}

std::tuple<Value*, Error*> Number::abs() {
    return std::make_tuple((new Number(std::abs(int_value)))->set_context(context), nullptr);
}

std::tuple<Value*, Error*> Number::floor() {
    return std::make_tuple((new Number(std::floor(int_value)))->set_context(context), nullptr);
}

std::tuple<Value*, Error*> Number::ceil() {
    return std::make_tuple((new Number(std::ceil(int_value)))->set_context(context), nullptr);
}

std::tuple<Value*, Error*> Number::sin() {
    return std::make_tuple((new Number(std::sin(int_value)))->set_context(context), nullptr);
}

std::tuple<Value*, Error*> Number::cos() {
    return std::make_tuple((new Number(std::cos(int_value)))->set_context(context), nullptr);
}

std::tuple<Value*, Error*> Number::tan() {
    return std::make_tuple((new Number(std::tan(int_value)))->set_context(context), nullptr);
}

Value* Number::copy() {
    Number* copy_ = new Number(value);
    copy_->set_pos(pos_start, pos_end);
    copy_->set_context(context);
    return copy_;
}

std::string Number::repr() {
    return std::to_string(value);
}

bool Number::is_true() {
    return value != 0;
}

std::tuple<Value*, Error*> String_::added_to(Value* other) {
    if (instanceof<String_>(other)) {
        return std::make_tuple((new String_(str_value + other->str_value))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

std::tuple<Value*, Error*> String_::multed_by(Value* other) {
    if (instanceof<Number>(other)) {
        return std::make_tuple((new String_((std::string)(other->int_value, str_value)))->set_context(context), nullptr);
    }
    return std::make_tuple(nullptr, (new Value)->illegal_operation(other));
}

Value* String_::copy() {
    String_* copy_ = new String_(str_value);
    copy_->set_pos(pos_start, pos_end);
    copy_->set_context(context);
    return copy_;
}

std::string String_::repr() {
    return str_value;
}

bool String_::is_true() {
    return str_value.length() > 0;
}
