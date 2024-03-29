//%Header {
/*****************************************************************************
 *
 * File: src/GL/GLQuad.cpp
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
//%Header } kyKCRgA938+WYb9WSdRDnA
/*
 * $Id: GLQuad.cpp,v 1.17 2005/05/19 13:01:59 southa Exp $
 * $Log: GLQuad.cpp,v $
 * Revision 1.17  2005/05/19 13:01:59  southa
 * Mac release work
 *
 * Revision 1.16  2004/01/02 21:13:05  southa
 * Source conditioning
 *
 * Revision 1.15  2003/10/04 12:22:58  southa
 * File renaming
 *
 * Revision 1.14  2003/09/17 19:40:29  southa
 * Source conditioning upgrades
 *
 * Revision 1.13  2003/08/21 23:08:25  southa
 * Fixed file headers
 *
 * Revision 1.12  2003/01/13 14:31:55  southa
 * Build frameworks for Mac OS X
 *
 * Revision 1.11  2003/01/12 17:32:50  southa
 * Mushcore work
 *
 * Revision 1.10  2002/12/29 20:59:52  southa
 * More build fixes
 *
 * Revision 1.9  2002/12/20 13:17:35  southa
 * Namespace changes, licence changes and source conditioning
 *
 * Revision 1.8  2002/10/22 20:42:01  southa
 * Source conditioning
 *
 * Revision 1.7  2002/10/17 15:50:58  southa
 * Config saving, pause and quit
 *
 * Revision 1.6  2002/08/27 08:56:19  southa
 * Source conditioning
 *
 * Revision 1.5  2002/08/07 13:36:47  southa
 * Conditioned source
 *
 * Revision 1.4  2002/08/01 16:47:10  southa
 * First multi-box collsion checking
 *
 * Revision 1.3  2002/07/23 14:22:52  southa
 * Rotation fix
 *
 * Revision 1.2  2002/07/23 14:10:46  southa
 * Added InfernalMotion
 *
 * Revision 1.1  2002/07/16 17:48:07  southa
 * Collision and optimisation work
 *
 */

#include "GLQuad.h"
#include "GLRectangle.h"
#include "GLStandard.h"
#include "GLState.h"

#include "GLSTL.h"

using namespace Mushware;
using namespace std;


GLQuad::GLQuad(const GLRectangle& inRectangle)
{
    m_points[0].x=inRectangle.xmin;
    m_points[0].y=inRectangle.ymin;
    m_points[1].x=inRectangle.xmax;
    m_points[1].y=inRectangle.ymin;
    m_points[2].x=inRectangle.xmax;
    m_points[2].y=inRectangle.ymax;
    m_points[3].x=inRectangle.xmin;
    m_points[3].y=inRectangle.ymax;
}    

const GLQuad&
GLQuad::operator+=(const GLPoint& inPoint)
{
    for (U32 i=0; i<4; ++i)
    {
        m_points[i].x += inPoint.x;
        m_points[i].y += inPoint.y;
    }
    return *this;
}

void
GLQuad::CentreGet(GLPoint& outPoint) const
{
    tVal x=0;
    tVal y=0;
    
    for (U32 i=0; i<4; ++i)
    {
        x += m_points[i].x/4;
        y += m_points[i].y/4;
    }
    outPoint.x=x;
    outPoint.y=y;
}

void
GLQuad::RotateAboutCentre(tVal inAngle)
{
    GLPoint centre;
    CentreGet(centre);
    for (U32 i=0; i<4; ++i)
    {
        tVal x = m_points[i].x - centre.x;
        tVal y = m_points[i].y - centre.y;
        
        m_points[i].x = centre.x + x*cos(inAngle) + y*sin(inAngle);
        m_points[i].y = centre.y + y*cos(inAngle) - x*sin(inAngle);
    }
}

const GLPoint&
GLQuad::PointGet(U32 inWhich) const
{
    MUSHCOREASSERT(inWhich < 4);
    return m_points[inWhich];
}

void
GLQuad::PointSet(U32 inWhich, const GLPoint &inPoint)
{
    MUSHCOREASSERT(inWhich < 4);
    m_points[inWhich] = inPoint;
}

void
GLQuad::BoundingRectangleGet(GLRectangle& outRect) const
{
    outRect=GLRectangle(m_points[0], m_points[0]);
    for (U32 i=1; i<4; ++i)
    {
        if (m_points[i].x < outRect.xmin) outRect.xmin=m_points[i].x;
        if (m_points[i].y < outRect.ymin) outRect.ymin=m_points[i].y;
        if (m_points[i].x > outRect.xmax) outRect.xmax=m_points[i].x;
        if (m_points[i].y > outRect.ymax) outRect.ymax=m_points[i].y;
    }
}

void
GLQuad::Render(void) const
{
    GLState::TextureDisable();
    glBegin(GL_LINE_LOOP);
    glVertex2f(m_points[0].x, m_points[0].y);
    glVertex2f(m_points[1].x, m_points[1].y);
    glVertex2f(m_points[2].x, m_points[2].y);
    glVertex2f(m_points[3].x, m_points[3].y);
    glEnd();
}
    
