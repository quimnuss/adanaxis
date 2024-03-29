//%includeGuardStart {
#ifndef GLUTAPPHANDLER_H
#define GLUTAPPHANDLER_H
//%includeGuardStart } V5lB/FaZQwmwFXWd5bH0SQ
//%Header {
/*****************************************************************************
 *
 * File: src/GL/GLUTAppHandler.h
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
//%Header } lp0gPUFfVraLuxKSs/muOA
/*
 * $Id: GLUTAppHandler.h,v 1.26 2005/05/19 13:02:01 southa Exp $
 * $Log: GLUTAppHandler.h,v $
 * Revision 1.26  2005/05/19 13:02:01  southa
 * Mac release work
 *
 * Revision 1.25  2005/02/03 21:02:51  southa
 * Build fixes
 *
 * Revision 1.24  2004/09/27 22:42:08  southa
 * MSVC compilation fixes
 *
 * Revision 1.23  2004/01/06 20:46:49  southa
 * Build fixes
 *
 * Revision 1.22  2004/01/02 21:13:06  southa
 * Source conditioning
 *
 * Revision 1.21  2003/09/17 19:40:30  southa
 * Source conditioning upgrades
 *
 * Revision 1.20  2003/08/21 23:08:32  southa
 * Fixed file headers
 *
 * Revision 1.19  2003/01/11 13:03:12  southa
 * Use Mushcore header
 *
 * Revision 1.18  2003/01/09 14:56:59  southa
 * Created Mushcore
 *
 * Revision 1.17  2002/12/20 13:17:36  southa
 * Namespace changes, licence changes and source conditioning
 *
 * Revision 1.16  2002/12/03 20:28:14  southa
 * Network, player and control work
 *
 * Revision 1.15  2002/11/18 18:55:56  southa
 * Game resume and quit
 *
 * Revision 1.14  2002/10/22 20:42:02  southa
 * Source conditioning
 *
 * Revision 1.13  2002/10/15 14:02:29  southa
 * Mode changes
 *
 * Revision 1.12  2002/10/14 18:13:17  southa
 * GLModeDef work
 *
 * Revision 1.11  2002/08/27 08:56:22  southa
 * Source conditioning
 *
 * Revision 1.10  2002/08/17 10:41:50  southa
 * Designer fixes
 *
 * Revision 1.9  2002/08/07 13:36:48  southa
 * Conditioned source
 *
 * Revision 1.8  2002/08/02 15:20:54  southa
 * Frame rate timing
 *
 * Revision 1.7  2002/08/02 12:56:39  southa
 * Working collision checking
 *
 * Revision 1.6  2002/07/19 15:44:40  southa
 * Graphic optimisations
 *
 * Revision 1.5  2002/07/10 16:37:39  southa
 * Cursor removal
 *
 * Revision 1.4  2002/07/06 18:04:18  southa
 * More designer work
 *
 * Revision 1.3  2002/06/27 12:36:06  southa
 * Build process fixes
 *
 * Revision 1.2  2002/06/23 10:42:33  southa
 * SDL input
 *
 * Revision 1.1  2002/06/21 18:50:16  southa
 * SDLAppHandler added
 *
 */

#ifdef HAVE_GL_GLUT_H
#include <GL/glut.h>
#endif

#ifdef HAVE_GLUT_GLUT_H
#include <GLUT/glut.h>
#endif

#include "GLAppHandler.h"
#include "GLKeys.h"
#include "GLModeDef.h"
#include "mushMushcore.h"

class GLKeyboardSignal;

#if 0

class GLUTAppHandler : public GLAppHandler
{
public:
    GLUTAppHandler();
    virtual ~GLUTAppHandler() {}
    virtual bool KeyStateGet(const GLKeys& inKey) const;
    virtual bool LatchedKeyStateTake(const GLKeys& inKey);
    virtual void MousePositionGet(Mushware::tVal& outX, Mushware::tVal& outY) const;
    virtual void UnboundedMousePositionGet(Mushware::S32& outX, Mushware::S32& outY) const;
    virtual void EnterScreen(const GLModeDef& inDef);
    virtual void PostRedisplay(void);
    virtual void SwapBuffers(void);
    virtual Mushware::U32 WidthGet(void) const {return m_width;}
    virtual Mushware::U32 HeightGet(void) const {return m_height;}
    virtual Mushware::U32 MillisecondsGet(void) const;
    virtual void SetCursorState(bool inValue);
    virtual const GLModeDef& CurrentModeDefGet(void);
    virtual void PollForControlEvents(void);
    virtual void AppQuit(void);
    virtual void KeysOfInterestSet(const std::vector<GLKeys::tKeyValue>& inKeyValues);
    virtual void ReadHistoricControlState(Mushware::S32& outUnboundedMouseX, Mushware::S32& outUnboundedMouseY, std::vector<bool>& outKeys, Mushware::tVal inMsec);
    
protected:
    virtual void Initialise(void);
    virtual void Idle(void);
    virtual void MainLoop(void);
    virtual void Display(void);
    virtual void KeyboardSignal(const GLKeyboardSignal& inSignal);
    virtual void Signal(const MushcoreAppSignal& inSignal);
    bool IsVisible(void) {return m_visible;}

    // GLUT specific
    void RegisterHandlers(void);
    static GLKeys TranslateSpecialKey(int inKey);

private:
    static void IdleHandler(void);
    static void DisplayHandler(void);
    static void VisibilityHandler(int inState);
    static void KeyboardHandler(unsigned char inKey, int inX, int inY);
    static void KeyboardUpHandler(unsigned char inKey, int inX, int inY);
    static void SpecialHandler(int inKey, int inX, int inY);
    static void SpecialUpHandler(int inKey, int inX, int inY);
    static void PassiveMotionHandler(int inX, int inY);
    bool m_visible;
    std::vector<bool> m_keyState;
    std::vector<bool> m_latchedKeyState;
    Mushware::U32 m_width;
    Mushware::U32 m_height;
    Mushware::U32 m_bpp;
    GLModeDef m_modeDef;

    static Mushware::S32 m_mouseX;
    static Mushware::S32 m_mouseY;
    static Mushware::S32 m_lastMouseX;
    static Mushware::S32 m_lastMouseY;
    static Mushware::S32 m_mouseXDelta;
    static Mushware::S32 m_mouseYDelta;
    static bool m_lastMouseValid;
};

#endif

//%includeGuardEnd {
#endif
//%includeGuardEnd } hNb4yLSsimk5RFvFdUzHEw
