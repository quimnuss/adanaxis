//%includeGuardStart {
#ifndef ADANAXISSAVEDATA_H
#define ADANAXISSAVEDATA_H
//%includeGuardStart } Phse6V6gEtb3LIdaz1rQlw
//%Header {
/*****************************************************************************
 *
 * File: src/Adanaxis/AdanaxisSaveData.h
 *
 * Copyright: Andy Southgate 2005
 *
 * This file may be used and distributed under the terms of the Mushware
 * software licence version 1.0, under the terms for 'Proprietary original
 * source files'.  If not supplied with this software, a copy of the licence
 * can be obtained from Mushware Limited via http://www.mushware.com/.
 * One of your options under that licence is to use and distribute this file
 * under the terms of the GNU General Public Licence version 2.
 *
 * This software carries NO WARRANTY of any kind.
 *
 ****************************************************************************/
//%Header } lP1KYvwjO8U4fVO07cIsNw
/*
 * $Id: AdanaxisSaveData.h,v 1.10 2005/08/01 13:09:57 southa Exp $
 * $Log: AdanaxisSaveData.h,v $
 * Revision 1.10  2005/08/01 13:09:57  southa
 * Collision messaging
 *
 * Revision 1.9  2005/07/29 14:59:49  southa
 * Maptor access
 *
 * Revision 1.8  2005/07/29 11:52:43  southa
 * MushcoreMaptor created
 *
 * Revision 1.7  2005/07/29 08:27:47  southa
 * Collision work
 *
 * Revision 1.6  2005/07/12 12:18:17  southa
 * Projectile work
 *
 * Revision 1.5  2005/06/30 14:26:35  southa
 * Adanaxis work
 *
 * Revision 1.4  2005/06/29 11:11:15  southa
 * Camera and rendering work
 *
 * Revision 1.3  2005/06/20 14:30:33  southa
 * Adanaxis work
 *
 * Revision 1.2  2005/06/16 17:25:37  southa
 * Client/server work
 *
 * Revision 1.1  2005/06/14 20:39:40  southa
 * Adanaxis work
 *
 */

#include "AdanaxisStandard.h"

#include "AdanaxisPieceKhazi.h"
#include "AdanaxisPieceProjectile.h"

#include "API/mushGame.h"
#include "API/mushMushGame.h"

//:xml1base MushGameSaveData
//:generate virtual standard ostream xml1 nocopy
class AdanaxisSaveData : public MushGameSaveData
{
public:
    typedef AdanaxisPieceProjectile tProjectile;
    typedef MushcoreMaptor<tProjectile> tProjectileList;
    typedef AdanaxisPieceKhazi tKhazi;
    typedef MushcoreMaptor<tKhazi> tKhaziList;

    AdanaxisSaveData();
    virtual ~AdanaxisSaveData() {}

    tProjectile& Projectile(Mushware::U32 inNum) { return m_projectileList.Get(inNum); }
    const tProjectile& Projectile(Mushware::U32 inNum) const { return m_projectileList.Get(inNum); }
    tKhazi& Khazi(Mushware::U32 inNum) { return m_khaziList.Get(inNum); }
    const tKhazi& Khazi(Mushware::U32 inNum) const { return m_khaziList.Get(inNum); }
    
private:
    tProjectileList m_projectileList; //:read :wref
    tKhaziList m_khaziList; //:read :wref
    MushcoreData<GameDialogue> m_dialogues; //:read :wref
    
//%classPrototypes {
public:
    const tProjectileList& ProjectileList(void) const { return m_projectileList; }
    // Writable reference for m_projectileList
    tProjectileList& ProjectileListWRef(void) { return m_projectileList; }
    const tKhaziList& KhaziList(void) const { return m_khaziList; }
    // Writable reference for m_khaziList
    tKhaziList& KhaziListWRef(void) { return m_khaziList; }
    const MushcoreData<GameDialogue>& Dialogues(void) const { return m_dialogues; }
    // Writable reference for m_dialogues
    MushcoreData<GameDialogue>& DialoguesWRef(void) { return m_dialogues; }
    virtual const char *AutoName(void) const;
    virtual MushcoreVirtualObject *AutoClone(void) const;
    virtual MushcoreVirtualObject *AutoCreate(void) const;
    static MushcoreVirtualObject *AutoVirtualFactory(void);
    virtual void AutoPrint(std::ostream& ioOut) const;
    virtual bool AutoXMLDataProcess(MushcoreXMLIStream& ioIn, const std::string& inTagStr);
    virtual void AutoXMLPrint(MushcoreXMLOStream& ioOut) const;
//%classPrototypes } j/3JActG1IT8dYrr+N96mQ
};

//%inlineHeader {
inline std::ostream&
operator<<(std::ostream& ioOut, const AdanaxisSaveData& inObj)
{
    inObj.AutoPrint(ioOut);
    return ioOut;
}
//%inlineHeader } +jNhUDLSnXe2KY7BCQ1TrA
//%includeGuardEnd {
#endif
//%includeGuardEnd } hNb4yLSsimk5RFvFdUzHEw
