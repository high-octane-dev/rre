#include "x360_index_buffer.hpp"
#include "x360_video_card.hpp"
#include "emotion_engine.hpp"

// OFFSET: 0x00412cd0, STATUS: COMPLETE
X360IndexBuffer::X360IndexBuffer(D3DFORMAT fmt, int param2) : IndexBuffer() {
    field1_0x4 = 0;
    Create(fmt, param2);
}

// OFFSET: 0x00412b80, STATUS: WIP
int X360IndexBuffer::Create(D3DFORMAT fmt, int _index_count) {
    if (fmt == D3DFMT_INDEX16) {
        index_count = _index_count;
        bytes_per_index = 2;
        format = D3DFMT_INDEX16;
        // The original game does some wierd VertexShaderVersion checks to choose between D3DUSAGE_WRITEONLY and some other stuff... not sure what's going on there.
        return SUCCEEDED(g_D3DDevice9->CreateIndexBuffer(index_count * 2, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &index_buffer, nullptr));
    }
    return 0;
}
