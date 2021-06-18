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

	void _SetFreeCamSpeed(float a_speed)
	{
		const auto ini = RE::INISettingCollection::GetSingleton();
		const auto speed = ini->GetSetting("fFreeCameraTranslationSpeed:Camera");
		if (speed)
			speed->data.f = a_speed;
	}

	// Enable freecam controls
	void _sub_140C11AE0(RE::ControlMap* a_controlMap, int64_t a_arg2)
	{
		using func_t = decltype(&_sub_140C11AE0);
		REL::Relocation<func_t> sub_140C11AE0{ REL::ID(67243) };
		sub_140C11AE0(a_controlMap, a_arg2);
	}

	// Disable freecam controls?
	void _sub_140C11BC0(RE::ControlMap* a_controlMap, int64_t a_arg2)
	{
		using func_t = decltype(&_sub_140C11BC0);
		REL::Relocation<func_t> sub_140C11BC0{ REL::ID(67244) };
		sub_140C11BC0(a_controlMap, a_arg2);
	}
}

namespace Papyrus::Camera
{
	using VM = RE::BSScript::IVirtualMachine;

	bool IsFreeCam(RE::StaticFunctionTag*)
	{
		const auto camera = RE::PlayerCamera::GetSingleton();
		return camera ? _IsFreeCam(camera) : false;
	}

	void EnableFreeCam(RE::StaticFunctionTag*, bool a_stopTime)
	{
		const auto camera = RE::PlayerCamera::GetSingleton();
		if (camera) {
			if (!_IsFreeCam(camera)) {
				_ToggleFreeCam(camera, a_stopTime);

				const auto control = RE::ControlMap::GetSingleton();
				_sub_140C11AE0(control, 13);
			}
		}
	}

	void DisableFreeCam(RE::StaticFunctionTag*)
	{
		const auto camera = RE::PlayerCamera::GetSingleton();
		if (camera) {
			if (_IsFreeCam(camera)) {
				_ToggleFreeCam(camera, false);

				const auto control = RE::ControlMap::GetSingleton();
				_sub_140C11BC0(control, 13);
			}
		}
	}

	void SetFreeCamSpeed(RE::StaticFunctionTag*, float a_speed)
	{
		if (a_speed >= 0.0f)
			_SetFreeCamSpeed(a_speed);
	}

	void SetFOV(RE::StaticFunctionTag*, float a_value, bool a_firstPerson)
	{
		const auto camera = RE::PlayerCamera::GetSingleton();
		if (camera) {
			if (a_firstPerson)
				camera->firstPersonFOV = a_value;
			else
				camera->worldFOV = a_value;
		}
	}

	std::vector<float> GetCameraPos(RE::StaticFunctionTag*)
	{
		const auto camera = RE::PlayerCamera::GetSingleton();

		auto coords = camera->pos; // NiPoint3 object

		std::vector<float> ret{ coords[0], coords[1], coords[2] };
		return ret;
	}

	void SetCameraPos(RE::StaticFunctionTag*, float x, float y, float z)
	{
		const auto camera = RE::PlayerCamera::GetSingleton();

		RE::NiPoint3 coords = {x, y, z};

		camera->pos = coords;
	}

    void Bind(VM& a_vm)
	{
		const auto obj = "OSANative"sv;

		BIND(SetCameraPos);
		BIND(GetCameraPos);
		BIND(IsFreeCam);
		BIND(EnableFreeCam);
		BIND(DisableFreeCam);
		BIND(SetFreeCamSpeed);
		BIND(SetFOV);
	}
}