/*
 * Chunk.h
 *
 *  Created on: Sep 22, 2019
 *      Author: coldfusion
 */

#ifndef CHUNK_H_
#define CHUNK_H_

#include <array>

#include <glm/glm.hpp>

#include "ChunkBlockProto.h"

using ChunkColumn = std::array<ChunkBlockProto, 16>;

class Chunk {
public:

	void setWorldPos(const glm::vec3& _pos){
		worldPos = _pos;

		for(int i = 0; i < blocks.size(); ++i){
			ChunkBlockProto& cBlock = blocks[i];

			glm::vec3 newPos = _pos;
			newPos.y = i * 16;
			cBlock.pos = newPos;
		}
	}

	const glm::vec3& getWorldPos() {
		return worldPos;
	}

	ChunkColumn& getBlocks(){
		return blocks;
	}

private:
	glm::vec3 worldPos;
	ChunkColumn blocks;
};


#endif /* CHUNK_H_ */
