//%Header {
/*****************************************************************************
 *
 * File: src/Mushcore/MushcoreAutoMonkey.cpp
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
//%Header } FwGIMg2frgpqY4SiiHc3oA
/*
 * $Id: MushcoreAutoMonkey.cpp,v 1.7 2005/05/19 13:02:14 southa Exp $
 * $Log: MushcoreAutoMonkey.cpp,v $
 * Revision 1.7  2005/05/19 13:02:14  southa
 * Mac release work
 *
 * Revision 1.6  2004/01/02 21:13:12  southa
 * Source conditioning
 *
 * Revision 1.5  2003/09/17 19:40:34  southa
 * Source conditioning upgrades
 *
 * Revision 1.4  2003/08/21 23:09:04  southa
 * Fixed file headers
 *
 * Revision 1.3  2003/01/20 12:23:22  southa
 * Code and interface tidying
 *
 * Revision 1.2  2003/01/12 17:32:58  southa
 * Mushcore work
 *
 * Revision 1.1  2003/01/09 14:57:05  southa
 * Created Mushcore
 *
 * Revision 1.7  2002/12/29 20:59:50  southa
 * More build fixes
 *
 * Revision 1.6  2002/12/20 13:17:31  southa
 * Namespace changes, licence changes and source conditioning
 *
 * Revision 1.5  2002/10/22 20:41:57  southa
 * Source conditioning
 *
 * Revision 1.4  2002/08/27 08:56:15  southa
 * Source conditioning
 *
 * Revision 1.3  2002/08/07 13:36:44  southa
 * Conditioned source
 *
 * Revision 1.2  2002/07/06 18:04:14  southa
 * More designer work
 *
 * Revision 1.1  2002/07/02 09:12:53  southa
 * Name changed to MushcoreAutoMonkey
 *
 * Revision 1.7  2002/06/27 12:36:01  southa
 * Build process fixes
 *
 * Revision 1.6  2002/05/10 16:39:38  southa
 * Changed .hp files to .h
 *
 * Revision 1.5  2002/05/09 17:10:38  southa
 * Fixed for gcc 3.0
 *
 * Revision 1.4  2002/03/07 22:24:32  southa
 * Command interpreter working
 *
 * Revision 1.3  2002/02/26 17:01:40  southa
 * Completed Sprite loader
 *
 * Revision 1.2  2002/02/23 17:54:45  southa
 * Added GIF loader and GL tests
 *
 * Revision 1.1  2002/02/23 11:43:34  southa
 * Added MushcoreAutoMonkey
 *
 */

#include "MushcoreAutoMonkey.h"

#include "MushcoreFail.h"
#include "MushcoreSwitches.h"

#include "MushcoreSTL.h"

using namespace Mushware;
using namespace std;

MushcoreAutoMonkey::~MushcoreAutoMonkey()
{
    --*m_refCtrPtr;
    MUSHCORE_IFMONKEYTESTING(cerr << "Automonkey destroyed, refcount to " << ReferenceCountGet() << endl);
    if (*m_refCtrPtr == 0)
    {
        delete m_refCtrPtr;
        m_refCtrPtr=NULL;
        MUSHCORE_IFMONKEYTESTING(cerr << "MushcoreAutoMonkey workspace freed" << endl); 
    }
}

MushcoreAutoMonkey::MushcoreAutoMonkey(const MushcoreAutoMonkey& inMonkey)
{
    m_refCtrPtr=inMonkey.m_refCtrPtr;
    ++*m_refCtrPtr;
    MUSHCORE_IFMONKEYTESTING(cerr << "Automonkey copied, refcount to " << ReferenceCountGet() << endl);
}

void
MushcoreAutoMonkey::Swap(MushcoreAutoMonkey& inMonkey)
{
    S32 *temp=m_refCtrPtr;
    m_refCtrPtr=inMonkey.m_refCtrPtr;
    inMonkey.m_refCtrPtr=temp;
}

MushcoreAutoMonkey&
MushcoreAutoMonkey::operator=(const MushcoreAutoMonkey& inMonkey)
{
    MushcoreAutoMonkey temp(inMonkey);
    Swap(temp);
    return *this;
}

bool
MushcoreAutoMonkey::FreeInDestructor(void *inDataPtr) const
{
    if (*m_refCtrPtr < 1) throw(MushcoreLogicFail("MushcoreAutoMonkey fault"));
    MUSHCORE_IFMONKEYTESTING(cerr << "Automonkey InDestructorFree, refcount is " << ReferenceCountGet() << endl);
    return (*m_refCtrPtr == 1  && inDataPtr != NULL);
}

