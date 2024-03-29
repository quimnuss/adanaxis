//%Header {
/*****************************************************************************
 *
 * File: src/GL/GLLine.cpp
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
//%Header } BAwofPDl7/zGnQG6rrxV/A
/*
 * $Id: GLLine.cpp,v 1.15 2005/05/19 13:01:59 southa Exp $
 * $Log: GLLine.cpp,v $
 * Revision 1.15  2005/05/19 13:01:59  southa
 * Mac release work
 *
 * Revision 1.14  2004/01/02 21:13:05  southa
 * Source conditioning
 *
 * Revision 1.13  2003/09/17 19:40:29  southa
 * Source conditioning upgrades
 *
 * Revision 1.12  2003/08/21 23:08:23  southa
 * Fixed file headers
 *
 * Revision 1.11  2003/01/13 14:31:54  southa
 * Build frameworks for Mac OS X
 *
 * Revision 1.10  2002/12/29 20:59:52  southa
 * More build fixes
 *
 * Revision 1.9  2002/12/20 13:17:35  southa
 * Namespace changes, licence changes and source conditioning
 *
 * Revision 1.8  2002/10/22 20:42:00  southa
 * Source conditioning
 *
 * Revision 1.7  2002/10/17 15:50:58  southa
 * Config saving, pause and quit
 *
 * Revision 1.6  2002/10/10 13:51:16  southa
 * Speed fixes and various others
 *
 * Revision 1.5  2002/08/27 08:56:19  southa
 * Source conditioning
 *
 * Revision 1.4  2002/08/07 13:36:47  southa
 * Conditioned source
 *
 * Revision 1.3  2002/08/01 16:47:09  southa
 * First multi-box collsion checking
 *
 * Revision 1.2  2002/07/18 11:40:34  southa
 * Overplotting and movement
 *
 * Revision 1.1  2002/07/16 17:48:07  southa
 * Collision and optimisation work
 *
 */

#include "GLLine.h"

#include "GLStandard.h"
#include "GLState.h"
#include "GLUtils.h"

#include "GLSTL.h"

using namespace Mushware;
using namespace std;

GLLine::GLLine(const GLPoint& inStart, const GLPoint& inEnd):
    m_start(inStart),
    m_end(inEnd)
{
}

void
GLLine::CentreGet(GLPoint& outPoint) const
{
    outPoint=(m_start+m_end)/2;
}

void
GLLine::RotateAboutCentre(tVal inAngle)
{
    GLPoint centre;
    CentreGet(centre);

    m_start.RotateAboutZ(centre, inAngle);
    m_end.RotateAboutZ(centre, inAngle);    
}

const GLPoint&
GLLine::GetStart(void) const
{
    return m_start;
}

const GLPoint&
GLLine::GetEnd(void) const
{
    return m_end;
}

bool
GLLine::IsIntersecting(const GLLine& inLine) const
{
    // This function has been rearranged to avoid a MacOS X compiler bug
    GLPoint a(m_start);
    GLPoint b(m_end);
    GLPoint c(inLine.GetStart());
    GLPoint d(inLine.GetEnd());

    b-=a;
    d-=c;
    
    tVal f = b.x*d.y - b.y*d.x;

    tVal m = d.y*(c.x-a.x)+d.x*(a.y-c.y);
    tVal n = b.y*(c.x-a.x)+b.x*(a.y-c.y);
    
    bool isIntersecting;
    if (f < 0)
    {
        isIntersecting = (m<=0 && m>f && n<=0 && n>f);
    }
    else
    {
        isIntersecting = (m>=0 && m<f && n>=0 && n<f);
    }
    if (isIntersecting)
    {
        return true;
    }
    return false;
}

void
GLLine::Render(void) const
{
    GLState::TextureDisable();
    glBegin(GL_LINES);
    GLUtils::Vertex(m_start.x, m_start.y);
    GLUtils::Vertex(m_end.x, m_end.y);
    glEnd();
}
