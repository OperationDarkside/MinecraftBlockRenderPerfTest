/*
 * ChunkBlockProto.h
 *
 *  Created on: Sep 23, 2019
 *      Author: coldfusion
 */

#ifndef CHUNKBLOCKPROTO_H_
#define CHUNKBLOCKPROTO_H_

#include <glm/glm.hpp>

#include <array>

const unsigned char CHUNK_SIZE = 16;

using ChunkBlockData = std::array<std::array<std::array<unsigned char, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE>;

class ChunkBlockProto {

public:
	glm::vec3 pos;
	ChunkBlockData data;
};



#endif /* CHUNKBLOCKPROTO_H_ */
