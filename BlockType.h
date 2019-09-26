/*
 * BlockType.h
 *
 *  Created on: Sep 22, 2019
 *      Author: coldfusion
 */

#ifndef BLOCKTYPE_H_
#define BLOCKTYPE_H_

#include <string>
#include <array>

using BlockFaceTex = std::array<float, 8>;

class BlockType {
public:

	unsigned char id;
	std::string name;

	bool opaque;

	BlockFaceTex front;
	BlockFaceTex right;
	BlockFaceTex back;
	BlockFaceTex left;
	BlockFaceTex top;
	BlockFaceTex bottom;
};


#endif /* BLOCKTYPE_H_ */
