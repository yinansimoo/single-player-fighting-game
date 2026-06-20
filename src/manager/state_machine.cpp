#include "state_machine.h"

void StateMachine::on_update(float delta)
{
    if (current_state == nullptr)
    {
        return;
    }

    if (need_init)
    {
        current_state->on_enter();
        need_init = false;
    }

    current_state->on_update(delta);
}

void StateMachine::set_entry(const std::string &id)
{
    auto it = state_pool.find(id);
    if (it == state_pool.end())
    {
        std::string error = "set_entry失败：" + id;

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "状态机错误",
            error.c_str(),
            NULL);

        throw std::runtime_error(error);
    }
    current_state = it->second;
}

void StateMachine::switch_to(const std::string &id)
{
    if (current_state != nullptr)
    {
        current_state->on_exit();
    }

    auto it = state_pool.find(id);
    if (it == state_pool.end())
    {
        std::string error = "switch_to失败：" + id;

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "状态机错误",
            error.c_str(),
            NULL);

        throw std::runtime_error(error);
    }
    current_state = it->second;

    current_state->on_enter();
}

void StateMachine::register_state(const std::string &id, StateNode *state_node)
{
    auto it = state_pool.find(id);
    if (it != state_pool.end())
    {
        return;
    }

    state_pool.emplace(id, state_node);
}
