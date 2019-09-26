/*
 * ChunkBlockProtoRenderer.h
 *
 *  Created on: Sep 24, 2019
 *      Author: coldfusion
 */

#ifndef CHUNKBLOCKPROTORENDERER_H_
#define CHUNKBLOCKPROTORENDERER_H_

#include <glm/glm.hpp>

#include <array>
#include <vector>
#include <unordered_map>

#include "BlockType.h"
#include "ChunkBlockProto.h"
#include "BlockRenderData.h"

using OpaquenessData = std::array<std::array<std::array<bool, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE>;
using BlockFacesData = std::array<std::array<std::array<BlockRenderData, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE>;

class ChunkBlockProtoRenderer {
public:
	void init() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(float), &vboData[0],
		GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboData.size() * sizeof(unsigned int), &eboData[0], GL_STATIC_DRAW);

		// position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
				(void*) 0);
		glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
						(void*) (3 * sizeof(float)));

		glBindVertexArray(0);
	}

	void createBlockFaces(const ChunkBlockData &_data,
			std::unordered_map<unsigned char, BlockType> &types) {
		OpaquenessData opData;

		for (int x = 0; x < CHUNK_SIZE; ++x) {
			for (int y = 0; y < CHUNK_SIZE; ++y) {
				for (int z = 0; z < CHUNK_SIZE; ++z) {
					unsigned char blockTypeId = _data[x][y][z];
					if (blockTypeId == 0) {
						// Air
						opData[x][y][z] = false;
						continue;
					}
					BlockType &type = types[blockTypeId];
					opData[x][y][z] = type.opaque;
				}
			}
		}

		faceCount = 0;

		for (int x = 0; x < CHUNK_SIZE; ++x) {
			for (int y = 0; y < CHUNK_SIZE; ++y) {
				for (int z = 0; z < CHUNK_SIZE; ++z) {
					bool opaque = opData[x][y][z];
					if(!opaque){
						continue;
					}
					BlockRenderData &bld = blockFaces[x][y][z];

					// FRONT
					if (z == 15) {
						bld.front = true;
						faceCount++;
					} else {
						bool opaqueFront = opData[x][y][z + 1];
						bld.front = !opaqueFront;
						faceCount += !opaqueFront;
					}

					// BACK
					if (z == 0) {
						bld.back = true;
					} else {
						bool opaqueBack = opData[x][y][z - 1];
						bld.back = !opaqueBack;
						faceCount += !opaqueBack;
					}

					// LEFT
					if (x == 0) {
						bld.left = true;
						faceCount++;
					} else {
						bool opaqueLeft = opData[x - 1][y][z];
						bld.left = !opaqueLeft;
						faceCount += !opaqueLeft;
					}

					// RIGHT
					if (x == 15) {
						bld.right = true;
						faceCount++;
					} else {
						bool opaqueRight = opData[x + 1][y][z];
						bld.right = !opaqueRight;
						faceCount += !opaqueRight;
					}

					// TOP
					if (y == 15) {
						bld.top = true;
						faceCount++;
					} else {
						bool opaqueTop = opData[x][y + 1][z];
						bld.top = !opaqueTop;
						faceCount += !opaqueTop;
					}

					// BOTTOM
					if (y == 0) {
						bld.bottom = true;
						faceCount++;
					} else {
						bool opaqueBottom = opData[x][y - 1][z];
						bld.bottom = !opaqueBottom;
						faceCount += !opaqueBottom;
					}

					if (bld.front || bld.back || bld.left || bld.right
							|| bld.top || bld.bottom) {
						bld.render = true;
					} else {
						bld.render = false;
					}
				}
			}
		}
	}

	void rebuildVBO(const ChunkBlockData &_data,
			std::unordered_map<unsigned char, BlockType> &types) {
		unsigned int eboCntr = 0;
		vboData.clear();
		vboData.reserve(faceCount * 20);
		eboData.clear();
		eboData.reserve(faceCount * 6);

		for (int x = 0; x < CHUNK_SIZE; ++x) {
			for (int y = 0; y < CHUNK_SIZE; ++y) {
				for (int z = 0; z < CHUNK_SIZE; ++z) {
					BlockRenderData &renderData = blockFaces[x][y][z];
					if (!renderData.render) {
						continue;
					}

					unsigned char blockTypeId = _data[x][y][z];
					BlockType &type = types[blockTypeId];

					// FRONT
					if (renderData.front) {
						std::array<float, 20> vboFrontData {
							(float) x, (float) y, (float) z + 1, type.front[0], type.front[1],
							(float) x + 1, (float) y, (float) z + 1, type.front[2], type.front[3],
							(float) x, (float) y + 1, (float) z + 1, type.front[4], type.front[5],
							(float) x + 1, (float) y + 1, (float) z + 1, type.front[6], type.front[7]
						};
						vboData.insert(vboData.end(), vboFrontData.begin(),
								vboFrontData.end());

						unsigned int bot_left = eboCntr++;
						unsigned int bot_right = eboCntr++;
						unsigned int top_left = eboCntr++;
						unsigned int top_right = eboCntr++;
						eboData.insert(eboData.end(), {bot_left, bot_right, top_right, bot_left, top_right, top_left});
					}
					// BACK
					if(renderData.back){
						std::array<float, 20> vboBackData {
							(float) x + 1, (float) y, (float) z, type.back[0], type.back[1],
							(float) x, (float) y, (float) z, type.back[2], type.back[3],
							(float) x + 1, (float) y + 1, (float) z, type.back[4], type.back[5],
							(float) x, (float) y + 1, (float) z, type.back[6], type.back[7]
						};
						vboData.insert(vboData.end(), vboBackData.begin(),
								vboBackData.end());

						// viewed from the back
						unsigned int bot_left = eboCntr++;
						unsigned int bot_right = eboCntr++;
						unsigned int top_left = eboCntr++;
						unsigned int top_right = eboCntr++;
						eboData.insert(eboData.end(), {bot_left, bot_right, top_right, bot_left, top_right, top_left});
					}
					// RIGHT
					if(renderData.right){
						std::array<float, 20> vboRightData {
							(float) x + 1, (float) y, (float) z + 1, type.right[0], type.right[1],
							(float) x + 1, (float) y, (float) z, type.right[2], type.right[3],
							(float) x + 1, (float) y + 1, (float) z + 1, type.right[4], type.right[5],
							(float) x + 1, (float) y + 1, (float) z, type.right[6], type.right[7]
						};
						vboData.insert(vboData.end(), vboRightData.begin(),
								vboRightData.end());

						// viewed from the right
						unsigned int bot_left = eboCntr++;
						unsigned int bot_right = eboCntr++;
						unsigned int top_left = eboCntr++;
						unsigned int top_right = eboCntr++;
						eboData.insert(eboData.end(), {bot_left, bot_right, top_right, bot_left, top_right, top_left});
					}
					// LEFT
					if(renderData.left){
						std::array<float, 20> vboLeftData {
							(float) x, (float) y, (float) z, type.left[0], type.left[1],
							(float) x, (float) y, (float) z + 1, type.left[2], type.left[3],
							(float) x, (float) y + 1, (float) z, type.left[4], type.left[5],
							(float) x, (float) y + 1, (float) z + 1, type.left[6], type.left[7]
						};
						vboData.insert(vboData.end(), vboLeftData.begin(),
								vboLeftData.end());

						// viewed from the left
						unsigned int bot_left = eboCntr++;
						unsigned int bot_right = eboCntr++;
						unsigned int top_left = eboCntr++;
						unsigned int top_right = eboCntr++;
						eboData.insert(eboData.end(), {bot_left, bot_right, top_right, bot_left, top_right, top_left});
					}
					// TOP
					if(renderData.top){
						std::array<float, 20> vboTopData {
							(float) x, (float) y + 1, (float) z + 1, type.top[0], type.top[1],
							(float) x + 1, (float) y + 1, (float) z + 1, type.top[2], type.top[3],
							(float) x, (float) y + 1, (float) z, type.top[4], type.top[5],
							(float) x + 1, (float) y + 1, (float) z, type.top[6], type.top[7]
						};
						vboData.insert(vboData.end(), vboTopData.begin(),
								vboTopData.end());

						// viewed from the left
						unsigned int bot_left = eboCntr++;
						unsigned int bot_right = eboCntr++;
						unsigned int top_left = eboCntr++;
						unsigned int top_right = eboCntr++;
						eboData.insert(eboData.end(), {bot_left, bot_right, top_right, bot_left, top_right, top_left});
					}
					// BOTTOM
					if(renderData.bottom){
						std::array<float, 20> vboBottomData {
							(float) x, (float) y, (float) z, type.bottom[0], type.bottom[1],
							(float) x + 1, (float) y, (float) z, type.bottom[2], type.bottom[3],
							(float) x, (float) y, (float) z + 1, type.bottom[4], type.bottom[5],
							(float) x + 1, (float) y, (float) z + 1, type.bottom[6], type.bottom[7]
						};
						vboData.insert(vboData.end(), vboBottomData.begin(),
								vboBottomData.end());

						// viewed from the left
						unsigned int bot_left = eboCntr++;
						unsigned int bot_right = eboCntr++;
						unsigned int top_left = eboCntr++;
						unsigned int top_right = eboCntr++;
						eboData.insert(eboData.end(), {bot_left, bot_right, top_right, bot_left, top_right, top_left});
					}
				}
			}
		}
	}

	void draw() {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, eboData.size(), GL_UNSIGNED_INT, 0);
	}

	void deleteStuff(){
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	glm::vec3& getModel(){
		return model;
	}

	void setModel(glm::vec3 _model){
		model = _model;
	}

private:
	unsigned int VBO, VAO, EBO;
	glm::vec3 model;
	unsigned short faceCount = 0;
	BlockFacesData blockFaces;
	std::vector<float> vboData;
	std::vector<unsigned int> eboData;
};

#endif /* CHUNKBLOCKPROTORENDERER_H_ */
