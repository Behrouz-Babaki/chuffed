#include <cstdio>

#include <chuffed/core/engine.h>
#include <chuffed/core/propagator.h>
#include <chuffed/branching/branching.h>
#include <chuffed/vars/modelling.h>
#include <chuffed/globals/mddglobals.h>

class SmallDFA : public Problem
{
  public:
	vec<IntVar *> x;
	IntVar *cost;

	SmallDFA()
	{

		size_t horizon = 10;
		// three variables, each having a domain of 1..3
		createVars(x, horizon, 1, 3);
		createVar(cost, 1, 2 * horizon);

		int q = 3;
		int s = 3;
		int transition[3][3] = {
			{1, 0, 0},
			{1, 2, 3},
			{0, 2, 3}};
		int weights[3][3] = {
			{2, 2, 2},
			{1, 2, 1},
			{1, 1, 1}};
		vec<vec<int>> d;
		vec<vec<int>> w;
		for (int i = 0; i < 3; i++)
		{
			d.push();
			w.push();
			for (int j = 0; j < 3; j++)
			{
				d.last().push(transition[i][j]);
				w.last().push(weights[i][j]);
			}
		}
		int q0 = 3;
		vec<int> f;
		f.push(1);
		MDDOpts mopts;

		// void wmdd_cost_regular(vec<IntVar*>& x, int q, int s, vec<vec<int> >& d, vec<vec<int> >& w,
		//     int q0, vec<int>& f, IntVar* cost, const MDDOpts& mopts);
		wmdd_cost_regular(x, q, s, d, w, q0, f, cost, mopts);

		// branching
		branch(x, VAR_INORDER, VAL_MIN);
		output_vars(x);
		output_var(cost);

		optimize(cost, OPT_MIN);
	}

	void print(std::ostream &os)
	{
		os << cost->getVal() << "\n";
		for (size_t i = 0; i < x.size(); i++)
			os << x[i]->getVal() << "\t";
		os << "\n";
	}
};

int main(int argc, char **argv)
{
	engine.solve(new SmallDFA());
	return 0;
}
