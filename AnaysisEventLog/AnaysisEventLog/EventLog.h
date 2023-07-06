#pragma once

typedef unsigned char UC;

#define _CHUNK_HEADER_SIZE 0x10000
struct _chunkHeader { // 0x10000 사이즈 고정
	UC chunkSignature[8];

	UC firstEventRecordNumber[8];
	UC lastEventRecordNumber[8];

	UC firstEventRecordIdentifier[8];
	UC lastEventRecordIdentifier[8];

	UC headerSize[4];
	UC lastEventRecordDataOffset[4];
	UC freeSpaceOffset[4];
	UC eventRecordsChecksum[4];

	UC unknown_1[64];
	UC unknown_2[4];

	UC checksum[4];

	UC commonStringOffsetArray[256];
	UC templatePtr[128];

};

struct _fileHeader {
	UC fileSignature[8];

	UC firstChunkNumber[8];
	UC lastChunkNumber[8];
	UC nextRecordIdentifier[8];

	UC headerSize[4];

	UC minorFormatVersion[2];
	UC majorFormatVersion[2];

	UC headerBlockSize[2];

	UC numberOfChunks[2]; // Chunks의 갯수

	UC unknown_1[76];

	UC fileFlags[4];
	UC checksum[4];

	UC unknown_2[3968]; // headerBlockSize - headerSize
};

