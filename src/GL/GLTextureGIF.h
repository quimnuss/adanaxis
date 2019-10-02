//%includeGuardStart {
#ifndef GLTEXTUREGIF_H
#define GLTEXTUREGIF_H
//%includeGuardStart } VMbl1m0z+keYDf3fAr+QjQ
//%Header {
/*****************************************************************************
 *
 * File: src/GL/GLTextureGIF.h
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
//%Header } RVdNxiMr6Y9TlH4NgzlzDw
/*
 * $Id: GLTextureGIF.h,v 1.12 2005/05/19 13:02:00 southa Exp $
 * $Log: GLTextureGIF.h,v $
 * Revision 1.12  2005/05/19 13:02:00  southa
 * Mac release work
 *
 * Revision 1.11  2004/01/02 21:13:06  southa
 * Source conditioning
 *
 * Revision 1.10  2003/09/17 19:40:30  southa
 * Source conditioning upgrades
 *
 * Revision 1.9  2003/08/21 23:08:31  southa
 * Fixed file headers
 *
 * Revision 1.8  2002/12/29 20:30:53  southa
 * Work for gcc 3.1 build
 *
 * Revision 1.7  2002/12/20 13:17:36  southa
 * Namespace changes, licence changes and source conditioning
 *
 * Revision 1.6  2002/10/22 20:42:01  southa
 * Source conditioning
 *
 * Revision 1.5  2002/08/27 08:56:21  southa
 * Source conditioning
 *
 * Revision 1.4  2002/08/07 13:36:47  southa
 * Conditioned source
 *
 * Revision 1.3  2002/07/06 18:04:17  southa
 * More designer work
 *
 * Revision 1.2  2002/06/27 12:36:05  southa
 * Build process fixes
 *
 * Revision 1.1  2002/05/10 16:40:38  southa
 * Changed .hp files to .h
 *
 * Revision 1.1  2002/02/25 23:05:15  southa
 * Subclassed GLTexture
 *
 */

#include "GLTexture.h"

class GLTextureGIF : public GLTexture
{
public:
    GLTextureGIF(const std::string& inFilename);
    GLTextureGIF *Clone(void) const {return new GLTextureGIF(*this);}

protected:

private:
    void ThrowGifError(const std::string& inFilename, int inRC);
    const char *FiletypeName(void) const;
};

//%includeGuardEnd {
#endif
//%includeGuardEnd } hNb4yLSsimk5RFvFdUzHEw
