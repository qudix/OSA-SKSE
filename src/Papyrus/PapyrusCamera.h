#pragma once

namespace
{
	bool _IsFreeCam(RE::PlayerCamera* a_camera)
	{
		return a_camera->currentState == a_camera->cameraStates[RE::CameraState::kFree];
	}	
	
	typedef void (*_ToggleFreeCam_)(RE::PlayerCamera*, bool);
	void _ToggleFreeCam(RE::PlayerCamera* a_camera, bool a_stopTime)
	{
		REL::Relocation<_ToggleFreeCam_> ToggleFreeCamFunc{ REL::ID(49876) };
		ToggleFreeCamFunc(a_camera, a_stopTime);
	}
}

namespace Papyrus::Camera
{
	using VM = RE::BSScript::IVirtualMachine;

	bool IsFreeCam(RE::StaticFunctionTag*)
	{
		auto camera = RE::PlayerCamera::GetSingleton();
		if (camera)
			return _IsFreeCam(camera);

		return false;
	}

	void EnableFreeCam(RE::StaticFunctionTag*, bool a_stopTime, [[maybe_unused]] float a_speed)
	{
		auto camera = RE::PlayerCamera::GetSingleton();
		if (camera) {
			if (!_IsFreeCam(camera)) {
				logger::info("EnableFreeCam");
				_ToggleFreeCam(camera, a_stopTime);
			}
		}
	}

	void DisableFreeCam(RE::StaticFunctionTag*)
	{
		auto camera = RE::PlayerCamera::GetSingleton();
		if (camera) {
			if (_IsFreeCam(camera)) {
				logger::info("DisableFreeCam");
				_ToggleFreeCam(camera, false);
			}
		}
	}

	static constexpr char CLASS_NAME[] = "OSANative";
    bool Register(VM* a_vm)
	{
		if (!a_vm) {
			logger::critical("Papyrus Camera: Couldn't get VM");
			return false;
		}

		a_vm->RegisterFunction("IsFreeCam", CLASS_NAME, IsFreeCam);
		a_vm->RegisterFunction("EnableFreeCam", CLASS_NAME, EnableFreeCam);
		a_vm->RegisterFunction("DisableFreeCam", CLASS_NAME, DisableFreeCam);

		return true;
	}
}