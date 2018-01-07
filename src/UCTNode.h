/*
    This file is part of Yuki.
    Copyright (C) 2018 Guofeng Dai

    Yuki is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Yuki is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Yuki.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UCTNODE_H_INCLUDED
#define UCTNODE_H_INCLUDED

#include "config.h"

#include <tuple>
#include <atomic>
#include <limits>

#include "SMP.h"
#include "GameState.h"
#include "Network.h"

class UCTNode {
public:
    using sortnode_t = std::tuple<float, int, float, UCTNode*>;

    // When we visit a node, add this amount of virtual losses
    // to it to encourage other CPUs to explore other parts of the
    // search tree.
    static constexpr auto VIRTUAL_LOSS_COUNT = 3;

    explicit UCTNode(int vertex, float score, float init_eval);
    ~UCTNode();
    bool first_visit() const;
    bool has_children() const;
    bool create_children(std::atomic<int> & nodecount,
                         GameState & state, float & eval);
    float eval_state(GameState& state);
    void delete_child(UCTNode * child);
    bool valid() const;

    int get_move() const;
    int get_visits() const;
    float get_score() const;
    void set_score(float score);
    float get_eval(int tomove) const;
    double get_blackevals() const;
    void set_visits(int visits);
    void set_blackevals(double blacevals);
    void set_eval(float eval);
    void accumulate_eval(float eval);
    void virtual_loss(void);
    void virtual_loss_undo(void);
    void dirichlet_noise(float epsilon, float alpha);
    void randomize_first_proportionally();
    void update(float eval = std::numeric_limits<float>::quiet_NaN());

    UCTNode* uct_select_child(int color);
    UCTNode* get_first_child() const;
    UCTNode* get_nopass_child(FastState& state) const;
    UCTNode* get_sibling() const;

    void sort_root_children(int color);
    UCTNode* get_best_root_child(int color);
    SMP::Mutex & get_mutex();

private:
    UCTNode();
    void link_child(UCTNode * newchild);
    void link_nodelist(std::atomic<int> & nodecount,
                       std::vector<Network::scored_node> & nodelist,
                       float init_eval);

    // Tree data
    std::atomic<bool> m_has_children{false};
    UCTNode* m_firstchild{nullptr};
    UCTNode* m_nextsibling{nullptr};
    // Move
    int m_move;
    // UCT
    std::atomic<int> m_visits{0};
    std::atomic<int> m_virtual_loss{0};
    // UCT eval
    float m_score;
    float m_init_eval;
    std::atomic<double> m_blackevals{0};

    // Is someone adding scores to this node?
    // We don't need to unset this.
    bool m_is_expanding{false};
    SMP::Mutex m_nodemutex;
};

#endif
