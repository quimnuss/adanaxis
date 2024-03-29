//%Header {
/*****************************************************************************
 *
 * File: src/MushMesh/MushMesh4Mesh.cpp
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
//%Header } v136Oh1IVziX36Di81JIXQ
/*
 * $Id: MushMesh4Mesh.cpp,v 1.10 2005/08/01 17:58:25 southa Exp $
 * $Log: MushMesh4Mesh.cpp,v $
 * Revision 1.10  2005/08/01 17:58:25  southa
 * Object explosion
 *
 * Revision 1.9  2005/07/19 13:44:26  southa
 * MushMesh4Chunk work
 *
 * Revision 1.8  2005/07/14 12:50:31  southa
 * Extrusion work
 *
 * Revision 1.7  2005/07/13 16:45:05  southa
 * Extrusion work
 *
 * Revision 1.6  2005/07/12 20:39:05  southa
 * Mesh library work
 *
 * Revision 1.5  2005/07/04 11:10:43  southa
 * Rendering pipeline
 *
 * Revision 1.4  2005/07/02 00:42:38  southa
 * Conditioning tweaks
 *
 * Revision 1.3  2005/06/30 14:26:36  southa
 * Adanaxis work
 *
 * Revision 1.2  2005/06/30 12:34:59  southa
 * Mesh and source conditioner work
 *
 * Revision 1.1  2005/06/30 12:04:55  southa
 * Mesh work
 *
 */

#include "MushMesh4Mesh.h"

#include "MushMeshMushcoreIO.h"
#include "MushMeshSTL.h"

using namespace Mushware;
using namespace std;

MushMesh4Mesh::MushMesh4Mesh() :
    m_vertexCounter(0),
    m_faceCounter(0)
{
    AllTouch();
}

void
MushMesh4Mesh::VerticesTouch(void)
{
    m_normalsValid = false;
    m_centroidValid = false;
    m_boundingRadiusValid = false;
    U32 numFaces = m_faces.size();
    for (U32 i=0; i<numFaces; ++i)
    {
        m_faces[i].VerticesTouch();
    }
}

void
MushMesh4Mesh::AllTouch(void)
{
    for (U32 i=0; i<m_faces.size(); ++i)
    {
        m_faces[i].AllTouch();
    }
    for (U32 i=0; i<m_chunks.size(); ++i)
    {
        m_chunks[i].AllTouch();
    }
    VerticesTouch();
    m_connectivityValid = false;
}

void
MushMesh4Mesh::Prebuild(void)
{
    NormalsBuild();
    ConnectivityBuild();
    CentroidBuild();
    BoundingRadiusBuild();

    for (U32 i=0; i<m_faces.size(); ++i)
    {
        FaceCentroidBuild(i);
        FaceConnectivityBuild(i); 
        m_faces[i].VertexConnectivity();
    }
    
    for (U32 i=0; i<m_chunks.size(); ++i)
    {
        ChunkUniqueVertexListBuild(i);
        ChunkBoundingRadiusBuild(i);
        ChunkCentroidBuild(i);
    }
}

void
MushMesh4Mesh::NormalsBuild(void) const
{
    m_normalsValid = true;
}

void
MushMesh4Mesh::ConnectivityBuild(void) const
{
    m_numConnections = 0;
    m_connectivity.resize(m_vertices.size());
    for (U32 i=0; i<m_connectivity.size(); ++i)
    {
        m_connectivity[i].resize(0);
    }    
    
    for (U32 i=0; i<m_faces.size(); ++i)
    {
        const MushMesh4Face& faceRef = m_faces[i];
        const MushMesh4Face::tVertexList& vertexListRef = faceRef.VertexList();
        const MushMesh4Face::tVertexGroupSize& vertexGroupSizeRef = faceRef.VertexGroupSize();
        
        U32 listIndex = 0;
        for (U32 j=0; j<vertexGroupSizeRef.size(); ++j)
        {
            U32 vertexGroupSize = vertexGroupSizeRef[j];
            if (vertexGroupSize > 1)
            {
                for (U32 k=0; k<vertexGroupSize; ++k)
                {
                    U32 vertIndex = listIndex+k;
                    U32 otherVertIndex;
                    if (k+1 < vertexGroupSize)
                    {
                        otherVertIndex = listIndex+k+1;
                    }
                    else
                    {
                        otherVertIndex = listIndex;
                    }
                    
                    MUSHCOREASSERT(vertIndex < vertexListRef.size());
                    MUSHCOREASSERT(otherVertIndex < vertexListRef.size());

                    U32 vertNum = vertexListRef[vertIndex];
                    U32 otherVertNum = vertexListRef[otherVertIndex];
                    
                    MUSHCOREASSERT(vertNum < m_connectivity.size());
                    MUSHCOREASSERT(otherVertNum < m_connectivity.size());
                    
                    tConnection& vertexConnection = m_connectivity[vertNum];
                    tConnection& otherVertexConnection = m_connectivity[otherVertNum];

                    if (std::find(vertexConnection.begin(),
                                  vertexConnection.end(),
                                  otherVertNum) == vertexConnection.end())
                    {
                        vertexConnection.push_back(otherVertNum);
                        ++m_numConnections;
                    }
                    if (std::find(otherVertexConnection.begin(),
                                  otherVertexConnection.end(),
                                  vertNum) == otherVertexConnection.end())
                    {
                        otherVertexConnection.push_back(vertNum);
                        ++m_numConnections;
                    }
                }
            }
            listIndex += vertexGroupSize;
        }
    }
    
    for (U32 i=0; i<m_connectivity.size(); ++i)
    {
        std::sort(m_connectivity[i].begin(), m_connectivity[i].end());
    }  
    
    // Each connection counted twice (once from each end), so divide
    m_numConnections /= 2;
    
    m_connectivityValid = true;
}

void
MushMesh4Mesh::FaceConnectivityBuild(Mushware::U32 inFaceNum) const
{
    // cout << "Face connectivity for face " << inFaceNum <<  endl;

    const MushMesh4Face& srcFaceRef = Face(inFaceNum);
    
    const MushMesh4Face::tVertexList& srcUniqueVertexListRef = srcFaceRef.UniqueVertexList();
    
    srcFaceRef.FaceConnectivityWRef().resize(0);
    
    /* Loop through each face looking for those that are connected to the source face,
     * i.e. they share at least one facet
     */
    
    for (U32 i=0; i<m_faces.size(); ++i)
    {
        bool faceConnected = false;
        Mushware::U32 srcFacetNum = 0;
        Mushware::U32 testFacetNum = 0;
        
        // but don't check whether this face is connected to itself
        if (i != inFaceNum)
        {
            const MushMesh4Face& testFaceRef = m_faces[i];
            const MushMesh4Face::tVertexList& testUniqueVertexListRef = testFaceRef.UniqueVertexList();

            // Quick check - faces must have at least three vertices in common to share a facet
            U32 countedMatches = MushcoreUtil::CountMatchesInSortedUnique(srcUniqueVertexListRef, testUniqueVertexListRef, 16);
            if (countedMatches > 2)
            {
                // Candidate for match.  Now find the common facet
                const MushMesh4Face::tVertexList& srcVertexListRef = srcFaceRef.VertexList();
                const U32 srcVertexListSize = srcVertexListRef.size();
                const MushMesh4Face::tVertexGroupSize& srcVertexGroupSizeRef = srcFaceRef.VertexGroupSize();
                const MushMesh4Face::tVertexList& testVertexListRef = testFaceRef.VertexList();
                const U32 testVertexListSize = testVertexListRef.size();
                const MushMesh4Face::tVertexGroupSize& testVertexGroupSizeRef = testFaceRef.VertexGroupSize();

                /* Loop through all source face facets looking for one which is connected.  Once
                 * faceConnected is true after the facet-to-facet test we know the faces are
                 * connected and exit
                 */
                U32 srcVertexIndex = 0;
                srcFacetNum = 0;
                for (U32 srcVGIndex = 0; !faceConnected && srcVGIndex < srcVertexGroupSizeRef.size(); ++srcVGIndex)
                {
                    U32 srcVGSize = srcVertexGroupSizeRef[srcVGIndex];

                    U32 testVertexIndex = 0;
                    testFacetNum = 0;
                    
                    // Find one facet in the test face which connects to this source facet
                    for (U32 testVGIndex = 0; !faceConnected && testVGIndex < testVertexGroupSizeRef.size(); ++testVGIndex)
                    {
                        U32 testVGSize = testVertexGroupSizeRef[testVGIndex];
                        if (testVGSize == srcVGSize)
                        {
                            faceConnected = true;
                            // For every vertex in the source vertex group, find it in the test group
                            for (U32 srcInd = 0; faceConnected && srcInd < srcVGSize; ++srcInd)
                            {
                                if (srcVertexIndex + srcInd >= srcVertexListSize)
                                {
                                    throw MushcoreDataFail("Source face vertex index overrun");
                                }
                                Mushware::U32 srcVertexNum = srcVertexListRef[srcVertexIndex + srcInd];
                                bool vertexResult = false;
                                U32 testIndLimit = testVertexIndex + testVGSize;
                                for (U32 testInd = testVertexIndex; testInd < testIndLimit; ++testInd)
                                {
                                    if (testInd >= testVertexListSize)
                                    {
                                        throw MushcoreDataFail("Test face vertex index overrun");
                                    }
                                    if (srcVertexNum == testVertexListRef[testInd])
                                    {
                                        vertexResult = true;
                                        break;
                                    }
                                }
                                faceConnected = faceConnected && vertexResult;
                            }
                        }
                        testVertexIndex += testVGSize;
                        if (!faceConnected)
                        {
                            ++testFacetNum;
                        }
                    }
                    srcVertexIndex += srcVGSize;
                    if (!faceConnected)
                    {
                        ++srcFacetNum;
                    }
                }
            }
        }
        if (faceConnected)
        {
            // cout << "Face " << inFaceNum << " is connected to face " << i << " via " << srcFacetNum << " -> " << testFacetNum << endl;
            tFace::tFaceConnection connection;
            connection.FaceNumSet(i);
            connection.LocalFacetNumSet(srcFacetNum);
            connection.RemoteFacetNumSet(testFacetNum);
            srcFaceRef.FaceConnectivityWRef().push_back(connection);
        }
    }
    srcFaceRef.FaceConnectivityValidSet(true);
}

void
MushMesh4Mesh::CentroidBuild(void) const
{
    m_centroid = t4Val(0,0,0,0);
    U32 verticesSize = m_vertices.size();
    
    if (verticesSize == 0)
    {
        throw MushcoreDataFail("Attempt to build centroid of object with no vertices");
    }    
    
    for (U32 i=0; i<verticesSize; ++i)
    {
        m_centroid += m_vertices[i];
    }
    m_centroid /= verticesSize;
    
    m_centroidValid = true;
}

void
MushMesh4Mesh::BoundingRadiusBuild(void) const
{
    if (!m_centroidValid)
    {
        CentroidBuild();
    }
    
    tVal maxRadiusSquared = 0;
    
    U32 verticesSize = m_vertices.size();
    for (U32 i=0; i<verticesSize; ++i)
    {
        tVal radiusSquared = (m_centroid - m_vertices[i]).MagnitudeSquared();
        if (radiusSquared >  maxRadiusSquared)
        {
             maxRadiusSquared = radiusSquared;
        }
    }
    
    m_boundingRadius = std::sqrt(maxRadiusSquared);
    m_boundingRadiusValid = true;
}

void
MushMesh4Mesh::FaceCentroidBuild(Mushware::U32 inFaceNum) const
{
    const MushMesh4Face& faceRef = Face(inFaceNum);

    const MushMesh4Face::tVertexList& uniqueVertexList = faceRef.UniqueVertexList();

    U32 uniqueVertexListSize = uniqueVertexList.size();
    
    if (uniqueVertexListSize == 0)
    {
        throw MushcoreDataFail("Attempt to build centroid of face with no vertices");
    }    
    
    t4Val centroid;
    centroid.ToAdditiveIdentitySet();
    
    for (U32 i=0; i<uniqueVertexListSize; ++i)
    {
        centroid += Vertex(uniqueVertexList[i]);
    }
    faceRef.FaceCentroidSet(centroid / uniqueVertexListSize);
}

void
MushMesh4Mesh::ChunkUniqueVertexListBuild(Mushware::U32 inChunkNum) const
{
    const tChunk& chunkRef = Chunk(inChunkNum);

    MushMesh4Chunk::tVertexList& destUniqueVertexListRef = chunkRef.UniqueVertexListWRef();
    destUniqueVertexListRef.clear();
        
    const MushMesh4Chunk::tFaceList& faceListRef = chunkRef.FaceList();
    Mushware::U32 faceListSize = faceListRef.size();
    
    for (U32 i=0; i<faceListSize; ++i)
    {
        const MushMesh4Face& faceRef = Face(faceListRef[i]);
        const MushMesh4Face::tVertexList& srcUniqueVertexListRef = faceRef.UniqueVertexList();
        U32 srcUniqueVertexListSize = srcUniqueVertexListRef.size();
        for (U32 j=0; j<srcUniqueVertexListSize; ++j)
        {
            U32 vertNum = srcUniqueVertexListRef[j];
            if (std::find(destUniqueVertexListRef.begin(),
                          destUniqueVertexListRef.end(),
                          vertNum) == destUniqueVertexListRef.end())
            {
                destUniqueVertexListRef.push_back(vertNum);
            }
        }
    }
    chunkRef.UniqueVertexListValidSet(true);
}

void
MushMesh4Mesh::ChunkBoundingRadiusBuild(Mushware::U32 inChunkNum) const
{
    const tChunk& chunkRef = Chunk(inChunkNum);
    
    t4Val centroid = ChunkCentroid(inChunkNum);
        
    tVal maxRadiusSquared = 0;
    
    const MushMesh4Chunk::tVertexList& uniqueVertexList = ChunkUniqueVertexList(inChunkNum);
    
    U32 uniqueVertexListSize = uniqueVertexList.size();
    
    for (U32 i=0; i<uniqueVertexListSize; ++i)
    {
        tVal radiusSquared = (centroid - Vertex(uniqueVertexList[i])).MagnitudeSquared();
        if (radiusSquared >  maxRadiusSquared)
        {
            maxRadiusSquared = radiusSquared;
        }
    }
    
    chunkRef.BoundingRadiusSet(std::sqrt(maxRadiusSquared));
}

void
MushMesh4Mesh::ChunkCentroidBuild(Mushware::U32 inChunkNum) const
{
    const tChunk& chunkRef = Chunk(inChunkNum);
    
    const MushMesh4Chunk::tVertexList& uniqueVertexList = ChunkUniqueVertexList(inChunkNum);
    
    U32 uniqueVertexListSize = uniqueVertexList.size();
    
    if (uniqueVertexListSize == 0)
    {
        throw MushcoreDataFail("Attempt to build centroid of chunk with no vertices");
    }
    
    t4Val centroid(t4Val::AdditiveIdentity());
    
    for (U32 i=0; i<uniqueVertexListSize; ++i)
    {
        centroid += Vertex(uniqueVertexList[i]);
    }
    chunkRef.CentroidSet(centroid / uniqueVertexListSize);
}

//%outOfLineFunctions {

const char *MushMesh4Mesh::AutoName(void) const
{
    return "MushMesh4Mesh";
}

MushcoreVirtualObject *MushMesh4Mesh::AutoClone(void) const
{
    return new MushMesh4Mesh(*this);
}

MushcoreVirtualObject *MushMesh4Mesh::AutoCreate(void) const
{
    return new MushMesh4Mesh;
}

MushcoreVirtualObject *MushMesh4Mesh::AutoVirtualFactory(void)
{
    return new MushMesh4Mesh;
}
namespace
{
void AutoInstall(void)
{
    MushcoreFactory::Sgl().FactoryAdd("MushMesh4Mesh", MushMesh4Mesh::AutoVirtualFactory);
}
MushcoreInstaller AutoInstaller(AutoInstall);
} // end anonymous namespace
void
MushMesh4Mesh::AutoPrint(std::ostream& ioOut) const
{
    ioOut << "[";
    MushMeshMesh::AutoPrint(ioOut);
    ioOut << "vertices=" << m_vertices << ", ";
    ioOut << "texCoords=" << m_texCoords << ", ";
    ioOut << "faces=" << m_faces << ", ";
    ioOut << "vertexCounter=" << m_vertexCounter << ", ";
    ioOut << "faceCounter=" << m_faceCounter << ", ";
    ioOut << "faceGenerator=" << m_faceGenerator << ", ";
    ioOut << "vertexGenerator=" << m_vertexGenerator << ", ";
    ioOut << "chunks=" << m_chunks << ", ";
    ioOut << "normals=" << m_normals << ", ";
    ioOut << "connectivity=" << m_connectivity << ", ";
    ioOut << "centroid=" << m_centroid << ", ";
    ioOut << "boundingRadius=" << m_boundingRadius << ", ";
    ioOut << "faceCentroids=" << m_faceCentroids << ", ";
    ioOut << "faceBoundingRadii=" << m_faceBoundingRadii << ", ";
    ioOut << "numConnections=" << m_numConnections << ", ";
    ioOut << "normalsValid=" << m_normalsValid << ", ";
    ioOut << "connectivityValid=" << m_connectivityValid << ", ";
    ioOut << "centroidValid=" << m_centroidValid << ", ";
    ioOut << "boundingRadiusValid=" << m_boundingRadiusValid;
    ioOut << "]";
}
bool
MushMesh4Mesh::AutoXMLDataProcess(MushcoreXMLIStream& ioIn, const std::string& inTagStr)
{
    if (inTagStr == "obj")
    {
        AutoInputPrologue(ioIn);
        ioIn >> *this;
        AutoInputEpilogue(ioIn);
    }
    else if (inTagStr == "vertices")
    {
        ioIn >> m_vertices;
    }
    else if (inTagStr == "texCoords")
    {
        ioIn >> m_texCoords;
    }
    else if (inTagStr == "faces")
    {
        ioIn >> m_faces;
    }
    else if (inTagStr == "vertexCounter")
    {
        ioIn >> m_vertexCounter;
    }
    else if (inTagStr == "faceCounter")
    {
        ioIn >> m_faceCounter;
    }
    else if (inTagStr == "faceGenerator")
    {
        ioIn >> m_faceGenerator;
    }
    else if (inTagStr == "vertexGenerator")
    {
        ioIn >> m_vertexGenerator;
    }
    else if (inTagStr == "chunks")
    {
        ioIn >> m_chunks;
    }
    else if (inTagStr == "normals")
    {
        ioIn >> m_normals;
    }
    else if (inTagStr == "connectivity")
    {
        ioIn >> m_connectivity;
    }
    else if (inTagStr == "centroid")
    {
        ioIn >> m_centroid;
    }
    else if (inTagStr == "boundingRadius")
    {
        ioIn >> m_boundingRadius;
    }
    else if (inTagStr == "faceCentroids")
    {
        ioIn >> m_faceCentroids;
    }
    else if (inTagStr == "faceBoundingRadii")
    {
        ioIn >> m_faceBoundingRadii;
    }
    else if (inTagStr == "numConnections")
    {
        ioIn >> m_numConnections;
    }
    else if (inTagStr == "normalsValid")
    {
        ioIn >> m_normalsValid;
    }
    else if (inTagStr == "connectivityValid")
    {
        ioIn >> m_connectivityValid;
    }
    else if (inTagStr == "centroidValid")
    {
        ioIn >> m_centroidValid;
    }
    else if (inTagStr == "boundingRadiusValid")
    {
        ioIn >> m_boundingRadiusValid;
    }
    else if (MushMeshMesh::AutoXMLDataProcess(ioIn, inTagStr))
    {
        // Tag consumed by base class
    }
    else 
    {
        return false;
    }
    return true;
}
void
MushMesh4Mesh::AutoXMLPrint(MushcoreXMLOStream& ioOut) const
{
    MushMeshMesh::AutoXMLPrint(ioOut);
    ioOut.TagSet("vertices");
    ioOut << m_vertices;
    ioOut.TagSet("texCoords");
    ioOut << m_texCoords;
    ioOut.TagSet("faces");
    ioOut << m_faces;
    ioOut.TagSet("vertexCounter");
    ioOut << m_vertexCounter;
    ioOut.TagSet("faceCounter");
    ioOut << m_faceCounter;
    ioOut.TagSet("faceGenerator");
    ioOut << m_faceGenerator;
    ioOut.TagSet("vertexGenerator");
    ioOut << m_vertexGenerator;
    ioOut.TagSet("chunks");
    ioOut << m_chunks;
    ioOut.TagSet("normals");
    ioOut << m_normals;
    ioOut.TagSet("connectivity");
    ioOut << m_connectivity;
    ioOut.TagSet("centroid");
    ioOut << m_centroid;
    ioOut.TagSet("boundingRadius");
    ioOut << m_boundingRadius;
    ioOut.TagSet("faceCentroids");
    ioOut << m_faceCentroids;
    ioOut.TagSet("faceBoundingRadii");
    ioOut << m_faceBoundingRadii;
    ioOut.TagSet("numConnections");
    ioOut << m_numConnections;
    ioOut.TagSet("normalsValid");
    ioOut << m_normalsValid;
    ioOut.TagSet("connectivityValid");
    ioOut << m_connectivityValid;
    ioOut.TagSet("centroidValid");
    ioOut << m_centroidValid;
    ioOut.TagSet("boundingRadiusValid");
    ioOut << m_boundingRadiusValid;
}
//%outOfLineFunctions } tRHNwx/epHtokbpQGhNBiQ
