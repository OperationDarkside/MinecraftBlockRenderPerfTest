/*
 * ChunkRenderer.h
 *
 *  Created on: Sep 29, 2019
 *      Author: coldfusion
 */

#ifndef CHUNKRENDERER_H_
#define CHUNKRENDERER_H_

#include <array>

#include "Chunk.h"
#include "ChunkBlockProtoRenderer.h"

using ChunkBlockRenderers = std::array<ChunkBlockProtoRenderer, 16>;

class ChunkRenderer {

public:

	void createRenderData (Chunk& chunk, std::unordered_map<unsigned char, BlockType> &types){
		ChunkColumn& chunkCol = chunk.getBlocks();

		for(int i = 0; i < 16; ++i){
			ChunkBlockProto& chunkBlock = chunkCol[i];
			ChunkBlockProtoRenderer& renderer = chunkBlockRenderers[i];

			renderer.setModel(chunkBlock.pos);
			renderer.createBlockFaces(chunkBlock.data, types);
			renderer.rebuildVBO(chunkBlock.data, types);
			renderer.init();
		}
	}

	void draw(){
		for(auto& cbRenderer : chunkBlockRenderers){
			cbRenderer.draw();
		}
	}

	void deleteStuff(){
		for(auto& cbRenderer : chunkBlockRenderers){
			cbRenderer.deleteStuff();
		}
	}

private:
	ChunkBlockRenderers chunkBlockRenderers;
};



#endif /* CHUNKRENDERER_H_ */
