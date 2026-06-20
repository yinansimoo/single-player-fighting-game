#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "state_node.h"

#include "SDL.h"
#include <string>
#include <unordered_map>
#include <stdexcept>

class StateMachine
{
public:
    StateMachine() = default;
    ~StateMachine() = default;

    void on_update(float delta);

    void set_entry(const std::string &id);
    void switch_to(const std::string &id);
    void register_state(const std::string &id, StateNode *state_node);

private:
    bool need_init = true;
    StateNode *current_state = nullptr;
    std::unordered_map<std::string, StateNode *> state_pool;
};

#endif