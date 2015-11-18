//
// keyvi - A key value store.
//
// Copyright 2015 Hendrik Muhs<hendrik.muhs@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

/*
 * automata_test.cpp
 *
 *  Created on: Sep 20, 2015
 *      Author: hendrik
 */

#include <boost/test/unit_test.hpp>
#include "dictionary/fsa/automata.h"
#include "dictionary/fsa/traverser_types.h"
#include "dictionary/testing/temp_dictionary.h"

namespace keyvi {
namespace dictionary {
namespace fsa {

BOOST_AUTO_TEST_SUITE( AutomataTests )

BOOST_AUTO_TEST_CASE( GetOutGoingTransitionsTest ) {
  std::vector<std::string> test_data =
        { "\01cd", "aaaa", "aabb", "agbc", "ajcd", "azcd" };
  testing::TempDictionary dictionary(test_data);
  automata_t f = dictionary.GetFsa();

  traversal::TraversalStack<> stack;

  f->GetOutGoingTransitions(f->GetStartState(), stack.GetStates(), stack.payload_);

  BOOST_CHECK_EQUAL(2, stack.GetStates().payload_.transitions_.size());
  BOOST_CHECK_EQUAL(f->TryWalkTransition(f->GetStartState(), '\01'), stack.GetStates().payload_.transitions_[0].state);
  BOOST_CHECK_EQUAL(f->TryWalkTransition(f->GetStartState(), 'a'), stack.GetStates().payload_.transitions_[1].state);
  BOOST_CHECK_EQUAL('\01', stack.GetStates().payload_.transitions_[0].label);
  BOOST_CHECK_EQUAL('a', stack.GetStates().payload_.transitions_[1].label);

  // check all outgoings for 'a'
  uint32_t state_a = f->TryWalkTransition(f->GetStartState(), 'a');

  f->GetOutGoingTransitions(state_a, stack.GetStates(), stack.payload_);

  BOOST_CHECK_EQUAL(4, stack.GetStates().payload_.transitions_.size());
  BOOST_CHECK_EQUAL(f->TryWalkTransition(state_a, 'a'), stack.GetStates().payload_.transitions_[0].state);
  BOOST_CHECK_EQUAL(f->TryWalkTransition(state_a, 'g'), stack.GetStates().payload_.transitions_[1].state);
  BOOST_CHECK_EQUAL(f->TryWalkTransition(state_a, 'j'), stack.GetStates().payload_.transitions_[2].state);
  BOOST_CHECK_EQUAL(f->TryWalkTransition(state_a, 'z'), stack.GetStates().payload_.transitions_[3].state);

  BOOST_CHECK_EQUAL('a', stack.GetStates().payload_.transitions_[0].label);
  BOOST_CHECK_EQUAL('g', stack.GetStates().payload_.transitions_[1].label);
  BOOST_CHECK_EQUAL('j', stack.GetStates().payload_.transitions_[2].label);
  BOOST_CHECK_EQUAL('z', stack.GetStates().payload_.transitions_[3].label);
}

BOOST_AUTO_TEST_CASE( GetOutGoingTransitionsWeightTest ) {
  std::vector<std::pair<std::string, uint32_t>> test_data = {
            { "the fox jumped over the fence and broke his nose", 22 },
            { "the fox jumped over the fence and broke his feet", 24 },
            { "the fox jumped over the fence and broke his tongue", 444 },
            { "the fox jumped over the fence and broke his arm", 2 },
        };
  testing::TempDictionary dictionary(test_data);
  automata_t f = dictionary.GetFsa();

  traversal::TraversalStack<traversal::WeightedTransition> stack;

  f->GetOutGoingTransitions(f->GetStartState(), stack.GetStates(), stack.payload_);

  BOOST_CHECK_EQUAL(1, stack.GetStates().payload_.transitions_.size());
  BOOST_CHECK_EQUAL(444, stack.GetStates().payload_.transitions_[0].weight);
}

BOOST_AUTO_TEST_SUITE_END()

} /* namespace fsa */
} /* namespace dictionary */
} /* namespace keyvi */
