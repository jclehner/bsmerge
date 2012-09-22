#ifndef BSMERGE_STATIC_ASSERT_HH
#define BSMERGE_STATIC_ASSERT_HH

#define BSMERGE_STATIC_ASSERT(condition) bsmerge::StaticAssert<condition> staticAsserter__

namespace bsmerge {

template<bool B> class StaticAssert;
template<> class StaticAssert<true> {};

}

#endif
