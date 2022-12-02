#include <functional>
#include <iostream>
#include <optional>
#include <typeinfo>

#include "include/EitherMonad.h"

#define WATCH(x) std::cout << (#x) << ": " << x << '\n'


APPLICATION_ERROR(AttemptToDivideByZero, "Attempt to divide by zero", 1, GET_EXIT::True)
APPLICATION_ERROR(BadIndexScenario,"Access not existing index",2, GET_EXIT::True)
APPLICATION_ERROR(TestError,"TEST",3, GET_EXIT::False)


struct Scenario
{
	std::string txt;
	std::string toStr() { return txt; }
};

template<typename Stream>
Stream& operator<<(Stream& ss, Scenario obj) { return ss << obj.toStr(); }

Either<Error, Scenario> get_scenario(int sc) { 
	std::vector<Scenario> x {
		{ "jedna" },
		{ "dva" },
		{ "tri" }
	};
	
	if (sc >= 0 && sc < x.size()) { return x[sc]; }

	return BadIndexScenario()
		.updateDDate(
			"Bad index: " + std::to_string(sc) + " is not in range [0, "
			+ std::to_string(x.size() - 1) + "]\n"
			+ "\tfrom: " + __FILE__ + '(' + std::to_string(__LINE__) + ") " + __FUNCTION__ + "\n"
		);
}

Either<Error, std::string> fn(Scenario s)
{
	std::cout << "FN\n";
	return "!!!" + s.toStr() + '\n';
}

Either<Error, int> get_test_error()
{
	return TestError();
}

int main(int argc, char** argv)
{
	std::cout << ((Do ( 0 ) >>= get_scenario ) >>= fn );
	std::cout << ((Do ( 1 ) >>= get_scenario ) >>= fn );
	std::cout << ((Do ( 4 ) >>= get_scenario ) >>= fn );
	
	//auto x = DoubleDo(2, get_scenario);
	//std::cout << '\n' << typeid(x).name() << '\n';
	std::cout << ( DoubleDo( 2, get_scenario ) ) << "\n\n";
	std::cout << DoubleDo( 5, get_scenario ) << "\n\n";

	//std::cout << _Do(5, get_scenario) << '\n';
	//std::cout << _Do(5, get_scenario, fn) << '\n';

	error_handle(std::cout, *fromLeft(get_test_error()));
	error_handle(std::cout, *fromLeft((Do ( 3 ) >>= get_scenario ) >>= fn ));

	std::vector<Either<Error, std::string>> es = {
		((Do ( 4 ) >>= get_scenario ) >>= fn ),
		((Do ( 2 ) >>= get_scenario ) >>= fn ),
		((Do ( 3 ) >>= get_scenario ) >>= fn ),
		((Do ( 2 ) >>= get_scenario ) >>= fn )
	};

	auto lfs = lefts(es);
	if (lfs.size()) { for (auto er : lfs) { } return 0; }
	
	return 0;
}

