#include "lattice.h"

void
CLatticeFrame::clear()
{
	m_type = UNUSED;
	m_bwType = NO_BESTWORD;
	//m_lexiconStates.clear();
	m_latticeStates.clear();
	m_wstr.clear();
	m_bestWords.clear();

	CLexiconStates::iterator it = m_lexiconStates.begin();
	CLexiconStates::iterator ite = m_lexiconStates.end();
	for (; it != ite; it++) {
		delete (*it);
	}
	m_lexiconStates.clear();
}


void
CLatticeFrame::print(std::string prefix)
{
    if (m_bwType & BESTWORD) printf("B");
    if (m_bwType & USER_SELECTED) printf("U");
    printf("\n");

    prefix += "    ";
    printf("  Lexicon States:\n");
    //for_each(m_lexiconStates.begin(), m_lexiconStates.end(),
    //         bind2nd(mem_fun_ref(&TLexiconState::print), prefix));

    printf("  Lattice States:\n");
    for_each(m_latticeStates.begin(), m_latticeStates.end(),
             bind2nd(mem_fun_ref(&TLatticeState::print), prefix));
    printf("\n");
}

