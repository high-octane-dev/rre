#include "shader_pass_descriptor.hpp"

// OFFSET: 0x005647f0, STATUS: TODO
ShaderPassDescriptor::ShaderPassDescriptor() : BaseObject() {
}

// OFFSET: 0x00595a40, STATUS: TODO
ShaderPassDescriptor::~ShaderPassDescriptor() {
}

// OFFSET: 0x0040a4a0, STATUS: TODO
X360ShaderPassDescriptor::X360ShaderPassDescriptor() {
}

// OFFSET: 0x0040a500, STATUS: TODO
X360ShaderPassDescriptor::~X360ShaderPassDescriptor() {
}

// OFFSET: 0x0040c8b0, STATUS: TODO
int X360ShaderPassDescriptor::Unk0(char*) {
	return 0;
}

// OFFSET: 0x0040b200, STATUS: TODO
int X360ShaderPassDescriptor::Unk1(int) {
	return 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360ShaderPassDescriptor::Unk2() {
}

// OFFSET: 0x0040a4f0, STATUS: TODO
int X360ShaderPassDescriptor::GetTextureStringTableIndex(int index) {
	return 0;
}
