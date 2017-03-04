/*
  Zevra, a UCI chess playing engine
  Copyright (C) 2016-2017 Oleg Smirnov (author)
  Zevra is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.
  Zevra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hash.hpp"

Hash::Hash() : prev(nullptr), flag(EMPTY) {}

Hash::~Hash() {
	clean();
}

bool Hash::back() {
	if(prev) {
		if(!prev->empty()) {
			move = prev->top();
			prev->pop();

			return true;
		}
	}

	return false;
}

void Hash::recordPrev() {
	/*if(flag == EXACT) {
		if(!prev) {
			prev = new std::stack<BitMove>();
		}

		prev->push(move);
	}*/
}

void Hash::clean() {
	if(prev) {
		while(!prev->empty()) {
			prev->pop();
		}

		delete prev;
		prev = nullptr;
	}

	flag = EMPTY;
}
