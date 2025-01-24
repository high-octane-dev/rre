#include "language_definition.hpp"
#include "game.hpp"
#include "cars_game.hpp"

// OFFSET: 0x00616ea0, STATUS: COMPLETE
LanguageDefinition::LanguageDefinition() {
	name[0] = '\0';
	prefix[0] = '\0';
	prefix_index = -1;
}

// OFFSET: 0x00618340, STATUS: COMPLETE
void LanguageDefinition::Read(ParameterBlock* file) {
	file->ReadParameterBlock(name);
	file->GetParameter("Prefix", "E", prefix, 32);
	reinterpret_cast<CarsGame*>(lpGame)->language_prefixes.CLAddItem(prefix);
	prefix_index = -1;
	for (std::size_t i = 0; i < reinterpret_cast<CarsGame*>(lpGame)->language_prefixes.Length(); i++) {
		if (reinterpret_cast<CarsGame*>(lpGame)->language_prefixes.GetItem(i) == prefix) {
			prefix_index = i;
		}
	}
}
