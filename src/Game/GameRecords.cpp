//%Header {
/*****************************************************************************
 *
 * File: src/Game/GameRecords.cpp
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
//%Header } rrsMgvREfE4iBVLy/rJDMQ
/*
 * $Id: GameRecords.cpp,v 1.22 2005/05/19 13:02:03 southa Exp $
 * $Log: GameRecords.cpp,v $
 * Revision 1.22  2005/05/19 13:02:03  southa
 * Mac release work
 *
 * Revision 1.21  2004/01/02 11:56:59  southa
 * MushPie created
 *
 * Revision 1.20  2004/01/01 23:04:02  southa
 * XCode fixes
 *
 * Revision 1.19  2003/10/04 18:12:13  southa
 * Readded
 *
 * Revision 1.17  2003/10/04 12:23:02  southa
 * File renaming
 *
 * Revision 1.16  2003/09/17 19:40:33  southa
 * Source conditioning upgrades
 *
 * Revision 1.15  2003/08/21 23:08:53  southa
 * Fixed file headers
 *
 * Revision 1.14  2003/01/13 14:32:00  southa
 * Build frameworks for Mac OS X
 *
 * Revision 1.13  2003/01/12 17:32:56  southa
 * Mushcore work
 *
 * Revision 1.12  2003/01/09 14:57:03  southa
 * Created Mushcore
 *
 * Revision 1.11  2003/01/07 17:13:43  southa
 * Fixes for gcc 3.1
 *
 * Revision 1.10  2002/12/29 20:59:57  southa
 * More build fixes
 *
 * Revision 1.9  2002/12/20 13:17:42  southa
 * Namespace changes, licence changes and source conditioning
 *
 * Revision 1.8  2002/11/24 23:18:24  southa
 * Added type name accessor to MushcorePickle
 *
 * Revision 1.7  2002/10/22 20:42:05  southa
 * Source conditioning
 *
 * Revision 1.6  2002/08/27 08:56:25  southa
 * Source conditioning
 *
 * Revision 1.5  2002/08/24 17:08:32  southa
 * Split time fixes
 *
 * Revision 1.4  2002/08/24 10:52:47  southa
 * World records
 *
 * Revision 1.3  2002/08/23 16:40:17  southa
 * Pickling correction
 *
 * Revision 1.2  2002/08/22 10:11:11  southa
 * Save records, spacebar dialogues
 *
 * Revision 1.1  2002/08/19 21:42:38  southa
 * Display of records and lap/split times
 *
 */

#include "GameRecords.h"

#include "GameSTL.h"

using namespace Mushware;
using namespace std;

GameRecords::GameRecords()
{
    Reset();
}

void
GameRecords::SplitTimeSet(U32 inWhich, GameTimer::tMsec inMsec)
{
    if (inWhich >= m_splitTimes.size())
    {
        m_splitTimes.resize(inWhich+1);
    }
    if (inWhich >= m_splitTimeValid.size())
    {
        m_splitTimeValid.resize(inWhich+1, false);
    }
    m_splitTimes[inWhich] = inMsec;
    m_splitTimeValid[inWhich] = true;
}

void
GameRecords::SplitTimePropose(U32 inWhich, GameTimer::tMsec inMsec)
{
    if (!SplitTimeValid(inWhich) || inMsec < SplitTimeGet(inWhich))
    {
        SplitTimeSet(inWhich, inMsec);
    }
}

GameTimer::tMsec
GameRecords::SplitTimeGet(U32 inWhich) const
{
    MUSHCOREASSERT(inWhich < m_splitTimes.size());
    return m_splitTimes[inWhich];
}

bool
GameRecords::SplitTimeValid(U32 inWhich) const
{
    if (inWhich >= m_splitTimeValid.size())
    {
        return false;
    }
    MUSHCOREASSERT(inWhich < m_splitTimeValid.size());
    return m_splitTimeValid[inWhich];
}

void
GameRecords::LapTimeSet(GameTimer::tMsec inMsec)
{
    m_lapTime = inMsec;
    m_lapTimeValid = true;
}

void
GameRecords::LapTimePropose(GameTimer::tMsec inMsec)
{
    if (!LapTimeValid() || inMsec < LapTimeGet())
    {
        LapTimeSet(inMsec);
    }
}

GameTimer::tMsec
GameRecords::LapTimeGet(void) const
{
    MUSHCOREASSERT(m_lapTimeValid);
    return m_lapTime;
}

bool
GameRecords::LapTimeValid(void) const
{
    return m_lapTimeValid;
}

void
GameRecords::RaceTimeSet(GameTimer::tMsec inMsec)
{
    m_raceTime = inMsec;
    m_raceTimeValid = true;
}

void
GameRecords::RaceTimePropose(GameTimer::tMsec inMsec)
{
    if (!RaceTimeValid() || inMsec < RaceTimeGet())
    {
        RaceTimeSet(inMsec);
    }
}

GameTimer::tMsec
GameRecords::RaceTimeGet(void) const
{
    MUSHCOREASSERT(m_raceTimeValid);
    return m_raceTime;
}

bool
GameRecords::RaceTimeValid(void) const
{
    return m_raceTimeValid;
}

bool
GameRecords::RecordsPropose(const GameRecords& inRecords)
{
    for (U32 i=0; inRecords.SplitTimeValid(i); ++i)
    {
        SplitTimePropose(i, inRecords.SplitTimeGet(i));
        if (i>1e5) throw("Value runaway in GameRecords::RecordsPropose");
    }
    if (inRecords.LapTimeValid())
    {
        LapTimePropose(inRecords.LapTimeGet());
    }
    if (inRecords.RaceTimeValid())
    {
        RaceTimePropose(inRecords.RaceTimeGet());
    }
    return false; // for the moment
}

void
GameRecords::Reset(void)
{
    m_splitTimes.resize(0);
    m_splitTimeValid.resize(0);
    m_lapTimeValid = false;
    m_raceTimeValid = false;
}

void
GameRecords::HandleSplitTimeEnd(MushcoreXML& inXML)
{
    istringstream data(inXML.TopData());
    const char *failMessage="Bad format for splittime.  Should be <splittime>100,200,350</splittime>";
    char comma;
    do
    {
        tVal splitTime;
        if (!(data >> splitTime)) inXML.Throw(failMessage);
        m_splitTimes.push_back(splitTime);
        m_splitTimeValid.push_back(true);
    } while ((data >> comma) && comma == ',');
}

void
GameRecords::HandleLapTimeEnd(MushcoreXML& inXML)
{
    istringstream data(inXML.TopData());
    const char *failMessage="Bad format for laptime.  Should be <laptime>1400</laptime>";
    if (!(data >> m_lapTime)) inXML.Throw(failMessage);
    m_lapTimeValid=true;
}

void
GameRecords::HandleRaceTimeEnd(MushcoreXML& inXML)
{
    istringstream data(inXML.TopData());
    const char *failMessage="Bad format for racetime.  Should be <racetime>7500</racetime>";
    if (!(data >> m_raceTime)) inXML.Throw(failMessage);
    m_raceTimeValid=true;
}

void
GameRecords::HandleRecordsStart(MushcoreXML& inXML)
{
    m_pickleState = kPickleData;
}

void
GameRecords::HandleRecordsEnd(MushcoreXML& inXML)
{
    inXML.StopHandler();
    UnpickleEpilogue();
}

void
GameRecords::NullHandler(MushcoreXML& inXML)
{
}

void
GameRecords::Pickle(ostream& inOut, const string& inPrefix) const
{
    inOut << inPrefix << "<records>" << endl;
    if (m_splitTimes.size() > 0 && SplitTimeValid(0))
    {
        inOut << inPrefix << "  <splittime>";
        for (U32 i=0; i<m_splitTimes.size(); ++i)
        {
            if (!SplitTimeValid(i)) break;
            inOut << SplitTimeGet(i);
            if (i+1 != m_splitTimes.size()) inOut << ",";
        }
        inOut << "</splittime>" << endl;
    }
    if (LapTimeValid())
    {
        inOut << inPrefix << "  <laptime>" << LapTimeGet() << "</laptime>" << endl;
    }
    if (RaceTimeValid())
    {
        inOut << inPrefix << "  <racetime>" << RaceTimeGet() << "</racetime>" << endl;
    }
    inOut << inPrefix << "</records>" << endl;
}

void
GameRecords::UnpicklePrologue(void)
{
    m_startTable.resize(kPickleNumStates);
    m_endTable.resize(kPickleNumStates);

    m_startTable[kPickleInit]["records"] = &GameRecords::HandleRecordsStart;
    m_startTable[kPickleData]["splittime"] = &GameRecords::NullHandler;
    m_endTable[kPickleData]["splittime"] = &GameRecords::HandleSplitTimeEnd;
    m_startTable[kPickleData]["laptime"] = &GameRecords::NullHandler;
    m_endTable[kPickleData]["laptime"] = &GameRecords::HandleLapTimeEnd;
    m_startTable[kPickleData]["racetime"] = &GameRecords::NullHandler;
    m_endTable[kPickleData]["racetime"] = &GameRecords::HandleRaceTimeEnd;
    m_startTable[kPickleData]["checksum"] = &GameRecords::NullHandler;
    m_endTable[kPickleData]["checksum"] = &GameRecords::NullHandler;
    m_endTable[kPickleData]["records"] = &GameRecords::HandleRecordsEnd;
    m_pickleState=kPickleInit;
    Reset();
}

void
GameRecords::UnpickleEpilogue(void)
{
    m_startTable.clear();
    m_endTable.clear();
}

void
GameRecords::Unpickle(MushcoreXML& inXML)
{
    UnpicklePrologue();
    inXML.ParseStream(*this);
}

void
GameRecords::XMLStartHandler(MushcoreXML& inXML)
{
ElementFunctionMap::iterator p2 = m_startTable[m_pickleState].find(inXML.TopTag());

    if (p2 != m_startTable[m_pickleState].end())
    {
        (this->*p2->second)(inXML);
    }
    else
    {
        ostringstream message;
        message << "Unexpected tag <" << inXML.TopTag() << "> in Records.  Potential matches are";
ElementFunctionMap::iterator p = m_startTable[m_pickleState].begin();
        while (p != m_startTable[m_pickleState].end())
        {
            message << " <" << p->first << ">";
            p++;
        }
        inXML.Throw(message.str());
    }
}

void
GameRecords::XMLEndHandler(MushcoreXML& inXML)
{
ElementFunctionMap::iterator p2 = m_endTable[m_pickleState].find(inXML.TopTag());

    if (p2 != m_endTable[m_pickleState].end())
    {
        (this->*p2->second)(inXML);
    }
    else
    {
        ostringstream message;
        message << "Unexpected end of tag </" << inXML.TopTag() << "> in Records.  Potential matches are";
ElementFunctionMap::iterator p = m_endTable[m_pickleState].begin();
        while (p != m_endTable[m_pickleState].end())
        {
            message << " <" << p->first << ">";
            p++;
        }
        inXML.Throw(message.str());
    }
}

void
GameRecords::XMLDataHandler(MushcoreXML& inXML)
{
}

char *
GameRecords::TypeNameGet(void) const
{
    return "gamerecords";
}

