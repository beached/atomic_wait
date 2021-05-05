// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/
//

#include <atomic>
#include <cpp20/atomic_wait>
#include <cpp20/latch>
#include <thread>

int main( ) {
	auto v = std::atomic<int>( 1 );
	auto th = std::thread( [&] {
		auto current = v.load( );
		while( current != 0 ) {
			std::atomic_wait( current );
			current = v.load( );
		}
		std::cout << "C: Found 0\n";
		std::cout << "C: Setting 1\n";
		v.store( 1 );
	} );
	std::cout << "P: Setting 0\n";
	v.store( 0 );
	auto current = v.load( );
	while( current == 1 ) {
		std::atomic_wait( current );
		current = v.load( );
	}
	std::cout << "P: Found 1\n";
	th.join( );
}
