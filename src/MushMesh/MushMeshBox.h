//%includeGuardStart {
#ifndef MUSHMESHBOX_H
#define MUSHMESHBOX_H
//%includeGuardStart } ypsitLQ7bckO5My8vBP2yQ
//%Header {
/*****************************************************************************
 *
 * File: src/MushMesh/MushMeshBox.h
 *
 * Author: Andy Southgate 2002-2005
 *
 * This file contains original work by Andy Southgate.  The author and his
 * employer (Mushware Limited) irrevocably waive all of their copyright rights
 * vested in this particular version of this file to the furthest extent
 * permitted.  The author and Mushware Limited also irrevocably waive any and
 * all of their intellectual property rights arising from said file and its
 * creation that would otherwise restrict the rights of any party to use and/or
 * distribute the use of, the techniques and methods used herein.  A written
 * waiver can be obtained via http://www.mushware.com/.
 *
 * This software carries NO WARRANTY of any kind.
 *
 ****************************************************************************/
//%Header } N3TXbL5QkWJoBhjw68fw1Q
/*
 * $Id: MushMeshBox.h,v 1.5 2005/05/19 13:02:10 southa Exp $
 * $Log: MushMeshBox.h,v $
 * Revision 1.5  2005/05/19 13:02:10  southa
 * Mac release work
 *
 * Revision 1.4  2004/01/02 21:13:10  southa
 * Source conditioning
 *
 * Revision 1.3  2003/10/20 13:02:53  southa
 * Patch fixes and testing
 *
 * Revision 1.2  2003/10/17 19:33:10  southa
 * Mesh patches
 *
 * Revision 1.1  2003/10/17 12:27:19  southa
 * Line end fixes and more mesh work
 *
 */

#include "MushMeshStandard.h"

#include "MushMeshVector.h"

template <class T>
class MushMeshBox
{
public:
    typedef MushMeshVector<T, 2> tPoint;

    MushMeshBox();
    MushMeshBox(const tPoint& inStart, const tPoint& inEnd);
    MushMeshBox(const T& inStartX, const T& inStartY, const T& inEndX, const T& inEndY);

    const tPoint& StartGet(void) const { return m_start; }
    void StartSet(const tPoint& inPoint) { m_start = inPoint; }
    const tPoint& EndGet(void) const { return m_end; }
    void EndSet(const tPoint& inPoint) { m_end = inPoint; }
    const tPoint SizeGet(void) const { return m_end - m_start; }

    bool RegularIs(void);
    void RegularMake(void);

private:
    tPoint m_start;
    tPoint m_end;    

public:
    bool EqualIs(const MushMeshBox& inObj) const;
    void Print(std::ostream& ioOut) const;
};

template <class T>
inline
MushMeshBox<T>::MushMeshBox()
{
}

template <class T>
inline
MushMeshBox<T>::MushMeshBox(const tPoint& inStart, const tPoint& inEnd) :
    m_start(inStart),
    m_end(inEnd)
{
}

template <class T>
inline
MushMeshBox<T>::MushMeshBox(const T& inStartX, const T& inStartY, const T& inEndX, const T& inEndY) :
    m_start(tPoint(inStartX, inStartY)),
    m_end(tPoint(inEndX, inEndY))
{
}

template <class T>
inline bool
MushMeshBox<T>::RegularIs(void)
{
    return m_start.X() <= m_end.X() &&
           m_start.Y() <= m_end.Y();
}

template <class T>
inline void
MushMeshBox<T>::RegularMake(void)
{
    if (m_start.X() > m_end.X())
    {
        T tempValue;
        tempValue = m_start.X();
        m_start.XSet(m_end.X());
        m_end.XSet(tempValue);
    }
    if (m_start.Y() > m_end.Y())
    {
        T tempValue;
        tempValue = m_start.Y();
        m_start.YSet(m_end.Y());
        m_end.YSet(tempValue);
    }
}

template <class T>
inline bool
MushMeshBox<T>::EqualIs(const MushMeshBox& inObj) const
{
    return 1
        && (m_start == inObj.m_start)
        && (m_end == inObj.m_end)
    ;
}

template <class T>
inline void
MushMeshBox<T>::Print(std::ostream& ioOut) const
{
    ioOut << "[";
    ioOut << "start=" << m_start << ", ";
    ioOut << "end=" << m_end;
    ioOut << "]";
}


template <class T>
inline bool
operator==(const MushMeshBox<T>& inA, const MushMeshBox<T>& inB)
{
    return inA.EqualIs(inB);
}

template <class T>
inline bool
operator!=(const MushMeshBox<T>& inA, const MushMeshBox<T>& inB)
{
    return !inA.EqualIs(inB);
}

template <class T>
inline std::ostream&
operator<<(std::ostream& ioOut, const MushMeshBox<T>& inObj)
{
    inObj.Print(ioOut);
    return ioOut;
}

// Convenient types
namespace Mushware
{
    typedef MushMeshBox<Mushware::U32> t2BoxU32;
    typedef MushMeshBox<Mushware::tVal> t2BoxVal;
}

//%includeGuardEnd {
#endif
//%includeGuardEnd } hNb4yLSsimk5RFvFdUzHEw
