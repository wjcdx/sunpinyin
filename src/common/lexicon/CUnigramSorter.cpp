
double
CUnigramSorter::getCost(unsigned int wid)
{
    CThreadSlm::TState st(0, 0);
    return m_Model.transferNegLog(st, wid, st);
}

bool
CUnigramSorter::isSeen(unsigned int wid)
{
    CThreadSlm::TState st(0, 0);
    m_Model.transferNegLog(st, wid, st);
    //printf("    -log(pr(%d)) = %lf\n", wid, logpr);
    return(st.getLevel() == 1);
}

