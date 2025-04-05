#include "search-strategies.h"
#include "memusage.h"
#include <queue>
#include <unordered_set>


struct State_node {
    SearchState state;
    std::vector<int> path;

	// constructor
	State_node(SearchState new_state, std::vector<int> new_path) : state(new_state), path(new_path) {}
};

inline bool operator==(const std::shared_ptr<State_node> &a, const std::shared_ptr<State_node> &b) {
    return a->state == b->state;
}

inline bool operator==(const SearchState &a, const SearchState &b) {
    return a.state_ == b.state_;
}

std::vector<SearchAction> BreadthFirstSearch::solve(const SearchState &init_state) {
	std::queue<std::shared_ptr<State_node>> to_search;
	std::unordered_set<std::shared_ptr<State_node>> expanded;
	size_t mem_limit = mem_limit_ * 0.99f;

	std::shared_ptr<State_node> node = std::make_shared<State_node>(
		State_node(
			SearchState(init_state), {}
		));

	to_search.push(node);
	while (!to_search.empty()) {
		auto current_node = to_search.front();
		to_search.pop();

		if (current_node->state.isFinal()){
			// converts path to expected output format
			std::vector<SearchAction> final_path;
			SearchState state = SearchState(init_state);
			// reconstructs path to the final state and records action objects
			for (auto &i : current_node->path){
				std::vector<SearchAction> actions = state.actions();
				state.execute(actions[i]);
				final_path.push_back(actions[i]);
			}
			return final_path;
		}
		// memory limit check
		if (getCurrentRSS() > mem_limit)
			return {};

		std::vector<SearchAction> actions = current_node->state.actions();
		for (size_t i = 0; i < actions.size(); i++) {
			node = std::make_shared<State_node>(
				State_node(
					SearchState(current_node->state),
					std::vector<int>(current_node->path)
				));

			// checks if action is valid and result was not already expanded
			if (!(node->state).execute(actions[i]))
				continue;
			if (expanded.find(node) != expanded.end())
				continue;

			node->path.push_back(i);
			expanded.insert(node);
			to_search.push(node);
		}
	}
	return {};
}

std::vector<SearchAction> DepthFirstSearch::solve(const SearchState &init_state) {
	std::vector<
		std::pair<
			std::shared_ptr<SearchState>,
			std::vector<SearchAction>
		>> to_search;
	size_t mem_limit = mem_limit_ * 0.99f;
	size_t depth_limit = depth_limit_;

	to_search.push_back({
		std::make_shared<SearchState>(init_state), {}
	});
	while (!to_search.empty()) {
		auto [old_state, path] = to_search.back();
		to_search.pop_back();

		// creates copy of the object with new shared_ptr
		std::shared_ptr<SearchState> working_state = std::make_shared<SearchState>(*old_state);

		// path is empty only on the init_state
		if (!path.empty() && !working_state->execute(path.back()))
			continue;
		if (working_state->isFinal())
			return path;
		if (getCurrentRSS() > mem_limit)
			return {};
		if (path.size() == depth_limit)
			continue;

		std::vector<SearchAction> actions = working_state->actions();
		for (int i = actions.size() - 1; i >= 0; i--) {
			// states will be expanded when popped from to_search
			std::vector<SearchAction> new_path(path);
			new_path.push_back(actions[i]);

			to_search.push_back({working_state, new_path});
		}
	}
	return {};
}

double StudentHeuristic::distanceLowerBound(const GameState &state) const {
    return 0;
}

struct AStar_node {
    SearchState state;
    std::vector<int> path;
	int cost;

	// constructor
	AStar_node(SearchState new_state, std::vector<int> new_path, int new_cost) : state(new_state), path(new_path), cost(new_cost) {}
};

inline bool operator==(const std::shared_ptr<AStar_node> &a, const std::shared_ptr<AStar_node> &b) {
    return a->state == b->state;
}

inline bool operator<(const std::shared_ptr<AStar_node> &a, const std::shared_ptr<AStar_node> &b) {
    return a->cost > b->cost;
}

std::vector<SearchAction> AStarSearch::solve(const SearchState &init_state) {
	std::priority_queue<
		std::shared_ptr<AStar_node>,
		std::vector<std::shared_ptr<AStar_node>>,
		std::less<std::shared_ptr<AStar_node>>
		> to_search;
	std::unordered_set<std::shared_ptr<AStar_node>> expanded;
	size_t mem_limit = mem_limit_ * 0.99f;

	std::shared_ptr<AStar_node> node = std::make_shared<AStar_node>(
		AStar_node(
			SearchState(init_state), {}, 0
		));

	to_search.push(node);
	while (!to_search.empty()) {
		auto current_node = to_search.top();
		to_search.pop();

		if (current_node->state.isFinal()){
			// converts path to expected output format
			std::vector<SearchAction> final_path;
			SearchState state = SearchState(init_state);
			// reconstructs path to the final state and records action objects
			for (auto &i : current_node->path){
				std::vector<SearchAction> actions = state.actions();
				state.execute(actions[i]);
				final_path.push_back(actions[i]);
			}
			return final_path;
		}
		// memory limit check
		if (getCurrentRSS() > mem_limit)
			return {};

		std::vector<SearchAction> actions = current_node->state.actions();
		for (size_t i = 0; i < actions.size(); i++) {
			node = std::make_shared<AStar_node>(
				AStar_node(
					SearchState(current_node->state),
					std::vector<int>(current_node->path),
					0
				));

			// checks if action is valid and result was not already expanded
			if (!(node->state).execute(actions[i]))
				continue;

			node->cost = node->path.size() + compute_heuristic(node->state, *heuristic_);
			auto found = expanded.find(node);
			if (found != expanded.end() && (**found).cost <= node->cost)
				continue;

			node->path.push_back(i);
			expanded.insert(node);
			to_search.push(node);
		}
	}
	return {};
}