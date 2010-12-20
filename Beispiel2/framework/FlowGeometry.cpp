#include "FlowGeometry.h"
#include "reverseBytes.h"

bool FlowGeometry::readFromFile(char* header, FILE* fp, bool bigEndian)
{
	isFlipped = false;
	//determine the dimensions
    sscanf(header,"SN4DB %d %d %d",&dim[0],&dim[1],&dim[2]);
    std::cout << "Dimensions: " << dim[0] << " x " << dim[1] << " x " << dim[2] << std::endl;
    if (dim[2]!=1)
    {
        std::cerr << "Invalid Z dimension value." << std::endl;
        return false;
    }

	geometryData = new vec3[dim[0]*dim[1]];
	//read the data and check if everything went fine
    int result = fread(geometryData,sizeof(vec3),dim[0]*dim[1],fp);
	if (result != dim[0]*dim[1])
	{
		std::cerr << "+ Error reading grid file." << std::endl << std::endl;
		return false;
	}

	if (bigEndian)
		for (int j = 0; j < getDimX()*getDimY(); j++)
			for (int k = 0; k < 3; k++)
				geometryData[j][k] = reverseBytes<float>(geometryData[j][k]);

    //first vertex
	boundaryMin = vec3(getPos(0));
	//last vertex
	boundaryMax = vec3(getPos((dim[0]*dim[1]) - 1));
	boundarySize = boundaryMax - boundaryMin;

    //if X and Y are swapped... meaning that going in a row the y value increases and x stays the same
    if (getPosY(dim[0]-1)>(boundaryMin[1] + boundaryMax[1])*0.5)
    {
		//this flag will fix that, anywhere where it's neccerasy... watch it :)
        isFlipped = true;
        std::cout << "Flipped Y and X dimensions." << std::endl;
    }
    else isFlipped = false;        

    std::cout << "X Boundaries: " << boundaryMin[0] << " ... " << boundaryMax[0] << std::endl;
    std::cout << "Y Boundaries: " << boundaryMin[1] << " ... " << boundaryMax[1] << std::endl;
    
    return true;
}

FlowGeometry::FlowGeometry()
{
    geometryData = NULL;
}

FlowGeometry::~FlowGeometry()
{
    if (geometryData)
        delete[] geometryData;
}

///returns X index of the last vertex lying left to the position x and the Y index of the last vertex lying under the position y 
int FlowGeometry::getXYvtx(vec3 pos)
{
    int i;
	int j;
	//search for the column left to the vertex
    for (i = 0; (i < dim[0])&&(getPosX(getVtx(i,0)) < pos[0]); i++);
	//search for the row under the vertex
	for (j = 0; (j < dim[1])&&(getPosY(getVtx(0,j)) < pos[1]); j++);
	
	//return the vertex ID of the found vertex
	return getVtx((i<dim[0]) ? i : dim[0]-1, (j<dim[1]) ? j : dim[1]-1);
}

inline vec3 FlowGeometry::getPos(int vtxID)
{
	return geometryData[vtxID];
}

inline float FlowGeometry::getPosX(int vtxID)
{
	return geometryData[vtxID][0];
}

inline float FlowGeometry::getPosY(int vtxID)
{
	return geometryData[vtxID][1];
}

//TODO students: improve this
///a very slow and dumb routine, that finds the nearest vertex to the given position
int FlowGeometry::getNearestVtx(vec3 pos)
{
    int index = 0, step = dim[0] / 2, coord = 0;
    while (step > 0)
    {
        if (index + step < dim[coord] && getPos(getVtx(index + step, 0)).v[coord] < pos.v[coord])
        {
            index += step;
        } else {
            step /= 2;
        }
    }
    if (index + 1 < dim[coord] &&
        pos.v[coord] - getPos(getVtx(index, 0)).v[coord] >
        getPos(getVtx(index + 1, 0)).v[coord] - pos.v[coord])
        ++index;

    int indexX = index;


    index = 0, step = dim[1] / 2, coord = 1;
    while (step > 0)
    {
        if (index + step < dim[coord] && getPos(getVtx(0, index + step)).v[coord] < pos.v[coord])
        {
            index += step;
        } else {
            step /= 2;
        }
    }
    if (index + 1 < dim[coord] &&
        pos.v[coord] - getPos(getVtx(0, index)).v[coord] >
        getPos(getVtx(0, index + 1)).v[coord] - pos.v[coord])
        ++index;
    int indexY = index;

    return getVtx(indexX, indexY);
}

bool FlowGeometry::getInterpolationAt(vec3 pos, int* vtxID, float* coef)
{
    //if we are outside of the dataset, return false
	//please note, that this test is valid only for rectangular datasets (block, hurricane), but not for tube
    if ((pos[0]<boundaryMin[0])||(pos[1]<boundaryMin[1])||(pos[0]>boundaryMax[0])||(pos[1]>boundaryMax[1]))
        return false;

    // bilinear interpolation
    int vertexIndex = getNearestVtx(pos);
    vec3 vertexPos = geometryData[vertexIndex];

    // indices of the vertices in the cell are:
    // 2 -- 3
    // |    |
    // 0 -- 1
    if (getRightNeigh(vertexIndex) < 0 || pos.v[0] < vertexPos.v[0])
    {
        vtxID[0] = getLeftNeigh(vertexIndex);
        vtxID[1] = vertexIndex;
    } else {
        vtxID[0] = vertexIndex;
        vtxID[1] = getRightNeigh(vertexIndex);
    }

    /*
    vec3 v0 = geometryData[vtxID[0]], v1 = geometryData[vtxID[1]];
    if (vtxID[0] < 0 || vtxID[1] < 0 )
    {
        std::cout << "vtxIDs: " << vtxID[0] << ", " << vtxID[1] << std::endl;
        std::cout << std::endl;
    }
    */

    if (getBottomNeigh(vtxID[0]) < 0 || pos.v[1] < vertexPos.v[1])
    {
        vtxID[2] = getTopNeigh(vtxID[0]);
        vtxID[3] = getTopNeigh(vtxID[1]);
    } else {
        vtxID[2] = vtxID[0];
        vtxID[3] = vtxID[1];
        vtxID[0] = getBottomNeigh(vtxID[2]);
        vtxID[1] = getBottomNeigh(vtxID[3]);
    }

    /*
    vec3 v2 = geometryData[vtxID[2]], v3 = geometryData[vtxID[3]];
    if (vtxID[2] < 0 || vtxID[3] < 0)
    {
        std::cout << "vtxIDs: " << vtxID[0] << ", " << vtxID[1] << ", " << vtxID[2] << ", " << vtxID[3] << std::endl;
        std::cout << std::endl;
    }
    */

    float deltaX = geometryData[vtxID[1]].v[0] - geometryData[vtxID[0]].v[0],
        deltaY = geometryData[vtxID[0]].v[1] - geometryData[vtxID[2]].v[1];

    float deltaX1 = pos.v[0] - geometryData[vtxID[0]].v[0],
        deltaX2 = geometryData[vtxID[1]].v[0] - pos.v[0],
        deltaY1 = pos.v[1] - geometryData[vtxID[2]].v[1],
        deltaY2 = geometryData[vtxID[0]].v[1] - pos.v[1];

    float c = 1 / (deltaX * deltaY);
    coef[0] = deltaX2 * deltaY1 * c;
    coef[1] = deltaX1 * deltaY1 * c;
    coef[2] = deltaX2 * deltaY2 * c;
    coef[3] = deltaX1 * deltaY2 * c;
    return true;
}

float FlowGeometry::getMinX()
{
    return boundaryMin[0];
}

float FlowGeometry::getMaxX()
{
    return boundaryMax[0];
}

float FlowGeometry::getMinY()
{
    return boundaryMin[1];
}

float FlowGeometry::getMaxY()
{
    return boundaryMax[1];
}

int FlowGeometry::getDimX()
{
	return (isFlipped) ? dim[1] : dim[0];
}

int FlowGeometry::getDimY()
{
	return (isFlipped) ? dim[0] : dim[1];
}

inline int FlowGeometry::getDimZ()
{
    return dim[2];
}

int FlowGeometry::getVtx(int x, int y)
{
	//if we need to flip the rows and columns, we do it here 
	return (isFlipped)? (x*dim[1]) + y : (y*dim[0]) + x;
}

int FlowGeometry::getVtxX(int vtxID)
{
	//if we need to flip the rows and columns, we do it here
	return (isFlipped)? vtxID / dim[1] : vtxID % dim[0];
}

int FlowGeometry::getVtxY(int vtxID)
{
	//if we need to flip the rows and columns, we do it here
	return (isFlipped)? vtxID % dim[1] : vtxID / dim[0];
}

int FlowGeometry::getRightNeigh(int vtxID)
{
    int x = getVtxX(vtxID);
    return (x+1 < dim[0]) ? getVtx(x+1,getVtxY(vtxID)) : -1;
}

int FlowGeometry::getTopNeigh(int vtxID)
{
	//remember that the data is structured with (0,0) in the upper-left corner and (1,1) in the lower-right
	//that's why we are subtracting 1 to find the top neighbour
    int y = getVtxY(vtxID);
    return (y > 0) ? getVtx(getVtxX(vtxID), y-1) : -1;
}

int FlowGeometry::getLeftNeigh(int vtxID)
{
    int x = getVtxX(vtxID);
    return (x > 0) ? getVtx(x-1,getVtxY(vtxID)) : -1;
}

int FlowGeometry::getBottomNeigh(int vtxID)
{
	//remember that the data is structured with (0,0) in the upper-left corner and (1,1) in the lower-right
    int y = getVtxY(vtxID);
    return (y+1 < dim[1]) ? getVtx(getVtxX(vtxID), y+1) : -1;
}

vec3 FlowGeometry::normalizeCoords(vec3 pos)
{
	vec3 u(pos - boundaryMin);
	//scale each component according to the side length
	u[0] /= boundarySize[0];
	u[1] /= boundarySize[1];

	return u;
}

vec3 FlowGeometry::unNormalizeCoords(vec3 pos)
{
	vec3 u(pos);
	//multiply each component according to the side length
	u[0] *= boundarySize[0];
	u[1] *= boundarySize[1];

	return u += boundaryMin;
}
