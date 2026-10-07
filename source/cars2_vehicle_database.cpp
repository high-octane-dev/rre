#include "cars2_vehicle_database.hpp"
#include "action_manager.hpp"
#include "cars_game.hpp"
#include "util/rsstring_util.hpp"

Cars2VehicleDatabase* lpGlobalVehicleDatabase = nullptr;

// Reads every value of a repeated parameter into a newly allocated array of strings.
// OFFSET: INLINE, STATUS: COMPLETE
static char** ReadStringArray(ParameterBlock* file, const char* parameter, int len, std::size_t max_len) {
	char** strings = new char*[len];
	file->search.SetResetParameterSearch(0);
	for (int i = 0; i < len; i++) {
		char value[64]{};
		file->GetParameter(parameter, "", value, max_len);
		strings[i] = lpGlobalVehicleDatabase->AllocString(value);
	}
	file->search.SetResetParameterSearch(1);
	return strings;
}

// OFFSET: INLINE, STATUS: COMPLETE
static void DestroyUnlockAction(Cars2UnlockAction*& action) {
	if (action == nullptr) {
		return;
	}
	if (action->action_script != nullptr) {
		lpASManager->RemoveActionScript(action->name);
		action->action_script = nullptr;
	}
	if (action->name != nullptr) {
		lpGlobalVehicleDatabase->string_block_allocator->FreeString(action->name);
	}
	delete action;
	action = nullptr;
}

// OFFSET: INLINE, STATUS: COMPLETE
static Cars2UnlockAction* CreateUnlockAction(const char* name, const char* script_name, ParameterBlock* file) {
	Cars2UnlockAction* action = new Cars2UnlockAction();
	action->name = nullptr;
	action->action_script = nullptr;

	char action_name[64]{};
	RSStringUtil::Ssnprintf(action_name, sizeof(action_name), "%s", name);
	action->name = lpGlobalVehicleDatabase->AllocString(action_name);
	action->action_script = lpASManager->CreateActionScript(action->name, const_cast<char*>(script_name), file);
	return action;
}

// OFFSET: 0x00429cc0, STATUS: COMPLETE
Cars2VehiclePaintJob::Cars2VehiclePaintJob() {
	name = nullptr;
	materials = nullptr;
	materials_len = 0;
	default_locked = 1;
	locked = 1;
	bonus_content_item = nullptr;
}

// OFFSET: 0x0042bec0, STATUS: COMPLETE
void Cars2VehiclePaintJob::Create(ParameterBlock* file) {
	file->ReadParameterBlock(name);
	file->GetParameter("Locked", 1, &default_locked);
	locked = default_locked;
	materials_len = file->GetNumberOfParameterValues("Material");
	if (materials_len > 0) {
		materials = ReadStringArray(file, "Material", materials_len, 24);
	}
}

// OFFSET: 0x0042bfd0, STATUS: COMPLETE
int Cars2VehiclePaintJob::Serialize(void* buffer, int len) {
	*reinterpret_cast<int*>(buffer) = locked;
	return sizeof(int);
}

// OFFSET: 0x0042bff0, STATUS: COMPLETE
int Cars2VehiclePaintJob::DeSerialize(void* buffer, int len) {
	locked = *reinterpret_cast<int*>(buffer);
	return sizeof(int);
}

// OFFSET: 0x0042be60, STATUS: COMPLETE
Cars2VehiclePaintJob::~Cars2VehiclePaintJob() {
	for (int i = 0; i < materials_len; i++) {
		lpGlobalVehicleDatabase->string_block_allocator->FreeString(materials[i]);
	}
	if (materials != nullptr) {
		delete[] materials;
		materials = nullptr;
	}
	if (name != nullptr) {
		lpGlobalVehicleDatabase->string_block_allocator->FreeString(name);
	}
}

// OFFSET: 0x00429cf0, STATUS: COMPLETE
Cars2VehicleWheelSet::Cars2VehicleWheelSet() {
	name = nullptr;
	materials = nullptr;
	materials_len = 0;
	default_locked = 1;
	locked = 1;
	unk[3] = 0;
	unk[2] = 0;
	unk[1] = 0;
	unk[0] = 0;
	bonus_content_item = nullptr;
}

// OFFSET: 0x0042c070, STATUS: COMPLETE
void Cars2VehicleWheelSet::Create(ParameterBlock* file) {
	file->ReadParameterBlock(name);
	file->GetParameter("Locked", 1, &default_locked);
	locked = default_locked;
	materials_len = file->GetNumberOfParameterValues("Material");
	if (materials_len > 0) {
		materials = ReadStringArray(file, "Material", materials_len, 24);
	}
}

// OFFSET: 0x0042c180, STATUS: COMPLETE
int Cars2VehicleWheelSet::Serialize(void* buffer, int len) {
	int* out = reinterpret_cast<int*>(buffer);
	out[0] = unk[0];
	out[1] = unk[1];
	out[2] = unk[2];
	out[3] = unk[3];
	out[4] = locked;
	return sizeof(int) * 5;
}

// OFFSET: 0x0042c1b0, STATUS: COMPLETE
int Cars2VehicleWheelSet::DeSerialize(void* buffer, int len) {
	int* in = reinterpret_cast<int*>(buffer);
	unk[0] = in[0];
	unk[1] = in[1];
	unk[2] = in[2];
	unk[3] = in[3];
	locked = in[4];
	return sizeof(int) * 5;
}

// OFFSET: 0x0042c010, STATUS: COMPLETE
Cars2VehicleWheelSet::~Cars2VehicleWheelSet() {
	for (int i = 0; i < materials_len; i++) {
		lpGlobalVehicleDatabase->string_block_allocator->FreeString(materials[i]);
	}
	if (materials != nullptr) {
		delete[] materials;
		materials = nullptr;
	}
	if (name != nullptr) {
		lpGlobalVehicleDatabase->string_block_allocator->FreeString(name);
	}
}

// OFFSET: 0x0042c1e0, STATUS: COMPLETE
Cars2VehicleRecord::Cars2VehicleRecord() {
	paint_job_materials = nullptr;
	paint_job_materials_len = 0;
	paint_jobs = nullptr;
	paint_jobs_len = 0;
	wheel_set_materials = nullptr;
	wheel_set_materials_len = 0;
	wheel_sets = nullptr;
	wheel_sets_len = 0;
	name = nullptr;
	directory = nullptr;
	default_physics_file = nullptr;
	name_string_id = nullptr;
	animation_enabled = 1;
	playable = 0;
	default_locked = 1;
	locked = 1;
	bonus_content_item = nullptr;
}

// OFFSET: 0x004b4e80, STATUS: COMPLETE
void Cars2VehicleRecord::Create(ParameterBlock* file) {
	char value[64]{};

	file->ReadParameterBlock(name);
	file->GetParameter("Directory", name, value, sizeof(value));
	directory = lpGlobalVehicleDatabase->AllocString(value);

	char default_physics_file_name[64]{};
	RSStringUtil::Ssnprintf(default_physics_file_name, sizeof(default_physics_file_name), "%s.phy", name);
	file->GetParameter("DefaultPhysicsFile", default_physics_file_name, value, sizeof(value));
	default_physics_file = lpGlobalVehicleDatabase->AllocString(value);

	file->GetParameter("AnimationEnabled", 1, &animation_enabled);
	file->GetParameter("Boost", 1, &boost);
	file->GetParameter("Acceleration", 1, &acceleration);
	file->GetParameter("Handling", 1, &handling);
	file->GetParameter("Stability", 1, &stability);
	file->GetParameter("Playable", 0, &playable);

	file->GetParameter("NameStringID", "", value, sizeof(value));
	if (value[0] != '\0') {
		name_string_id = lpGlobalVehicleDatabase->AllocString(value);
	}

	file->GetParameter("Locked", 1, &default_locked);
	locked = default_locked;

	paint_job_materials_len = file->GetNumberOfParameterValues("PaintJobMaterial");
	if (paint_job_materials_len > 0) {
		paint_job_materials = ReadStringArray(file, "PaintJobMaterial", paint_job_materials_len, sizeof(value));

		paint_jobs_len = file->GetNumberOfParameterValues("PaintJob");
		if (paint_jobs_len > 0) {
			paint_jobs = new Cars2VehiclePaintJob[paint_jobs_len];
			file->search.SetResetParameterSearch(0);
			for (int i = 0; i < paint_jobs_len; i++) {
				file->GetParameter("PaintJob", "", value, sizeof(value));
				paint_jobs[i].name = lpGlobalVehicleDatabase->AllocString(value);
			}
			file->search.SetResetParameterSearch(1);
		}
	}

	wheel_set_materials_len = file->GetNumberOfParameterValues("WheelSetMaterial");
	if (wheel_set_materials_len > 0) {
		wheel_set_materials = ReadStringArray(file, "WheelSetMaterial", wheel_set_materials_len, sizeof(value));

		wheel_sets_len = file->GetNumberOfParameterValues("WheelSet");
		if (wheel_sets_len > 0) {
			wheel_sets = new Cars2VehicleWheelSet[wheel_sets_len];
			file->search.SetResetParameterSearch(0);
			for (int i = 0; i < wheel_sets_len; i++) {
				file->GetParameter("WheelSet", "", value, sizeof(value));
				wheel_sets[i].name = lpGlobalVehicleDatabase->AllocString(value);
			}
			file->search.SetResetParameterSearch(1);
		}
	}

	// Every paint job and wheel set is also registered with the vehicle database, so they can be looked up by name.
	for (int i = 0; i < paint_jobs_len; i++) {
		paint_jobs[i].Create(file);
		lpGlobalVehicleDatabase->paint_jobs.CLAddItem(&paint_jobs[i]);
	}
	for (int i = 0; i < wheel_sets_len; i++) {
		wheel_sets[i].Create(file);
		lpGlobalVehicleDatabase->wheel_sets.CLAddItem(&wheel_sets[i]);
	}
}

// OFFSET: 0x0042c3e0, STATUS: COMPLETE
Cars2VehiclePaintJob* Cars2VehicleRecord::GetPaintJob(const char* paint_job_name) {
	for (int i = 0; i < paint_jobs_len; i++) {
		if (_stricmp(paint_job_name, paint_jobs[i].name) == 0) {
			return &paint_jobs[i];
		}
	}
	return nullptr;
}

// OFFSET: 0x0042c490, STATUS: COMPLETE
void Cars2VehicleRecord::Reset() {
	locked = default_locked;
	for (int i = 0; i < paint_jobs_len; i++) {
		paint_jobs[i].locked = paint_jobs[i].default_locked;
	}
	for (int i = 0; i < wheel_sets_len; i++) {
		wheel_sets[i].unk[3] = 0;
		wheel_sets[i].unk[2] = 0;
		wheel_sets[i].unk[1] = 0;
		wheel_sets[i].unk[0] = 0;
		wheel_sets[i].locked = wheel_sets[i].default_locked;
	}
}

// OFFSET: 0x0042c550, STATUS: COMPLETE
int Cars2VehicleRecord::Serialize(void* buffer, int len) {
	*reinterpret_cast<int*>(buffer) = locked;
	int written = sizeof(int);
	for (int i = 0; i < paint_jobs_len; i++) {
		written += paint_jobs[i].Serialize(reinterpret_cast<std::uint8_t*>(buffer) + written, len - written);
	}
	for (int i = 0; i < wheel_sets_len; i++) {
		written += wheel_sets[i].Serialize(reinterpret_cast<std::uint8_t*>(buffer) + written, len - written);
	}
	return written;
}

// OFFSET: 0x0042c5e0, STATUS: COMPLETE
int Cars2VehicleRecord::DeSerialize(void* buffer, int len) {
	locked = *reinterpret_cast<int*>(buffer);
	int read = sizeof(int);
	for (int i = 0; i < paint_jobs_len; i++) {
		read += paint_jobs[i].DeSerialize(reinterpret_cast<std::uint8_t*>(buffer) + read, len - read);
	}
	for (int i = 0; i < wheel_sets_len; i++) {
		read += wheel_sets[i].DeSerialize(reinterpret_cast<std::uint8_t*>(buffer) + read, len - read);
	}
	return read;
}

// OFFSET: 0x0042c240, STATUS: COMPLETE
Cars2VehicleRecord::~Cars2VehicleRecord() {
	// The original game leaks the material strings themselves, and only frees the arrays.
	if (paint_job_materials != nullptr) {
		delete[] paint_job_materials;
		paint_job_materials = nullptr;
	}
	if (paint_jobs != nullptr) {
		delete[] paint_jobs;
		paint_jobs = nullptr;
	}
	if (wheel_set_materials != nullptr) {
		delete[] wheel_set_materials;
		wheel_set_materials = nullptr;
	}
	if (wheel_sets != nullptr) {
		delete[] wheel_sets;
		wheel_sets = nullptr;
	}
	if (name != nullptr) {
		lpGlobalVehicleDatabase->string_block_allocator->FreeString(name);
	}
	if (directory != nullptr) {
		lpGlobalVehicleDatabase->string_block_allocator->FreeString(directory);
	}
	if (default_physics_file != nullptr) {
		lpGlobalVehicleDatabase->string_block_allocator->FreeString(default_physics_file);
	}
	if (name_string_id != nullptr) {
		lpGlobalVehicleDatabase->string_block_allocator->FreeString(name_string_id);
	}
}

// OFFSET: 0x004761a0, STATUS: COMPLETE
Cars2VehicleDatabase::Cars2VehicleDatabase() {
	paint_job_unlock_action = nullptr;
	wheel_set_unlock_action = nullptr;
	vehicles = nullptr;
	vehicles_len = 0;
	vehicle_unlock_action = nullptr;
	string_block_allocator = nullptr;
	lpGlobalVehicleDatabase = this;
}

// OFFSET: 0x004b5610, STATUS: WIP
void Cars2VehicleDatabase::Create() {
	ParameterBlock file{};
	char path[260]{};
	RSStringUtil::Ssnprintf(path, sizeof(path), "%sVehicleInfo.dat", g_GlobalCharsContentDirectory);
	if (file.OpenFile(path, 0, -1, nullptr, -1) == 0) {
		return;
	}

	if (string_block_allocator == nullptr) {
		string_block_allocator = new StringBlockAllocator(0x6000, 0x2000);
		string_block_allocator->Create(0);
	}
	paint_jobs.CLNonMacroCreate(0x40, 0x10, 0x7fffffff);
	wheel_sets.CLNonMacroCreate(0x40, 0x10, 0x7fffffff);

	file.ReadParameterBlock("Vehicles");
	vehicles_len = file.GetNumberOfParameterValues("Vehicle");
	if (vehicles_len > 0) {
		vehicles = new Cars2VehicleRecord[vehicles_len];

		file.search.SetResetParameterSearch(0);
		for (int i = 0; i < vehicles_len; i++) {
			char vehicle_name[64]{};
			file.GetParameter("Vehicle", "", vehicle_name, sizeof(vehicle_name));
			vehicles[i].name = AllocString(vehicle_name);
		}
		file.search.SetResetParameterSearch(1);

		for (int i = 0; i < vehicles_len; i++) {
			vehicles[i].Create(&file);
		}
	}

	ParameterBlock action_script_file{};
	RSStringUtil::Ssnprintf(path, sizeof(path), "%sVehicleDatabase.as", g_StoryContentDirectory);
	// FIXME: The original game does not check lpASManager, but since we don't create it yet, we have to.
	if (lpASManager != nullptr && action_script_file.OpenFile(path, 0, -1, nullptr, -1) != 0) {
		paint_job_unlock_action = CreateUnlockAction("PaintJob_ItemUnlocked", "PaintJob", &action_script_file);
		wheel_set_unlock_action = CreateUnlockAction("WheelSet_ItemUnlocked", "WheelSet", &action_script_file);
		vehicle_unlock_action = CreateUnlockAction("Vehicle_ItemUnlocked", "Vehicle", &action_script_file);
	}
}

// OFFSET: 0x0042c6d0, STATUS: COMPLETE
Cars2VehicleRecord* Cars2VehicleDatabase::GetVehicle(const char* name) {
	for (int i = 0; i < vehicles_len; i++) {
		if (_stricmp(name, vehicles[i].name) == 0) {
			return &vehicles[i];
		}
	}
	return nullptr;
}

// OFFSET: 0x004763b0, STATUS: COMPLETE
Cars2VehiclePaintJob* Cars2VehicleDatabase::GetPaintJob(const char* name) {
	for (std::size_t i = 0; i < paint_jobs.Length(); i++) {
		if (_stricmp(name, paint_jobs[i]->name) == 0) {
			return paint_jobs[i];
		}
	}
	return nullptr;
}

// OFFSET: 0x00476400, STATUS: COMPLETE
Cars2VehicleWheelSet* Cars2VehicleDatabase::GetWheelSet(const char* name) {
	for (std::size_t i = 0; i < wheel_sets.Length(); i++) {
		if (_stricmp(name, wheel_sets[i]->name) == 0) {
			return wheel_sets[i];
		}
	}
	return nullptr;
}

// OFFSET: 0x0042c720, STATUS: COMPLETE
void Cars2VehicleDatabase::UnlockAllPlayableVehicles() {
	for (int i = 0; i < vehicles_len; i++) {
		if (vehicles[i].playable != 0) {
			vehicles[i].locked = 0;
		}
	}
}

// OFFSET: 0x0042c750, STATUS: COMPLETE
int Cars2VehicleDatabase::Serialize(void* buffer, int len) {
	int written = 0;
	for (int i = 0; i < vehicles_len; i++) {
		written += vehicles[i].Serialize(reinterpret_cast<std::uint8_t*>(buffer) + written, len - written);
	}
	return written;
}

// OFFSET: 0x0042c7a0, STATUS: COMPLETE
int Cars2VehicleDatabase::DeSerialize(void* buffer, int len) {
	int read = 0;
	for (int i = 0; i < vehicles_len; i++) {
		read += vehicles[i].DeSerialize(reinterpret_cast<std::uint8_t*>(buffer) + read, len - read);
	}
	return read;
}

// OFFSET: 0x004761f0, STATUS: COMPLETE
Cars2VehicleDatabase::~Cars2VehicleDatabase() {
	if (vehicles != nullptr) {
		delete[] vehicles;
		vehicles = nullptr;
	}
	DestroyUnlockAction(vehicle_unlock_action);
	paint_jobs.Clear();
	DestroyUnlockAction(paint_job_unlock_action);
	wheel_sets.Clear();
	DestroyUnlockAction(wheel_set_unlock_action);
	if (string_block_allocator != nullptr) {
		delete string_block_allocator;
		string_block_allocator = nullptr;
	}
	lpGlobalVehicleDatabase = nullptr;
}
