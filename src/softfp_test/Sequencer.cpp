
#include "Sequencer.hpp"

namespace postrisc {
namespace fpu {

/******************************************************************************
* per-test fixture
******************************************************************************/
Sequencer::Sequencer(void)
{
    BOOST_TEST_MESSAGE("Sequencer ctor");
}

Sequencer::~Sequencer(void)
{
    BOOST_TEST_MESSAGE("Sequencer dtor");
}

} //namespace fpu
} // namespace postrisc
